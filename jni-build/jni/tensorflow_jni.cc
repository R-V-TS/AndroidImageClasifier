/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "dct2Dblind.h"

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>

#include "tensorflow/examples/android/jni/jni_utils.cc"

#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/types.pb.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

#include <string>
#include <array>
#include <vector>


#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024

using namespace tensorflow;

namespace {
    typedef struct { // Type define for bitmap
        uint8 red;
        uint8 green;
        uint8 blue;
        uint8 alpha;
    } RGBA;
}

std::unique_ptr<Session> session; // Session tensorflow
bool isConfig = false;

void bitmapToY(RGBA* bitmap, int width, int height, double *image)
{
    for(int i = 0; i < height*width; i++)
            *(image+i) = (0.299 * static_cast<float>((bitmap+i)->red)) + (0.587 * static_cast<float>((bitmap+i)->green)) + (0.114 * static_cast<float>((bitmap+i)->blue));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_rostislav_photoapp_TensorflowInterface_initTF(JNIEnv* env, jobject thiz, jobject java_asset_manager, jstring model)
{
    const char* model_string = env->GetStringUTFChars(model, NULL);

    SessionOptions options;

    ConfigProto& config = options.config;

    session.reset(NewSession(options));

    GraphDef tf_graph;

    AAssetManager* const asset_manager = AAssetManager_fromJava(env, java_asset_manager);

    ReadFileToProto(asset_manager, model_string, &tf_graph);

    LOG(INFO) << "Tensor input: " << model_string;
    tensorflow::Status s = session->Create(tf_graph);
    LOG(INFO) << "Status graph is " << s;

    if(s.ok()) {
        isConfig = true;
        return 0;
    }

    LOG(ERROR) << "Status graph is " << s;
    return -1;
}

extern "C" JNIEXPORT jdouble JNICALL
Java_com_example_rostislav_photoapp_TensorflowInterface_loadPredict(JNIEnv* env, jobject thiz, jobject bitmap)
{
    if(isConfig)
    {
        AndroidBitmapInfo info;
        AndroidBitmap_getInfo(env, bitmap, &info);
        LOG(INFO) << "Bitmap info " << info.width << " x " << info.height;

        void* pixels;
        AndroidBitmap_lockPixels(env, bitmap, &pixels);

        auto image = static_cast<RGBA*>(pixels);

        double y_image[info.height][info.width];
        bitmapToY(image, info.width, info.height, &y_image[0][0]);

        LOG(INFO) << "Bitmap info " << y_image[0][0];

        AndroidBitmap_unlockPixels(env, bitmap);

        double coff[16];
        dct2Dblind(&y_image[0][0], info.height, info.width, &coff[0]);
        for(int i = 0; i < 16; i++) // FOR DEBUG
        {
            LOG(INFO) << "Coff info " << i << " = " << coff[i];
        }

        Tensor inputTensor(DT_FLOAT, TensorShape({1, 16}));

        auto inpTensor = inputTensor.tensor<float, 2>();

        for(int i = 0; i < 16; i++)
        {
            inpTensor(0, i) = coff[i];
        }

        LOG(INFO) << "Tensor input: " << inpTensor;

        std::vector<std::pair<std::string, Tensor>> input({{"dense_input/Tanh", inputTensor}});

        std::vector<Tensor> output;
        std::vector<std::string> output_names({"dense_output/BiasAdd"});

        tensorflow::Status s;
        s = session->Run(input, output_names, {}, &output);
        LOG(INFO) << "Session info " << s;

        Eigen::TensorMap<Eigen::Tensor<float, 1, Eigen::RowMajor>,Eigen::Aligned> out = (&output[0])->flat<float>();
        LOG(INFO) << "Out info " << out;

        float value = -1.5;
        if(s.ok()) value = static_cast<float >(out(0));
        return value;
    } else{
        return -1.1;
    }
}
