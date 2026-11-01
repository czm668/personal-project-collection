#include "wifi.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WebServer.h>

// 敏感配置（天气 API key / 推送 key）统一放在 include/secrets.h，该文件已加入 .gitignore。
// 首次编译请复制 include/secrets.example.h 为 include/secrets.h 并填入自己的 key。
#include "secrets.h"

WiFiMulti WifiMulti;
Preferences preferences;
WebServer configServer(80);  // 配网服务器

#define CITY "WS7GQBRNR6V8"    // 厦门城市ID
#define LANGUAGE "zh-Hans"     // 简体中文
#define HOST "api.seniverse.com"

// NTP 配置
#define NTP_SERVER1 "ntp.aliyun.com"
#define NTP_SERVER2 "cn.ntp.org.cn"
#define GMT_OFFSET_SEC 8 * 3600    // 东八区 UTC+8
#define DST_OFFSET_SEC 0

// WiFi 配网相关
static String savedSSID = "";
static String savedPASS = "";
static bool apMode = false;  // AP配网模式标志
static bool configSuccess = false;  // 配网成功标志

WeatherData weatherData = {
  .city = "厦门",
  .text = "--",
  .temperature = "--",
  .isSuccess = false
};

bool ntpSynced = false;
unsigned long ntpSyncMillis = 0;

// 保存 WiFi 信息到 Flash
void saveWiFiConfig(const char* ssid, const char* password) {
  preferences.begin("wifi", false);
  preferences.putString("ssid", ssid);
  preferences.putString("pass", password);
  preferences.end();
  savedSSID = ssid;
  savedPASS = password;
  Serial.println("[WiFi] WiFi 信息已保存");
}

// 从 Flash 读取 WiFi 信息
bool loadWiFiConfig() {
  preferences.begin("wifi", true);
  savedSSID = preferences.getString("ssid", "");
  savedPASS = preferences.getString("pass", "");
  preferences.end();
  
  // 强制清除WiFi信息（调试用）
  // preferences.begin("wifi", false);
  // preferences.clear();
  // preferences.end();
  // savedSSID = "";
  // savedPASS = "";
  
  if (savedSSID.length() > 0) {
    Serial.print("[WiFi] 读取到保存的WiFi: ");
    Serial.println(savedSSID);
    return true;
  }
  Serial.println("[WiFi] 未找到保存的WiFi信息");
  return false;
}

// AP模式配网网页
const char* configPage = R"rawliteral(
<!DOCTYPE html>
<html lang='zh-CN'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>WiFi配网</title>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; }
  body {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    background: linear-gradient(135deg, #0f0c29, #302b63, #24243e);
    min-height: 100vh;
    color: #fff;
    padding: 20px;
    display: flex;
    justify-content: center;
    align-items: center;
  }
  .container {
    width: 100%;
    max-width: 360px;
  }
  .header {
    text-align: center;
    margin-bottom: 30px;
  }
  .header h1 {
    font-size: 24px;
    margin-bottom: 8px;
  }
  .header p {
    font-size: 13px;
    color: #8888aa;
  }
  .card {
    background: rgba(255,255,255,0.06);
    border: 1px solid rgba(255,255,255,0.1);
    border-radius: 16px;
    padding: 24px;
    backdrop-filter: blur(10px);
  }
  .form-group {
    margin-bottom: 20px;
  }
  .form-group label {
    display: block;
    font-size: 13px;
    color: #8888aa;
    margin-bottom: 8px;
    text-transform: uppercase;
    letter-spacing: 1px;
  }
  .form-group input, .form-group select {
    width: 100%;
    padding: 14px 16px;
    font-size: 15px;
    background: rgba(255,255,255,0.08);
    border: 1px solid rgba(255,255,255,0.15);
    border-radius: 12px;
    color: #fff;
    outline: none;
    transition: border-color 0.3s;
  }
  .form-group input:focus, .form-group select:focus {
    border-color: #667eea;
  }
  .form-group input::placeholder {
    color: #666;
  }
  .btn {
    width: 100%;
    padding: 16px;
    font-size: 16px;
    font-weight: 600;
    border: none;
    border-radius: 12px;
    cursor: pointer;
    transition: all 0.3s ease;
    background: linear-gradient(135deg, #667eea, #764ba2);
    color: #fff;
    margin-top: 10px;
  }
  .btn:active { transform: scale(0.98); }
  .info {
    margin-top: 20px;
    padding: 16px;
    background: rgba(255,255,255,0.04);
    border-radius: 12px;
    font-size: 13px;
    color: #8888aa;
    line-height: 1.6;
  }
  .scan-btn {
    width: 100%;
    padding: 12px;
    font-size: 14px;
    background: rgba(255,255,255,0.08);
    border: 1px solid rgba(255,255,255,0.15);
    border-radius: 12px;
    color: #fff;
    cursor: pointer;
    margin-bottom: 10px;
  }
</style>
</head>
<body>
<div class='container'>
  <div class='header'>
    <h1>WiFi 配网</h1>
    <p>ESP32 智能家居</p>
  </div>
  <div class='card'>
    <form action='/config' method='POST'>
      <div class='form-group'>
        <label>WiFi 名称</label>
        <input type='text' name='ssid' placeholder='输入WiFi名称' required>
      </div>
      <div class='form-group'>
        <label>WiFi 密码</label>
        <input type='password' name='pass' placeholder='输入WiFi密码' required>
      </div>
      <button type='submit' class='btn'>连接 WiFi</button>
    </form>
    <div class='info'>
      <strong>提示：</strong><br>
      1. 请确保 WiFi 是 2.4GHz 频段<br>
      2. ESP32 不支持 5GHz WiFi<br>
      3. 连接成功后设备将自动重启
    </div>
  </div>
</div>
</body>
</html>
)rawliteral";

// 配网成功页面
const char* successPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<title>配网成功</title>
<style>
body{font-family:Arial;background:#1a1a2e;color:#fff;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0}
h1{color:#43e97b}
</style>
</head>
<body>
<h1>配网成功！</h1>
<p>正在连接WiFi...</p>
</body>
</html>
)rawliteral";

// 处理配网页面
void handleConfigPage() {
  configServer.send(200, "text/html", configPage);
}

// 处理 Captive Portal 检测（iOS/Android 自动弹窗）
void handleCaptivePortal() {
  String host = configServer.hostHeader();
  String url = configServer.uri();
  
  Serial.print("[Captive] 请求: ");
  Serial.print(host);
  Serial.println(url);
  
  // 直接返回配网页面，不要重定向
  configServer.send(200, "text/html", configPage);
}

// 处理各种 Captive Portal 检测 URL
void handleApple() { handleCaptivePortal(); }
void handleAndroid() { handleCaptivePortal(); }
void handleMicrosoft() { handleCaptivePortal(); }

// 处理配网提交
void handleConfigSubmit() {
  String ssid = configServer.arg("ssid");
  String pass = configServer.arg("pass");
  
  Serial.println("[配网] 收到WiFi信息：");
  Serial.print("  SSID: ");
  Serial.println(ssid);
  Serial.print("  PASS: ");
  Serial.println(pass.length() > 0 ? "******" : "(空)");
  
  // 保存WiFi信息
  saveWiFiConfig(ssid.c_str(), pass.c_str());
  
  // 发送成功页面
  configServer.send(200, "text/html", successPage);
  
  // 标记配网成功
  configSuccess = true;
}

// 启动AP配网模式
void startAPConfig() {
  apMode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-Config", "");  // 开放热点，无密码
  
  IPAddress IP = WiFi.softAPIP();
  Serial.println("[AP模式] 配网热点已启动");
  Serial.print("[AP模式] 热点名称: ESP32-Config");
  Serial.println("[AP模式] 热点密码: 无");
  Serial.print("[AP模式] 访问地址: ");
  Serial.println(IP);
  
  // 启动配网服务器
  configServer.on("/", HTTP_GET, handleConfigPage);
  configServer.on("/config", HTTP_POST, handleConfigSubmit);
  
  // Captive Portal 检测 URL（让手机自动弹窗）
  configServer.on("/generate_204", HTTP_GET, handleAndroid);           // Android
  configServer.on("/gen_204", HTTP_GET, handleAndroid);                // Android
  configServer.on("/ncsi.txt", HTTP_GET, handleMicrosoft);             // Windows
  configServer.on("/hotspot-detect.html", HTTP_GET, handleApple);      // iOS/macOS
  configServer.on("/library/test/success.html", HTTP_GET, handleApple); // iOS/macOS
  configServer.on("/connectivitycheck.gstatic.com", HTTP_GET, handleAndroid);
  configServer.on("/captive.apple.com", HTTP_GET, handleApple);
  configServer.onNotFound(handleCaptivePortal);  // 其他未知请求也重定向
  
  configServer.begin();
  Serial.println("[AP模式] 配网服务器已启动");
  Serial.println("[AP模式] 请连接热点后访问 http://192.168.4.1");
}

// 检查是否在AP配网模式
bool isAPMode() {
  return apMode;
}

// 处理配网循环
void handleConfigLoop() {
  if (apMode) {
    configServer.handleClient();
    
    // 配网成功，尝试连接WiFi
    if (configSuccess) {
      delay(2000);
      Serial.println("[AP模式] 配网完成，尝试连接WiFi...");
      
      WiFi.mode(WIFI_STA);
      WiFi.begin(savedSSID.c_str(), savedPASS.c_str());
      
      int retry = 0;
      while (WiFi.status() != WL_CONNECTED && retry < 40) {
        delay(500);
        Serial.print(".");
        retry++;
      }
      
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] 连接成功！");
        Serial.print("[WiFi] IP地址: ");
        Serial.println(WiFi.localIP().toString());
        apMode = false;
        configSuccess = false;
        configServer.stop();
        return;
      } else {
        Serial.println("\n[WiFi] 连接失败，重新启动AP模式...");
        configSuccess = false;
        startAPConfig();
      }
    }
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);

  // 1. 先尝试读取保存的 WiFi 信息
  if (loadWiFiConfig()) {
    Serial.println("[WiFi] 正在连接保存的WiFi...");
    WiFi.begin(savedSSID.c_str(), savedPASS.c_str());

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 40) {
      delay(500);
      Serial.print(".");
      retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n[WiFi] 连接成功！");
      Serial.print("[WiFi] IP地址: ");
      Serial.println(WiFi.localIP().toString());
      return;
    }
    Serial.println("\n[WiFi] 连接失败，启动AP配网模式...");
  } else {
    Serial.println("[WiFi] 未找到WiFi信息，启动AP配网模式...");
  }

  // 2. 启动AP配网模式
  startAPConfig();
}

// WiFi 重连检查（在 loop 中调用）
void checkWiFiConnection() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 10000) return;
  lastCheck = millis();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] WiFi 断开，尝试重连...");
    WiFi.disconnect();
    delay(100);

    if (savedSSID.length() > 0) {
      WiFi.begin(savedSSID.c_str(), savedPASS.c_str());
    } else {
      WifiMulti.run();
    }

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
      delay(500);
      retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("[WiFi] 重连成功！");
      Serial.print("[WiFi] IP地址: ");
      Serial.println(WiFi.localIP().toString());
    } else {
      Serial.println("[WiFi] 重连失败");
    }
  }
}

// 初始化 NTP 时间同步
void initNTP() {
  Serial.println("[NTP] 正在同步网络时间...");
  configTime(GMT_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2);

  struct tm timeinfo;
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 10) {
    delay(500);
    retry++;
    Serial.print(".");
  }

  if (retry < 10) {
    ntpSynced = true;
    ntpSyncMillis = millis();
    Serial.println("\n[NTP] 时间同步成功！");
    Serial.print("[NTP] 当前时间: ");
    Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
  } else {
    ntpSynced = false;
    Serial.println("\n[NTP] 时间同步失败，将重试");
  }
}

// 获取格式化时间字符串
String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    if (millis() - ntpSyncMillis > 60000) {
      Serial.println("[NTP] 时间丢失，尝试重新同步...");
      configTime(GMT_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2);
      ntpSyncMillis = millis();
    }
    return "--:--:--";
  }
  ntpSynced = true;

  char buf[9];
  sprintf(buf, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buf);
}

bool updateWeather() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[天气] WiFi未连接，跳过更新");
    weatherData.isSuccess = false;
    return false;
  }

  WiFiClient client;
  client.setTimeout(5000);

  if (!client.connect(HOST, 80)) {
    Serial.println("[天气] 连接服务器失败");
    weatherData.isSuccess = false;
    return false;
  }

  String getUrl = "/v3/weather/now.json?key=";
  getUrl += PRIVATE_KEY;
  getUrl += "&location=";
  getUrl += CITY;
  getUrl += "&language=";
  getUrl += LANGUAGE;

  String req = "GET " + getUrl + " HTTP/1.1\r\n";
  req += "Host: " + String(HOST) + "\r\n";
  req += "Connection: close\r\n\r\n";

  client.print(req);

  String fullResponse = "";
  unsigned long startMillis = millis();

  while ((millis() - startMillis < 5000) && fullResponse.length() < 1024) {
    while (client.available()) {
      fullResponse += (char)client.read();
    }
    delay(10);
  }
  client.stop();

  int jsonStart = fullResponse.indexOf("{");
  if (jsonStart == -1) {
    weatherData.isSuccess = false;
    return false;
  }

  String jsonStr = fullResponse.substring(jsonStart);

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, jsonStr);
  if (err) {
    weatherData.isSuccess = false;
    return false;
  }

  JsonObject results = doc["results"][0];
  if (!results) {
    weatherData.isSuccess = false;
    return false;
  }

  JsonObject now = results["now"];
  if (!now) {
    weatherData.isSuccess = false;
    return false;
  }

  if (now["temperature"].is<const char*>()) {
    weatherData.temperature = now["temperature"].as<String>();
  } else {
    weatherData.temperature = "--";
  }

  if (now["text"].is<const char*>()) {
    weatherData.text = now["text"].as<String>();
  } else {
    weatherData.text = "--";
  }

  weatherData.isSuccess = true;
  return true;
}

// ========== 蜂鸣器提示音 ==========

void playTone(int frequency, int duration) {
  ledcAttachPin(7, 2);
  ledcSetup(2, frequency, 8);
  ledcWrite(2, 128);
  delay(duration);
  ledcDetachPin(7);
  pinMode(7, INPUT);
}

void playWelcomeTone() {
  Serial.println("[蜂鸣器] 播放欢迎提示音");
  playTone(1000, 200);
  delay(100);
  playTone(1500, 300);
}

void playGoodbyeTone() {
  Serial.println("[蜂鸣器] 播放告别提示音");
  playTone(1500, 200);
  delay(100);
  playTone(1000, 300);
}

void playWeatherTone() {
  Serial.println("[蜂鸣器] 播放天气提示音");
  playTone(1200, 150);
  delay(50);
  playTone(1200, 150);
  delay(50);
  playTone(1200, 150);
}

bool initBaiduTTS(const char* appId, const char* apiKey, const char* secretKey) {
  Serial.println("[音频] 使用蜂鸣器提示音代替语音");
  return true;
}

bool playTTS(const char* text) {
  String txt = String(text);
  if (txt.indexOf("欢迎") >= 0) {
    playWelcomeTone();
  } else if (txt.indexOf("顺风") >= 0 || txt.indexOf("告别") >= 0) {
    playGoodbyeTone();
  } else if (txt.indexOf("天气") >= 0 || txt.indexOf("温度") >= 0) {
    playWeatherTone();
  } else {
    playTone(1000, 200);
  }
  return true;
}

void audioLoop() {
  // 蜂鸣器不需要循环处理
}

// ========== Pushdeer 推送功能 ==========

void sendPushdeerAlert(String title, String content) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[Pushdeer] WiFi 未连接，无法发送推送");
    return;
  }

  WiFiClient client;
  if (!client.connect("api2.pushdeer.com", 80)) {
    Serial.println("[Pushdeer] 连接服务器失败");
    return;
  }

  String encodedTitle = "";
  for (int i = 0; i < title.length(); i++) {
    char c = title.charAt(i);
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encodedTitle += c;
    } else {
      char buf[4];
      sprintf(buf, "%%%02X", (unsigned char)c);
      encodedTitle += buf;
    }
  }

  String encodedContent = "";
  for (int i = 0; i < content.length(); i++) {
    char c = content.charAt(i);
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encodedContent += c;
    } else {
      char buf[4];
      sprintf(buf, "%%%02X", (unsigned char)c);
      encodedContent += buf;
    }
  }

  String url = "/message/push?pushkey=" + String(PUSHDEER_KEY);
  url += "&text=" + encodedContent;
  url += "&desp=" + encodedTitle;
  url += "&type=markdown";

  String request = "GET " + url + " HTTP/1.1\r\n";
  request += "Host: api2.pushdeer.com\r\n";
  request += "Connection: close\r\n\r\n";

  client.print(request);

  unsigned long start = millis();
  while (millis() - start < 3000) {
    while (client.available()) {
      String line = client.readStringUntil('\n');
      if (line.indexOf("200") > 0 || line.indexOf("success") > 0) {
        Serial.println("[Pushdeer] 推送发送成功");
        client.stop();
        return;
      }
    }
    delay(10);
  }

  client.stop();
  Serial.println("[Pushdeer] 推送发送完成");
}
