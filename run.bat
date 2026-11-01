@echo off
cd /d "%~dp0"
if not exist ParkManSys.exe (
    echo 尚未编译，请先双击运行 build.bat 完成编译。
    echo.
    pause
    exit /b 1
)
set ROLE=%1
if "%ROLE%"=="" set ROLE=admin
ParkManSys.exe %ROLE% 123456
echo.
pause
