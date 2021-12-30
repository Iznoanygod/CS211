#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc < 2){
        printf("error\n");
        return 0;
    }
    char *out = malloc(argc * sizeof(char));
    for(int i = 1; i < argc; i++){
        int length = strlen(argv[i]);
        out[i-1] = argv[i][length-1];
    }
    out[argc-1] = '\0';
    printf("%s\n",out);
    free(out);
    return 0;
}

