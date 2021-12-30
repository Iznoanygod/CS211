#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    int m1, n1, m2, n2;
    fscanf(fl, "%d\t%d\n", &m1, &n1);
    int **arrm1 = malloc(m1 * sizeof(int*));
    for(int i = 0; i < m1; i++){
        arrm1[i] = malloc(n1 * sizeof(int));
    }
    for(int i = 0; i < m1; i++){
        for(int j = 0; j < n1 - 1; j++){
            fscanf(fl, "%d\t", &arrm1[i][j]);
        }
        fscanf(fl, "%d\n", &arrm1[i][n1-1]);
    }
    fscanf(fl, "%d\t%d\n", &m2, &n2);
    int **arrm2 = malloc(m2 * sizeof(int*));
    for(int i = 0; i < m2; i++){
        arrm2[i] = malloc(n2 * sizeof(int));
    }
    for(int i = 0; i < m2; i++){
        for(int j = 0; j < n2 - 1; j++){
            fscanf(fl, "%d\t", &arrm2[i][j]);
        }
        fscanf(fl, "%d\n", &arrm2[i][n2-1]);
    }
    if(n1 != m2){
        for(int i = 0; i < m1; i++)
            free(arrm1[i]);
        for(int i = 0; i < m2; i++)
            free(arrm2[i]);
        free(arrm1);
        free(arrm2);
        printf("bad-matrices");
        return 0;
    }
    int **res = malloc(m1 * sizeof(int*));
    for(int i = 0; i < m1; i++) {
        res[i] = malloc(n2 * sizeof(int));
    }
    for(int i = 0; i < m1; i++){
        for(int j = 0; j < n2; j++){
            res[i][j] = 0;
            for(int k = 0; k < m2; k++){
                res[i][j] += arrm1[i][k] * arrm2[k][j];
            }
        }
    }
    for(int i = 0; i < m1; i++){
        for(int j = 0; j < n2 - 1; j++){
            printf("%d\t",res[i][j]);
        }
        printf("%d\n",res[i][n2 - 1]);
    }
    for(int i = 0; i < m1; i++){
        free(res[i]);
        free(arrm1[i]);
    }
    for(int i = 0; i < m2; i++)
        free(arrm2[i]);
    free(arrm1);
    free(arrm2);
    free(res);
    return 0;
}
