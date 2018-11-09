package com.example.rostislav.photoapp

import android.app.AlertDialog
import android.app.Dialog
import android.content.DialogInterface
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

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

        button_predict.setOnClickListener {
           val tf = TensorflowInterface()
            textView.text = tf.init(assets, "file:///android_asset/tf.pb").toString()
            textView.text = tf.predict(photoBitmap!!).toString()
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
            }
        })

        
        return builder.create()
    }

    private fun bitmapShow()
    {
        if(photoBitmap != null) {
            textView.text = "Images loading complete"
            imageView.setImageBitmap(photoBitmap)
        }
    }

    private fun photoLoader()
    {
        val inStream = assets.open(bitmaps[fileNumber])
        photoBitmap = BitmapFactory.decodeStream(inStream)

        bitmapShow()
    }


}





