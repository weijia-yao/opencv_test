#include "stdio.h"
#include "string.h"
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv)
{
    CvCapture* pCapture = 0;
    IplImage* pVideoFrame = 0;
    int       i;
    char      filename[50];

    pCapture = cvCaptureFromCAM( CV_CAP_ANY );
    if(!pCapture)
    {
        fprintf(stderr, "Failed to initialize video capture \n");
        return -1;
    }

    for(i=0; i<3; i++)
    {
        pVideoFrame = cvQueryFrame( pCapture );
        if(!pVideoFrame)
        {
            fprintf(stderr, "Failed to get a video frame \n");
        }

        sprintf(filename, "VideoFrame%d.jpg", i+1);
        if( !cvSaveImage( filename, pVideoFrame) )
        {
            fprintf(stderr, "Failed to write image to file %s\n",filename);
        }
    }

    cvReleaseCapture( &pCapture );

    return 0;
}
