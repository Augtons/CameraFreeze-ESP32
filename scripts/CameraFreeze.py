# -*- coding: utf-8 -*-
import threading
import time
import cv2
import pyvirtualcam
from paho.mqtt import client as mqtt_client

cap: cv2.VideoCapture = cv2.VideoCapture(0)

w, h = cap.get(cv2.CAP_PROP_FRAME_WIDTH), cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
frame = cv2.cvtColor(cap.read()[1], cv2.COLOR_BGR2RGB)
status = 1

id = f"python-mqtt-{time.time}"
broker = "broker-cn.emqx.io"
port = 1883

topic_ctrl = "/augtons/exam/camera/ctrl"
topic_resp = "/augtons/exam/camera/resp"

mqttclient = mqtt_client.Client(id)

def get_frame():
    global frame, status
    while True:
        if status:
            frame = cv2.cvtColor(cap.read()[1], cv2.COLOR_BGR2RGB)
            time.sleep(0.025)
        else:
            time.sleep(0.05)

def display():
    global frame, w, h

    with pyvirtualcam.Camera(width=int(w), height=int(h), fps=20) as cam:
        while True:
            cam.send(frame)
            time.sleep(0.025)

def onConnect(client: mqtt_client, userdata, flags, rc):
    print(f"连接成功, 已连接到：{broker}:{port}")
    mqttclient.subscribe(topic_ctrl)
    print(f"成功订阅到：{topic_ctrl}")

def onMessage(client, userdata, msg):
    global status
    data: str = msg.payload.decode()
    print(f"接收到：{data}")
    try:
        status = int(data) #1 拍； 2定
        if (status == 0):
            mqttclient.publish(topic_resp, "0".encode('UTF8'))
        else:
            mqttclient.publish(topic_resp, "1".encode('UTF8'))
    except:
        print(f"{data}不是数字")
        pass

def receiveMqttCtrl():
    print("链接")
    mqttclient.on_connect = onConnect
    mqttclient.on_message = onMessage
    mqttclient.connect_async(broker, port=port)
    mqttclient.loop_start()
    while True:
        pass

def switchByEnter():
    global status
    while True:
        input(f"Press Enter to switch, now({status})")
        status = not status


if __name__ == '__main__':
    threading.Thread(target=switchByEnter).start()
    threading.Thread(target=get_frame).start()
    threading.Thread(target=display).start()

    threading.Thread(target=receiveMqttCtrl).start()
