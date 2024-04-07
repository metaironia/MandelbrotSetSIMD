#include "../lib/TXLib.h"

#include <assert.h>
#include <math.h>

#include "mandelbrot_computation.h"
#include "graphics.h"

GraphicsFunc MandelbrotDraw (void) {

    txCreateWindow (WINDOW_SIZE_X, WINDOW_SIZE_Y);

    RGBQUAD *vmem_buffer = txVideoMemory();
    assert (vmem_buffer);

    Win32::_fpreset();

    ComputationConfig config = {};
    ConfigCtor (&config);

    if (1) {
        /* if (txGetAsyncKeyState (VK_LEFT)) {


        }

        if (txGetAsyncKeyState (VK_RIGHT))
        if (txGetAsyncKeyState (VK_UP))
        if (txGetAsyncKeyState (VK_DOWN)) */

        txBegin();

        MandelbrotComputeSIMD (vmem_buffer, &config);

        txEnd();
    }

    ConfigDtor (&config);

    return GRAPHICS_FUNC_STATUS_OK;
}

GraphicsFunc PixelColorSet (RGBQUAD *videomem,       size_t pixel_x, 
                            size_t   pixel_y,  const size_t iter_num) {

    assert (videomem);

    size_t   curr_pixel_pos = (pixel_y) * WINDOW_SIZE_X + pixel_x;
    RGBQUAD *current_pixel  = videomem + curr_pixel_pos;

    if (iter_num >= MAX_COMPUTATION_NUM) {

        *current_pixel = {};
        return GRAPHICS_FUNC_STATUS_OK;
    }

//    current_pixel -> rgbBlue  = (BYTE) (sqrt (iter_num) * 100); 
    current_pixel -> rgbGreen = (BYTE) (sqrt (sqrt (iter_num)) * 256);
//    current_pixel -> rgbRed   = (BYTE) (iter_num * iter_num * iter_num); 

    return GRAPHICS_FUNC_STATUS_OK;
}