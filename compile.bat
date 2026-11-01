@echo off
cd /d "%~dp0"
echo ==============================================
echo   设备巡检系统 - 一键编译（管理员端 + 工程师端）
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

echo [1/2] 编译管理员端 admin.exe ...
gcc -Wall -Wextra -std=c99 -g -DAPP_ADMIN -o admin.exe src\main.c src\common.c src\persistence.c src\session.c src\user_manager.c src\device_manager.c src\maintenance.c
if errorlevel 1 goto fail

echo [2/2] 编译工程师端 engineer.exe ...
gcc -Wall -Wextra -std=c99 -g -DAPP_ENGINEER -o engineer.exe src\main.c src\common.c src\persistence.c src\session.c src\user_manager.c src\device_manager.c src\maintenance.c
if errorlevel 1 goto fail

echo.
echo 编译成功：
echo   管理员端: admin.exe
echo   工程师端: engineer.exe
echo 两个程序可同时运行（双开），共享 data 目录数据。
echo.
pause
exit /b 0

:fail
echo.
echo 编译失败，请检查上方错误信息。
echo.
pause
exit /b 1
