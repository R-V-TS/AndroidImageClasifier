package com.example.rostislav.photoapp

import android.content.res.AssetManager
import android.graphics.Bitmap
import android.util.Log

class TensorflowInterface {

    public fun init(assetManager: AssetManager, model: String): Int {
        return initTF(assetManager, model)
    }

    public fun predict(bitmap: Bitmap): Double {
        return loadPredict(bitmap)
    }

    external fun initTF(assetManager: AssetManager, model: String): Int
    external fun loadPredict(bitmap: Bitmap): Double

    companion object {
        init {
            try {
                System.loadLibrary("tensorflow_demo")
            }catch (e: UnsatisfiedLinkError)
            {
                Log.d("TF", "Error")
            }
        }
    }
}