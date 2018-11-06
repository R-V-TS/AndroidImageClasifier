#include <jni.h>
#include <string>
#include <sstream>
#include <iostream>
#include <array>

#include "dct2Dblind.cpp"
#include "image_utils.cpp"

using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_rostislav_photoapp_ImageManipulator_findCoefficient(JNIEnv *env, jobject instance, jintArray image, jint sizeX, jint sizeY) {

    jint *body = (env)->GetIntArrayElements(image, 0);

    int size = sizeX*sizeY;
    double array[size];
    double coff[16];
    //{0.6387, 0.0754, -0.3374, 1.6208, 0.1740, 0.0158, 0.5732, 2.4450, 0.1217, 0.0127, 0.7794, 2.5839, 0.0656, 0.0047, 1.1797, 3.9341};

    rgb_to_y(body, &array[0], size);

    dct2Dblind(&array[0], sizeX, sizeY, &coff[0]);

    normalize_coefficient(&coff[0]);

    ostringstream os;

    for(int i = 0; i<16; i++)
    {
        os << coff[i];
        os << " ";
    }

    string str = os.str();

    return env->NewStringUTF(str.c_str());
}

