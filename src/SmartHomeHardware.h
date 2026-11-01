#ifndef SMART_HOME_HARDWARE_H
#define SMART_HOME_HARDWARE_H
#include <Arduino.h>
#include <U8g2lib.h>

// 引脚定义
#define LED_PIN        3
#define FAN_PIN        4
#define SERVO_PIN      5
#define DHT_PIN        0
#define BUZZER_PIN     7
#define OLED_SDA       8
#define OLED_SCL       9
#define LDR_PIN        1
#define PIR_PIN        2   // PIR人体感应引脚
#define SPEAKER_PIN    10   // 喇叭引脚（PWM音频输出）

// 舵机配置
#define SERVO_CH       0
#define SERVO_FREQ     50
#define SERVO_RES      10

// 新增：风扇PWM配置（和舵机通道不冲突）
#define FAN_CH         1
#define FAN_FREQ       5000
#define FAN_RES        8

// 功能阈值
#define TEMP_ALARM     32
#define LDR_DARK       500

// 全局变量声明
extern bool ledState;
extern bool fanState;
extern bool doorIsOpen;
extern bool autoMode;
extern float currentTemp;
extern float currentHum;
extern bool servoIsBusy;

// 函数声明
void initHardware();
void initOLED();
void OLED_show();
bool readDHT();
void toggleLED();
void toggleFan();
void openDoor();
void closeDoor();
int readLDR();
void buzzerAlarm(bool on);
void checkAutoLight();
void checkTempAlarm();
void checkAutoLEDByWeather();  // 根据天气和时间段自动开关LED

// PIR 防盗功能
void initPIR();                    // 初始化 PIR
bool checkPIR();                   // 检测人体移动
void checkSecurity();              // 防盗检测主函数

#endif
