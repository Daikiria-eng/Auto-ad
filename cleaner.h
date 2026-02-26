#include<stdio.h>

#ifndef WIN_CLEANER
#define WIN_CLEANER

char** retrieve_users();
void clean_dns();
int delete_recursively(const char* dir_path);

#endif