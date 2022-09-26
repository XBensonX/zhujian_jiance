import cv2

_CAMERA_WIDTH = 640  #攝影機擷取影像寬度
_CAMERA_HEIGH = 480  #攝影機擷取影像高度

cap = cv2.VideoCapture(0)
# 設定擷取影像的尺寸大小
cap.set(cv2.CAP_PROP_FRAME_WIDTH, _CAMERA_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, _CAMERA_HEIGH)


i=0
while True:
	ret, frame = cap.read()
	frame=cv2.flip(frame,0)
	cv2.imshow('Video', frame)
	if cv2.waitKey(1) & 0xFF == ord('q'):
        	break;
	elif cv2.waitKey(1) & 0xFF == ord('s'): 
        	i = i + 1;
        	cv2.imwrite('photo' + str(i) + '.png',frame)
        	
video_capture.release()
cv2.destroyAllWindows()
