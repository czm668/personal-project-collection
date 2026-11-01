#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include <Arduino.h>

// 只声明函数，不包含任何WebServer相关内容
void initWebServer();
void webServerLoop(); // 新增：封装handleClient()

#endif