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

void listdir(const char *name, int level) {
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name))) return;
    if (!(entry = readdir(dir))) return;
    do {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || entry->d_name[0] == '.') continue;
        char path[1024];
        int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = 0;
        if (entry->d_type == DT_DIR) {
            printf("%*s[%s]\n", level * 2, "", entry->d_name);
            listdir(path, level + 1);
        } else {
            size_t size = flow_file_length(path);
            unsigned char* content = flow_io_file(path);
            printf("%*s - %s [%s]\n", level * 2, "", entry->d_name, flow_md5(content, size));
        }
    } while ((entry = readdir(dir)));
    closedir(dir);
}
  
void test() {        
    printf("Creating watch\n");
    struct watch_t* watch = watch_new("/Users/bernardobreder/git/flowlng/src");
    
    printf("Search changed\n");
    for (;;) {
        watch_changed(watch);
        flow_thread_sleep(500);
    }
    // printf("[2] Search changed\n");
    // watch_changed(watch);
    // printf("[3] Search changed\n");
    // watch_changed(watch);;
    // printf("[4] Search changed\n");
    // watch_changed(watch);
    // printf("[5] Search changed\n"); ; ; ;
    // watch_changed(watch);
    // printf("[6] Search changed\n");
    // watch_changed(watch);
    // printf("[7] Search changed\n"); 
    // watch_changed(watch);
    // printf("[8] Search changed\n");
    // watch_changed(watch);
    // printf("[9] Search changed\n");
    // watch_changed(watch);
    
    watch_free(watch);
    
    // listdir(".", 0);
    // char *filePath = "src/main.c";
    // struct stat attrib;
    // stat(filePath, &attrib);
    // char date[10];
    // struct tm *ptm = gmtime(&(attrib.st_mtime));
    // printf("%d:%d:%d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    // strftime(date, 10, "%d-%m-%y", gmtime(&(attrib.st_mtime)));
    // printf("The file %s was last modified at %s\n", filePath, date);
    // date[0] = 0;
}
