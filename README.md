# ESP32-C3 智能家居控制系统

基于 ESP32-C3（RISC-V）的智能家居控制系统：**环境监测 + 远程控制 + 安防告警 + 自动控制策略**，支持手机/电脑浏览器远程访问，配 OLED 本地显示与蜂鸣器语音提示。

## 功能

| 模块 | 说明 |
|---|---|
| 环境监测 | DHT11 温湿度（2 秒周期）、LDR 光照 ADC、心知天气室外数据（1 小时更新）、OLED 实时显示 |
| 智能控制 | 自动/手动模式；自动模式按「天气 + 时段」控制照明、按「室内外温差」控制风扇；SG90 舵机模拟门禁 |
| 安全防护 | HC-SR501 PIR 人体感应，连续 5 次确认 + 60 秒推送冷却防误报；本地蜂鸣器报警 + Pushdeer 远程推送 |
| 智能配网 | AP 热点 + Captive Portal 强制门户，兼容 iOS / Android / Windows 自动弹窗；配置写入 Flash 掉电不丢，断线自动重连 |
| Web 面板 | 嵌入式服务端渲染的响应式控制面板（HTML/CSS/JS 内嵌 C++） |
| 语音提示 | PWM 蜂鸣器提示音（启动 / 开门 / 关门 / 天气播报） |

## 硬件

| 模块 | 型号 |
|---|---|
| 主控 | ESP32-C3（RISC-V, 160MHz） |
| 温湿度 | DHT11 |
| 人体感应 | HC-SR501 PIR（检测距离 3–7 m） |
| 显示 | SSD1306 OLED 0.96" 128×64（I2C） |
| 门禁执行 | SG90 舵机（PWM） |
| 光照 | LDR 光敏电阻（ADC） |
| 提示 | 无源蜂鸣器（PWM / LEDC） |

## 快速开始

1. 安装 [PlatformIO](https://platformio.org/)（VS Code 插件或 CLI）
2. 配置密钥（**仓库不含密钥**）：

   ```bash
   cp include/secrets.example.h include/secrets.h
   ```

   在 `include/secrets.h` 填入自己的密钥：
   - 心知天气 API 私钥：https://www.seniverse.com/
   - Pushdeer 推送 key：https://www.pushdeer.com/

   `include/secrets.h` 已被 `.gitignore` 忽略，不会被提交。

3. 编译烧录并查看串口：

   ```bash
   pio run -t upload
   pio device monitor
   ```

4. 首次上电自动进入 AP 模式：连接热点 `ESP32-Config`，浏览器会自动弹出配网页面，输入 WiFi 密码后自动重启并连接。

## 技术要点

- **手写 DHT11 单总线驱动**：未使用第三方库，微秒级时序控制，含数据校验和与超时保护
- **跨平台 Captive Portal**：兼容 iOS（hotspot-detect.html）/ Android（generate_204）/ Windows（ncsi.txt）的自动弹窗探测
- **多源数据融合决策**：室内传感器 + 室外天气 API → 照明与风扇自动控制
- **PIR 三重防误报**：连续确认 + 推送冷却 + 计数重置
- **PWM 资源复用**：LEDC 通道合理分配（舵机 50Hz / 风扇 5kHz / 蜂鸣器音频），避免资源冲突
- **优雅降级**：WiFi 断开使用本地数据、NTP 双服务器冗余重同步、天气 API 失败显示占位符

## 目录结构

```
src/
  main.cpp                 主流程（初始化、主循环、断线重连）
  SmartHomeHardware.cpp    传感器采集、执行器控制、防盗逻辑
  web_server.cpp           Web 控制面板与 HTTP 接口
  wifi.cpp                 配网、NTP、天气 API、推送、蜂鸣器语音
include/
  wifi.h / web_server.h    模块头文件
  secrets.example.h        密钥模板（复制为 secrets.h 使用）
platformio.ini             PlatformIO 工程配置
```

## 验收结果

课程设计验收：6 项功能全部通过（自动配网、网络时间与天气可视化、远程开关门、远程温度控制、红外防盗、自动控制模式）；Web 控制响应 ≤500ms；硬件成本控制在百元内。

## 说明

课程设计项目，代码用于学习与交流。联网功能需自备心知天气与 Pushdeer 密钥。