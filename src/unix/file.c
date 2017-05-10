#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include "file.h"

size_t flow_file_length(const char* filename) {
	// http://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
	struct stat st;
	stat(filename, &st);
	return st.st_size; 
}

time_t flow_file_modif(const char* filename) {
	struct stat st;
	struct utimbuf new_times;
    stat(filename, &st);
// TIMESPEC_TO_TIMEVAL(&new_times[0], &st.st_mtime);
// gettimeofday(&new_times[1], NULL);
//	if (utime(filename, &new_times) < 0) return 0;
    return mktime(gmtime(&st.st_mtime));
//	return new_times.modtime;
}

void flow_resource_node_free(struct flow_resource_node_t* self, unsigned char deep) {
	free(self->path);
	free(self->name);
	if (deep && self->next) flow_resource_node_free(self->next, deep);
	free(self);
}

struct flow_resource_node_t* flow_file_list(char* name) {
	DIR *dir;
    struct dirent* entry;
    
	if (!(dir = opendir(name))) return 0;
    if (!(entry = readdir(dir))) return 0;

	struct flow_resource_node_t* node = 0;
    do {
		if (entry->d_type != DT_DIR && entry->d_type != DT_REG) continue;
        if (entry->d_name[0] == '.') continue;

		// printf("%s\n", entry->d_name);
        char path[1024];
        int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = 0;
		// printf("%s\n", path);
		
		struct flow_resource_node_t* data = (struct flow_resource_node_t*) malloc(sizeof(struct flow_resource_node_t));
		data->is_file = entry->d_type == DT_REG;
        data->path = strdup(path);
		data->name = strdup(entry->d_name);
		data->next = node;
		node = data;
    } while ((entry = readdir(dir)));

    closedir(dir);
	return node;
}

struct flow_resource_node_t* flow_file_list_deep(char* path) {
	struct flow_resource_node_t* node = flow_file_list(path);
	struct flow_resource_node_t* aux = node;
	while (aux) {
		if (!aux->is_file) {
			struct flow_resource_node_t* deep = flow_file_list_deep(aux->path);
			if (deep) {
				struct flow_resource_node_t* tmp = deep;
				while (tmp->next) { tmp = tmp->next; }
				tmp->next = node;
				node = deep;
			}
		}
		aux = aux->next;
	}
    return node;
}
