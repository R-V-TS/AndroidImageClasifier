package com.example.rostislav.photoapp

import android.graphics.Bitmap

class ImageManipulator(val image: Bitmap)
{
    private var yMatrix: Array<DoubleArray>? = null
    fun getPixelRGB(x: Int, y: Int): IntArray
    {
        val color = image.getPixel(x, y)
        val arrayRGB = mutableListOf(0,0,0)
        arrayRGB[0] = color shr 16 and 0xff
        arrayRGB[1] = color shr 8 and 0xff
        arrayRGB[2] = color and 0xff
        return arrayRGB.toIntArray()
    }

    fun getYMatrix(): Array<DoubleArray>{
        val matrix = mutableListOf<DoubleArray>()
        lateinit var rgb: IntArray
        for(i in 0..image.height-1)
        {
            val array = mutableListOf<Double>()
            for(j in 0..image.width-1)
            {
                rgb = getPixelRGB(j, i)
                array.add((0.299*rgb[0])+(0.587*rgb[1]+(0.114*rgb[2])))
            }
            matrix.add(array.toDoubleArray())
        }
        yMatrix = matrix.toTypedArray()
        return yMatrix!!
    }

    fun transformImageToArray(): DoubleArray
    {
        if(yMatrix == null)
            getYMatrix()

        var arrayImg = mutableListOf<Double>()
        for(i in 0..image.height-1)
            for(j in 0..image.width-1)
                arrayImg.add(yMatrix!![i][j])
        return arrayImg.toDoubleArray()
    }
}