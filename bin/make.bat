@echo off
clear
echo Clearing...
rm -f dist\flow.exe
mkdir dist
echo Compiling...
gcc -I inc src\*.c -o dist\flow.exe
echo Executing...
dist\flow
echo Finished