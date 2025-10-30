#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1

typedef struct str_int_dict {
    char chave[100];
    int valor;
} Dictionary;

// Retorna 0 se começa, outros valores se for diferente
int strstartswith(char *str, char *prefix){ 
    int n = strlen(prefix);
    
    if (n == 0) return 1; //Não considera se o prefixo for vazio
    if (n > strlen(str)) return 1; //Pode causar erro, então já retorna o falso que a comparação retornaria
    return strncmp(str, prefix, n); //Compara apenas os primeiros N caracteres
}

int isLabel(char *linha){
    char *next_char;
    char *occurrence = strstr(linha, ":");
    if (occurrence == NULL) return 0; //Se não houver
    
    next_char = occurrence + 1; //Se houver, pega o caracter seguinte
    if (*next_char == '\0' || *next_char == ' ' || *next_char == '\n') return 1; //Compara com finais válidos

    return 0; //: foi usado na atribuição, não na declaração do label
}

void main(){
    FILE *cod_intermediario;
    cod_intermediario = fopen("Cod_Intermediario.pvm", "rt");
    int linha_counter = 0;
    char linha[100];
    int lbl_counter = 0, leader_counter = 0;
    int leaders[100];
    Dictionary *labels[100];

    //Primeira iteração: identificação e localização dos labels
    while (fgets(linha, 100, cod_intermediario)){
        if (DEBUG) printf("Linha: %s\n", linha);
        
        if ( isLabel(linha) ){ //Procura : sem nada depois (para não confundir com atribuição)
            char lbl_name[100];  
            strcpy(lbl_name, linha);
            strtok(lbl_name, ":"); //Já salva na própria string usada
            if (DEBUG) printf("Nome: %s\n", lbl_name);

            labels[lbl_counter] = (Dictionary*)malloc(sizeof(Dictionary));
            strcpy(labels[lbl_counter]->chave, lbl_name);
            labels[lbl_counter++]->valor = linha_counter; //Aproveita para incrementar
        } 
        
        linha_counter++;
        if (DEBUG) ("Passou por %d\n", linha_counter);
    }

    fseek(cod_intermediario, 0, SEEK_SET); //Retorna para o início do arquivo
    linha_counter = 0;
    //Segunda iteração: identificação de líderes (fica mais fácil em duas iterações, pois garante que todos os labels já foram identificados)
    while (fgets(linha, 100, cod_intermediario)){
        if (linha_counter == 0) leaders[leader_counter++] = linha_counter; //Primeira linha é líder
        if (DEBUG) printf("Linha: %s\n", linha);
        if (strstartswith(linha, "GOTO") == 0) {
            char *lbl_name, tmp[100];  
            strcpy(tmp, linha);
            strtok(tmp, " "); 
            lbl_name = strtok(NULL, "\n"); //Pega o que está até o fim da linha 

            for (int i = 0; i < lbl_counter; i++){
                if (DEBUG) printf("Nome procurado: '%s'; Nome atual: '%s'\n", lbl_name, labels[i]->chave);
                if (strcmp(labels[i]->chave, lbl_name) == 0){
                    leaders[leader_counter++] = labels[i]->valor; //Mantém a posição dos líderes  
                    break;
                }
            }
            //TODO: Pensar no que fazer se o label ainda não foi declarado
        } else if (strstartswith(linha, "GOFALSE") == 0) {
            char *lbl_name, tmp[100];  
            strcpy(tmp, linha);
            strtok(tmp, " "); 
            lbl_name = strtok(NULL, "\n"); //Pega o que está até o fim da linha 

            leaders[leader_counter++] = linha_counter+1; //Linha seguinte a um desvio condicional
            for (int i = 0; i < lbl_counter; i++){
                if (DEBUG) printf("Nome procurado: '%s'; Nome atual: '%s'\n", lbl_name, labels[i]->chave);
                if (strcmp(labels[i]->chave, lbl_name) == 0){
                    leaders[leader_counter++] = labels[i]->valor; //Alvo de desvio 
                    break;
                }
            }
        }
        
        linha_counter++;
        if (DEBUG) ("Passou por %d\n", linha_counter);
    }

    if (DEBUG){
        printf("Conjunto labels e as linhas que aparecem (começando em 0):\n");

        for (int i = 0; i < lbl_counter; i++){
            printf("%s: %d\n", labels[i]->chave, labels[i]->valor);
        }
    }

    if (DEBUG){
        printf("Linhas contendo líderes:\n");
        
        for (int i = 0; i < leader_counter; i++){
            printf("%d\n", leaders[i]);
        }
    }

    //Escrevendo saída com os blocos
    FILE *output = fopen("block_flux_graph.pvm", "wt");
    
}