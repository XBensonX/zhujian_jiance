import cv2
from datetime import datetime
import os

_CAMERA_WIDTH = 640  #攝影機擷取影像寬度
_CAMERA_HEIGH = 480  #攝影機擷取影像高度

video_dir = ".\\videos\\"
video_name = "video_" #儲存影片名稱前綴
file_type = ".avi" #儲存影片副檔名

cap = cv2.VideoCapture(0) #取得相機

# 設定擷取影像的尺寸大小
cap.set(cv2.CAP_PROP_FRAME_WIDTH, _CAMERA_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, _CAMERA_HEIGH)

fourcc = cv2.VideoWriter_fourcc(*"XVID")
FPS = 30  #擷取影片頻率

class Video:
    def __init__(self):
        if not os.path.exists(video_dir): # 如果路徑不存在
            os.mkdir(video_dir) # 創建資料夾

    def Record(self): # 錄影
        isWrite = False
        while(cap.isOpened()):
            ret, frame = cap.read()
            if ret == True:
                if cv2.waitKey(10) & 0xFF == ord("r"): # 寫入影格
                    isWrite = True
                    now = datetime.now()
                    saved_name = video_name + now.strftime("%Y%m%d_%H%M%S") + file_type # 依照檔名取名
                    out = cv2.VideoWriter(video_dir + saved_name, fourcc, FPS, (_CAMERA_WIDTH, _CAMERA_HEIGH))
                    print("writing to " + saved_name)
                elif cv2.waitKey(10) & 0xFF == ord("s"): # 結束並截止寫入
                    isWrite = False
                    print("finish")
                    cap.release()
                    out.release()
                    cv2.destroyAllWindows()
                    break

                if isWrite:
                    out.write(frame)

                cv2.imshow("frame", frame)
            else:
                break
        return saved_name

    def GetImgsFromVideo(self, video_name, time_F): # 逐幀切割
        video_images = []
        vc = cv2.VideoCapture(video_name)
        
        c = 1
        
        if vc.isOpened(): #是否開啟
            rval, video_frame = vc.read()
        else:
            rval = False

        while rval:   #至結束
            rval, video_frame = vc.read()
            
            if(c % time_F == 0): #幀數擷取
                video_images.append(video_frame)     
            c = c + 1
        vc.release()
        
        return video_images