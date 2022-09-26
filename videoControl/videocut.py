import cv2


output_folder = 'C:/Users/jojo/Desktop/project/yolact/labelout/'
def get_images_from_video(video_name, time_F):
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

time_F = 1    #每過幾幀擷取一次
video_name = 'video_0.avi' #影片名稱
video_images = get_images_from_video(video_name, time_F) #讀取影片並轉成圖片

for i in range(0, len(video_images)): #擷取圖片並存取
    cv2.imshow('windows', video_images[i])
    cv2.waitKey(100)
    file_name = '{}{:08d}.jpg'.format(output_folder,i)
    cv2.imwrite(file_name, video_images[i])

    
#img=cv2.imread('images_'+str(i)+'_張',0)
print(i+1,'張')
#cv2.imwrite(video_images,video_images[i])    

cv2.destroyAllWindows
