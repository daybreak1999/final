# final

在openmv上運作final.py獲取line和apriltag的資訊，透過uart傳給mbed，mbed透過line和apriltag的資訊，首先執行line tracking，在看到apriltag之後調整朝向apriltag直行，同時透過ping的回傳值判斷距離，在前方近處有物體時停下。
