#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isV(char c);
int fV(char *arr);

int main(int argc, char** argv){
    if(argc < 2){
        printf("error\n");
        return 0;
    }
    for(int i = 1; i < argc; i++){
        if(isV(argv[i][0])==1){
            printf("%syay ",argv[i]);
        }
        else{
            int pos = fV(argv[i]);
            for(int k = pos; k < strlen(argv[i]); k++){
                printf("%c", argv[i][k]);
            }
            for(int k = 0; k < pos; k++){
                printf("%c", argv[i][k]);
            }
            printf("ay ");
            
        }
    }
    printf("\n");
    return 0;
}

int isV(char c){
     if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||  c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
         return 1;
     else
         return 0;
 }

int fV(char *arr){
    for(int i = 0; i < sizeof(arr); i++){
        if(isV(arr[i]) == 1)
            return i;
    }
    return -1;
}

