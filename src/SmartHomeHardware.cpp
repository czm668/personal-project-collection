#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "SmartHomeHardware.h"
#include "wifi.h"

bool ledState = false;
bool fanState = false;
bool doorIsOpen = false;
bool autoMode = false;
float currentTemp = 0;
float currentHum = 0;
bool servoIsBusy = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void initOLED() {
  u8g2.begin();
  u8g2.enableUTF8Print();  // 启用UTF-8中文支持
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);  // 中文字体
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("系统就绪");
  u8g2.sendBuffer();
  Serial.println("OLED初始化完成");
}

void OLED_show() {
  if (servoIsBusy) return;

  u8g2.clearBuffer();

  // 第一行：时间（右对齐）
  String timeStr = getFormattedTime();
  int timeWidth = timeStr.length() * 6;
  u8g2.setCursor(128 - timeWidth, 12);
  u8g2.print(timeStr);

  // 第二行：室内温湿度
  u8g2.setCursor(0, 28);
  u8g2.print("室内:");
  u8g2.print((int)currentTemp);
  u8g2.print("℃ ");
  u8g2.print((int)currentHum);
  u8g2.print("%");

  // 第三行：室外天气
  u8g2.setCursor(0, 44);
  if (weatherData.isSuccess) {
    u8g2.print("室外:");
    u8g2.print(weatherData.temperature);
    u8g2.print("℃ ");
    u8g2.print(weatherData.text);
  } else {
    u8g2.print("室外: 获取失败");
  }

  // 第四行：门状态和自动模式
  u8g2.setCursor(0, 60);
  u8g2.print(doorIsOpen ? "门:开 " : "门:关 ");
  u8g2.print(autoMode ? "自动:开" : "自动:关");

  u8g2.sendBuffer();
}

bool readDHT() {
  if (servoIsBusy) return false;
  uint8_t data[5] = {0};
  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(30);
  digitalWrite(DHT_PIN, HIGH);
  delayMicroseconds(60);
  pinMode(DHT_PIN, INPUT_PULLUP);
  delayMicroseconds(10);
  unsigned long start = millis();
  while (digitalRead(DHT_PIN) == HIGH) {
    if (millis() - start > 10) return false;
  }
  start = millis();
  while (digitalRead(DHT_PIN) == LOW) {
    if (millis() - start > 10) return false;
  }
  start = millis();
  while (digitalRead(DHT_PIN) == HIGH) {
    if (millis() - start > 10) return false;
  }
  for (int i=0;i<5;i++) {
    uint8_t byte = 0;
    for (int b=0;b<8;b++) {
      start = millis();
      while (digitalRead(DHT_PIN) == LOW) if (millis()-start>10) return false;
      delayMicroseconds(50);
      if (digitalRead(DHT_PIN) == HIGH) byte |= (1 << (7-b));
      start = millis();
      while (digitalRead(DHT_PIN) == HIGH) if (millis()-start>10) return false;
    }
    data[i] = byte;
  }
  if (data[0]+data[1]+data[2]+data[3] != data[4]) return false;
  currentHum = data[0];
  currentTemp = data[2];
  return true;
}

void openDoor() {
  if (servoIsBusy || doorIsOpen) return;
  servoIsBusy = true;
  doorIsOpen = true;
  ledcSetup(SERVO_CH, SERVO_FREQ, SERVO_RES);
  ledcAttachPin(SERVO_PIN, SERVO_CH);
  delay(10);
  // 测试：延长延迟时间
  ledcWrite(SERVO_CH, 125);
  delay(800);  // 调回 800ms
  ledcDetachPin(SERVO_PIN);
  Serial.println("✅ 门已打开");
  // 播放欢迎语音
  playTTS("欢迎回家");
  servoIsBusy = false;
}

void closeDoor() {
  if (servoIsBusy || !doorIsOpen) return;
  servoIsBusy = true;
  doorIsOpen = false;
  ledcSetup(SERVO_CH, SERVO_FREQ, SERVO_RES);
  ledcAttachPin(SERVO_PIN, SERVO_CH);
  delay(10);
  // 测试：延长延迟时间
  ledcWrite(SERVO_CH, 26);
  delay(800);  // 调回 800ms
  ledcDetachPin(SERVO_PIN);
  digitalWrite(LED_PIN, LOW);
  ledcDetachPin(FAN_PIN);
  pinMode(FAN_PIN, INPUT);
  buzzerAlarm(false);
  ledState = false;
  fanState = false;
  Serial.println("✅ 门已关闭");
  // 播放告别语音
  playTTS("一路顺风");
  servoIsBusy = false;
}

void initHardware() {
  Serial.begin(115200);
  delay(100);
  Serial.println("--- 系统开始初始化 ---");
  initOLED();
  delay(200);
  pinMode(LED_PIN, OUTPUT);
  ledcSetup(FAN_CH, FAN_FREQ, FAN_RES);
  pinMode(FAN_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DHT_PIN, INPUT_PULLUP);
  pinMode(LDR_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);
  ledcSetup(SERVO_CH, SERVO_FREQ, SERVO_RES);
  pinMode(SERVO_PIN, INPUT_PULLDOWN);
  Serial.println("舵机就绪");
  delay(300);
  closeDoor();
  Serial.println("✅ 全部硬件初始化完成");
}

int readLDR() { return analogRead(LDR_PIN); }
void buzzerAlarm(bool on) { digitalWrite(BUZZER_PIN, on); }
void toggleLED() { if (!doorIsOpen || autoMode || servoIsBusy) return; ledState = !ledState; digitalWrite(LED_PIN, ledState); }
void toggleFan() { if (!doorIsOpen || autoMode || servoIsBusy) return; fanState = !fanState; if (fanState) { ledcAttachPin(FAN_PIN, FAN_CH); ledcWrite(FAN_CH,255); } else { ledcDetachPin(FAN_PIN); pinMode(FAN_PIN, INPUT); } }
void checkAutoLight() { if (!autoMode || !doorIsOpen || servoIsBusy) return; bool need = readLDR() < LDR_DARK; digitalWrite(LED_PIN, need); ledState = need; }
void checkTempAlarm() {
  if (servoIsBusy) return;
  if (autoMode && doorIsOpen && currentTemp > TEMP_ALARM) {
    buzzerAlarm(true);
  } else {
    buzzerAlarm(false);
  }
}

// 根据天气和时间段自动开关LED
// 晴天：白天关、晚上开
// 多云/阴/雨天：白天开、晚上关
void checkAutoLEDByWeather() {
  if (!autoMode || !doorIsOpen || servoIsBusy) return;

  // 获取当前时间
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int hour = timeinfo.tm_hour;
  bool isDaytime = (hour >= 6 && hour < 18);  // 6:00-18:00为白天

  // 判断天气类型
  bool isSunny = false;
  if (weatherData.isSuccess) {
    String weather = weatherData.text;
    // 晴天相关关键词
    if (weather.indexOf("晴") >= 0 || weather.indexOf("Sun") >= 0 ||
        weather.indexOf("Clear") >= 0) {
      isSunny = true;
    }
  }

  // LED开关逻辑
  bool needLED = false;
  if (isSunny) {
    // 晴天：白天关、晚上开
    needLED = !isDaytime;
  } else {
    // 多云/阴/雨天：白天开、晚上关
    needLED = isDaytime;
  }

  digitalWrite(LED_PIN, needLED);
  ledState = needLED;

  Serial.print("[自动LED] 天气:");
  Serial.print(weatherData.text);
  Serial.print(" 时段:");
  Serial.print(isDaytime ? "白天" : "晚上");
  Serial.print(" LED:");
  Serial.println(needLED ? "开" : "关");
}

// ========== PIR 防盗功能 ==========

static bool pirInitialized = false;
static unsigned long lastPIRTrigger = 0;
static unsigned long lastAlertTime = 0;
static int pirTriggerCount = 0;         // 连续检测次数
static unsigned long pirCountResetTime = 0;  // 计数重置时间
#define PIR_COOLDOWN 5000      // PIR 触发冷却时间 5 秒
#define ALERT_COOLDOWN 60000   // 报警推送冷却时间 60 秒
#define PIR_TRIGGER_THRESHOLD 5  // 连续检测到 5 次才推送

// 初始化 PIR
void initPIR() {
  pinMode(PIR_PIN, INPUT);
  pirInitialized = true;
  Serial.println("[PIR] 人体感应模块初始化完成");
}

// 检测人体移动（简化版，无冷却时间，用于连续计数）
bool checkPIR() {
  if (!pirInitialized) return false;
  return digitalRead(PIR_PIN) == HIGH;
}

// 防盗检测主函数
void checkSecurity() {
  // 只在自动模式、门关闭时启用防盗
  Serial.print("[防盗检查] 自动模式:");
  Serial.print(autoMode);
  Serial.print(" 门状态:");
  Serial.print(doorIsOpen ? "开" : "关");
  Serial.print(" PIR状态:");
  Serial.println(digitalRead(PIR_PIN));

  if (!autoMode || doorIsOpen) return;

  // 检测人体移动
  if (checkPIR()) {
    unsigned long now = millis();
    pirTriggerCount++;
    Serial.print("[防盗] PIR 检测到人体！连续次数:");
    Serial.println(pirTriggerCount);

    // 触发蜂鸣器报警
    buzzerAlarm(true);

    // 连续检测到 5 次才发送推送
    if (pirTriggerCount >= PIR_TRIGGER_THRESHOLD) {
      pirCountResetTime = now;  // 记录触发时间

      // 发送推送（有冷却时间）
      Serial.print("[防盗] 距离上次推送:");
      Serial.print((now - lastAlertTime) / 1000);
      Serial.println("秒");

      if (now - lastAlertTime > ALERT_COOLDOWN) {
        lastAlertTime = now;
        pirTriggerCount = 0;  // 重置计数
        Serial.println("[防盗] 准备发送 Pushdeer 推送...");

        // 获取当前时间
        String timeStr = getFormattedTime();

        sendPushdeerAlert(
          "🏠 智能家居安全警报",
          "**检测到异常移动！**\n\n"
          "⏰ 时间：" + timeStr + "\n"
          "🌡️ 室内温度：" + String(currentTemp) + "℃\n"
          "🚪 门状态：关闭\n\n"
          "请检查家中情况！"
        );

        Serial.println("[防盗] 已发送安全警报推送");
      } else {
        pirTriggerCount = 0;  // 冷却时间内重置计数
        Serial.println("[防盗] 冷却时间内，跳过推送");
      }
    }
  } else {
    // 没有检测到人体
    buzzerAlarm(false);

    // 10 秒内没有再次检测到，重置计数
    if (millis() - pirCountResetTime > 10000) {
      pirTriggerCount = 0;
    }
  }
}
