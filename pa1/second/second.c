#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
}Node;

void delete(Node **list, int val);
void print(Node *list);
void insert(Node **list, int val);
void freeList(Node **list);

int main(int argc, char** argv){
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n"); 
        return 0;
    }
    char c;
    int i;
    Node *list = NULL;
    while(fscanf(fl, "%c\t%d\n", &c, &i) != EOF){
        if(c == 'i'){
            insert(&list, i);
        }
        if(c == 'd'){
            delete(&list, i);
        }
    }
    print(list);
    freeList(&list);
    return 0;
}

void insert(Node **list, int val){
    Node *new = malloc(sizeof(Node));
    new->val = val;
    if(*list == NULL || (*list)->val > new->val){
        new->next = *list;
        *list = new;
        return;
    }
    Node *temp = *list;
    Node *prev;
    while(temp != NULL && temp->val < val){
        prev = temp;
        temp = temp->next;
    }
    if(temp != NULL && temp->val == val){
        free(new);
        return;
    }
    prev->next = new;
    new->next = temp;
}

void delete(Node **list, int val){
    Node *temp = *list;
    if(temp == NULL)
        return;
    if(temp->val == val){
        *list = temp->next;
        free(temp);
        return;
    }
    Node *prev;
    while(temp->next != NULL){
        prev = temp;
        temp = temp->next;
        if(temp->val == val){
            prev->next = temp->next;
            free(temp);
            return;
        }
    }
    return;
}

void print(Node *list){
    int nodes= 0;
    int values[500];
    while(list != NULL){
        values[nodes] = list->val;
        nodes++;
        list = list->next;
    }
    printf("%d\n",nodes);
    for(int i = 0; i < nodes; i++){
        printf("%d\t",values[i]);
    }
    printf("\n");
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
