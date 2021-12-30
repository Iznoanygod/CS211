#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Table{
    char key[100];
    int val;
}Table;

void freeTable(Table* t);
int has(Table* t, int tin, char key[]);
Table* sort(Table* t, Table* input, int tin, int in);
Table* put(Table* t, int tin, char key[], int val);
int get(Table* t, int tin, char key[]);
void solve(Table* in, int ina, Table* out, int outa, Table* circ, int circa, int gcode);
int mux(int bits, int* muxin, int* muxselect);
int* dec(int bits, int* decin);
int and(int arg1, int arg2);
int or(int arg1, int arg2);
int xor(int arg1, int arg2);
int nand(int arg1, int arg2);
int nor(int arg1, int arg2);
int xnor(int arg1, int arg2);
int not(int arg);
int* gcodegen(int bits);
int* gtob(int bits, int* grey);

int main(int argc, char** argv){
    FILE *fl;
    fl=fopen(argv[1],"r");
    if(fl == NULL){
        printf("error\n");
        return 0;
    }
    int ins;
    fscanf(fl, "INPUTVAR %d", &ins);
    Table* inputtable = malloc(ins * sizeof(Table));
    for(int i = 0; i < ins; i++){
        char inp[100];
        fscanf(fl, "%s", inp);
        strcpy(inputtable[i].key, inp);
    }
    int outs;
    fscanf(fl, "\nOUTPUTVAR %d", &outs);
    Table* outputtable = malloc(outs * sizeof(Table));
    for(int i = 0; i < outs; i++){
        char out[100];
        fscanf(fl, "%s", out);
        strcpy(outputtable[i].key, out);
    }
    Table* circuittable = malloc(sizeof(Table));
    int circs = 0;
    for(int i = 0; 1; i++){
        char temp[100];
        if(fscanf(fl, "%s", temp) != 1){
            break;
        }
        circuittable = realloc(circuittable, (i + 1) * sizeof(Table));
        circs++;
        if(strcmp(temp, "AND") == 0 || strcmp(temp, "OR") == 0 || strcmp(temp, "NAND") == 0 || strcmp(temp, "NOR") == 0 || strcmp(temp, "XOR") == 0 || strcmp(temp, "XNOR") == 0){
            char a[100];
            strcat(temp, " ");
            fscanf(fl, "%s", a);
            strcat(temp, a);
            strcat(temp, " ");
            fscanf(fl, "%s", a);
            strcat(temp, a);
            strcat(temp, " ");
            fscanf(fl, "%s", a);
            strcat(temp, a);
            strcpy(circuittable[i].key, temp);
        }
        else if(strcmp(temp, "NOT") == 0){
            char a[100];
            strcat(temp, " ");
            fscanf(fl, "%s", a);
            strcat(temp, a);
            strcat(temp, " ");
            fscanf(fl, "%s", a);
            strcat(temp, a);
            strcpy(circuittable[i].key, temp);
        }
        else if(strcmp(temp, "MULTIPLEXER") == 0){
            char a[100];
            strcat(temp, " ");
            int bits;
            fscanf(fl, "%d", &bits);
            sprintf(a, "%d", bits);
            strcat(temp, a);
            for(int j = 0; j < bits; j++){
                strcat(temp, " ");
                fscanf(fl, "%s", a);
                strcat(temp, a);
            }
            for(int j = 0; j < (int)(log2(bits));j++){
                strcat(temp, " ");
                fscanf(fl, "%s", a);
                strcat(temp, a);
            }
            strcat(temp, " ");
            fscanf(fl, "%s", a);
            strcat(temp, a);
            strcpy(circuittable[i].key, temp);
        }
        else if(strcmp(temp, "DECODER") == 0){
            char a[100];
            strcat(temp, " ");
            int bits;
            fscanf(fl, "%d", &bits);
            sprintf(a, "%d", bits);
            strcat(temp, a);
            for(int j = 0; j < bits; j++){
                strcat(temp, " ");
                fscanf(fl, "%s", a);
                strcat(temp, a);
            }
            for(int j = 0; j < 1 << bits; j++){
                strcat(temp, " ");
                fscanf(fl, "%s", a);               
                strcat(temp, a);
            }
            strcpy(circuittable[i].key, temp);
        }
    }
    circuittable = sort(circuittable, inputtable, circs, ins);
    
    int* gcode = gcodegen(ins);
    for(int i = 0; i < 1 << ins; i++){
        solve(inputtable, ins, outputtable, outs, circuittable, circs, gcode[i]);
    }
    freeTable(inputtable);
    freeTable(outputtable);
    freeTable(circuittable);
    free(gcode);
    fclose(fl);
    return 0;
}

void solve(Table* in, int ina, Table* out, int outa, Table* circ, int circa, int gcode){
    for(int i = ina - 1; i > -1; i--){
        in[i].val = gcode & 1;
        gcode = gcode >> 1;
    }
    Table* vars = malloc(sizeof(Table) * (ina + 2));
    for(int i = 0; i < ina; i++){
        printf("%d ", in[i].val);
        strcpy(vars[i].key, in[i].key);
        vars[i].val = in[i].val;
    }
    strcpy(vars[ina].key, "0");vars[ina].val = 0;
    strcpy(vars[ina+1].key, "1"); vars[ina+1].val = 1;
    int size = ina + 2;
    for(int i = 0; i < circa; i++){
        char a[50];
        sscanf(circ[i].key, "%s", a);
        if(strcmp(a, "AND") == 0){
            char arg1[50], arg2[50], outp[50];
            sscanf(circ[i].key, "%*s %s %s %s", arg1, arg2, outp);
            int temp = and(get(vars, size, arg1), get(vars, size, arg2));
            vars = put(vars, size, outp, temp);
            size++;
        }
        else if(strcmp(a, "OR") == 0){
            char arg1[50], arg2[50], outp[50];
            sscanf(circ[i].key, "%*s %s %s %s", arg1, arg2, outp);
            int temp = or(get(vars, size, arg1), get(vars, size, arg2));
            vars = put(vars, size, outp, temp);
            size++;
        }
        else if(strcmp(a, "NAND") == 0){
            char arg1[50], arg2[50], outp[50];
            sscanf(circ[i].key, "%*s %s %s %s", arg1, arg2, outp);
            int temp = nand(get(vars, size, arg1), get(vars, size, arg2));
            vars = put(vars, size, outp, temp);
            size++;
        }
	    else if(strcmp(a, "NOR") == 0){
	        char arg1[50], arg2[50], outp[50];
            sscanf(circ[i].key, "%*s %s %s %s", arg1, arg2, outp);
            int temp = nor(get(vars, size, arg1), get(vars, size, arg2));
            vars = put(vars, size, outp, temp);
            size++;
	    }
	    else if(strcmp(a, "XOR") == 0){
            char arg1[50], arg2[50], outp[50];
            sscanf(circ[i].key, "%*s %s %s %s", arg1, arg2, outp);
            int temp = xor(get(vars, size, arg1), get(vars, size, arg2));
            vars = put(vars, size, outp, temp);
            size++;
	    }
	    else if(strcmp(a, "XNOR") == 0){
            char arg1[50], arg2[50], outp[50];
            sscanf(circ[i].key, "%*s %s %s %s", arg1, arg2, outp);
            int temp = xnor(get(vars, size, arg1), get(vars, size, arg2));
            vars = put(vars, size, outp, temp);
            size++;
        }
        else if(strcmp(a, "NOT") == 0){
            char arg1[50], arg2[50];
            sscanf(circ[i].key, "%*s %s %s", arg1, arg2);
            int temp = not(get(vars, size, arg1));
            vars = put(vars, size, arg2, temp);
            size++;
        }
        else if(strcmp(a, "MULTIPLEXER") == 0){
            int bit;
            char argl[100];
            sscanf(circ[i].key, "MULTIPLEXER %d %[^\n]", &bit, argl);
            char tesr[100];
            int* min = malloc(bit * sizeof(int));
            for(int j = 0; j < bit; j++){
                sscanf(argl, "%s %[^\n]", tesr, argl); 
                min[j] = get(vars, size, tesr);
            }
            int* sel = malloc((int)(log2(bit)) * sizeof(int));
            for(int j = 0; j < (int)(log2(bit)); j++){
                sscanf(argl, "%s %[^\n]", tesr, argl);
                sel[j] = get(vars, size, tesr);
            }
            char outp[100];
            sscanf(argl, "%s", outp);
            int temp = mux(bit, min, sel);
            vars = put(vars, size, outp, temp);
            free(min);
            free(sel);
            size++;
        }
        else if(strcmp(a, "DECODER") == 0){
            int bit;
            char argl[100];
            sscanf(circ[i].key, "DECODER %d %[^\n]", &bit, argl);
            char tesr[100];
            int* din = malloc(bit * sizeof(int));
            for(int j = 0; j < bit; j++){
                sscanf(argl, "%s %[^\n]", tesr, argl);
                din[j] = get(vars, size, tesr);
            }
            char dout[100][100];
            for(int j = 0; j < 1 << bit; j++){
                sscanf(argl, "%s %[^\n]", dout[j], argl);
            }
            int* drec = dec(bit, din);
            for(int j = 0; j < 1 << bit; j++){
                vars = put(vars, size, dout[j], drec[j]);
                size++;
            }
            free(din);
            free(drec);
        }
    }
    for(int i = 0; i < outa; i++){
        char temp[50];
        strcpy(temp, out[i].key);
        printf("%d ", get(vars, size, temp));
    }
    printf("\n");
    freeTable(vars);
}

int has(Table* t, int tin, char key[]){
    if(strcmp(key, "1") == 0 || strcmp(key, "0") == 0)
        return 1;
    for(int i = 0; i < tin; i++){
        if(strcmp(t[i].key, key) == 0)
            return 1;
    }
    return 0;
}

Table* sort(Table* t, Table* input, int tin, int in){
    Table* vals = malloc(in * sizeof(Table));
    for(int i = 0; i < in; i++){
        vals[i] = input[i];
    }
    int sorted = 1;
    for(int i = 0; 1; i = (i + 1) % tin){
        char op[100];
        sscanf(t[i].key, "%s", op);
        if(strcmp(op, "AND") == 0 || strcmp(op, "OR") == 0 || strcmp(op, "NAND") == 0 || strcmp(op, "NOR") == 0 || strcmp(op, "XOR") == 0 || strcmp(op, "XNOR") == 0){
            char arg1[100], arg2[100], out[100];
            sscanf(t[i].key, "%*s %s %s %s", arg1, arg2, out);
            if(has(vals, in, arg1) && has(vals, in, arg2)){
                vals = put(vals, in, out, 0);
                in++;
            }
            else{
                Table a = t[i];
                for(int j = i; j < tin - 1; j++){
                    t[j] = t[j + 1];
                }
                i--;
                t[tin - 1] = a;
                sorted = 0;
            }
        }
        else if(strcmp(op, "NOT") == 0){
            char arg1[100], arg2[100];
            sscanf(t[i].key, "%*s %s %s", arg1, arg2);
            if(has(vals, in, arg1)){
                vals = put(vals, in, arg2, 0);
                in++;
            }
            else{
                Table a = t[i];
                for(int j = i; j < tin - 1; j++){
                    t[j] = t[j + 1];
                }
                i--;
                t[tin - 1] = a;
                sorted = 0;
            }
        }
        else if(strcmp(op, "MULTIPLEXER") == 0){
            int bit;
            char argl[100];
            sscanf(t[i].key, "MULTIPLEXER %d %[^\n]", &bit, argl);
            char tesr[100];
            int check = 1;
            for(int j = 0; j < bit; j++){
                sscanf(argl, "%s %[^\n]", tesr, argl); 
                if(!has(vals, in, tesr)){
                    check = 0;
                }
            }
            for(int j = 0; j < (int)(log2(bit)); j++){
                sscanf(argl, "%s %[^\n]", tesr, argl);
                if(!has(vals, in, tesr)){
                    check = 0;
                }
            }
            char outp[100];
            sscanf(argl, "%s", outp);
            if(check == 0){
                Table a = t[i];
                for(int j = i; j < tin - 1; j++){
                    t[j] = t[j + 1];
                }
                i--;
                t[tin - 1] = a;
                sorted = 0;
            }
            else{
                vals = put(vals, in, outp, 0);
                in++;
            }
        }
        else if(strcmp(op, "DECODER") == 0){
            int bit;
            char argl[100];
            sscanf(t[i].key, "DECODER %d %[^\n]", &bit, argl);
            char tesr[100];
            int check = 1;
            for(int j = 0; j < bit; j++){
                sscanf(argl, "%s %[^\n]", tesr, argl);
                if(!has(vals, in, tesr)){
                    check = 0;
                }
            }
            char dout[100][100];
            for(int j = 0; j < 1 << bit; j++){
                sscanf(argl, "%s %[^\n]", dout[j], argl);
            }
            if(check == 0){
                Table a = t[i];
                for(int j = i; j < tin - 1; j++){
                    t[j] = t[j + 1];
                }
                i--;
                t[tin-1] = a;
                sorted = 0;
            }
            else{
                for(int j = 0; j < 1 << bit; j++){
                    vals = put(vals, in, dout[j], 0);
                    in++;
                }
            }

        }
        if(i + 1 == tin && sorted){
            break;
        }
        else
            sorted = 1;
    }
    free(vals);
    return t;
}

Table* put(Table* t, int tin, char key[], int val){
    t = realloc(t, sizeof(Table) * (tin+1));
    strcpy(t[tin].key, key);
    t[tin].val = val;
    return t;
}

int get(Table* t, int tin, char key[]){
    for(int i = 0; i < tin; i++){
        if(strcmp(t[i].key, key) == 0){
            return t[i].val;
        }
    }
    return -1;
}

void freeTable(Table* t){
    free(t);
}



int mux(int bits, int* muxin, int* muxselect){
    int* bin = gtob((int)log2(bits), muxselect);
    int temp = 0;
    for(int i = 0; i <(int)log2(bits); i++){
        temp = temp << 1;
        temp = temp + bin[i];
    }
    free(bin);
    return muxin[temp];
}

int* dec(int bits, int decin[]){
    int* bin = gtob(bits, decin);
    int* output = calloc(1 << bits, sizeof(int));
    int temp = 0;
    for(int i = 0; i < bits; i++){
        temp = temp << 1;
        temp = temp + bin[i];
    }
    output[temp] = 1;
    free(bin);
    return output;
}

int and(int arg1, int arg2){
    return (arg1 & arg2);
}
int or(int arg1, int arg2){
    return (arg1 | arg2);
}
int xor(int arg1, int arg2){
    return (arg1 ^ arg2);
}
int nand(int arg1, int arg2){
    return 1 & !(arg1 & arg2);
}
int nor(int arg1, int arg2){
    return 1 & !(arg1 | arg2);
}
int xnor(int arg1, int arg2){
    return 1 & !(arg1 ^ arg2);
}
int not(int arg){
    return 1 & !arg;
}
int* gcodegen(int bits){
    int* out = malloc(sizeof(int) * (1 << bits));
    out[0] = 0;
    out[1] = 1;
    if(bits == 1)
        return out;
    int add = 1;
    for(int i = 1; i < bits; i++){
        add = add << 1;
        for(int j = 0; j < (1 << i); j++){
            out[2 * add - j - 1] = out[j] + add;
        }
    }
    return out;
}

int* gtob(int bits, int* grey){
    int* binary = malloc(sizeof(int) * bits);
    binary[0] = grey[0];
    for(int i = 1; i < bits; i++){
        binary[i] = binary[i - 1] ^ grey[i];
    }
    return binary;
}
