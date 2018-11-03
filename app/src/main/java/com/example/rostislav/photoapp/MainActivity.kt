package com.example.rostislav.photoapp

import android.app.AlertDialog
import android.app.Dialog
import android.content.DialogInterface
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*
import org.tensorflow.contrib.android.TensorFlowInferenceInterface


class MainActivity : AppCompatActivity() {

    private var array: DoubleArray? = null
    private var photoBitmap: Bitmap? = null
    private val coff = mutableListOf<Double>()
    private var tensorflowInterface: TensorFlowInferenceInterface? = null

    private val bitmaps = arrayOf("image.bmp", "moon.bmp", "trees.bmp")
    private var fileNumber = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        button_load.setOnClickListener {
            showDialog(1)
        }
        button_process.setOnClickListener {
            if(array == null)
                Toast.makeText(this, "Error! Image not loading", Toast.LENGTH_SHORT).show()
            else
            {
                textView.text = "Coefficient calculating..... Please wait"
                val k = findCoefficient(array!!, photoBitmap!!.height, photoBitmap!!.width).split(" ")
                for(i in 0..k.size-2)
                    coff.add(k[i].toDouble())
                var str: String = " "
                for(i in 0..3)
                    str += "freq $i: ${coff[i*4]} ${coff[i*4+1]} ${coff[i*4+2]} ${coff[i*4+3]} \n"
                textView.text = str
            }
        }
        button_predict.setOnClickListener {
            tensorflowInterface = TensorFlowInferenceInterface(assets, "tensorflow_lite_MLP_self.pb")
            val out = predicter()
            textView.text = out.toString()
        }
    }

    override fun onCreateDialog(id: Int): Dialog {
        val array = arrayOf("cameraman", "moon", "trees")

        val builder = AlertDialog.Builder(this)
        builder.setTitle("Select image")
        builder.setItems(array, object:DialogInterface.OnClickListener
        {
            override fun onClick(dialog: DialogInterface?, which: Int) {
                fileNumber = which
                photoLoader()
                bitmapShow()
            }
        })
        return builder.create()
    }

    private fun bitmapShow()
    {
        if(photoBitmap != null && array != null) {
            textView.text = "Images loading complete"
            imageView.setImageBitmap(photoBitmap)
        }
    }

    private fun photoLoader()
    {
        val inStream = assets.open(bitmaps[fileNumber])
        photoBitmap = BitmapFactory.decodeStream(inStream)

        //val ans = findKoff()
        val imgManipulator = ImageManipulator(photoBitmap!!)
        array = imgManipulator.transformImageToArray()
    }

    private fun predicter(): Double {
        val output: FloatArray = FloatArray(1)



        if(tensorflowInterface != null)
        {
            var inp = mutableListOf<Double>()
            for(i in 0..15)
                inp.add(mapminmax(coff[i], i))
            val input = inp.toDoubleArray()
            with(tensorflowInterface!!)
            {
                feed("dense_input/Tanh", input, 1.toLong(), input.size.toLong())
                run(arrayOf("dense_output/BiasAdd"))
                fetch("dense_output/BiasAdd", output)
            }
        }
        return mapminmax_back(output[0].toDouble(), 16)
    }


    external fun findCoefficient(image: DoubleArray, sizeX: Int, sizeY: Int): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}





