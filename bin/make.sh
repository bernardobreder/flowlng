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
    src/js/*.c \
    src/test/js/*.c \
    -framework CoreServices \
    -w -O3 -o dist/flow

echo Executing...
dist/flow -t

echo Finished
