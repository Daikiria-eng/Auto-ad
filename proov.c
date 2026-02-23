#include<stdio.h>
#include<stdlib.h>

int main(){
    char **h=malloc(sizeof(char*)*2);
    h[0]="fdsac";
    h[1]="asdf";
    printf("v: %d\nd: %d\n",sizeof(h),sizeof(h[0]));

    return 0;
}