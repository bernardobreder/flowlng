clear
rm -rf dist
mkdir dist
gcc -I inc src/*.c src/mac/*.c -o dist/flow -framework CoreServices
dist/flow