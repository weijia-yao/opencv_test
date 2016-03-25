#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <string>

#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <opencv_test/FaceDetectionConfig.h>

using namespace std;
double      search_scale = 1;
int         min_neigh = 3;
bool        canny_enable = true;
int         min_img_size = 24;
string      filename;

// Dynamic reconfigure
void face_detection_callback(opencv_test::FaceDetectionConfig &config, uint32_t level)
{
    search_scale    = config.search_scale;
    min_neigh       = config.min_neigh;
    canny_enable    = config.canny_enable;
    min_img_size    = config.min_img_size;
    filename       = config.Classifier;
    ROS_INFO("Reconfigure Request: %f %d %s %d %s",
               search_scale ,
               min_neigh,
               canny_enable?"True":"False",
               min_img_size,
               filename.c_str());
}


int main(int argc, char** argv)
{

    ros::init(argc, argv, "face_detection");
    ros::Time::init();
    ros::Rate loop_rate(10);
    dynamic_reconfigure::Server<opencv_test::FaceDetectionConfig> server;
    dynamic_reconfigure::Server<opencv_test::FaceDetectionConfig>::CallbackType f;

    f = boost::bind(&face_detection_callback, _1, _2);
    server.setCallback(f);

    // declarations
    CvHaarClassifierCascade* pCascade = 0;         // the face detector
    CvMemStorage* pStorage = 0;                    // expandable memory buffer
    CvSeq* pFaceRectSeq;                           // list of detected faces
    int i;

    // initializations
    IplImage * pInpImg = (argc > 1) ? cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR) : 0;
    pStorage = cvCreateMemStorage(0);


    while(ros::ok())
    {
        pCascade = (CvHaarClassifierCascade *)cvLoad(filename.c_str());
        // validate that everything initialized properly
        if( !pInpImg || !pStorage || !pCascade )
        {
            printf("Initialization failed: %s \n",
                        (!pInpImg)?   "didn't load image file" :
                        (!pCascade)?  "didn't load Haar cascade -- "
                                      "make sure path is correct" :
                                      "failed to allocate memory for data storage");
            exit(-1);
        }

        // detect faces in image
        pFaceRectSeq = cvHaarDetectObjects
            (pInpImg, pCascade, pStorage,
            search_scale,                            // increase search scale by 10% each pass
            min_neigh,                              // drop groups of fewer than three detections
            canny_enable,       // skip regions unlikely to contain a face
            cvSize(min_img_size,min_img_size));

         // create a window to display detected faces
         cvNamedWindow("Haar Window", CV_WINDOW_AUTOSIZE);

         // draw a rectangular outline around each detection
         IplImage* pImgClone = cvCloneImage(pInpImg);
         for(i=0;i<(pFaceRectSeq? pFaceRectSeq->total:0); i++ )
         {
             CvRect * r = (CvRect*)cvGetSeqElem(pFaceRectSeq, i);
             CvPoint pt1 = { r->x, r->y };
             CvPoint pt2 = { r->x + r->width, r->y + r->height };
             cvRectangle(pImgClone, pt1, pt2, CV_RGB(0,255,0), 3, 4, 0);
         }

         ros::spinOnce();
         loop_rate.sleep();

         // display face detections
         cvShowImage("Haar Window", pImgClone);
         if( char(27)==cvWaitKey(1))
             break;
         cvReleaseImage(&pImgClone);
    }

     // clean up and release resources
     cvDestroyWindow("Haar Window");
     cvReleaseImage(&pInpImg);
     if(pCascade) cvReleaseHaarClassifierCascade(&pCascade);
     if(pStorage) cvReleaseMemStorage(&pStorage);

     return 0;
}

