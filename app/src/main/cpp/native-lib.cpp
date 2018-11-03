#include <jni.h>
#include <string>
#include <sstream>
#include <iostream>
#include <array>

#include "dct2Dblind.cpp"


using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_rostislav_photoapp_MainActivity_findCoefficient(JNIEnv *env, jobject instance, jdoubleArray image, jint sizeX, jint sizeY) {

    jdouble *body = (env)->GetDoubleArrayElements(image, 0);

    double coff[16];
    dct2Dblind(body, sizeX, sizeY, &coff[0]);

    ostringstream os;

    for(int i = 0; i<16; i++)
    {
        os << coff[i];
        os << " ";
    }

    string str = os.str();

    return env->NewStringUTF(str.c_str());
}

