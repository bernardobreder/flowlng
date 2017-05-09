clear

echo Clearing...
rm -rf dist
mkdir dist

echo Compiling...
gcc \
    -I inc \
    src/*.c \
    src/mac/*.c \
    -o dist/flow \
    -framework CoreServices

echo Executing...
dist/flow --test

echo Finished