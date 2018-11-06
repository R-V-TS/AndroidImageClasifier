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

    var imgManipulator: ImageManipulator? = null
    private var array: IntArray? = null
    private var photoBitmap: Bitmap? = null
    private val coff = mutableListOf<Double>()

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
                val str = imgManipulator?.calcCoff(array!!, photoBitmap!!.height, photoBitmap!!.width)
                textView.text = str
            }
        }
        button_predict.setOnClickListener {
            val input = imgManipulator?.image_coff
            val tensorFlowInferenceInterface = TensorFlowInferenceInterface(assets, "tensorflow_lite_MLP_self.pb")
            tensorFlowInferenceInterface.feed("dense_input/Tanh", input, 1, 16)
            tensorFlowInferenceInterface.run(arrayOf("dense_output/BiasAdd"))
            val output = floatArrayOf(-1f)
            tensorFlowInferenceInterface.fetch("dense_output/BiasAdd", output)
            textView.text = output[0].toString()
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
        imgManipulator = ImageManipulator(photoBitmap!!)
        array = imgManipulator?.transformImageToArray()
    }
}





