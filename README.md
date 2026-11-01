# 课程设计项目合集

大学期间完成的 5 个课程设计 / 实训项目。

由于各项目技术栈差异较大（PlatformIO / qmake / Makefile / hvigor），若全部放在同一分支会互相干扰，
因此**每个项目放在独立分支**，每个分支都是一个可直接克隆、编译运行的完整工程。
本 `main` 分支仅作为导航页。

**GitHub**：[@czm668](https://github.com/czm668)

---

## 项目导航

| # | 项目 | 分支 | 技术栈 | 核心亮点 |
|---|---|---|---|---|
| 1 | **ESP32-C3 智能家居控制系统** | [`esp32-smart-home`](https://github.com/czm668/personal-project-collection/tree/esp32-smart-home) | C++ / Arduino / PlatformIO / ESP32-C3 (RISC-V) | 手写 DHT11 单总线时序驱动（微秒级 + 校验保护、未依赖第三方库）；跨平台 Captive Portal 配网（iOS / Android / Windows 自动弹窗）；天气 API + 室内传感器多源融合控制策略；PIR「连续 5 次确认 + 60 秒冷却」三重防误报并远程推送；嵌入式服务端渲染响应式 Web 控制面板。6 项验收功能全部通过 |
| 2 | **酒店客房管理系统** | [`qt-hotel-management`](https://github.com/czm668/personal-project-collection/tree/qt-hotel-management) | C++17 / Qt 6.5.3 / MySQL 8.0 | MVC 三层架构 + 6 个单例 Manager；用 SQL 时间区间重叠判定实现预订冲突检测，避免全表遍历；预订与客房状态自动联动；全量参数化查询防注入、正则校验、连续 3 次失败锁定。8 个界面 / 16 个类 / 4 张表 |
| 3 | **设备巡检系统** | [`c-device-inspection-system`](https://github.com/czm668/personal-project-collection/tree/c-device-inspection-system) | C99 / MinGW / Makefile | 管理员端与工程师端双程序共享数据目录自动同步；多终端登录的会话聚合在线判定（30 分钟有效期）；`临时文件 + rename` 原子写保证数据一致；每日幂等生成 15% 异常设备入维护队列。7 项验收用例全部通过 |
| 4 | **HarmonyOS 交管业务模拟应用** | [`harmonyos-traffic-app-demo`](https://github.com/czm668/personal-project-collection/tree/harmonyos-traffic-app-demo) | ArkTS / ArkUI / DevEco Studio / API 9 | 违章查询、车驾管业务、资讯分类、个人中心等六大模块；手机 / 平板 / 折叠屏多设备响应式布局；模拟器实测首页加载 2.1s、滑动帧率 60FPS |
| 5 | **停车场管理系统** | [`c-parking-management-system`](https://github.com/czm668/personal-project-collection/tree/c-parking-management-system) | C / Makefile | 管理员与普通用户双角色权限；白天 / 夜间 / 节假日分时计费规则；月卡登记与续费；车辆、费率、月卡、停车记录 4 类数据文件持久化，启动自动加载 |

---

## 如何查看某个项目

**网页方式**：在仓库页面左上角切换到对应分支，`README` 与代码会一并切换。

**命令行方式**：

```bash
# 直接克隆指定分支
git clone -b esp32-smart-home https://github.com/czm668/personal-project-collection.git

# 或克隆后切换
git clone https://github.com/czm668/personal-project-collection.git
cd personal-project-collection
git checkout qt-hotel-management
```

---

## 运行环境对照

每个分支的 `README` 里都有完整的功能说明、编译步骤与设计要点。环境需求速查：

| 项目 | 需要准备 | 快速运行 |
|---|---|---|
| ESP32-C3 智能家居 | PlatformIO + ESP32-C3 开发板 | `pio run -t upload`；首次需复制 `include/secrets.example.h` 为 `secrets.h` 并填入自己的天气 / 推送密钥 |
| Qt 酒店管理 | Qt 6.5.3（MinGW 64-bit）+ MySQL 8.0 | Qt Creator 打开 `src/hotel_management.pro`；数据库可自动初始化，或导入 `sql/hotel_db.sql` |
| 设备巡检系统 | MinGW-w64 gcc（Windows）或 gcc + make（Linux） | Windows：`compile.bat` → `run.bat`；Linux：`make` |
| HarmonyOS 交管应用 | DevEco Studio + API 9 SDK | 用 DevEco Studio 打开工程，首次自动拉取 ohpm 依赖 |
| 停车场管理系统 | MinGW-w64 gcc 或 gcc + make | Windows：`build.bat` → `run.bat`；Linux：`make` |

> 各分支均已排除编译产物与第三方依赖（对应各自 `.gitignore`），按 README 编译即可。

---

## 技术关键词

| 方向 | 关键词 |
|---|---|
| 嵌入式 / 物联网 | ESP32-C3 (RISC-V)、STM32F103 (Cortex-M3)、Arduino 框架、PlatformIO、Keil MDK、PWM / ADC / I2C / SPI / UART / EXTI / 单总线、WiFi、NTP、HTTP、Captive Portal、PIR |
| 桌面 / 上位机 | Qt 6、C++17、Qt Widgets / Designer、QMYSQL、MVC、单例模式、参数化查询 |
| 移动应用 | HarmonyOS、ArkTS、ArkUI、DevEco Studio、API 9 |
| 编程语言 | C99、C++、Java、JavaScript / HTML / CSS、SQL |
| 数据结构与工程 | 单链表、链队列、文件持久化、原子写、多端数据同步、会话管理、模块化分层设计 |
| 文档能力 | 需求分析、概要设计、详细设计、项目计划、验收报告 |

---

## 说明

- 本仓库收录的均为**个人独立完成**的课程设计 / 实训作业，用于学习与交流；4 人团队项目（STM32 智能电子琴）未包含在内。
- `esp32-smart-home` 分支**不含任何真实密钥**：仓库只提供 `include/secrets.example.h` 模板，使用者需自备心知天气 API key 与 Pushdeer key。
- `harmonyos-traffic-app-demo` 为**非官方教学模拟项目**，全部数据为本地模拟数据，与任何政府机构无关；界面素材仅用于学习交流，如涉及版权问题请联系删除。
- 各分支代码中的示例用户信息均已做匿名化处理。
- 课程设计作业是否公开，请遵循所在学校与任课教师的相关规定。