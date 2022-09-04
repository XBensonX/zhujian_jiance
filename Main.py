import VideoOperation

import cv2
import os

# YOLACT config
YOLACT_MODEL_PATH = ".\\yolact\\weights\\yolact_base_98_6000.pth"
YOLACT_SCORE_THRESHOLD = 0.15
YOLACT_TOP_K = 15
YOLACT_INPUT_IMGS_PATH = ".\\yolact\\evalinput\\"
YOLACT_RESULT_IMGS_PATH = ".\\yolact\\evaloutput\\"

if __name__ == "__main__":

    video = VideoOperation.Video()

    # 1. 開始錄影
    video_name = video.Record()
    #video_name = "test_video.avi"

    # 2. 逐幀擷取
    FRAME_PRE_TIME = 1    #每過幾幀擷取一次
    video_images = video.GetImgsFromVideo(".\\videos\\" + video_name, FRAME_PRE_TIME) #讀取影片並轉成圖片

    frame_path = YOLACT_INPUT_IMGS_PATH + video_name.split(".")[0] + "\\"
    os.mkdir(frame_path) # 逐幀切割，並以影片名稱為資料夾名稱

    for i in range(0, len(video_images) - 1): # 擷取圖片並存取
        cv2.imshow("windows", video_images[i])
        cv2.waitKey(100)
        file_name = "{}{:08d}.jpg".format(frame_path, i)
        print(file_name)
        cv2.imwrite(file_name, video_images[i])
    cv2.destroyAllWindows()

    # 3. Yolact eval
    result_path = YOLACT_RESULT_IMGS_PATH + video_name.split(".")[0] + "\\"
    eval_command = "python ./yolact/eval.py --trained_model={} --score_threshold={} --top_k={} --images={}:{}".format(YOLACT_MODEL_PATH, YOLACT_SCORE_THRESHOLD, YOLACT_TOP_K, frame_path, result_path)
    os.system('cmd /c ' + eval_command)