#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Block {
    int updateval;
    unsigned long long tag;
    int validbit;
}Block;
int memoryreads = 0;
int memorywrites = 0;
int cachehit = 0;
int cachemiss = 0;

int pow2(int a);
int main(int argc, char** argv){
    if(argc != 7){
        printf("error\n");
        return 0;
    }
    int cachesize = atoi(argv[1]);
    if(!pow2(cachesize)){
        printf("error\n");
        return 0;
    }
    int blocksize = atoi(argv[2]);
    if(!pow2(blocksize)){
        printf("error\n");
        return 0;
    }
    int pflag = 0;
    char* cachepolicy = argv[3];
    if(strcmp(cachepolicy, "fifo") && strcmp(cachepolicy, "lru")){
        printf("error\n");
        return 0;
    }
    if(!strcmp(cachepolicy, "lru")){
        pflag = 1;
    }
    char* associativityin = argv[4];
    int prefetchsize = atoi(argv[5]);
    FILE *fl;
    fl=fopen(argv[6],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    int associativity;
    if(!strcmp(associativityin, "direct")){
        associativity = 1;
    }
    else if(strstr(associativityin, "assoc:")){
        associativity = atoi(associativityin + 6);
        if(!associativity || !pow2(associativity)){
            printf("error\n");
            fclose(fl);
            return 0;
        }
    }
    else if(strstr(associativityin, "assoc")){
        associativity = cachesize / blocksize;
    }
    long long offsetbits = (long long)log2(blocksize);
    long long indexbits = (long long)log2(cachesize / (associativity * blocksize));
    long long tagbits = 48 - offsetbits - indexbits;
    
    Block** CacheSets = malloc((1 << indexbits) * sizeof(Block*));
    for(int i = 0; i < (1<<indexbits); i++){
        CacheSets[i] = malloc(associativity * sizeof(Block));
        for(int j = 0; j < associativity; j++){
            CacheSets[i][j].tag = 0;
            CacheSets[i][j].validbit = 0;
            CacheSets[i][j].updateval = 0;
        }
    }
    int i = 0;
    while(1){
        i++;
        char intype;
        unsigned long long address;
        char instuff[20];
        fscanf(fl, "%c %s\n", &intype, instuff);
        if(!strcmp(instuff, "eof")){
            fclose(fl);
            for(i = 0; i < (1 << indexbits); i++)
                free(CacheSets[i]);
            free(CacheSets);
            break;
        }
        sscanf(instuff, "0x%llx", &address); 
        long long indexmask = ((1 << indexbits) - 1) << offsetbits;
        unsigned long long tagmask = ((1LL << tagbits) - 1)  << (offsetbits + indexbits);
        
        unsigned long long tag = (address & tagmask) >> (offsetbits + indexbits);
        long long index = (address & indexmask) >> offsetbits;
        int fflag = 0;
        if(intype == 'R'){
            fflag = 0;
            for(int j = 0; j < associativity; j++){
                if(CacheSets[index][j].validbit && CacheSets[index][j].tag == tag){
                    if(pflag){
                        CacheSets[index][j].updateval = i;
                    }
                    cachehit++;
                    fflag = 1;
                    break;
                }
            }
            if(!fflag){
                cachemiss++;
                memoryreads++;
                int smallest = i;
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval < smallest)
                        smallest = CacheSets[index][j].updateval;
                }
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval == smallest){
                        CacheSets[index][j].updateval = i;
                        CacheSets[index][j].tag = tag;
                        CacheSets[index][j].validbit = 1;
                        break;
                    }
                }
            }
        }
        else {
            fflag = 0;
            for(int j = 0; j < associativity; j++){
                if(CacheSets[index][j].validbit && CacheSets[index][j].tag == tag){
                    if(pflag){
                        CacheSets[index][j].updateval = i;
                    }
                    cachehit++;
                    memorywrites++;
                    fflag = 1;
                    break;
                }
            }
            if(!fflag){
                cachemiss++;
                memorywrites++;
                memoryreads++;
                int smallest = i;
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval < smallest)
                        smallest = CacheSets[index][j].updateval;
                }
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval == smallest){
                        CacheSets[index][j].updateval = i;
                        CacheSets[index][j].tag = tag;
                        CacheSets[index][j].validbit = 1;
                        break;
                    }
                }
                
            }

        }
    }
    printf("no-prefetch\n");
    printf("Memory reads: %d\n", memoryreads);
    printf("Memory writes: %d\n", memorywrites);
    printf("Cache hits: %d\n", cachehit);
    printf("Cache misses: %d\n", cachemiss);
    
    memoryreads = 0;
    memorywrites = 0;
    cachehit = 0;
    cachemiss = 0;
    i = 0;
    fl=fopen(argv[6],"r");
    CacheSets = malloc((1 << indexbits) * sizeof(Block*));
    for(int k = 0; k < (1<<indexbits); k++){
        CacheSets[k] = malloc(associativity * sizeof(Block));
        for(int j = 0; j < associativity; j++){
            CacheSets[k][j].tag = 0;
            CacheSets[k][j].validbit = 0;
            CacheSets[k][j].updateval = 0;
        }
    }

    
    while(1){
        i++;
        char intype;
        unsigned long long address;
        char instuff[20];
        fscanf(fl, "%c %s\n", &intype, instuff);
        if(!strcmp(instuff, "eof")){
            fclose(fl);
            for(i = 0; i < (1 << indexbits); i++)
                free(CacheSets[i]);
            free(CacheSets);
            break;
        }
        sscanf(instuff, "0x%llx", &address);
        long long indexmask = ((1 << indexbits) - 1) << offsetbits;
        unsigned long long tagmask = ((1LL << tagbits) - 1)  << (offsetbits + indexbits);

        unsigned long long tag = (address & tagmask) >> (offsetbits + indexbits);
        long long index = (address & indexmask) >> offsetbits;
        int fflag = 0;
        if(intype == 'R'){
            fflag = 0;
            for(int j = 0; j < associativity; j++){
                if(CacheSets[index][j].validbit && CacheSets[index][j].tag == tag){
                    if(pflag)
                        CacheSets[index][j].updateval = i;
                    cachehit++;
                    fflag = 1;
                    break;
                }
            }
            if(!fflag){
                cachemiss++;
                memoryreads++;
                int smallest = i;
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval < smallest)
                        smallest = CacheSets[index][j].updateval;
                }
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval == smallest){
                        CacheSets[index][j].updateval = i;
                        CacheSets[index][j].tag = tag;
                        CacheSets[index][j].validbit = 1;
                        break;
                    }
                }
                unsigned long long preaddress = address;
                for(int e = 0; e < prefetchsize; e++){
                    i++;
                    int cflag = 0;
                    preaddress = preaddress + blocksize;
                    unsigned long long pretag = (preaddress & tagmask) >> (offsetbits + indexbits);
                    long long preindex = (preaddress & indexmask) >> offsetbits;
                    smallest = i;
                    for(int k = 0; k < associativity; k++){
                        if(CacheSets[preindex][k].tag == pretag){
                            cflag = 1;
                            break;
                        }
                        if(CacheSets[preindex][k].updateval < smallest)
                            smallest = CacheSets[preindex][k].updateval;
                    }
                    if(cflag)
                        continue;
                    for(int k = 0; k < associativity; k++){
                        if(CacheSets[preindex][k].updateval == smallest){
                            memoryreads++;
                            CacheSets[preindex][k].updateval = i;
                            CacheSets[preindex][k].tag = pretag;
                            CacheSets[preindex][k].validbit = 1;
                            break;
                        }
                    }
                }

            }
        }
        else {
            fflag = 0;
            for(int j = 0; j < associativity; j++){
                if(CacheSets[index][j].validbit && CacheSets[index][j].tag == tag){
                    if(pflag)
                        CacheSets[index][j].updateval = i;
                    cachehit++;
                    memorywrites++;
                    fflag = 1;
                    break;
                }
            }
            if(!fflag){
                cachemiss++;
                memorywrites++;
                memoryreads++;
                int smallest = i;
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval < smallest)
                        smallest = CacheSets[index][j].updateval;
                }
                for(int j = 0; j < associativity; j++){
                    if(CacheSets[index][j].updateval == smallest){
                        CacheSets[index][j].updateval = i;
                        CacheSets[index][j].tag = tag;
                        CacheSets[index][j].validbit = 1;
                        break;
                    }
                }
                unsigned long long preaddress = address;
                for(int e = 0; e < prefetchsize; e++){
                    i++;
                    int cflag = 0;
                    preaddress = preaddress + blocksize;
                    unsigned long long pretag = (preaddress & tagmask) >> (offsetbits + indexbits);
                    long long preindex = (preaddress & indexmask) >> offsetbits;
                    smallest = i;
                    for(int k = 0; k < associativity; k++){
                        if(CacheSets[preindex][k].tag == pretag){
                            cflag = 1;
                            break;
                        }
                        if(CacheSets[preindex][k].updateval < smallest)
                            smallest = CacheSets[preindex][k].updateval;
                    }
                    if(cflag)
                        continue;
                    for(int k = 0; k < associativity; k++){
                        if(CacheSets[preindex][k].updateval == smallest){
                            memoryreads++;
                            CacheSets[preindex][k].updateval = i;
                            CacheSets[preindex][k].tag = pretag;
                            CacheSets[preindex][k].validbit = 1;
                            break;
                        }
                    }
                }

            }

        }
    }
    printf("with-prefetch\n");
    printf("Memory reads: %d\n", memoryreads);
    printf("Memory writes: %d\n", memorywrites);
    printf("Cache hits: %d\n", cachehit);
    printf("Cache misses: %d\n", cachemiss);

    return 0;
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
