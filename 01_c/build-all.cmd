@echo off
setlocal
set BASE=%~dp0

echo ============================================
echo  第 1 周一键编译（需要先把 gcc 装好并加进 PATH）
echo ============================================
echo.

where gcc >nul 2>nul
if errorlevel 1 (
    echo [错误] 找不到 gcc，先按《第1周执行手册》第二节装好编译器。
    exit /b 1
)

echo --- w1d1 ---
gcc -std=c11 -Wall -Wextra -Wpedantic -g "%BASE%w1d1\main.c" -o "%BASE%w1d1\w1d1.exe"
if errorlevel 1 echo [编译失败] w1d1

echo --- w1d2 ---
gcc -std=c11 -Wall -Wextra -g "%BASE%w1d2\main.c" -o "%BASE%w1d2\w1d2.exe"
if errorlevel 1 echo [编译失败] w1d2

echo --- w1d3 ---
gcc -std=c11 -Wall -Wextra -g "%BASE%w1d3\main.c" -o "%BASE%w1d3\w1d3.exe"
if errorlevel 1 echo [编译失败] w1d3

echo --- w1d4 ---
gcc -std=c11 -Wall -Wextra -g "%BASE%w1d4\main.c" "%BASE%w1d4\util.c" -o "%BASE%w1d4\w1d4.exe"
if errorlevel 1 echo [编译失败] w1d4

echo --- w1d5 ---
gcc -std=c11 -Wall -Wextra -g "%BASE%w1d5\main.c" "%BASE%w1d5\led.c" -o "%BASE%w1d5\w1d5.exe"
if errorlevel 1 echo [编译失败] w1d5

echo.
echo 编译结束。手动运行某一天：
echo    %BASE%w1d1\w1d1.exe
echo.
endlocal
