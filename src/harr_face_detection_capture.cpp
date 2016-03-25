#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include "string.h"

int main(int argc, char** argv)
{
    // declarations
    CvHaarClassifierCascade* pCascade = 0;         // the face detector
    CvMemStorage* pStorage = 0;                    // expandable memory buffer
    CvSeq* pFaceRectSeq;                           // list of detected faces
    int i;

    int index=0;
    char      filename[50];

    CvCapture* pCapture = 0;

    // initializations
    IplImage * pInpImg =0; //= (argc > 1) ? cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR) : 0;
    pStorage = cvCreateMemStorage(0);
    pCascade = (CvHaarClassifierCascade *)cvLoad
    ("/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml");
    // validate that everything initialized properly
    if( !pStorage || !pCascade )
    {
        printf("Initialization failed: %s \n",
                    (!pCascade)?  "didn't load Haar cascade -- "
                                  "make sure path is correct" :
                                  "failed to allocate memory for data storage");
        exit(-1);
    }

    pCapture = cvCaptureFromCAM( CV_CAP_ANY );
    if(!pCapture)
    {
        fprintf(stderr, "Failed to initialize video capture \n");
        return -1;
    }

    while(1)
    {
        pInpImg = cvQueryFrame( pCapture );
        if(!pInpImg)
        {
            fprintf(stderr, "Failed to get a video frame \n");
        }

        // detect faces in image
        pFaceRectSeq = cvHaarDetectObjects
            (pInpImg, pCascade, pStorage,
            1.1,                            // increase search scale by 10% each pass
            3,                              // drop groups of fewer than three detections
            CV_HAAR_DO_CANNY_PRUNING,       // skip regions unlikely to contain a face
            cvSize(0,0));

         // use XML default for smallest search scale
         // create a window to display detected faces
         cvNamedWindow("Haar Window", CV_WINDOW_AUTOSIZE);
         // draw a rectangular outline around each detection
         for(i=0;i<(pFaceRectSeq? pFaceRectSeq->total:0); i++ )
         {
             CvRect * r = (CvRect*)cvGetSeqElem(pFaceRectSeq, i);
             CvPoint pt1 = { r->x, r->y };
             CvPoint pt2 = { r->x + r->width, r->y + r->height };
             cvRectangle(pInpImg, pt1, pt2, CV_RGB(0,255,0), 3, 4, 0);
         }

         // display face detections
         cvShowImage("Haar Window", pInpImg);

         sprintf(filename, "VideoFrame%d.jpg", index+1);
         if( !cvSaveImage( filename, pInpImg) )
         {
             fprintf(stderr, "Failed to write image to file %s\n",filename);
         }
         index++;
    }
     cvReleaseCapture( &pCapture );
     cvWaitKey(0);
     cvDestroyWindow("Haar Window");
     // clean up and release resources
     cvReleaseImage(&pInpImg);
     if(pCascade) cvReleaseHaarClassifierCascade(&pCascade);
     if(pStorage) cvReleaseMemStorage(&pStorage);

     return 0;
}

