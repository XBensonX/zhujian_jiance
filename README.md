#  zhujian jiance內檔案功能
- video.py --影像錄製
- videocut.py --影像擷取
- picturesetting.py --圖片調整(用不到)
- eval image.txt --驗證執行命令
- train.txt --訓練執行命令
# zhujian jiance/yolact內檔案功能
- train.py --訓練
- eval.py --驗證
- labelme2coco --將label好的檔案轉成coco Dataset
- evalinput --放入要驗證的資料
- evaloutput --驗證完成後的資料
- labelout --訓練的資料&label完的資料
- weights --權重檔(訓練好的model)
# zhujian jiance/yolact/data內檔案功能
- config.py --相關參數設定
- coco --由labelme2coco轉好的Dataset存入(比較資料&訓練資料)


# test_data
- video_0 videocut.py擷取圖片
- coco放入 .yolact/data/
- evalinput放入 .yolact/
- evaloutput放入 .yolact/
- labelout放入 .yolact/
- weights放入 .yolact/
