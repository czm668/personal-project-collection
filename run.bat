@echo off
cd /d "%~dp0"
if not exist admin.exe goto notbuilt
if not exist engineer.exe goto notbuilt
echo 正在启动管理员端与工程师端（两个窗口），可同时登录操作。
start "设备巡检-管理员端" cmd /k admin.exe
start "设备巡检-工程师端" cmd /k engineer.exe
exit /b 0

:notbuilt
echo 尚未编译，请先双击运行 compile.bat 完成编译。
echo.
pause
exit /b 1
