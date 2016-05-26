#include "Object.h"

Object::Object()
{
	//set values for default constructor
	setType("Object");
	setColor(Scalar(0,0,0));

}

Object::Object(string name){

	setType(name);
	
	if(name=="blue"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

        //setHSVmin(Scalar(92,0,0));
        //setHSVmax(Scalar(124,256,256));
        setHSVmin(Scalar(100,200,200));
        setHSVmax(Scalar(140,255,255));

        //BGR value for Blue:
		setColor(Scalar(255,0,0));

	}
	if(name=="green"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

        //setHSVmin(Scalar(34,50,50));
        //setHSVmax(Scalar(80,220,200));
        setHSVmin(Scalar(40,100,100));
        setHSVmax(Scalar(80,200,200));
        //BGR value for green:
		setColor(Scalar(0,255,0));

	}
	if(name=="yellow"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

        //setHSVmin(Scalar(20,124,123));
        //setHSVmax(Scalar(30,256,256));
        setHSVmin(Scalar(20,100,123));
        setHSVmax(Scalar(40,200,256));
        //BGR value for yellow:
		setColor(Scalar(0,255,255));

	}

    if(name == "cyan")
    {

        //setHSVmin(Scalar(175,80,80));
        //setHSVmax(Scalar(185,100,100));
        setHSVmin(Scalar(90,190,123));
        setHSVmax(Scalar(110,256,256));

        setColor(Scalar(255,255,0));
    }

    if(name=="orange"){

        //TODO: use "calibration mode" to find HSV min
        //and HSV max values

        //setHSVmin(Scalar(20,124,123));
        //setHSVmax(Scalar(30,256,256));
        setHSVmin(Scalar(0,113,147));
        setHSVmax(Scalar(25,195,239));
        //BGR value for yellow:
        setColor(Scalar(0,128,255));

    }

    if(name == "magenta")
    {
        setHSVmin(Scalar(152,120,189));
        setHSVmax(Scalar(195,162,256));
        setColor(Scalar(255,0,255));
    }
   if(name == "violet")
    {
        setHSVmin(Scalar(148,42,104));
        setHSVmax(Scalar(168,162,200));
        setColor(Scalar(128,0,128));
    }
}

Object::~Object(void)
{
}

int Object::getXPos(){

	return Object::xPos;

}

void Object::setXPos(int x){

	Object::xPos = x;

}

int Object::getYPos(){

	return Object::yPos;

}

void Object::setYPos(int y){

	Object::yPos = y;

}

Scalar Object::getHSVmin(){

	return Object::HSVmin;

}
Scalar Object::getHSVmax(){

	return Object::HSVmax;
}

void Object::setHSVmin(Scalar min){

	Object::HSVmin = min;
}


void Object::setHSVmax(Scalar max){

	Object::HSVmax = max;
}
