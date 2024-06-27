import subprocess
import serial
import os
import time
arduino=serial.Serial(port='/dev/ttyACM0',baudrate=9600,timeout=.1)
def get_data():
    result=subprocess.run(['python3.12','issInfo.py'],capture_output=True,text=True)
    float1,float2=map(float,result.stdout.strip().split())
    return float1,float2
def send_data(ser,float1,float2):
    #ser=serial.Serial('/dev/ttyUSB0',9600)
    time.sleep(2)
    data=f"{float1} {float2}\n"
    ser.write(data.encode())
    ser.flush()
while True:
    float1,float2=get_data()
    send_data(arduino,float1,float2)
    time.sleep(10)
