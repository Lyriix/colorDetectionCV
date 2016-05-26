#pragma once
#ifndef MULTIPLETRACKINGOBJECT_HPP
#define MULTIPLETRACKINGOBJECT_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Object.h"

#include "../interface/myWindow.hpp"
#include <QApplication>

//#include "opencv/highgui.h"
#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv/cv.h"
//#include "cv.h"
//#include "highgui.h"
#include "opencv2/core.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;



struct trackingObject
{
public:
    trackingObject();
    ~trackingObject();

    /** Création des barres pour la calibration des couleurs */
    void createTrackbars();
    /** Dessiner l'object couleur détecté et ses paramètres */
    void drawObject(vector<Object> theObjects, Mat &frame, Mat &temp, vector<vector<Point>> contours, vector<cv::Vec4i> hierarchy);
    void drawObject(vector<Object> theObjects, Mat &frame);
    /** Opération d'érosion et de dilatation */
    void morphOps(Mat &thresh);

    /** Suivi des mouvements des objects */
    void trackFilteredObject(Mat threshold, Mat HSV, Mat &cameraFeed);
    void trackFilteredObject(Object theObject, Mat threshold, Mat HSV, Mat &cameraFeed);

    /** Fonction global de traitement */
    void multipleTracking(Mat frame);

private :

    //names that will appear at the top of each window
    const string windowName ;
    const string windowName1 ;
    const string windowName2 ;
    const string windowName3 ;
    const string trackbarWindowName ;

    //The following for canny edge detec
    Mat dst, detected_edges;
    Mat src, src_gray;

    /** NOm donné à la fenetre d'affichage par la suite */
    char* window_name ;
    int lowThreshold;
    int const max_lowThreshold;

};

/** fonction inutile */
void on_trackbar();
string intToString(int number);



#endif
