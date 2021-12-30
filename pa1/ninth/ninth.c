#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *left, *right;
}Node;

void insert(Node **tree, int val);
void search(Node **tree, int val);
void freeTree(Node **tree);
void delete(Node **tree, int val);

int main(int argc, char** argv){
     FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    char c;
    int i;
    Node *tree = NULL;
    while(fscanf(fl, "%c\t%d\n", &c, &i) != EOF){
        if(c == 'i'){
            insert(&tree, i);
        }
        if(c == 's'){
            search(&tree, i);
        }
        if(c == 'd'){
            delete(&tree, i);
        }
    }
    freeTree(&tree);
    return 0;
}
void insert(Node **tree, int val){
    Node *new = malloc(sizeof(Node));
    new->val = val;
    new->left = NULL;
    new->right = NULL;
    if(*tree == NULL){
        *tree = new;
        printf("inserted 1\n");
        return;
    }
    Node *temp = *tree;
    int h = 2;
    while(temp != NULL){
        if(temp->val == val){
            printf("duplicate\n");
            free(new);
            return;
        }
        else if(temp->val > val){
            if(temp->left == NULL){
                temp->left = new;
                printf("inserted %d\n", h);
                return;
            }
            else{
                temp = temp->left;
            }
        }
        else if(temp->val < val){
            if(temp->right == NULL){
                temp->right = new;
                printf("inserted %d\n", h);
                return;
            }
            else{
                temp = temp->right;
            }
        }
        h += 1;
    }
}

void search(Node **tree, int val){
    int h = 1;
    Node *temp = *tree;
    while(temp != NULL){
        if(temp->val == val){
            printf("present %d\n", h);
            return;
        }
        else if(temp->val > val){
            temp = temp->left;
        }
        else if(temp->val < val){
            temp = temp->right;
        }
        h++;
    }
    printf("absent\n");
}

void freeTree(Node **tree){
    if(*tree == NULL){
        return;
    }
    Node *temp = *tree;
    freeTree(&temp->left);
    freeTree(&temp->right);
    free(temp);
}

void delete(Node **tree, int val){
    Node *temp = *tree;
    Node *prev;
    Node *root = *tree;
    while(temp != NULL){
        if(temp->val == val){
            printf("success\n");
            if(temp == *tree){
                if(temp->left == NULL && temp->right == NULL){
                    *tree = NULL;
                    free(temp);
                    return;
                }
                if(temp->left == NULL){
                    *tree = temp->right;
                    free(temp);
                    return;
                }
                if(temp->right == NULL){
                    *tree = temp->left;
                    free(temp);
                    return;
                }
                if(temp->right->left == NULL){
                    temp->right->left = temp->left;
                    *tree = temp->right;
                    free(temp);
                    return;
                }
                temp = temp->right;
                while(temp->left != NULL){
                    prev = temp;
                    temp = temp->left;
                }
                prev->left = temp->right;
                temp->left = root->left;
                temp->right = root->right;
                free(root);
                *tree = temp;
                return;
            }
            if(prev->left == temp){
                if(temp->left == NULL && temp->right == NULL){
                    prev->left = NULL;
                    free(temp);
                    return;
                }   
                if(temp->left == NULL){
                    prev->left = temp->right;
                    free(temp);
                    return;
                }   
                if(temp->right == NULL){
                    prev->left = temp->left;
                    free(temp);
                    return;
                }
                if(temp->right->left == NULL){
                   temp->right->left = temp->left;
                   prev->left = temp->right;
                   free(temp);
                   return;
                }
                Node *tempt = temp->right;
                Node *prevt;
                while(tempt->left != NULL){
                    prevt = tempt;
                    tempt = tempt->left;
                }
                prevt->left = tempt->right;
                tempt->left = temp->left;
                tempt->right = temp->right;
                prev->left = tempt;
                free(temp);
                return;
            }
            if(prev->right == temp){
                if(temp->left == NULL && temp->right == NULL){
                    if(temp->left == NULL){
                        prev->right = NULL;
                        free(temp);
                        return;
                    }       
                }   
                if(temp->left == NULL){
                    prev->right = temp->right;
                    free(temp);
                    return;
                }   
                if(temp->right == NULL){
                    prev->right = temp->left;
                    free(temp);
                    return;
                }
                if(temp->right->left == NULL){
                    temp->right->left = temp->left;
                    prev->right = temp->right;
                    free(temp);
                    return;
                }
                Node *tempt = temp->right;
                Node *prevt;
                while(tempt->left != NULL){
                    prevt = tempt;
                    tempt = tempt->left;
                }
                prevt->left = tempt->right;
                tempt->left = temp->left;
                tempt->right = temp->right;
                prev->right = tempt;
                free(temp);
                return;
            }
        }
        else if(temp->val > val){
            prev = temp;
            temp = temp->left;
        }
        else if(temp->val < val){
            prev = temp;
            temp = temp->right;
        }
    }
    printf("fail\n");
}
