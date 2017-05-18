@echo off
clear
echo Clearing...
rm -rf dist
mkdir dist
echo Compiling...
gcc -I inc src\*.c src\js\*.c src\test\js\*.c src\windows\string.c -w -lUser32 -o dist\flow.exe
echo Executing...
dist\flow.exe -t
echo Finished