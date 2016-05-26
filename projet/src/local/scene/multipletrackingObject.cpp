#include "multipletrackingObject.hpp"

int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

void on_trackbar(int, void*)
{
    //This function gets called wenever a trackbar position is changed
}

string intToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}


void trackingObject::createTrackbars()
{

    //int H_MIN = 0;
    //int H_MAX = 256;
    //int S_MIN = 0;
    //int S_MAX = 256;
    //int V_MIN = 0;
    //int V_MAX = 256;

    //create window for trackbars
    namedWindow(trackbarWindowName,0);
    //create memory to store trackbar name on window
    char TrackbarName[50];
    sprintf( TrackbarName, "H_MIN", H_MIN);
    sprintf( TrackbarName, "H_MAX", H_MAX);
    sprintf( TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MAX);
    //create Trackbars and insert them into window
    //3 parameters are : the adress of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg.H_HIGH),
    //and the function that is called whenever the trackbar is moved(eg.on_trackbar)

    createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
    createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
    createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
    createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
    createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
    createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}


void trackingObject::drawObject(vector<Object> theObjects, Mat &frame, Mat &temp, vector<vector<Point>> contours, vector<cv::Vec4i> hierarchy)
{
    for(int i=0; i<theObjects.size(); i++)
    {
        cv::drawContours(frame, contours, i, theObjects.at(i).getColor(),3,8, hierarchy);
        cv::circle(frame,
                   cv::Point(theObjects.at(i).getXPos(),
                             theObjects.at(i).getYPos()),
                   5,
                   theObjects.at(i).getColor());
        cv::putText(frame,
                    intToString(theObjects.at(i).getXPos())+ " , " + intToString(theObjects.at(i).getYPos()),
                    cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos()+20),
                    1,
                    1,
                    theObjects.at(i).getColor());
        cv::putText(frame,
                    theObjects.at(i).getType(),
                    cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos()-20),
                    1,
                    2,
                    theObjects.at(i).getColor());
    }
}

void trackingObject::drawObject(vector<Object> theObjects, Mat &frame)
{
    for(int i=0; i<theObjects.size(); i++)
    {
        cv::circle(frame,
                   cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos()),
                   10,
                   cv::Scalar(0,0,255));
        cv::putText(frame,intToString(theObjects.at(i).getXPos())+ " , " + intToString(theObjects.at(i).getYPos()),cv::Point(theObjects.at(i).getXPos(),theObjects.at(i).getYPos()+20),1,1,Scalar(0,255,0));
        cv::putText(frame,theObjects.at(i).getType(),cv::Point(theObjects.at(i).getXPos(),theObjects.at(i).getYPos()-30),1,2,theObjects.at(i).getColor());
    }
}

//Create structuring element that will be used to "dilate and erode" image

void trackingObject::morphOps(Mat &thresh)
{
    //Element for morpho operations : Rectangle : 3 by 3 px
    Mat erodeElement = getStructuringElement(MORPH_RECT,cv::Size(3,3));
    //We dilate with a larger element so make sure object is nicel visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, cv::Size(8,8));

    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);
    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
}


void trackingObject::trackFilteredObject(Mat threshold, Mat HSV, Mat &cameraFeed)
{
    //max number of objects to be detected in frame
    const int MAX_NUM_OBJECTS=50;
    //default capture width and height
    const int FRAME_WIDTH = 640;
    const int FRAME_HEIGHT = 480;
    //minimum and maximum object area
    const int MIN_OBJECT_AREA = 20*20;
    const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

    std::vector<Object> objects;
    Mat temp;
    threshold.copyTo(temp);
    //On utilise ces vecteurs pour la sortie de findContours()
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    //trouver les contours de l'image filré en utilisant findContours fonction de l'image filtré
    cv::findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
    //utilisation de la méthode des moments pour trouver notre objet filtré
    double refArea = 0;
    double objectFound = false;
    if(hierarchy.size() > 0)
    {
        int numObjects = hierarchy.size();
        //si il y a plus d'objets que le MAX_NUM_OBJECTS on a un filtre bruité
        if(numObjects < MAX_NUM_OBJECTS)
        {
            for(int index = 0; index >= 0; index = hierarchy[index][0])
            {
                cv::Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;
                //si l'aire est inférieure à 20 px par 20 px c'est probablement du bruit
                //Si l'aire est equivalenete au 3/2 de l'image c'est probablement un mauvais filtre
                //On ne garde que les objects avec l'aire la plus large pour conserver une zone de recherche de réference
                //On itere et compare cette zone avec celle de la prochaine itération
                if(area > MIN_OBJECT_AREA)
                {
                    Object object;

                    object.setXPos(moment.m10/area);
                    object.setYPos(moment.m01/area);

                    objects.push_back(object);

                    objectFound = true;
                }
                else objectFound = false;
            }
            //Montrer à l'utilisateur qu'on à trouver un objet
            if(objectFound == true)
            {
                //on indique sa localisation à l'écran
                drawObject(objects, cameraFeed);
            }
        }
        else cv::putText(cameraFeed,"TOO MUCH NOISE, ADJUST FILTER", Point(0,50),1,2,Scalar(0,0,255),2);
    }
}

void trackingObject::trackFilteredObject(Object theObject, Mat threshold, Mat HSV, Mat &cameraFeed)
{

    //max number of objects to be detected in frame
    const int MAX_NUM_OBJECTS=50;
    //default capture width and height
    const int FRAME_WIDTH = 640;
    const int FRAME_HEIGHT = 480;
    //minimum and maximum object area
    const int MIN_OBJECT_AREA = 20*20;
    const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

    std::vector<Object> objects;
    Mat temp;
    threshold.copyTo(temp);
    //On utilise ces vecteurs pour la sortie de findContours()
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    //trouver les contours de l'image filré en utilisant findContours fonction de l'image filtré
    cv::findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
    //utilisation de la méthode des moments pour trouver notre objet filtré
    double refArea = 0;
    double objectFound = false;
    if(hierarchy.size() > 0)
    {
        int numObjects = hierarchy.size();
        //si il y a plus d'objets que le MAX_NUM_OBJECTS on a un filtre bruité
        if(numObjects < MAX_NUM_OBJECTS)
        {
            for(int index = 0; index >= 0; index = hierarchy[index][0])
            {
                cv::Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;
                //si l'aire est inférieure à 20 px par 20 px c'est probablement du bruit
                //Si l'aire est equivalenete au 3/2 de l'image c'est probablement un mauvais filtre
                //On ne garde que les objects avec l'aire la plus large pour conserver une zone de recherche de réference
                //On itere et compare cette zone avec celle de la prochaine itération
                if(area > MIN_OBJECT_AREA)
                {
                    Object object;

                    object.setXPos(moment.m10/area);
                    object.setYPos(moment.m01/area);
                    object.setType(theObject.getType());
                    object.setColor(theObject.getColor());

                    objects.push_back(object);

                    objectFound = true;
                }
                else objectFound = false;
            }
            //Montrer à l'utilisateur qu'on à trouver un objet
            if(objectFound == true)
            {
                //on indique sa localisation à l'écran
                drawObject(objects, cameraFeed,temp,contours,hierarchy);
            }
        }
        else cv::putText(cameraFeed,"TOO MUCH NOISE, ADJUST FILTER", Point(0,50),1,2,Scalar(0,0,255),2);
    }
}


void trackingObject::multipleTracking(cv::Mat frame) //frame image issue de la caméra
{
    bool calibration_mode = false;
    Mat cameraFeed;
    Mat threshold;
    Mat HSV;


    if(calibration_mode)
    {
        createTrackbars();
    }
    cameraFeed = frame ;
    src =cameraFeed ;

    if(! src.data) {
        std::cout<<"Erreur durant la lecture de frame pendant le tracking"<<std::endl;
        exit(1);
    }
    //convertir de RGB (BGR) à l'espace couleur HSV (Hue Saturation Value)
    cv::cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
    if(calibration_mode==true)
    {
        //Calibration mode must be false
        //if true, we track objects based on the HSV Slider
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,cv::Scalar(H_MIN,S_MIN,V_MIN),cv::Scalar(H_MAX,S_MAX,V_MAX),threshold);
        morphOps(threshold);
        cv::imshow(windowName2,threshold);
        //Pour la detection des bords
        dst.create(src.size(), src.type());
        //Conversion en NG
        cv::cvtColor(src,src_gray,CV_BGR2GRAY);
        //creation de fenetre
        cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
        //Création d'une trackbar pour l'utilisateur pour entrer le seuil
      //  createTrackbar("Min Threshold:",window_name,&lowThreshold,max_lowThreshold);
        //Affichage
        trackFilteredObject(threshold, HSV, cameraFeed);
    }
    else
    {
        // On crée des objects temporaires pour utiliser leurs fonctions membres et infos
        Object magenta("magenta"), yellow("yellow"), violet("violet"), green("green"), cyan("cyan"), orange("orange");


        //Recherche objects jaunes
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,yellow.getHSVmin(),yellow.getHSVmax(),threshold);
        morphOps(threshold);
        trackFilteredObject(yellow,threshold,HSV,cameraFeed);

        //Recherche objects vert
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,green.getHSVmin(),green.getHSVmax(),threshold);
        morphOps(threshold);
        trackFilteredObject(green,threshold,HSV,cameraFeed);

        //Recherche objects cyan
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,cyan.getHSVmin(),cyan.getHSVmax(),threshold);
        morphOps(threshold);
        trackFilteredObject(cyan,threshold,HSV,cameraFeed);

        //Recherche objects orange
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,orange.getHSVmin(),orange.getHSVmax(),threshold);
        morphOps(threshold);
        trackFilteredObject(orange,threshold,HSV,cameraFeed);

        //Recherche objects violet
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,violet.getHSVmin(),violet.getHSVmax(),threshold);
        morphOps(threshold);
        trackFilteredObject(violet,threshold,HSV,cameraFeed);

        //Recherche objects magenta
        cv::cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
        cv::inRange(HSV,magenta.getHSVmin(),magenta.getHSVmax(),threshold);
        morphOps(threshold);
        trackFilteredObject(magenta,threshold,HSV,cameraFeed);


    }
    //show frame
    imshow(windowName,cameraFeed);
}


trackingObject::trackingObject():
    windowName("Original Image"),
    windowName1("HSV Image"),
    windowName2("Threshold Image"),
    windowName3("After Morpho Ops"),
    trackbarWindowName("Trackbars"),
    window_name("Edge_Map"),
    max_lowThreshold(255)
{

}

trackingObject::~trackingObject(){ }

















