@echo off
clear
echo Clearing...
rm -f dist\flow.exe
mkdir dist
echo Compiling...
gcc -I inc src\*.c -lUser32 -o dist\flow.exe
echo Executing...
dist\flow src\main.c
echo Finished