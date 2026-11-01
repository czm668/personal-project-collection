#ifndef WIFI_H
#define WIFI_H
#include <Arduino.h>

// 天气数据结构体
typedef struct {
  String city;        // 城市名
  String text;        // 天气状况（晴/多云/雨等）
  String temperature; // 实时温度
  bool isSuccess;     // 是否获取成功
} WeatherData;

extern WeatherData weatherData;

// NTP 时间相关
extern bool ntpSynced;           // NTP 是否同步成功
extern unsigned long ntpSyncMillis; // 上次同步的时间戳

void initWiFi();
void checkWiFiConnection();      // WiFi 断线重连检查
void handleConfigLoop();         // AP配网循环处理
bool isAPMode();                 // 是否在AP配网模式
bool updateWeather();            // 天气更新函数
void initNTP();                  // 初始化 NTP 时间同步
String getFormattedTime();       // 获取格式化时间字符串 "HH:MM:SS"

// 百度语音合成
bool initBaiduTTS(const char* appId, const char* apiKey, const char* secretKey);
bool playTTS(const char* text);
void audioLoop();  // 音频循环处理

// Pushdeer 推送
void sendPushdeerAlert(String title, String content);

#endif
