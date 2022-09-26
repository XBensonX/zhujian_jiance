import cv2

_CAMERA_WIDTH = 640  #攝影機擷取影像寬度
_CAMERA_HEIGH = 480  #攝影機擷取影像高度

video_name = 'video_' #儲存影片名稱
file_type = '.avi' #儲存影片副檔名

cap = cv2.VideoCapture(0)
# 設定擷取影像的尺寸大小
cap.set(cv2.CAP_PROP_FRAME_WIDTH, _CAMERA_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, _CAMERA_HEIGH)

fourcc = cv2.VideoWriter_fourcc(*'XVID')
FPS = 30  #擷取影片頻率

write_flag = 0  #判斷是否為寫入模式
video_counter = 0  #計數儲存影片數量

while(cap.isOpened()):
    ret, frame = cap.read()
    frame=cv2.flip(frame,0)
    if ret == True:
        if cv2.waitKey(10) & 0xFF == ord('r') and write_flag == 0: # 寫入影格
            write_flag = 1
            save_name = video_name + str(video_counter) + file_type
            out = cv2.VideoWriter(save_name, fourcc, FPS, (_CAMERA_WIDTH, _CAMERA_HEIGH))
            print('writing to ' + save_name)
        elif cv2.waitKey(10) & 0xFF == ord('t') and write_flag == 1: #截止寫入
            write_flag = 0
            video_counter = video_counter + 1
            print('finish')
        elif cv2.waitKey(10) & 0xFF == ord('s'): # 結束
            cap.release()
            out.release()
            cv2.destroyAllWindows()
            break

        if (write_flag == 1):
            out.write(frame)

        cv2.imshow('frame',frame)
    else:
        break
