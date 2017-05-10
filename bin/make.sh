clear

echo Clearing...
rm -rf dist
mkdir dist

echo Compiling...
gcc \
    -I inc \
    src/*.c \
    src/unix/*.c \
    src/mac/*.c \
    -w -g -o dist/flow

echo Executing...
dist/flow

echo Finished