#include "cv.h"
#include "highgui.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    IplImage* pImg = 0;

    // Load am image from file
    pImg = cvLoadImage("input.jpg", CV_LOAD_IMAGE_UNCHANGED);
    if(!pImg)
    {
        fprintf(stderr, "Failed to load image \n");
        return -1;
    }

    // Write image to a file using a different format
    if( !cvSaveImage("output.png", pImg) )
    {
        fprintf(stderr, "Failed to write image file \n");
    }

    // Free image memory
    cvReleaseImage(&pImg);

    return 0;
}
