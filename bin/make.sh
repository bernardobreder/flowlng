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
    -o dist/flow

echo Executing...
dist/flow --test

echo Finished