#include <string.h>
#include <stdio.h>
#include "main.h"
#include "watch.h"
#include "file.h"

int main(int argc, char **argv)
{
    int testMode = 0;
    int i;
    for (i = 1 ; i < argc ; i++) {
        char *arg = argv[i];
        if (!strcmp(arg, "-t") || !strcmp(arg, "--test")) { testMode = 1; }
    }

    test();
    if (testMode) {
    } else {
        printf("Usage:\n");
        printf("  flow [options] <file.js>\n");
        printf("  flow [mode]\n");
        printf("\n");
        printf("Example:\n");
        printf("  flow index.js\n");
        printf("  flow -t\n");
        printf("  flow -p\n");
        printf("  flow -e \"console.log('test')\"\n");
        printf("  flow -e \"console.mylog('test')\" index.js\n");
        printf("  flow -w\n");
        printf("  flow -w src\n");
        printf("  flow -c .\n");
        printf("  flow -c index.js\n");
        printf("\n");
        printf("Mode:\n");
        printf("  -t, --test      Execute all tests of flow executable\n");
        printf("  -r, --repl      Enter the playground for test command\n");
        printf("  -e, --eval      Execute one command with start source\n");
        printf("  -w, --watch     Compile all sources of project\n");
        printf("  -c, --compile   Enter the watch mode compiling all changes\n");
        printf("\n");
        printf("Options:\n");
        printf("\n");
    }
}
