#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
}Node;

int insert(Node **list, int val);
void freeList(Node **list);

int main(int argc, char** argv){
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    Node *list = NULL;
    int m;
    fscanf(fl, "%d\n", &m);
    int **mat = malloc(m * sizeof(int*));
    for(int i = 0; i < m; i++){
        mat[i] = malloc(m * sizeof(int));
    }
    for(int i = 0; i < m; i++){
        for(int j = 0; j < m - 1; j++){
            fscanf(fl, "%d\t", &mat[i][j]);
        }
        fscanf(fl, "%d\n", &mat[i][m-1]);
    }
    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            if(insert(&list, mat[i][j])==0){
                printf("not-magic\n");
                for(int k = 0; k < m; k++){
                     free(mat[k]);
                }
                free(mat);
                freeList(&list);
                return 0;
            }
        }
    }
    freeList(&list);
    int val = 0;
    for(int i = 0; i < m; i++){
        val += mat[0][i];
    }
    for(int i = 0; i < m; i++){
        int c = 0;
        int d = 0;
        for(int j = 0; j < m; j++){
            c += mat[i][j];
            d += mat[j][i];
        }
        if(c != d || d != val){
            for(int f = 0; f < m; f++){
                free(mat[f]);
            }
            free(mat);
            printf("not-magic\n");
            return 0;
        }
    }
    int x = 0;
    int y = 0;
    for(int i = 0; i < m; i++){
        x += mat[i][i];
        y += mat[i][m - i - 1];
    }
    if(x == y && y == val)
        printf("magic\n");
    else
        printf("not-magic\n");
    for(int i = 0; i < m; i++){
        free(mat[i]);
    }
    free(mat);
    return 0;
}

int insert(Node **list, int val){
    Node *new = malloc(sizeof(Node));
    new->val = val;
    if(*list == NULL || (*list)->val > new->val){
        new->next = *list;
        *list = new;
        return 1;
    }
    Node *temp = *list;
    Node *prev = temp;
    while(temp != NULL && temp->val < val){
        prev = temp;
        temp = temp->next;
    }
    if(temp != NULL && temp->val == val){
        free(new);
        return 0;
    }
    prev->next = new;
    new->next = temp;
    return 1;
}

void freeList(Node **list){
    Node *temp = *list;
    Node *prev;
    if(temp == NULL)
        return;
    while(temp->next != NULL){
        prev = temp;
        temp = temp->next;
        free(prev);
    }
    free(temp);
}

