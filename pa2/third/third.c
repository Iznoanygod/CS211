#include <stdlib.h>
#include <stdio.h>

double** transpose(double** arr, int row, int col);
double** multiply(double** arr1, double** arr2, int row1, int col1, int row2, int col2);
double** inverse(double** a, int n);
void freeMatrix(double** arr, int n);

int main(int argc, char** argv){
    FILE *fl;
	fl=fopen(argv[1],"r");
	if(fl == NULL){
		printf("error\n");
		free(fl);
        return 0;
	}
    int cols = 0;
    fscanf(fl, "%d\n", &cols);
    cols++;
    int rows = 0;
    fscanf(fl, "%d\n", &rows);
    double **X = malloc(rows * sizeof(double*));
    double **Y = malloc(rows * sizeof(double*));
    for(int i = 0; i < rows; i++){
        X[i] = malloc(cols * sizeof(double));
        Y[i] = malloc(sizeof(double));
    }
    for(int i = 0; i < rows; i++){
        X[i][0] = 1;
        for(int j = 1; j < cols; j++){
            fscanf(fl, "%lf,", &X[i][j]);
        }
        fscanf(fl, "%lf\n", &Y[i][0]);
    }
    fclose(fl);
    double** Xt = transpose(X, rows, cols);
    double** XtX = multiply(Xt, X, cols, rows, rows, cols);
    double** XtXi = inverse(XtX, cols);
    double** XtXiXt = multiply(XtXi, Xt, cols, cols, cols, rows);
    double** W = multiply(XtXiXt, Y, cols, rows, rows, 1);
    fl=fopen(argv[2],"r");
    if(fl == NULL){
        printf("error\n");
        free(fl);
    }
    else{
        int vals = 0;
        fscanf(fl, "%d\n", &vals);
        for(int i = 0; i < vals; i++){
            double** param = malloc(sizeof(double*));    
            param[0] = malloc(cols * sizeof(double));
            param[0][0] = 1;
            for(int j = 1; j < cols-1; j++)
                fscanf(fl, "%lf,", &param[0][j]);
            fscanf(fl, "%lf\n", &param[0][cols - 1]);
            double** result = multiply(param, W, 1, cols, cols, 1);
            printf("%0.0lf\n", result[0][0]);
            freeMatrix(param, 1);
            freeMatrix(result, 1);
        }
    }
    fclose(fl);
    freeMatrix(Xt, cols);
    freeMatrix(XtX, cols);
    freeMatrix(XtXi, cols);
    freeMatrix(XtXiXt, cols);
    freeMatrix(W, cols);
    freeMatrix(X, rows);
    freeMatrix(Y, rows);
    return 0;
}

double** transpose(double** arr, int row, int col){
    double** Xt = malloc(col * sizeof(double*));
    for(int i = 0; i < col; i++)
        Xt[i] = malloc(row * sizeof(double));
    for(int i = 0; i < col; i++){
        for(int j = 0; j < row; j++){
            Xt[i][j] = arr[j][i];
        }
    }
    return Xt;
}
double** multiply(double** arr1, double** arr2, int row1, int col1, int row2, int col2){
    double** res = malloc(row1 * sizeof(double*));
    for(int i = 0; i < row1; i++) {
        res[i] = malloc(col2 * sizeof(double));
    }
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            res[i][j] = 0;
            for(int k = 0; k < row2; k++){
                res[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }
    return res;
}
double** inverse(double** a, int n){
    double** arr = malloc(n * sizeof(double*));
    for(int i = 0; i < n; i++)
        arr[i] = malloc(n * sizeof(double));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            arr[i][j] = a[i][j];
    double** res = malloc(n * sizeof(double*));
    for(int i = 0; i < n; i++)
        res[i] = malloc(n * sizeof(double));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            res[i][j] = 0;
            if(i == j)
                res[i][j] = 1;
        }
    }
    for(int i = 0; i < n; i++){
        double factor = arr[i][i];
        for(int j = 0; j < n; j++){
            arr[i][j] /= factor;
            res[i][j] /= factor;
        }
        for(int j = 0; j < n; j++){
            if(i == j)
                continue;
            double fac = arr[j][i];
            for(int k = 0; k < n; k++){
                arr[j][k] = arr[j][k] - arr[i][k] * fac;
                res[j][k] = res[j][k] - res[i][k] * fac;
            }
        }

    }
    freeMatrix(arr, n);
    return res;
}
void freeMatrix(double** arr, int n){
    for(int i = 0; i < n; i++){
        free(arr[i]);
    }
    free(arr);
}
