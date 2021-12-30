#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
     int val;
     struct Node *next;
}Node;

int hash(int val);
void insert(Node** list, int val);
void search(Node** list, int val);
void freeTable(Node** table);
void freeList(Node **list);

int main(int argc, char** argv){
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    Node** table = malloc(1000*sizeof(Node*));
    for(int i = 0; i < 1000; i++){
       table[i] = NULL;
    }
    char c;
    int i;
    while(fscanf(fl, "%c\t%d\n", &c, &i) != EOF){
        int tabVal = hash(i);
        if(c == 'i'){
            insert(&table[tabVal], i);
        }
        if(c == 's'){
            search(&table[tabVal], i);
        }
    }
    freeTable(table);
    return 0;
}

int hash(int val){
    return (val % 1000 + 1000) % 1000;
}

void search(Node** list, int val){
    if(*list == NULL){
        printf("absent\n");
        return;
    }
    Node *temp = *list;
    for(;temp != NULL && temp->val <= val;temp=temp->next){
        if(temp->val == val){
            printf("present\n");
            return;
        }
    }
    printf("absent\n");
}

void insert(Node** list, int val){
    Node *new = malloc(sizeof(Node));
    new->val = val;
    if(*list == NULL || (*list)->val > new->val){
        new->next = *list;
        *list = new;
        printf("inserted\n");
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
        printf("duplicate\n");
        return;
    }
    prev->next = new;
    new->next = temp;
    printf("inserted\n");
}

void freeTable(Node **table){
    for(int i = 0; i < 1000; i++){
        freeList((table + i));
    }
    free(table);
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

