#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<dirent.h>
#include<string.h>
#include "cleaner.h"

const char *USERS_BASE="C:\\Users\\";
char *users[1024];
char *temp_user_dir="\\AppData\\Local\\Temp";


char** retrieve_users(){
    size_t cap=16;
    char **users_dirs=malloc(cap*sizeof(char*));

    struct dirent *de;
    DIR *users_dir=opendir(USERS_BASE);

    if(users_dir==NULL){
        perror("[X] Hubo un error al leer usuarios");
    }

    size_t users_count=0;
    size_t max_length=256;
    while((de=readdir(users_dir))!=NULL){
        if(strcmp(de->d_name, ".")==0||strcmp(de->d_name,"..")==0){
            continue;
        }

        if(users_count>=cap){
            cap*=2;
            users_dirs=realloc(users_dirs, cap*sizeof(char*));
        }
        if(users_dirs==NULL){
            perror("[X] Error al leer usuarios\n");
        }

        users_dirs[users_count]=malloc(max_length);
        if(users_dirs[users_count]==NULL){
            perror("[X] Error al crear usuario\n");
        }

        snprintf(users_dirs[users_count], max_length, "%s%s%s",USERS_BASE,de->d_name,temp_user_dir);

        users_dirs[users_count][max_length-1]='\0';
        users_count++;
    }
    
    users_dirs=realloc(users_dirs,(users_count+1)*sizeof(char*));
    users_dirs[users_count]=NULL;

    closedir(users_dir);

    return users_dirs;
}

int delete_recursively(const char* dir_path){
    char search_path[MAX_PATH];
    WIN32_FIND_DATA fd;
    HANDLE hfind;

    snprintf(search_path,MAX_PATH,"%s\\*",dir_path);
    
    hfind=FindFirstFile(search_path,&fd);
    
    if(hfind==INVALID_HANDLE_VALUE){
        return 1;
    }
    
    do{
        const char* _name=fd.cFileName;
        
        if(strcmp(_name,".")==0||strcmp(_name,"..")==0)
            continue;
        
        char full[MAX_PATH];

        snprintf(full,MAX_PATH,"%s\\%s",dir_path,_name);
        
        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if(fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY){
                printf("[X] Permission denied deleting %s\n",full);
                
                return 1;
            }else delete_recursively(full);
        }else{
            printf("[*] Deleting FILE: %s\n",full);
            DeleteFile(full);
        }
        
        if(strcmp(full,dir_path)!=0){
            if(!RemoveDirectory(full))
                printf("[*] Deleting DIR: %s\n",full);
        }
    }while(FindNextFile(hfind,&fd));
    
    FindClose(hfind);
    
    return 0;
}

void clean_dns(){
    system("ipconfig -flushdns");
}