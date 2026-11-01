@echo off
cd /d "%~dp0"
echo ==============================================
echo   停车场管理系统 - 一键编译
echo ==============================================
echo.

where gcc >nul 2>nul
if errorlevel 1 (
    echo [错误] 未找到 gcc 编译器。
    echo        请安装 MinGW-w64 并将 gcc 加入 PATH（例如 C:\mingw64\bin）。
    echo.
    pause
    exit /b 1
)

echo 正在编译 ParkManSys.exe ...
gcc -Wall -Wextra -std=c99 -g -o ParkManSys.exe src\main.c src\menu.c src\car.c src\fee.c src\month_card.c src\park_record.c -lm
if errorlevel 1 goto fail

echo.
echo 编译成功：ParkManSys.exe
echo   管理员:   build.bat 编译后运行  run.bat            （默认管理员）
echo   普通用户: run.bat user
echo.
pause
exit /b 0

:fail
echo.
echo 编译失败，请检查上方错误信息。
echo.
pause
exit /b 1
