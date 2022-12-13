# 一、项目简介

远程定格OBS虚拟摄像头，基于MQTT协议

# 二、协议

协议使用MQTT

规定：发送 `1` 为解冻摄像头，发送 `0` 为冻结摄像头

|Topic|作用|
|-----|----|
|/augtons/exam/camera/ctrl|ESP32发控制信号|
|/augtons/exam/camera/resp|PC端回传结果给ESP32|