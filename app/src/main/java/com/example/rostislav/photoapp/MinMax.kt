package com.example.rostislav.photoapp

val maximus = arrayOf<Double>(0.943, 0.0911, 2.87, 24.57, 0.33, 0.045, 2.769, 14.091, 0.2812, 0.0173, 9.705, 204.841, 0.154, 0.0083, 18.811, 757.186, 0.437757)
val minimus = arrayOf<Double>(0.2367, 0.0024, -3.638, 1.38, 0.0492, 0.0018, -0.4913, 1.966, 0.00632, 6.727, -0.5408, 1.8566, 0.0014, 6.6458, 0.1879, 2.4729, -0.2384)

fun mapminmax(value: Double, i: Int): Double = (((1 - (-1)) * (value - minimus[i]))/(maximus[i] - minimus[i]) + (-1))

fun mapminmax_back(value: Double, i: Int): Double = (((value - (-1))*(maximus[i] - minimus[i]))/(1 - (-1)) + minimus[i])