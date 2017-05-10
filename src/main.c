#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "main.h"
#include "watch.h"
#include "file.h"
#include "input.h"

// --signal SIGKILL \
    
// void registerSignal() {
//     struct sigaction action;
//     action.sa_handler = close_handler;
//     sigemptyset(&action.sa_mask);
//     action.sa_flags = 0;

//     if (sigaction(SIGTERM, &action, nullptr) == 0)
//     {
//     FSW_ELOG(_("SIGTERM handler registered.\n"));
//     }
//     else
//     {
//     cerr << _("SIGTERM handler registration failed.") << endl;
//     }

//     if (sigaction(SIGABRT, &action, nullptr) == 0)
//     {
//     FSW_ELOG(_("SIGABRT handler registered.\n"));
//     }
//     else
//     {
//     cerr << _("SIGABRT handler registration failed.") << endl;
//     }

//     if (sigaction(SIGINT, &action, nullptr) == 0)
//     {
//     FSW_ELOG(_("SIGINT handler registered.\n"));
//     }
//     else
//     {
//     cerr << _("SIGINT handler registration failed") << endl;
//     }
// }

int main(int argc, char **argv)
{
    int test_mode = 0, help_mode = 0;
    int i;
    for (i = 1 ; i < argc ; i++) {
        char *arg = argv[i];
        if (!strcmp(arg, "-t") || !strcmp(arg, "--test")) { test_mode = 1; }
        if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) { help_mode = 1; }
    }

    if (test_mode) {
        test();
    } else if (help_mode) {
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
    } else {
        printf("Creating watch\n");
        struct watch_t* watch = watch_new();
        watch_dir(watch, "/Users/bernardobreder/git/flowlng/src");
        watch_loop(watch, flow_input_perform);
        watch_free(watch);
    }
}
