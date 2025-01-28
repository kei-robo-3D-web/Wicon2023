import bluepy
import cv2
import torch
import numpy as np
from picamera2 import Picamera2

HANDLE_R = 0x002d
HANDLE_L = 0x002d

devadrR = "68:67:25:ec:56:86"   # 実際にはmicro:bit のアドレスを記述
devadrL = "68:67:25:ee:de:4a"

xmi = 0
ymi = 0
xma = 0
yma = 0

picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"format": 'XRGB8888',"size":(640, 480)}))
picam2.start()

def main():
    model = torch.hub.load("//home/youthworks//yolov5", 'custom',path  = '/home/youthworks/yolov5/onebest.pt',source = 'local',device = 'cpu',force_reload = True)
    
    periR = bluepy.btle.Peripheral()
    periR.connect(devadrR, bluepy.btle.ADDR_TYPE_PUBLIC)
    
    periL = bluepy.btle.Peripheral()
    periL.connect(devadrL, bluepy.btle.ADDR_TYPE_PUBLIC)
    
    Rout = b'b'
    Lout = b'b'
    
    
    while True:  
        #periL.writeCharacteristic(HANDLE_L, b'a' )
        #periR.writeCharacteristic(HANDLE_R, b'a' )
        
        img = picam2.capture_array()
        result = model(img)
        key = cv2.waitKey(1)
        obj = result.pandas().xyxy[0]
        print(obj)
        result.render()
        
        count = len(obj)
        
        xmi = 0
        xma = 0
        ymi = 0
        yma = 0
        
        if count > 0:
            name = obj.name[0]
            xmi = obj.xmin[0]
            ymi = obj.ymin[0]
            xma = obj.xmax[0]
            yma = obj.ymax[0]
            
        objectx = (xmi + xma) / 2
        objecty = (ymi + yma) / 2
            
        if objectx >= 300 :
            Lout = b'a'
        else : 
            Lout = b'n'
        if objectx > 0 and objectx < 300 :
            Rout = b'a'
        else : 
            Rout = b'n'
        
        cv2.imshow("camera",result.ims[0])
        periR.writeCharacteristic(HANDLE_R, Rout )
        periL.writeCharacteristic(HANDLE_L, Lout )    
    
    
if __name__ == "__main__":
    main()
