#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include<windows.h>
#include<ftw.h>
#include "cleaner.h"
#include "welcome.h"
#include "repairer.h"

void destroy();
DWORD WINAPI split_pss(LPVOID lp_param);
void execute_clean();

void destroy(){
    const char *temp_dirs[]={
        "C:\\Windows\\Temp",
        "C:\\Windows\\Prefetch"
    };
    char** users_dirs=retrieve_users();
    for(int i=0;users_dirs[i]!=NULL;i++){
        delete_recursively(users_dirs[i]);
    }
    
    delete_recursively(temp_dirs[0]);
    delete_recursively(temp_dirs[1]);
    
    clean_dns();

    for(size_t i=0;users_dirs[i]!=NULL;i++){
        free(users_dirs[i]);
    }
    free(users_dirs);
}

DWORD WINAPI split_pss(LPVOID lp_param){
    int function=(int)(intptr_t)lp_param;
    if(function==1) destroy();
    else if(function==2) repair_images();

    return 0;
}

void execute_clean(){
    HANDLE h_thread[2];
    for(int i=0;i<2;i++){
        int ifunc=i+1;
        h_thread[i]=CreateThread(
            NULL,
            0,
            split_pss,
            (LPVOID)(intptr_t)ifunc,
            0,
            NULL
        );
    }
    
    if(h_thread==NULL)
        printf("[X] Error spliting the cleaning: %lu\n",GetLastError());
    
    WaitForMultipleObjects(2,h_thread,TRUE,INFINITE);
    for(int i=0;i<2;i++)
        CloseHandle(h_thread[i]);
}

int main(int argc,char **argv){
    greeting();
    execute_clean();
    printf("\n\n[+] Finished\n");

    return 0;
}
