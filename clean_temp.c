#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include<ftw.h>

void list_files();
void clean_them_all();

const char *USERS_BASE="C:\\Users\\";
char *users[1024];
char *temp_user_dir="\\AppData\\Local\\Temp";
const char *temp_dirs[]={
    "C:\\Windows\\Temp",
    "C:\\Windows\\Prefetch",
    "C:\\Windows\\Prefessssch"
};

void list_files(){
    for(int i=0;i<sizeof(temp_dirs)/sizeof(temp_dirs[0]);i++){
        if(access(temp_dirs[i], F_OK)==0){
            printf("DIR: %s\n",temp_dirs[i]);
        }
    }

    int result;
    for(int i=0;i<sizeof(users)/sizeof(users[0]);i++){
        //result=snprintf(users, sizeof(users), "%s\\%s",user,temp_user_dir);
    }
}

char** retrieve_users(){
    char **users_dirs=NULL;

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

        users_dirs=(char**)realloc(users_dirs, (users_count+2)*sizeof(char*));
        if(users_dirs==NULL){
            perror("[X] Error al leer usuarios\n");
        }

        users_dirs[users_count]=malloc(max_length);
        if(users_dirs[users_count]==NULL){
            perror("[X] Error al crear usuario\n");
        }

        snprintf(users_dirs[users_count], max_length-1, "%s%s%s",USERS_BASE,de->d_name,temp_user_dir);

        users_dirs[users_count][max_length-1]='\0';
        users_count++;
    }

    closedir(users_dir);

    for(size_t i=0;users_dirs[i]!=NULL;i++){
        free(users_dirs[i]);
    }
    free(users_dirs);

    return users_dirs;
}

void free_space(char**to_free){
    for(size_t i=0;to_free[i]!=NULL;i++){
        free(to_free[i]);
    }
    free(to_free);
}

void clean_them_all(){
    char** users_dirs=retrieve_users();
    for(int i=0;users_dirs[i]!=NULL;i++){
        printf("DIR: %s\nEXS: %d\n",users_dirs[i],access(users_dirs[i],F_OK));
    }

    //free_space(users_dirs);
}

int main(int argc,char **argv){
    clean_them_all();

    return 0;
}
