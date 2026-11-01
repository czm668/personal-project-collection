#include "web_server.h"
#include <SmartHomeHardware.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include "wifi.h"
WebServer server(80);

// 新增：处理自动模式开关
void handleAutoModeToggle() {
  autoMode = !autoMode;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: 页面不存在");
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang='zh-CN'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<meta http-equiv='refresh' content='5'>
<title>智能家居控制</title>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; }
  body {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    background: linear-gradient(135deg, #0f0c29, #302b63, #24243e);
    min-height: 100vh;
    color: #fff;
    padding: 20px;
  }
  .container {
    max-width: 400px;
    margin: 0 auto;
    padding-top: 30px;
  }
  .header {
    text-align: center;
    margin-bottom: 30px;
  }
  .header h1 {
    font-size: 24px;
    font-weight: 600;
    background: linear-gradient(90deg, #00d2ff, #3a7bd5);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
  }
  .header p {
    font-size: 13px;
    color: #8888aa;
    margin-top: 6px;
  }
  .card {
    background: rgba(255,255,255,0.06);
    border: 1px solid rgba(255,255,255,0.1);
    border-radius: 16px;
    padding: 20px;
    margin-bottom: 16px;
    backdrop-filter: blur(10px);
  }
  .card-title {
    font-size: 13px;
    color: #8888aa;
    text-transform: uppercase;
    letter-spacing: 1px;
    margin-bottom: 14px;
  }
  .btn-row {
    display: flex;
    gap: 12px;
  }
  .btn {
    flex: 1;
    padding: 14px 0;
    font-size: 15px;
    font-weight: 500;
    border: none;
    border-radius: 12px;
    cursor: pointer;
    transition: all 0.3s ease;
    text-decoration: none;
    display: block;
    text-align: center;
  }
  .btn:active { transform: scale(0.96); }
  .btn:disabled {
    opacity: 0.35;
    cursor: not-allowed;
    transform: none;
  }
  .btn-green {
    background: linear-gradient(135deg, #43e97b, #38f9d7);
    color: #0a3d2a;
  }
  .btn-green:disabled { background: #2a5a3a; color: #5a8a6a; }
  .btn-red {
    background: linear-gradient(135deg, #f5576c, #ff6a88);
    color: #fff;
  }
  .btn-red:disabled { background: #5a2a3a; color: #8a5a6a; }
  .btn-blue {
    background: linear-gradient(135deg, #667eea, #764ba2);
    color: #fff;
  }
  .btn-blue.active {
    background: linear-gradient(135deg, #f093fb, #f5576c);
  }
  .btn-orange {
    background: linear-gradient(135deg, #f6d365, #fda085);
    color: #5a3a1a;
  }
  .status-bar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 12px 0;
    border-bottom: 1px solid rgba(255,255,255,0.06);
  }
  .status-bar:last-child { border-bottom: none; }
  .status-label {
    font-size: 14px;
    color: #ccc;
  }
  .status-value {
    font-size: 14px;
    font-weight: 600;
  }
  .status-on { color: #43e97b; }
  .status-off { color: #f5576c; }
  .status-auto { color: #667eea; }
  .status-manual { color: #8888aa; }
  .section {
    margin-top: 24px;
  }
  .section-title {
    font-size: 13px;
    color: #8888aa;
    text-transform: uppercase;
    letter-spacing: 1px;
    margin-bottom: 12px;
  }
  .toggle-card {
    background: rgba(255,255,255,0.06);
    border: 1px solid rgba(255,255,255,0.1);
    border-radius: 16px;
    padding: 16px 20px;
    margin-bottom: 12px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    backdrop-filter: blur(10px);
  }
  .toggle-info h3 {
    font-size: 15px;
    font-weight: 500;
    margin-bottom: 4px;
  }
  .toggle-info p {
    font-size: 12px;
    color: #8888aa;
  }
  .toggle-switch {
    width: 52px;
    height: 28px;
    background: #333;
    border-radius: 14px;
    position: relative;
    cursor: pointer;
    transition: background 0.3s;
    border: none;
    padding: 0;
  }
  .toggle-switch.on {
    background: linear-gradient(135deg, #43e97b, #38f9d7);
  }
  .toggle-switch::after {
    content: '';
    position: absolute;
    width: 22px;
    height: 22px;
    background: #fff;
    border-radius: 50%;
    top: 3px;
    left: 3px;
    transition: transform 0.3s;
    box-shadow: 0 2px 4px rgba(0,0,0,0.3);
  }
  .toggle-switch.on::after {
    transform: translateX(24px);
  }
  .door-btns {
    display: flex;
    gap: 12px;
    margin-top: 12px;
  }
  .door-btns .btn {
    flex: 1;
    padding: 16px 0;
    font-size: 16px;
    font-weight: 600;
    border-radius: 14px;
  }
  .data-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 12px;
    margin-bottom: 16px;
  }
  .data-card {
    background: rgba(255,255,255,0.06);
    border: 1px solid rgba(255,255,255,0.1);
    border-radius: 16px;
    padding: 16px;
    text-align: center;
    backdrop-filter: blur(10px);
  }
  .data-card .icon {
    font-size: 24px;
    margin-bottom: 8px;
  }
  .data-card .value {
    font-size: 22px;
    font-weight: 700;
    margin-bottom: 4px;
  }
  .data-card .label {
    font-size: 12px;
    color: #8888aa;
  }
  .data-card.temp .value { color: #f5576c; }
  .data-card.hum .value { color: #00d2ff; }
  .data-card.weather .value { color: #fda085; font-size: 16px; }
  .data-card.outdoor .value { color: #43e97b; }
  .data-card.time .value { color: #667eea; font-size: 18px; }
  .data-card.ldr .value { color: #f6d365; }
  .data-full {
    grid-column: 1 / -1;
  }
  .weather-card {
    background: rgba(255,255,255,0.06);
    border: 1px solid rgba(255,255,255,0.1);
    border-radius: 16px;
    padding: 16px 20px;
    margin-bottom: 16px;
    backdrop-filter: blur(10px);
    display: flex;
    justify-content: space-between;
    align-items: center;
  }
  .weather-info .weather-icon { font-size: 28px; }
  .weather-info .weather-text {
    font-size: 14px;
    color: #ccc;
    margin-top: 4px;
  }
  .weather-temp {
    font-size: 28px;
    font-weight: 700;
    color: #fda085;
  }
  .weather-temp span {
    font-size: 14px;
    color: #8888aa;
  }
</style>
</head>
<body>
<div class='container'>
  <div class='header'>
    <h1>Smart Home</h1>
    <p>ESP32-C3 智能家居控制系统</p>
  </div>

  <!-- 时间 -->
  <div class='card' style='text-align:center; padding: 14px 20px;'>
    <div style='font-size: 28px; font-weight: 700; color: #667eea; letter-spacing: 2px;' id='time'>)rawliteral";

  html += getFormattedTime();
  html += "</div>";
  html += "<div style='font-size: 12px; color: #8888aa; margin-top: 4px;'>当前时间</div></div>";

  // 室内温湿度 + 光照
  html += "<div class='data-grid'>";
  html += "<div class='data-card temp'><div class='icon'>🌡️</div><div class='value'>";
  html += String((int)currentTemp) + "℃";
  html += "</div><div class='label'>室内温度</div></div>";
  html += "<div class='data-card hum'><div class='icon'>💧</div><div class='value'>";
  html += String((int)currentHum) + "%";
  html += "</div><div class='label'>室内湿度</div></div>";
  html += "<div class='data-card ldr'><div class='icon'>☀️</div><div class='value'>";
  html += String(readLDR());
  html += "</div><div class='label'>光照强度</div></div>";
  html += "<div class='data-card' style='";
  html += digitalRead(PIR_PIN) ? "border-color: #f5576c;" : "";
  html += "'><div class='icon'>👁️</div><div class='value' style='";
  html += digitalRead(PIR_PIN) ? "color: #f5576c;" : "color: #43e97b;";
  html += "'>";
  html += digitalRead(PIR_PIN) ? "有人" : "无人";
  html += "</div><div class='label'>人体感应</div></div></div>";

  // 室外天气
  html += R"rawliteral(
  <div class='weather-card'>
    <div class='weather-info'>
      <div class='weather-icon'>🌤️</div>
      <div class='weather-text'>室外天气)rawliteral";

  html += " · " + String(weatherData.city);
  html += R"rawliteral(</div>
    </div>
    <div class='weather-temp'>)rawliteral";

  if (weatherData.isSuccess) {
    html += weatherData.temperature + "<span>℃</span>";
  } else {
    html += "--<span>℃</span>";
  }
  html += R"rawliteral(</div>
  </div>
  <div class='card' style='padding: 12px 20px; text-align: center;'>
    <div style='font-size: 16px; color: #ccc;'>)rawliteral";

  if (weatherData.isSuccess) {
    html += weatherData.text;
  } else {
    html += "天气数据获取失败";
  }
  html += R"rawliteral(
  </div>

  )rawliteral";

  // 系统状态面板
  html += R"rawliteral(
  <div class='card'>
    <div class='card-title'>系统状态</div>
    <div class='status-bar'>
      <span class='status-label'>自动模式</span>
      <span class='status-value )rawliteral";

  html += autoMode ? "status-auto'>已开启" : "status-manual'>已关闭";
  html += "</span></div>";

  html += R"rawliteral(
    <div class='status-bar'>
      <span class='status-label'>门状态</span>
      <span class='status-value )rawliteral";

  html += doorIsOpen ? "status-on'>已打开" : "status-off'>已关闭";
  html += "</span></div>";

  html += R"rawliteral(
    <div class='status-bar'>
      <span class='status-label'>LED</span>
      <span class='status-value )rawliteral";

  html += ledState ? "status-on'>已开启" : "status-off'>已关闭";
  html += "</span></div>";

  html += R"rawliteral(
    <div class='status-bar'>
      <span class='status-label'>风扇</span>
      <span class='status-value )rawliteral";

  html += fanState ? "status-on'>已开启" : "status-off'>已关闭";
  html += "</span></div></div>";

  // 自动模式
  html += "<div class='toggle-card'>";
  html += "<div class='toggle-info'><h3>自动模式</h3><p>自动控制LED和风扇</p></div>";
  html += "<form action='/AutoMode' method='POST' style='margin:0'>";
  html += "<button class='toggle-switch " + String(autoMode ? "on" : "") + "' type='submit'></button>";
  html += "</form></div>";

  // LED 控制
  html += "<div class='toggle-card'>";
  html += "<div class='toggle-info'><h3>LED 灯</h3><p>" + String(autoMode ? "自动模式中" : "手动控制") + "</p></div>";
  html += "<form action='/LED' method='POST' style='margin:0'>";
  html += "<button class='toggle-switch " + String(ledState ? "on" : "") + "' type='submit' " + String(autoMode ? "disabled" : "") + "></button>";
  html += "</form></div>";

  // 风扇控制
  html += "<div class='toggle-card'>";
  html += "<div class='toggle-info'><h3>风扇</h3><p>" + String(autoMode ? "自动模式中" : "手动控制") + "</p></div>";
  html += "<form action='/Fan' method='POST' style='margin:0'>";
  html += "<button class='toggle-switch " + String(fanState ? "on" : "") + "' type='submit' " + String(autoMode ? "disabled" : "") + "></button>";
  html += "</form></div>";

  // 门控制
  html += R"rawliteral(
  <div class='section'>
    <div class='section-title'>门控制</div>
    <div class='door-btns'>
      <form action='/Door/Open' method='POST' style='flex:1'>
        <button class='btn btn-green' type='submit'>开门</button>
      </form>
      <form action='/Door/Close' method='POST' style='flex:1'>
        <button class='btn btn-red' type='submit'>关门</button>
      </form>
    </div>
  </div>
</div>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleLED() {
  toggleLED();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleFan() {
  toggleFan();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDoorOpen() {
  openDoor();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDoorClose() {
  closeDoor();
  server.sendHeader("Location", "/");
  server.send(303);
}

void initWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/Fan", HTTP_POST, handleFan);
  server.on("/Door/Open", HTTP_POST, handleDoorOpen);
  server.on("/Door/Close", HTTP_POST, handleDoorClose);
  server.on("/AutoMode", HTTP_POST, handleAutoModeToggle);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web服务器启动");
}

void webServerLoop() {
  server.handleClient();
}
