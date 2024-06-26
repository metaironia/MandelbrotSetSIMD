#include "../lib/TXLib.h"

#include <assert.h>
#include <math.h>

#include "mandelbrot_computation.h"
#include "graphics.h"
#include "dsl.h"

GraphicsFunc MandelbrotDraw (void) {

    txCreateWindow (WINDOW_SIZE_X, WINDOW_SIZE_Y);

    RGBQUAD *vmem_buffer = txVideoMemory();
    assert (vmem_buffer);

    Win32::_fpreset();

    ComputationConfig config = {};
    ConfigCtor (&config);

#ifdef BENCHMARK
        FILE *benchmark_results = fopen ("benchmark_results.txt", "w");

        RdtscTest (benchmark_results);

        MandelbrotComputeSillyNoSIMD (vmem_buffer, &config);
        BENCHMARK_RES_ ("silly no SIMD computations");

        MandelbrotComputeSensibleNoSIMD (vmem_buffer, &config);
        BENCHMARK_RES_ ("sensible no SIMD computations");

        MandelbrotComputeSIMD (vmem_buffer, &config);
        BENCHMARK_RES_ ("SIMD computations");

        fclose (benchmark_results);
        benchmark_results = NULL;
#endif

#ifndef BENCHMARK
    while (!txGetAsyncKeyState (VK_ESCAPE)) {
        
        ControlButtonPressCheck (&config);

        txBegin();

        MandelbrotComputeSIMD (vmem_buffer, &config);

        txEnd();
    }
#endif

    ConfigDtor (&config);

    return GRAPHICS_FUNC_STATUS_OK;
}

GraphicsFunc ControlButtonPressCheck (ComputationConfig *config) {

    assert (config);

    if (txGetAsyncKeyState (VK_LEFT)) {

        NUM_OFFSET_AXIS_X_ -= NUM_DELTA_X_ * 10;
        INTR_OFFSET_AXIS_X_ = _mm256_sub_pd (INTR_OFFSET_AXIS_X_, _mm256_mul_pd (INTR_DELTA_X_, INTR_CONST_TEN));
    }

    if (txGetAsyncKeyState (VK_RIGHT)) {

        NUM_OFFSET_AXIS_X_ += NUM_DELTA_X_ * 10;
        INTR_OFFSET_AXIS_X_ = _mm256_add_pd (INTR_OFFSET_AXIS_X_, _mm256_mul_pd (INTR_DELTA_X_, INTR_CONST_TEN));
    }

    if (txGetAsyncKeyState (VK_UP)) {

        NUM_OFFSET_AXIS_Y_ += NUM_DELTA_Y_ * 10;
        INTR_OFFSET_AXIS_Y_ = _mm256_add_pd (INTR_OFFSET_AXIS_Y_, _mm256_mul_pd (INTR_DELTA_Y_, INTR_CONST_TEN));
    }

    if (txGetAsyncKeyState (VK_DOWN)) {

        NUM_OFFSET_AXIS_Y_ -= NUM_DELTA_Y_ * 10;
        INTR_OFFSET_AXIS_Y_ = _mm256_sub_pd (INTR_OFFSET_AXIS_Y_, _mm256_mul_pd (INTR_DELTA_Y_, INTR_CONST_TEN));
    }

    if (txGetAsyncKeyState (VK_F1)) {

        NUM_DELTA_Y_ *= 2;
        INTR_DELTA_Y_ = _mm256_mul_pd (INTR_DELTA_Y_, INTR_CONST_TWO); 

        NUM_DELTA_X_ *= 2;
        INTR_DELTA_X_ = _mm256_mul_pd (INTR_DELTA_X_, INTR_CONST_TWO);

        NUM_STEP_X_ *= 2;
        INTR_STEP_X_ = _mm256_mul_pd (INTR_STEP_X_, INTR_CONST_TWO);
    }

    if (txGetAsyncKeyState (VK_F2)) {

        NUM_DELTA_Y_ /= 2;
        INTR_DELTA_Y_ = _mm256_div_pd (INTR_DELTA_Y_, INTR_CONST_TWO); 

        NUM_DELTA_X_ /= 2;
        INTR_DELTA_X_ = _mm256_div_pd (INTR_DELTA_X_, INTR_CONST_TWO);

        NUM_STEP_X_ /= 2;
        INTR_STEP_X_ = _mm256_div_pd (INTR_STEP_X_, INTR_CONST_TWO);
    }

    return GRAPHICS_FUNC_STATUS_OK;
}

GraphicsFunc PixelColorSet (      RGBQUAD *videomem, const size_t pixel_x, 
                            const size_t   pixel_y,  const size_t iter_num) {

    assert (videomem);

    size_t   curr_pixel_pos = (pixel_y) * WINDOW_SIZE_X + pixel_x;
    RGBQUAD *current_pixel  = videomem + curr_pixel_pos;

    if (iter_num >= MAX_COMPUTATION_NUM) {

        *current_pixel = {};
        return GRAPHICS_FUNC_STATUS_OK;
    }

    current_pixel -> rgbBlue  = (BYTE) (sqrt (iter_num) * 100); 
    current_pixel -> rgbGreen = (BYTE) (sqrt (sqrt (iter_num)) * 256);
    current_pixel -> rgbRed   = (BYTE) (iter_num * iter_num * iter_num); 

    return GRAPHICS_FUNC_STATUS_OK;
}