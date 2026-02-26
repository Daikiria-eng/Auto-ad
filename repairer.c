#include<stdio.h>
#include<windows.h>
#include "repairer.h"

void repair_images(){
    STARTUPINFO si={sizeof(si)};
    PROCESS_INFORMATION pi;
    
    char *cmd[]={
        "dism /online /cleanup-image /checkhealth",
        "dism /online /cleanup-image /scanhealth",
        "dism /online /cleanup-image /restorehealth"
    };

    for(int i=0;i<3;i++){
        ZeroMemory(&si,sizeof(si));
        si.cb=sizeof(si);
        ZeroMemory(&pi,sizeof(pi));
        

        BOOL success = CreateProcess(
            NULL,       // No module name (use command line)
            cmd[i],        // Command line
            NULL,       // Process handle not inheritable
            NULL,       // Thread handle not inheritable
            FALSE,      // No handle inheritance
            0,          // No creation flags
            NULL,       // Use parent's environment block
            NULL,       // Use parent's starting directory
            &si,        // Pointer to STARTUPINFO
            &pi         // Pointer to PROCESS_INFORMATION
        );
        
        if(!success){
            printf("[X] Failed to create proces: (%lu)\n",GetLastError());
        }
        
        WaitForSingleObject(pi.hProcess,INFINITE);
        
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess,&exit_code);
        printf("[+] Exit Code: %lu\n",exit_code);
        
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}