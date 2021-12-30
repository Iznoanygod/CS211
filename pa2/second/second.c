#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int pow2(int a);
int* pows2(int val);
int hex2int(char a);
char int2hex(int a);
int validGrid(char** a);
int solved(char** a);
int recursiveSolve(char** a);

int main(int argc, char** argv){
    FILE *fl;
	fl=fopen(argv[1],"r");
	if(fl == NULL){
		printf("error\n");
		free(fl);
        return 0;
	}
	char **sudoku = malloc(16 * sizeof(int*));
	for(int i = 0; i < 16; i++){
		sudoku[i] = malloc(16 * sizeof(int));
	}
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 15; j++){
			fscanf(fl, "%c\t", &sudoku[i][j]);
		}
		fscanf(fl, "%c\n", &sudoku[i][15]);
	}
    fclose(fl);
    if(validGrid(sudoku) == 0){
        printf("no-solution\n");
        for(int i = 0; i < 16; i++)
            free(sudoku[i]);
        free(sudoku);
        return 0;
    }
    int changed=1;
    while(changed){
        changed = 0;
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                int known = 0;
                if(sudoku[i][j] == '-'){
                    for(int k = 0; k < 16; k++){
                        if(sudoku[i][k] != '-')
                            known = known | (1<<hex2int(sudoku[i][k]));
                        if(sudoku[k][j] != '-')
                            known = known | (1<<hex2int(sudoku[k][j]));
                        if(sudoku[4 * (i / 4) + k % 4][4 * (j / 4) + k/4] != '-')
                            known = known | (1<<hex2int(sudoku[4 * (i / 4) + k % 4][4 * (j / 4) + k/4]));
                    }
                    if(pow2(65535 - known)){
                        changed = 1;
                        sudoku[i][j] = int2hex((int) log2(65535 - known));
                    }
                }
            }

        }
    }
    if(recursiveSolve(sudoku) == 0){
        printf("no-solution\n");
        for(int i = 0; i < 16; i++)
            free(sudoku[i]);
        free(sudoku);
        return 0;
    }
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 15; j++)
            printf("%c\t",sudoku[i][j]);
        printf("%c\n", sudoku[i][15]);
    }
    for(int i = 0; i < 16; i++)
        free(sudoku[i]);
    free(sudoku);
    return 0;
}

int hex2int(char a){
    return (a <= '9') ? a - '0' : (a & 0x7) + 9;
}
char int2hex(int a){
    return (a <= 9) ? a + '0' : (a - 9) | 0x40;
}
int pow2(int a){
    int t = 1;
    while(a >= t){
        if(a == t){
            return 1;
        }
        t = t<<1;
    }

    return 0;
}
int* pows2(int val){
    int c = 0;
    int n = val;
    while(n){
        c += n & 1;
        n = n >> 1;
    }
    int* ret = malloc((c + 1) * sizeof(int));
    int i = 0;
    for(int tes = 0; tes < 17; tes++){
        if(val & 1){
            ret[n+1] = (int)pow(2,i);
            n++;
        }
        i++;
        val = val >> 1;
    }
    ret[0] = c;
    return ret;
}

int recursiveSolve(char** a){
    int i=0, j=0;
    if(solved(a))
        return 1;
    while(i < 16 && a[i][j] != '-'){
        j = (j + 1) % 16;
        if(j == 0)
            i++;
    }
    if(i == 16)
        return 0;
    int known = 0;
    for(int k = 0; k < 16; k++){
        if(a[i][k] != '-')
            known = known | (1<<hex2int(a[i][k]));
        if(a[k][j] != '-')
            known = known | (1<<hex2int(a[k][j]));
        if(a[4 * (i / 4) + k % 4][4 * (j / 4) + k/4] != '-')
            known = known | (1<<hex2int(a[4 * (i / 4) + k % 4][4 * (j / 4) + k/4]));
    }
    int* vars = pows2(65535 - known);
    if(vars[0] == 0){
        a[i][j] = '-';
        free(vars);
        return 0;
    }
    for(int ext = 0; ext < vars[0]; ext++){
        a[i][j] = int2hex((int) log2(vars[ext + 1]));
        if(recursiveSolve(a)){
            free(vars);
            return 1;
        }
    }
    a[i][j] = '-';
    free(vars);
    return 0;
}

int solved(char** a){
    for(int i = 0; i < 16; i++){
        int check[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int j = 0; j < 16; j++){
            if(a[i][j] == '-')
                return 0;
            check[hex2int(a[i][j])]++;
            if(check[hex2int(a[i][j])] == 2){
                return 0;
            }
        }
    }
    for(int j = 0; j < 16; j++){
        int check[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int i = 0; i < 16; i++){
            if(a[i][j] == '-')
                return 0;
            check[hex2int(a[i][j])]++;
            if(check[hex2int(a[i][j])] == 2){
                return 0;
            }
        }
    }
    for(int i = 0; i < 16; i++){
        int check[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int j = 0; j < 16; j++){
            if(a[4* (i / 4) + j / 4][4 * (i % 4) + j % 4] == '-')
                return 0;
            check[hex2int(a[4* (i / 4) + j / 4][4 * (i % 4) + j % 4])]++;
            if(check[hex2int(a[4* (i / 4) + j / 4][4 * (i % 4) + j % 4])] == 2){
                return 0;
            }
            
        }
    }
    return 1;
}

int validGrid(char** a){
    for(int i = 0; i < 16; i++){
        int check[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int j = 0; j < 16; j++){
            if(a[i][j] != '-'){
                check[hex2int(a[i][j])]++;
                if(check[hex2int(a[i][j])] == 2){
                    return 0;
                }
            }
        }
    }
    for(int j = 0; j < 16; j++){
        int check[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int i = 0; i < 16; i++){
            if(a[i][j] != '-'){
                check[hex2int(a[i][j])]++;
                if(check[hex2int(a[i][j])] == 2){
                    return 0;
                }
            }

        }

    }
    for(int i = 0; i < 16; i++){
        int check[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int j = 0; j < 16; j++){
            if(a[4* (i / 4) + j / 4][4 * (i % 4) + j % 4] != '-'){
                check[hex2int(a[4* (i / 4) + j / 4][4 * (i % 4) + j % 4])]++;
                if(check[hex2int(a[4* (i / 4) + j / 4][4 * (i % 4) + j % 4])] == 2){
                    return 0;
                }
            }
        }
    }
    return 1;
}
