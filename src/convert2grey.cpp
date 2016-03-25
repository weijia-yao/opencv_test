#include "stdio.h"
#include "string.h"
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv)
{
    IplImage* pRGBImg = 0;
    IplImage* pGreyImg = 0;

    pRGBImg = cvLoadImage("input.jpg", CV_LOAD_IMAGE_UNCHANGED);
    if(!pRGBImg)
    {
        fprintf(stderr, "Failed to load image\n");
        return -1;
    }

    pGreyImg = cvCreateImage(cvSize(pRGBImg->width, pRGBImg->height), pRGBImg->depth, 1);
    cvCvtColor(pRGBImg, pGreyImg, CV_RGB2GRAY);
    if( !cvSaveImage("output_grey.jpg",pGreyImg) )
    {
        fprintf(stderr, "Failed to write image file\n");
    }

    cvReleaseImage(&pRGBImg);
    cvReleaseImage(&pGreyImg);

    return 0;
}
