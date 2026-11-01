#include <Arduino.h>
#include "SmartHomeHardware.h"
#include "wifi.h"
#include "web_server.h"

unsigned long timer = 0;
unsigned long weatherTimer = 0;
bool lastAutoMode = false;  // 记录上一次自动模式状态
bool weatherReportedToday = false;  // 今天是否已播报天气
int lastReportedDay = -1;  // 上次播报的日期

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== 系统启动 ===");

  initHardware();
  initWiFi();
  initNTP();        // 初始化 NTP 时间同步

  // 初始化蜂鸣器提示音
  initBaiduTTS("", "", "");
  playTTS("智能家居系统启动成功");

  initWebServer();

  updateWeather();

  // 初始化 PIR 人体感应
  initPIR();

  Serial.println("=== 系统启动完成 ===");
}

void loop() {
  // AP配网模式优先处理
  handleConfigLoop();

  // AP模式下不运行主Web服务器（避免端口冲突）
  if (!isAPMode()) {
    webServerLoop();
  }
  audioLoop();  // 音频循环处理

  // WiFi 断线重连检查
  checkWiFiConnection();

  // PIR 测试代码 - 打印 GPIO 2 状态
  static unsigned long pirTestTimer = 0;
  if (millis() - pirTestTimer >= 500) {
    pirTestTimer = millis();
    int pirState = digitalRead(2);
    Serial.print("[PIR测试] GPIO 2 状态: ");
    Serial.println(pirState);
  }

  // 防盗检测（每 500ms 检查一次）
  static unsigned long securityTimer = 0;
  if (millis() - securityTimer >= 500) {
    securityTimer = millis();
    checkSecurity();
  }

  if (millis() - timer >= 2000) {
    timer = millis();
    readDHT();
    OLED_show();

    checkTempAlarm();  // 始终检查，非自动模式时自动关闭蜂鸣器

    // 检测自动模式切换：从自动→手动时，关闭所有设备
    if (lastAutoMode && !autoMode) {
      if (fanState) {
        ledcDetachPin(FAN_PIN);
        pinMode(FAN_PIN, INPUT);
        fanState = false;
      }
      if (ledState) {
        digitalWrite(LED_PIN, LOW);
        ledState = false;
      }
      buzzerAlarm(false);
      Serial.println("[模式切换] 自动→手动，已关闭所有设备");
    }
    lastAutoMode = autoMode;

    if (autoMode && doorIsOpen) {
      checkAutoLEDByWeather();  // 根据天气和时间段自动开关LED

      // 获取室外温度（天气API返回的是字符串，转为整数）
      int outdoorTemp = weatherData.isSuccess ? weatherData.temperature.toInt() : 0;

      // 风扇自动逻辑：室内>室外 或 室内>30度
      bool needFan = (currentTemp > outdoorTemp) || (currentTemp > 30);

      Serial.print("[自动] 室内:");
      Serial.print(currentTemp);
      Serial.print("℃ 室外:");
      Serial.print(outdoorTemp);
      Serial.print("℃ 风扇:");
      Serial.print(fanState ? "开" : "关");
      Serial.print(" 需要风扇:");
      Serial.println(needFan ? "是" : "否");

      if (needFan && !fanState) {
        ledcAttachPin(FAN_PIN, FAN_CH);
        ledcWrite(FAN_CH, 255);
        fanState = true;
        Serial.println("[自动] 风扇已启动");
      } else if (!needFan && fanState) {
        ledcDetachPin(FAN_PIN);
        pinMode(FAN_PIN, INPUT);
        fanState = false;
        Serial.println("[自动] 风扇已关闭");
      }
    }
  }

  if (millis() - weatherTimer >= 60 * 60 * 1000) {
    weatherTimer = millis();
    Serial.println("--- 开始更新天气数据 ---");
    updateWeather();
  }

  // 每天早上 8 点播报天气
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentDay = timeinfo.tm_mday;
    int currentHour = timeinfo.tm_hour;

    // 新的一天，重置播报标志
    if (currentDay != lastReportedDay) {
      weatherReportedToday = false;
      lastReportedDay = currentDay;
    }

    // 早上 8 点且未播报过，播报天气
    if (currentHour == 8 && !weatherReportedToday && weatherData.isSuccess) {
      weatherReportedToday = true;

      // 构建天气播报文本
      String weatherText = "早上好，今日" + weatherData.text + "，气温" + weatherData.temperature + "摄氏度";
      Serial.print("[语音播报] ");
      Serial.println(weatherText);
      playTTS(weatherText.c_str());
    }
  }
}
