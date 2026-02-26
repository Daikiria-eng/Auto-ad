#include<stdio.h>
#include<string.h>

void greeting(){
    char dashes[19];
    memset(dashes,'-',18);
    dashes[18]='\0';
    printf(" __      __.__       _________ .__                    \n");
    printf("/  \\    /  \\__| ____ \\_   ___ \\|  |   ____ _____    ____  \n");
    printf("\\   \\/\\/   /  |/    \\/    \\  \\/|  | _/ __ \\\\__  \\  /    \\ \n");
    printf(" \\        /|  |   |  \\     \\___|  |_\\  ___/ / __ \\|   |  \\\n");
    printf("  \\__/\\  / |__|___|  /\\______  /____/\\___  >____  /___|  /\n");
    printf("       \\/          \\/        \\/          \\/     \\/     \\/ \n");
    printf("<%s%s%s>\n\n\n",dashes,"+ By Daykiria Nomad +",dashes);
}
