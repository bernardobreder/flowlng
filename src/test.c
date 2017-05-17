#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "main.h"
#include "file.h"
#include "thread.h"
#include "md5.h"
#include "watch.h"
#include "test.h"
  
void test() {        
    test_js_lexer();
    test_js_parser();
    test_js_node();
}
