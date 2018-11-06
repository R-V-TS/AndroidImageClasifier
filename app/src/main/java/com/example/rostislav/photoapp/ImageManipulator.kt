package com.example.rostislav.photoapp

import android.graphics.Bitmap
import android.provider.ContactsContract

class ImageManipulator(val image: Bitmap)
{
    val image_coff = floatArrayOf(0f,0f,0f,0f,0f,0f,0f,0f,0f,0f,0f,0f,0f,0f,0f,0f)
    fun transformImageToArray(): IntArray
    {
        var arrayImg = mutableListOf<Int>()
        for(i in 0..image.height-1)
            for(j in 0..image.width-1)
                arrayImg.add(image.getPixel(i,j))
        return arrayImg.toIntArray()
    }

    fun calcCoff(array: IntArray, height: Int, width: Int): String
    {
        val k = findCoefficient(array, height, width).split(" ")
        var str: String = " "
        for(i in 0..3) {
            image_coff[i * 4] = k[i * 4].toFloat()
            image_coff[i*4 + 1] = k[i*4 + 1].toFloat()
            image_coff[i*4 + 2] = k[i*4 + 2].toFloat()
            image_coff[i*4 + 3] = k[i*4 + 3].toFloat()
            str += "freq $i: ${k[i * 4]} ${k[i * 4 + 1]} ${k[i * 4 + 2]} ${k[i * 4 + 3]} \n"
        }
        return str
    }

    external fun findCoefficient(image: IntArray, sizeX: Int, sizeY: Int): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}