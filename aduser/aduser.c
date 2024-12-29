#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<wchar.h>

int check_lock(char* user){
    int locked;
    char command[203];
    snprintf(command,sizeof(command),"powershell -c iex \'get-aduser -identity %s -properties lockedout\'|findstr -i lockedout",user);
    FILE *process=popen(command,"r");
    if(!process){
        perror("popen() failed");
        exit(1);
    }

    char temp_str[1024];
    while(fgets(temp_str,sizeof(temp_str),process)!=NULL){
        char* is_locked=strstr(temp_str,"False");
        if(is_locked!=NULL){
            printf("[*] No estaba bloqueado\n");
            locked=0;
        }else{
            printf("[*] Estaba bloqueado\n");
            locked=1;
            break;
        }
    };
    pclose(process);
    return locked;
}

void print_status(char* user){
    char command[210];
    snprintf(command,sizeof(command),"powershell -c iex \'get-aduser -identity %s -properties displayname,lockedout,msDS-FailedInteractiveLogonCount,msDS-FailedInteractiveLogonCountAtLastSuccessfulLogon,msDS-LastFailedInteractiveLogonTime\'",user);
    FILE *process=popen(command,"r");
    if(!process){
        perror("popen() failed");
        exit(1);
    }

    char temp_str[1024];
    while(fgets(temp_str,sizeof(temp_str),process)!=NULL){
        printf("%s",temp_str);
    };
    pclose(process);
}

void unlock_user(char* user){
    char command[65];
    int locked=check_lock(user);
    if(locked==1){
        snprintf(command,sizeof(command),"powershell -c iex \'unlock-adaccount -identity %s\'",user);
        FILE* process=popen(command,"r");
        if(!process){
            perror("popen() failed when unlocking");
            exit(1);
        }
        pclose(process);
        printf("[+] Se desbloqueó el usuario de red\n");
    }
}

void set_password(char* user){
    char command[145];
    snprintf(command,sizeof(command),"powershell -c iex \'Set-ADAccountPassword -identity %s -Reset -NewPassword (ConvertTo-SecureString -AsPlainText \"contrasena generica\" -Force)\'",user);
    char next_logon[90];
    snprintf(next_logon,sizeof(next_logon),"powershell iex -c \'Set-ADUser -identity %s -ChangePasswordAtLogon $true\'",user);
    FILE* process=popen(command,"r");
    if(!process){
        perror("popen() failed when changing passsword");
        exit(1);
    }
    pclose(process);
    FILE* processs=popen(next_logon,"r");
    if(!processs){
        perror("popen() failed when trying to set onchange");
        exit(1);
    }
    pclose(processs);

    printf("[+] Se cambió la contraseña\n");
}

void set_clipboard(char *action){
    system("chcp.com 65001 > nul");
    wchar_t *good_morning=L"Buenos días.";
    wchar_t *good_afternoon=L"Buenas tardes.";
    wchar_t *good_night=L"Buenas noches.";
    wchar_t *fll_morning=L"Feliz día.";
    wchar_t *fll_afternoon=L"Feliz tarde.";
    wchar_t *fll_night=L"Feliz noche.";
    wchar_t *unlocked_msm=L"Se efectúa desbloqueo para el usuario de red, debe acceder con la misma contraseña -la última que se estableció-.";
    wchar_t *passwd_reset_msm=L"Se restablece contraseña para el usuario de red, debe acceder con \"contrasena generica\" sin las comillas";
    wchar_t complete[300];
    int time;

    FILE* time_cmd=popen("time /t","r");
    if(!time_cmd){
        perror("error getting time");
    }
    char time_rd[15];
    char hour_c[1];
    while((fgets(time_rd,sizeof(time_rd),time_cmd))!=NULL){
        strncpy(hour_c,time_rd+0,1);
        time=atoi(hour_c);
        if(strstr(time_rd,"p")){
            time+=12;
        }
    }

    if(time >= 5 && time < 12){
        if(action=="u"){
            snwprintf(complete,sizeof(complete)/sizeof(wchar_t),L"%ls\n%ls\n%ls",good_morning,unlocked_msm,fll_morning);
        }else{
            snwprintf(complete,sizeof(complete)/sizeof(wchar_t),L"%ls\n%ls\n%ls",good_morning,passwd_reset_msm,fll_morning);
        }
    }else if(time>=12&&time<18){
        if(action=="u"){
            snwprintf(complete,sizeof(complete)/sizeof(wchar_t),L"%ls\n%ls\n%ls",good_afternoon,unlocked_msm,fll_afternoon);
        }else{
            snwprintf(complete,sizeof(complete)/sizeof(wchar_t),L"%ls\n%ls\n%ls",good_afternoon,passwd_reset_msm,fll_afternoon);
        }
    }else if(time>=18 &&time<5){
        if(action=="u"){
            snwprintf(complete,sizeof(complete)/sizeof(wchar_t),L"%ls\n%ls\n%ls",good_night,unlocked_msm,fll_night);
        }else{
            snwprintf(complete,sizeof(complete)/sizeof(wchar_t),L"%ls\n%ls\n%ls",good_night,passwd_reset_msm,fll_night);
        }
    }

    if(OpenClipboard(NULL)){
        EmptyClipboard();
        size_t len=wcslen(complete)+1;
        HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,len*sizeof(wchar_t));
        if(hGlobal){
            memcpy(GlobalLock(hGlobal),complete,len*sizeof(wchar_t));
            GlobalUnlock(hGlobal);
            SetClipboardData(CF_UNICODETEXT,hGlobal);
        }
        CloseClipboard();
    }else{
        printf("No se pudo copiar en el portapapeles");
    }
}

int* argument_parser(int argc,char* argv[]){
    int *order=(int*)malloc(3*sizeof(int));
    char *user;
    for(int i=0;i<3;i++){
        order[i]=0;
    }

    if(order==NULL){
        return NULL;
    }
    for(int i=0;i<argc;i++){
        if(!strcmp(argv[i],"-i")){
            user=argv[i+1];
        }
        if(!strcmp(argv[i],"-u")){
            order[2]=1;
        }
        if(!strcmp(argv[i],"-c")){
            order[1]=1;
        }
        if(!strcmp(argv[i],"-p")){
            order[0]=1;
        }
    }
    
    if(order[0]==1) print_status(user);
    if(order[1]==1&&order[2]==1){
        unlock_user(user);
        set_password(user);
        set_clipboard("c");
    }else{
        if(order[2]==1){
            unlock_user(user);
            set_clipboard("u");
        }else if(order[1]==1){
            set_password(user);
            set_clipboard("c");
        }
    }

    return order;
}

int main(int argc,char* argv[]){
    system("chcp.com 65001 > nul");
    system("time /t & date /t");
    int size=3;
    char user[20];
    int *order=argument_parser(argc,argv);
    free(order);
    
    return 0;
}