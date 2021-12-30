#include <stdlib.h>
#include <stdio.h>

int isPrime(int i);

int main(int argc, char** argv){
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    char veh[500];
    fgets(veh,500,fl);
    int am = atoi(veh);
    char in[500];
    while(fgets(in,500,fl) != NULL && am != 0){
        int temp = atoi(in);
        int isP = 1;
        for(;temp > 0; temp = temp / 10){
            if(isPrime(temp) == 0){
                isP =0;
                break;
            }
        }
        if(isP == 1)
            printf("yes\n");
        else
            printf("no\n");
        am--;
    }
    
    fclose(fl);
    return 0;
}
int isPrime(int i){
    if(i < 2){
        return 0;
    }
    if(i == 2){
        return 1;
    }
    for(int j = 2; j < (i / 2) + 1; j++){
        if(i % j == 0){
            return 0;
        }
    }
    return 1;
}
