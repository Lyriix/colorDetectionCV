import numpy as np
import cv2

green = np.uint8([[[0,255,0 ]]])
hsv_green = cv2.cvtColor(green,cv2.COLOR_BGR2HSV)
print"green"
print hsv_green

blue = np.uint8([[[255,0,0 ]]])
hsv_blue = cv2.cvtColor(blue,cv2.COLOR_BGR2HSV)
print"blue"
print hsv_blue

yellow = np.uint8([[[0,255,255 ]]])
hsv_yellow = cv2.cvtColor(yellow,cv2.COLOR_BGR2HSV)
print"yellow"
print hsv_yellow

orange = np.uint8([[[0,128,255 ]]])
hsv_orange = cv2.cvtColor(orange,cv2.COLOR_BGR2HSV)
print"orange"
print hsv_orange

violet = np.uint8([[[128,0,128 ]]])
hsv_violet = cv2.cvtColor(violet,cv2.COLOR_BGR2HSV)
print"violet"
print hsv_violet

mag = np.uint8([[[255,0,0 ]]])
hsv_mag = cv2.cvtColor(mag,cv2.COLOR_BGR2HSV)
print"mag"
print hsv_mag


cap = cv2.VideoCapture(0)
while(True):
	ret, frame  = cap.read()

	cv2.cvtColor(frame,cv2.COLOR_BGR2HSV,frame)
	cv2.imshow('frame',frame)
	print frame[0,0]
	if(cv2.waitKey(1) & 0xFF == ord('q')):
		break

cap.release()
cv2.destroyAllWindows()
