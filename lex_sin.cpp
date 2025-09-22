#define DEBUG_MODE 1

#define TKId 1
#define TKVoid 2
#define TKInt 3
#define TKFloat 4
#define TKVirgula 5
#define TKDoisPontos 6
#define TKAbrePar 7
#define TKFechaPar 8
#define TKAtrib 9
#define TKPontoEVirgula 10
#define TKAbreChaves 11
#define TKFechaChaves 12
#define TKMais 13
#define TKDuploMais 14
#define TKProd 15
#define TKChar 16
#define TKSub 17
#define TKAbreColchete 18
#define TKFechaColchete 19
#define TKAtribMais 20
#define TKDouble 21
#define TKCteInt 22
#define TKElse 23
#define TKIf 24
#define TKString 25
#define TKBitNot 26
#define TKDiv 27
#define TKAtribDiv 28
#define TKPonto 29
#define TKAndLog 30
#define TKAtribAnd 31
#define TKAnd 32
#define TKConstChar 33
#define TKNot 34
#define TKNotEqual 35
#define TKOrLog 36
#define TKAtribOr 37
#define TKOr 38
#define TKLessThan 39
#define TKLessOrEqualThan 40 
#define TKGreaterThan 41
#define TKGreaterOrEqualThan 42 
#define TKXOR 43
#define TKXOREquals 44
#define TKLeftShift 45
#define TKAtribLeftShift 46
#define TKRightShift 47
#define TKAtribRightShift 48
#define TKPtrOpr 49
#define TKEquals 50
#define TKMod 51
#define TKAtribMod 52
#define TKSwitch 53
#define TKCase 54
#define TKDefault 55
#define TKDo 56
#define TKWhile 57
#define TKFor 58
#define TKEOF 59
#define TKStatic 60
#define TKConst 61
#define TKCteFloat 62
#define TKDuploMenos 63
#define TKAtribProd 64
#define TKAtribMenos 66

#define false 0
#define true 1

#define MAX_CODE 3000

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

char tokens[100][30] = {
    "TKId",            // 1
    "TKVoid",          // 2
    "TKInt",           // 3
    "TKFloat",         // 4
    "TKVirgula",       // 5
    "TKDoisPontos",    // 6
    "TKAbrePar",       // 7
    "TKFechaPar",      // 8
    "TKAtrib",         // 9
    "TKPontoEVirgula", // 10
    "TKAbreChaves",    // 11
    "TKFechaChaves",   // 12
    "TKMais",          // 13
    "TKDuploMais",     // 14
    "TKProd",          // 15
    "TKChar",          // 16
    "TKSub",           // 17
    "TKAbreColchete",  // 18
    "TKFechaColchete", // 19
    "TKAtribMais",     // 20
    "TKDouble",        // 21
    "TKCteInt",        // 22
    "TKElse",          // 23
    "TKIf",            // 24
    "TKString",        // 25
    "TKBitNot",        // 26
    "TKDiv",           // 27
    "TKAtribDiv",      // 28
    "TKPonto",         // 29
    "TKAndLog",        // 30
    "TKAtribAnd",      // 31
    "TKAnd",           // 32
    "TKConstChar",     // 33
    "TKNot",           // 34
    "TKNotEqual",      // 35
    "TKOrLog",         // 36
    "TKAtribOr",       // 37
    "TKOr",            // 38
    "TKLessThan",      // 39
    "TKLessOrEqualThan", // 40
    "TKGreaterThan",   // 41
    "TKGreaterOrEqualThan", // 42
    "TKXOR",           // 43
    "TKXOREquals",     // 44
    "TKLeftShift",     // 45
    "TKLeftShiftAssign", //46
    "TKRightShift",     // 47
    "TKRightShiftAssign", //48
    "TKPtrOpr",          //49
    "TKEquals",          //50
    "TKMod",             //51
    "TKModAssign",       //52
    "TKSwitch",          //53
    "TKCase",            //54
    "TKDefault",         //55
    "TKDo",              //56
    "TKWhile",           //57
    "TKFor",              //58
    "TKEOF",              //59
    "TKEStatoc",              //60
    "TKConst",              //61
    "TKCteFloat",              //62
    "TKAtribProd",          //63
    "TKDuploMenos",              //64
};  

int pos = 0;

int tk;
int label_counter = 0, temp_counter = 0;
char last_decl_id[64];
char lex[500];
int lin=1, col=0, curr_col;
FILE *arqin, *arq_intermediario;
char c; // �ltimo caracter lido do arquivo
FILE *arqout=fopen("Saida.txt","wt");

char* gera_label(){
    char *result;
    result = (char*)malloc(7);
    sprintf(result, "lbl%.3d", label_counter++);
    return result;
}

char* gera_temp(){
    char *result;
    result = (char*)malloc(5);
    sprintf(result, "T%.3d", temp_counter++);
    return result;
}

struct pal_res{char palavra[20]; int tk;};
struct pal_res lista_pal[]={{"void",TKVoid},
                  {"int",TKInt},
                  {"float",TKFloat},
                  {"char",TKChar},
                  {"double",TKDouble},
                  {"else",TKElse},
                  {"if",TKIf},
                  {"switch",TKSwitch},
                  {"case",TKCase},
                  {"default",TKDefault},
                  {"do",TKDo},
                  {"while",TKWhile},
                  {"for",TKFor},
                  {"fimtabela",TKEOF},
                  {"static",TKStatic},
                  {"const",TKConst}};


int palavra_reservada(char lex[])
{
    int postab=0;
    while (strcmp("fimtabela",lista_pal[postab].palavra)!=0)
    {
    if (strcmp(lex,lista_pal[postab].palavra)==0)
        return lista_pal[postab].tk;
    postab++;
    }
    return TKId;
}

void setColor(int color) {
    // Obtém o handle do console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Define a cor do texto (atributo)
    SetConsoleTextAttribute(hConsole, color);
}

void getToken(); // prot�tipos
void proxC();

// vari�veis globais para retrocesso

typedef struct contexto{long posglobal;
    int tkant;
    char cant;
    char lexant[20];
    int coluna;
} tcontexto;

tcontexto pilhacon[1000];
int topcontexto=0;
int backtrack_mode = 0;

tcontexto marcaPosToken() {
	pilhacon[topcontexto].posglobal=ftell(arqin);
	pilhacon[topcontexto].tkant=tk;
	pilhacon[topcontexto].cant=c;
    strcpy(pilhacon[topcontexto].lexant,lex);
    pilhacon[topcontexto].coluna=col;
    topcontexto++;
    return pilhacon[topcontexto-1];
    // backtrack_mode = 1; //indica que esta fazendo ma analise com o backtracking e nao deve exibir erros
}

void restauraPosToken() {
    topcontexto--;
	fseek(arqin,pilhacon[topcontexto].posglobal,SEEK_SET);
    c=pilhacon[topcontexto].cant;
	tk=pilhacon[topcontexto].tkant;
    strcpy(lex,pilhacon[topcontexto].lexant);
    col = pilhacon[topcontexto].coluna;

    backtrack_mode = 0; //indica que parou de fazer a analise do backtracking e pode voltar a mostrar erros
}

void proxC()
{
if (feof(arqin)) {
   c=-1;
//   printf("Chegou no fim de arquivo\n");
   return;
   }
fread(&c,1,1,arqin);
col++;
if (c=='\n'){ 
    lin++; col=0; curr_col=1;}
// else if (c==' ') col++;
//printf("Leu caracter %c\n",c);
}

void getToken2()
{
int estado=0,
    fim=0,
    posl=0;
    while (!fim)
    {
        if(c!=-1) lex[posl++]=c;
        switch(estado){
            case 0:
             curr_col = col;
            //  if (c =='\n') return;
             if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_')
                {proxC();estado=1;break;}
             if (c>='0' && c<='9')
                {
                    int floating_constant = 0;
                    while (c>='0' && c<='9'){
                        proxC();
                        if(c=='.'){
                            if (floating_constant != 0){
                                // goto erro_lexico;
                            }else{
                                lex[posl++] = c;
                                proxC();
                                floating_constant++;
                            }
                        }
                        if (c>='0' && c<='9')
                            lex[posl++] = c;

                    } 
                    lex[posl]='\0';
                    tk= floating_constant ? TKCteFloat : TKCteInt;
                    return;
                }
             if (c=='"')
                {
                proxC();
                while (c!='"')
				   {
				   lex[posl++]=c;
				   proxC();
				   }
                proxC();
                lex[posl]='\0';
                tk=TKString;
                return;
                }
             if (c=='\'')
                {
                proxC();
                while (c!='\'')
				   {
				   lex[posl++]=c;
				   proxC();
				   }
                lex[posl++]=c;
                lex[posl]='\0';
                tk=TKConstChar;
                proxC();
                return;
                }
             if (c=='='){
                proxC();
                if(c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKEquals;return;}
                else{lex[posl]='\0';tk=TKAtrib;/*printf("Reconheceu token TKAtrib\n");*/return;}
             }
             if (c=='%'){
                proxC();
                if(c == '>'){lex[posl++]='>';lex[posl]='\0';proxC();tk=TKFechaChaves;return;}
                else if(c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKAtribMod;return;}
                else {lex[posl]='\0';tk=TKMod;/*printf("Reconheceu token TKAtrib\n");*/return;}
             }
             if (c=='.'){lex[posl]='\0';proxC();tk=TKPonto;return; }
             if (c=='<'){
                proxC();
                if (c == '<'){ //shift
                    lex[posl++] = '<';
                    proxC();
                    if (c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKAtribLeftShift;return;}
                    else {lex[posl]='\0';tk=TKLeftShift;return;}
                }
                else if (c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKLessOrEqualThan;return;}
                else if (c == '%'){lex[posl++]='%';lex[posl]='\0';proxC();tk=TKAbreChaves;return;}
                else{ 
                    lex[posl]='\0';tk=TKLessThan;return; 
                }
             }
             if (c=='>'){
                proxC();
                if (c == '>'){ //shift
                    lex[posl++] = '>';
                    proxC();
                    if (c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKAtribRightShift;return;}
                    else {lex[posl]='\0';tk=TKRightShift;return;}
                }
                else if (c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKGreaterOrEqualThan;return;}
                else{ 
                    lex[posl]='\0';tk=TKGreaterThan;return; 
                }
             }
             if (c=='^')
             {
                proxC();
                if(c=='='){
                    lex[posl++]='='; lex[posl]='\0'; proxC(); tk=TKXOREquals; return;
                }
                else{lex[posl]='\0';tk=TKXOR;return; }
             }
             if (c=='+')
			    {
			    proxC();
				if (c=='+')
			       {
				   lex[posl++]='+';
     			   lex[posl]='\0';
				   proxC();
				   tk=TKDuploMais;/*printf("Reconheceu token TKDuploMais\n");*/return;
				   }
				else if (c=='=')
			       {
				   lex[posl++]='=';
     			   lex[posl]='\0';
				   proxC();
				   tk=TKAtribMais;/*printf("Reconheceu token TKDuploMais\n");*/return;
				   }
                else
				   {
	               lex[posl]='\0';
				   tk=TKMais;/*printf("Reconheceu token TKSoma\n");*/return;
				   }
			    }
             if (c=='/'){
                proxC();
                if (c=='/'){ //é um comentário em linha - deve ignorar
                    while(c!='\n'){
                        proxC();
                    }
                }
                else if (c=='*'){ //é um comentário em multiplas linhas - deve ignorar
                    int ended = 0;
                    while(ended == 0){
                        proxC();
                        while(c!='*'){
                            proxC();
                        }
                        proxC();
                        if(c=='/'){
                            proxC();
                            posl = 0;
                            ended = 1;
                            break;
                        }
                    }
                    break;
                }
                else if (c=='='){ //atribuição com divisao
                    lex[posl++]='=';lex[posl]='\0';tk=TKAtribDiv;proxC();return;
                }
                else {
                    lex[posl]='\0';
                    tk=TKDiv;/*printf("Reconheceu token TKDiv\n");*/return;
                }
             }
             if (c=='&'){
                proxC();
                if(c=='&'){
                    lex[posl++]='&';
                    lex[posl]='\0';
                    proxC();
                    tk=TKAndLog;
                    return;
                }
                else if(c=='='){
                    lex[posl++]='=';
                    lex[posl]='\0';
                    proxC();
                    tk=TKAtribAnd;
                    return;
                }else{
                    lex[posl]='\0';
                    tk=TKAnd;
                    // proxC();
                    return;
                }
             }
             if (c=='|'){
                proxC();
                if(c=='|'){
                    lex[posl++]='|';
                    lex[posl]='\0';
                    tk=TKOrLog;
                    proxC();
                    return;
                }
                else if(c=='='){
                    lex[posl++]='=';
                    lex[posl]='\0';
                    tk=TKAtribOr;
                    proxC();
                    return;
                }else{
                    lex[posl]='\0';
                    tk=TKOr;
                    // proxC();
                    return;
                }
             }
             if (c=='!'){
                proxC();
                if(c=='='){
                    lex[posl++]='=';
                    lex[posl]='\0';
                    tk=TKNotEqual;
                    proxC();
                    return;
                }
                else {
                    lex[posl]='\0';
                    tk=TKNot;
                    return;
                }
             }
             if (c=='-'){
                proxC();
                if (c == '>'){lex[posl++]='>';lex[posl]='\0';proxC();tk=TKPtrOpr;/*printf("Reconheceu token TKSub\n");*/return;}
                else if (c == '-') {lex[posl++]='-';lex[posl]='\0';proxC();tk=TKDuploMenos;return;}
                else if (c == '=') {lex[posl++]='=';lex[posl]='\0';proxC();tk=TKAtribMenos;return;}
                else{lex[posl]='\0';tk=TKSub;/*printf("Reconheceu token TKSub\n");*/return;}
            }
            if (c=='*'){
                proxC();
                if (c == '='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKAtribProd; return;}
                else{lex[posl]='\0';tk=TKProd;return;}
            }
             if (c=='['){lex[posl]='\0';proxC();tk=TKAbreColchete;/*printf("Reconheceu token TKAbrePar\n");*/return;}
             if (c==']'){lex[posl]='\0';proxC();tk=TKFechaColchete;/*printf("Reconheceu token TKAbrePar\n");*/return;}
             if (c=='('){lex[posl]='\0';proxC();tk=TKAbrePar;/*printf("Reconheceu token TKAbrePar\n");*/return;}
             if (c==')'){lex[posl]='\0';proxC();tk=TKFechaPar;/*printf("Reconheceu token FechaPar\n");*/return;}
             if (c=='{'){lex[posl]='\0';proxC();tk=TKAbreChaves;/*printf("Reconheceu token TKAbreChaves\n");*/return;}
             if (c=='}'){lex[posl]='\0';proxC();tk=TKFechaChaves;/*printf("Reconheceu token TKFechaChaves\n");*/return;}
             if (c==','){lex[posl]='\0';proxC();tk=TKVirgula;/*printf("Reconheceu token TKVirgula\n");*/return;}
             if (c==';'){lex[posl]='\0';proxC();tk=TKPontoEVirgula;/*printf("Reconheceu token TKPontoEVirgula\n");*/return;}
             if (c==':'){lex[posl]='\0';proxC();tk=TKDoisPontos;/*printf("Reconheceu token TKDoisPontos\n");*/return;}
             if (c=='~'){lex[posl]='\0';proxC();tk=TKBitNot;/*printf("Reconheceu token TKDoisPontos\n");*/return;}
             if (c==-1){
                printf("Arquivo acabando....");
                if (posl > 1) {  // já leu alguma coisa, finalize o token atual
                    printf("   ....   ");

                    lex[--posl] = '\0';  // remove o -1, termina string
                    if (estado == 1) {
                        tk = palavra_reservada(lex);
                    } else {
                        tk = TKId; // ou mantenha conforme a categoria apropriada
                    }
                    return;
                } else {  // nada foi lido, pode emitir TKEOF
                    printf("Acabou");
                    tk = TKEOF;
                    return;
                }
             }
             if (c==' ' || c=='\n' || c=='\t' || c=='\r') {proxC();posl--;break;}
             if (c=='\0') {tk=-1;return;}
            //  erro_lexico: //a beleza dos rotulos e do goto
             setColor(4);
             printf("Erro lexico: encontrou o caracter %c (%d) na linha %d\n",c,c,lin);
             fprintf(arqout,"Erro lexico: encontrou o caracter %c (%d) na linha %d\n",c,c,lin);
             setColor(7); //padrao (branco)
             exit(0);
             posl--;
             while (c!='\n') proxC();
             break;
      case 1:if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9') {proxC();break;}
             lex[--posl]='\0';
             int prev_tk = tk;
             tk=palavra_reservada(lex);
             if(TKId != tk){
                //setColor(2); //verde
                // col--;
            }
             else{
                //setColor(3); //azul
             }  
             //printf("reconheceu token %s\n",lex);
             return;
      } //switch
   }// while
}// fun��o

void getToken(){
    // printf("%s\n", tokens[tk-1]);
    getToken2();
    fprintf(arqout, "%s identified in '%s', at line %d and column %d\n", tokens[tk-1], lex, lin, curr_col);
}

int errored = 0;

void error(const char expected_token[]){
    if (errored == 0 && backtrack_mode == 0){
        setColor(4);
        printf("ERRO encontrado na linha %d, coluna %d: esperava token %s, mas encontrou %s em %s\n", lin, col, expected_token, tokens[tk-1], lex);
        fprintf(arqout,"ERRO encontrado na linha %d, coluna %d: esperava token %s, mas encontrou %s em %s\n", lin, col, expected_token, tokens[tk-1], lex);
        setColor(7);

        errored = 1;
    }
}

int expression(char place[MAX_CODE], char code[MAX_CODE]);

int primary_expression(char place[MAX_CODE], char code[MAX_CODE]){
    // printf("  _%s_  ", tokens[tk-1]);
    if (DEBUG_MODE) printf("Primary expression\n");
    if (tk == TKId || tk == TKCteInt || tk == TKCteFloat || tk == TKConstChar){
        if (tk == TKId){
            strcpy(place, lex); //lex é o id
            strcpy(code, ""); //lex é o id
        }else if (tk == TKCteInt || tk == TKCteFloat || tk == TKConstChar){
            char *temp = gera_temp();
            strcpy(place, temp);
            sprintf(code, "%s:=%s", temp, lex); //lex é constante
        }

        getToken(); //verifica aqui dentro porque ja comeca com o primeiro token lido. Entao esta sempre analisando "atrasado"
        return 1;
    }else if (tk == TKAbrePar){
        getToken();
        char e_place[MAX_CODE], e_code[MAX_CODE];
        if(expression(e_place, e_code)){
            if(tk == TKFechaPar){
                getToken();
                strcpy(place, e_place);
                strcpy(code, e_code);
                return 1;
            }
        }
    }
    
    error("Identifier or Constant");
    return 0;
}

int assignment_expression();
int postfix_expression(char place[MAX_CODE], char code[MAX_CODE]);
int pointer();

int argument_expression_list(char code[MAX_CODE]){
    if (DEBUG_MODE) printf("argument_expression_list");
    int retorno = 0;
    char e_place[MAX_CODE], e_code[MAX_CODE];
    if (expression(e_place, e_code)){
        sprintf(code, "%s\nparam %s", e_code, e_place);
        retorno = 1;
        while(tk == TKVirgula){
            getToken();
            if(!expression(e_place, e_code))
            { 
                retorno = 0; 
                break; 
            }
            sprintf(code, "%s\nparam %s", e_code, e_place);
        }
    }

    return retorno;
}

int postfix_expression(char place[MAX_CODE], char code[MAX_CODE]){ 
    if (DEBUG_MODE) printf("POSTFIX\n");

    // NÃO redeclare place/code aqui! Use os parâmetros de saída.
    if (!primary_expression(place, code)) return 0;

    // Loop para múltiplos sufixos em cadeia: a[i](...) .x ->y ++ ...
    while (1){
        if (tk == TKAbreColchete){                // E1 [ E2 ]
            getToken();
            char idx_place[MAX_CODE], idx_code[MAX_CODE];
            if (!expression(idx_place, idx_code)){
                error("expression inside []");
                return 0;
            }
            if (tk != TKFechaColchete){
                error("]");
                return 0;
            }
            getToken();

            // t := base [ idx ]
            char *t = gera_temp();
            sprintf(code, "%s\n%s\n%s := %s [ %s ]", code, idx_code, t, place, idx_place);
            strcpy(place, t);
            continue;
        }
        else if (tk == TKAbrePar){                // chamada de função
            // BACKTRACKING já feito por você; aqui assumo caminho "válido"
            tcontexto cont = marcaPosToken();
            getToken(); // consome '('

            int narg = 0;
            char args_code[MAX_CODE]; args_code[0] = '\0';

            if (tk == TKFechaPar){               // chamada vazia f()
                getToken();
            } else {
                // tenta parse + geração de params
                if (argument_expression_list(args_code) || tk != TKFechaPar){
                    // restaura se falhar
                    fseek(arqin,cont.posglobal,SEEK_SET);
                    c=cont.cant; tk=cont.tkant;
                    strcpy(lex,cont.lexant); col = cont.coluna;
                    return 0;
                }
                getToken(); // consome ')'

                // anexa código dos args
                sprintf(code, "%s\n%s", code, args_code);
            }

            // place contém o "nome" da função se primary era TKId; se não, é um temp com ponteiro p/ função.
            // Para função nomeada, call <place>, narg
            char *tret = gera_temp();
            sprintf(code, "%s\ncall %s, %d\n%s := RET", code, place, narg, tret);
            strcpy(place, tret);
            continue;
        }
        else if (tk == TKPonto){                  // E . id
            getToken();
            if (tk != TKId){
                error("Identifier after '.'");
                return 0;
            }
            char field[MAX_CODE]; strcpy(field, lex);
            getToken();

            char *t = gera_temp();
            sprintf(code, "%s\n%s := %s . %s", code, t, place, field);
            strcpy(place, t);
            continue;
        }
        else if (tk == TKPtrOpr){                 // E -> id
            getToken();
            if (tk != TKId){
                error("Identifier after '->'");
                return 0;
            }
            char field[MAX_CODE]; strcpy(field, lex);
            getToken();

            char *t = gera_temp();
            sprintf(code, "%s\n%s := %s -> %s", code, t, place, field);
            strcpy(place, t);
            continue;
        }
        else if (tk == TKDuploMais || tk == TKDuploMenos){   // pós ++ / --
            int op = tk;
            getToken();

            if (op == TKDuploMais)
                sprintf(code, "%s\nINC %s", code, place);
            else
                sprintf(code, "%s\nDEC %s", code, place);

            // strcpy(place, temp);
            continue;
        }
        else break;
    }

    return 1;
}

int unary_expression(char place[MAX_CODE], char code[MAX_CODE]){
    if (tk == TKAnd || tk == TKSub || tk == TKMais || tk == TKNot || tk == TKBitNot || tk == TKDuploMais || tk == TKDuploMenos || tk == TKProd){
        char un_op[MAX_CODE];
        strcpy(un_op, lex);
        getToken();

        char rhs_place[MAX_CODE], rhs_code[MAX_CODE];
        // return unary_expression(rhs_place, rhs_code); // chamada recursiva para o operando
        if (!unary_expression(rhs_place, rhs_code)){
            error("missing operand for unary operator");
            return 0;
        }

        strcpy(code, rhs_code);
        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s %s %s", code, rhs_code, temp, place, un_op, rhs_place); //ESTÁ ERRADO, CORRIGIR
        strcpy(place, temp);
        printf("%s", code);
        return 1;
    }

    return postfix_expression(place, code); // caso não tenha operador unário, segue para expressão pós-fixada
}

//------------Outros tipos de expressão------------------

int multiplicative_expression(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Multiplicative statement\n");
    char un_place[MAX_CODE], un_code[MAX_CODE];
    if (!unary_expression(un_place, un_code)) {
        return 0;
    }
    strcpy(place, un_place);
    strcpy(code,  un_code);

    // marcaPosToken();
    while (tk == TKProd || tk == TKDiv || tk == TKMod){
        char mult_op[MAX_CODE];
        strcpy(mult_op, lex);  
        getToken();            

        char un1_place[MAX_CODE], un1_code[MAX_CODE];
        if (!unary_expression(un1_place, un1_code)){
            error("expression after one of the following: *, / or %");
            return 0;
        }

        char *t = gera_temp();
        sprintf(code, "%s\n%s\n%s := %s %s %s", code, un1_code, t, place, mult_op, un1_place);
        strcpy(place, t);
    }

    // restauraPosToken();
    return 1;
}

int additive_expression(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Additive statement\n");
    char multex_place[MAX_CODE], multex_code[MAX_CODE];

    if (!multiplicative_expression(multex_place, multex_code)) return 0;

    strcpy(code, multex_code);
    strcpy(place, multex_place);
    //a cada novo + concatena as instruções 
    while (tk == TKMais || tk == TKSub){
        char add_op[MAX_CODE];
        strcpy(add_op, lex);
        getToken();

        char multex1_place[MAX_CODE], multex1_code[MAX_CODE];
        if (!multiplicative_expression(multex1_place, multex1_code)){
            error("expression after one of the following: + or -");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s %s %s", code, multex1_code, temp, place, add_op, multex1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int bitwise_shift(char place[MAX_CODE], char code[MAX_CODE]){
    char addex_place[MAX_CODE], addex_code[MAX_CODE];
    if (!additive_expression(addex_place, addex_code)) return 0;

    strcpy(code, addex_code);
    strcpy(place, addex_place);
    while (tk == TKLeftShift || tk == TKRightShift){
        char shift_op[MAX_CODE];
        strcpy(shift_op, lex);
        getToken();
        char addex1_place[MAX_CODE], addex1_code[MAX_CODE];

        if (!additive_expression(addex1_place, addex1_code)) {
            error("expression after one of the following: << or >>");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s %s %s", code, addex1_code, temp, place, shift_op, addex1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int relational_operators(char place[MAX_CODE], char code[MAX_CODE]){
    char shift_place[MAX_CODE], shift_code[MAX_CODE];
    if (!bitwise_shift(shift_place, shift_code)) return 0;

    // char new_place[MAX_CODE], new_code[MAX_CODE];
    strcpy(code, shift_code);
    strcpy(place, shift_place);
    while (tk == TKLessThan || tk == TKLessOrEqualThan || tk == TKGreaterThan || tk == TKGreaterOrEqualThan){
        char comparison[MAX_CODE];
        strcpy(comparison,lex);
        
        getToken();
        char shift1_place[MAX_CODE], shift1_code[MAX_CODE];
        if (!bitwise_shift(shift1_place, shift1_code)) {
            error("expression after one of the following: + or -");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s %s %s", code, shift1_code, temp, place, comparison, shift1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int relational(char place[MAX_CODE], char code[MAX_CODE]){
    char relops_place[MAX_CODE], relops_code[MAX_CODE];
    if (!relational_operators(relops_place, relops_code)) return 0;

    char new_place[MAX_CODE], new_code[MAX_CODE];
    strcpy(code, relops_code);
    strcpy(place, relops_place);
    while (tk == TKEquals || tk == TKNotEqual){
        char comparison[MAX_CODE];
        strcpy(comparison,lex);
        getToken();
        char relops1_place[MAX_CODE], relops1_code[MAX_CODE];
        if (!relational_operators(relops1_place, relops1_code)) {
            error("expression after one of the following: == or !=");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s %s %s", code, relops1_code, temp, place, comparison, relops1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int bitwise_and(char place[MAX_CODE], char code[MAX_CODE]){
    char relational_place[MAX_CODE], relational_code[MAX_CODE];
    if (!relational(relational_place, relational_code)) return 0;

    char new_place[MAX_CODE], new_code[MAX_CODE];
    strcpy(code, relational_code);
    strcpy(place, relational_place);
    while (tk == TKAnd){
        getToken();
        char relational1_place[MAX_CODE], relational1_code[MAX_CODE];
        if (!relational(relational_place, relational_code)) {
            error("expression after &");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s & %s", code, relational1_code, temp, place, relational1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int bitwise_xor(char place[MAX_CODE], char code[MAX_CODE]){
    char band_place[MAX_CODE], band_code[MAX_CODE];
    if (!bitwise_and(band_place, band_code)) return 0;

    char new_place[MAX_CODE], new_code[MAX_CODE];
    
    strcpy(code, band_code);
    strcpy(place, band_place);
    while (tk == TKXOR){
        getToken();
        char band1_place[MAX_CODE], band1_code[MAX_CODE];
        if (!bitwise_and(band_place, band_code)) {
            error("expression after ^");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s XOR %s", code, band1_code, temp, place, band1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int bitwise_or(char place[MAX_CODE], char code[MAX_CODE]){
    char bxor_place[MAX_CODE], bxor_code[MAX_CODE];
    if (!bitwise_xor(bxor_place, bxor_code)) return 0;

    char new_place[MAX_CODE], new_code[MAX_CODE];

    strcpy(code, bxor_code);
    strcpy(place, bxor_place);
    while (tk == TKOr){
        getToken();
        char xor1_place[MAX_CODE], xor1_code[MAX_CODE];
        if (!bitwise_xor(xor1_place, xor1_code)) {
            error("expression after |");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s | %s", code, xor1_code, temp, place, xor1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int logical_and(char place[MAX_CODE], char code[MAX_CODE]){
    char bor_place[MAX_CODE], bor_code[MAX_CODE];
    if (!bitwise_or(bor_place, bor_code)) return 0;

    char new_place[MAX_CODE], new_code[MAX_CODE];

    strcpy(code, bor_code);
    strcpy(place, bor_place);
    while (tk == TKAndLog){
        getToken();
        char addex1_place[MAX_CODE], addex1_code[MAX_CODE];
        if (!additive_expression(addex1_place, addex1_code)) {
            error("expression after &&");
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s AND %s", code, addex1_code, temp, place, addex1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}

int logical_or(char place[MAX_CODE], char code[MAX_CODE]){
    char and_place[MAX_CODE], and_code[MAX_CODE];
    if (!logical_and(and_place, and_code)) return 0; //Doesn't call error() because it's not necessarily an error, as it could be that the code ended or simply this expression is not being a part of it
    
    char new_code[MAX_CODE], new_place[MAX_CODE];
    
    strcpy(code, and_code);
    strcpy(place, and_place);
    while (tk == TKOrLog){
        getToken();
        char and1_place[MAX_CODE], and1_code[MAX_CODE];
        if (!logical_and(and1_place, and1_code)) {
            error("expression after ||"); //This error will most likely never be called as well, since TKPontoEVirgula will generate an error first
            return 0;
        }

        char *temp = gera_temp();
        sprintf(code, "%s\n%s\n%s:=%s OR %s", code, and1_code, temp, place, and1_place);
        strcpy(place, temp);
        printf("%s", code);
    }

    return 1;
}


int assignment_expression(char place[MAX_CODE], char code[MAX_CODE]){
    tcontexto cont = marcaPosToken();
    if (DEBUG_MODE) printf("Assignment expression\n");
    if (tk == TKId){
        char id[50];
        strcpy(id, lex);
        getToken();
        if(tk == TKAtrib || tk ==TKAtribAnd || tk ==TKAtribDiv || tk ==TKAtribLeftShift
            ||  tk ==TKAtribMais ||  tk ==TKAtribMenos || tk ==TKAtribMod || tk ==TKAtribOr
            || tk ==TKAtribProd  || tk ==TKAtribRightShift){ 
            getToken();
            //sprintf(code, "valor-l %s\n", id);
            char ae1_place[MAX_CODE], ae1_code[MAX_CODE];
            
            if(assignment_expression(ae1_place, ae1_code)){   
                // char code[256];
                sprintf(code, "%s\n%s:=%s", ae1_code, id, ae1_place);
                strcpy(place, id);
                // printf("%s", code);

                // fprintf(arq_intermediario, code);
                return 1;
            }
        }
    }

    // restauraPosToken();
    fseek(arqin,cont.posglobal,SEEK_SET);
    c=cont.cant;
    tk=cont.tkant;
    strcpy(lex,cont.lexant);
    col = cont.coluna;
    return logical_or(place, code); 
}

//Avalia as duas expressões, descarta primeira
int comma(char place[MAX_CODE], char code[MAX_CODE]){
    char ae_place[MAX_CODE], ae_code[MAX_CODE];
    if (!assignment_expression(place, code)) return 0;

    while (tk == TKVirgula){
        getToken();
        if (!assignment_expression(ae_place, ae_code)){
            error("expression after ','");
            return 0;
        }
        strcpy(place, ae_place);
        sprintf(code, "%s\n%s", code, ae_code);
        printf("%s", code);
        //place da última
    }

    return 1;
}

int expression(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Expression\n");
    if (comma(place, code)){ //Já manda o place como referência
        // sprintf(code,"%s", code);
        // fprintf(arq_intermediario, code);
        return 1;
    }
    
    
    return 0;
}

/*int pointer(){
    if (tk == TKProd){
        getToken();
        while (tk == TKProd) getToken();
        return 1;
    }

    return comma();
}*/

int statement_list(char place[MAX_CODE], char code[MAX_CODE]);
int statement(char place[MAX_CODE], char code[MAX_CODE]);

int compound_statement(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Compound statement\n");
    if (tk == TKAbreChaves){
        getToken();
        char st_place[MAX_CODE], st_code[MAX_CODE];
        if(tk == TKFechaChaves){
            getToken();
            return 1;
        } 
        else{
            while (tk != TKFechaChaves){
                if (statement(st_place, st_code)){ //statement_list(st_place, st_code)
                    // strcpy(code, st_code);
                    sprintf(code, "%s\n%s", code, st_code);
                    strcpy(place, st_place);
                    // getToken();
                }else error("'}' (compound)");;
            }

            return 1;
        } 
    } else error("'{' (compound)");
    
    return 0;
}

int expression_statement(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Expression statement\n");
    char e_place[MAX_CODE], e_code[MAX_CODE];
    if (tk == TKPontoEVirgula){
        getToken();
        return 1;
    }else if (expression(e_place, e_code)){
        if (tk == TKPontoEVirgula){
            strcpy(place, e_place);
            strcpy(code, e_code);
            getToken();
            return 1;
        }
    }
    
    error("';' (expression)");
    return 0;
}

int iteration_statement(char place[MAX_CODE], char code[MAX_CODE]){ 
    if (DEBUG_MODE) printf("Iteration statement\n");
    char e_place[MAX_CODE], e_code[MAX_CODE];
    char st_place[MAX_CODE], st_code[MAX_CODE];
    char initializing_code[MAX_CODE];
    char loop_code[MAX_CODE];
    char increase_code[MAX_CODE];

    if (tk == TKFor){
        getToken();
        if (tk == TKAbrePar){
            getToken();
            if(expression_statement(e_place, e_code)){
                sprintf(initializing_code, "%s", e_code);
                strcpy(e_code, "");
                strcpy(e_place, "");
                if (expression_statement(e_place, e_code)){
                    sprintf(loop_code, "%s", e_code);
                    strcpy(e_code, "");
                    strcpy(e_place, "");
                    if (tk == TKFechaPar){
                        getToken();
                        char st_place[MAX_CODE], st_code[MAX_CODE];
                        if (statement(st_place, st_code)){
                            // printf("=====\nSTATEMENT RETORNOU: %s\n======\n", st_code);
                            
                            // sprintf(statement_code, "%s", st_code);
                            strcpy(code, "");
                            if (strcmp(initializing_code, "") != 0){
                                printf("============================\nINITIALIZING CODE É ---%s--- PODE CRER\n===============================\n", initializing_code);
                                char *label_inicio = gera_label();
                                sprintf(code, "%s:\n %s", label_inicio, initializing_code);    
                            }

                            char *label_loop = gera_label();
                            char *label_end = gera_label();

                            if (strcmp(loop_code, "") != 0){
                                sprintf(code, "%s\n%s: %s\nGOFALSE %s", code, label_loop, loop_code, label_end);
                            }else{
                                sprintf(code, "%s\n%s:", code, label_loop);
                            }
                            
                            sprintf(code, "%s\n%s\nGOTO %s\n%s:", code, label_loop, st_code, label_end);
                            fprintf(arq_intermediario, code);
                            return 1;
                        }
                    }else {
                        if (expression(e_place, e_code)){
                            sprintf(increase_code, "%s", e_code);
                            if (tk == TKFechaPar){
                                getToken();
                                if (statement(st_place, st_code)){
                                    
                                    strcpy(code, "");
                                    if (strcmp(initializing_code, "") != 0){
                                        char *init_label = gera_label();
                                        printf("============================\nINITIALIZING CODE É ---%s--- PODE CRER\n===============================\n", initializing_code);
                                        sprintf(code, "%s:\n%s", init_label, initializing_code);    
                                    }

                                    char *loop_label = gera_label();
                                    char *inc_label = gera_label();
                                    char *end_label = gera_label();

                                    if (strcmp(loop_code, "") != 0){
                                        sprintf(code, "%s\n%s: %s\nGOFALSE %s", code, loop_label, loop_code, end_label);
                                    }else{
                                        sprintf(code, "%s\n%s:", code, loop_label);
                                    }
                                    sprintf(code, "%s\n%s\n%s:%s\nGOTO %s\n%s:", code, st_code, inc_label, increase_code, loop_label, end_label);
                                    //fprintf(arq_intermediario, code);
                                    return 1;
                                }
                            }
                        } else error("')' (iteration)");
                    }
                }else error("");
                
            }else error("')' (iteration)");
        }else error("'(' (iteration)");
    }else if (tk == TKWhile){
        getToken();
        if (tk == TKAbrePar){
            getToken();
            if (expression(e_place, e_code)){
                if(tk == TKFechaPar){
                    char st_place[MAX_CODE], st_code[MAX_CODE];
                    getToken();
                    if (statement(st_place, st_code)){                        
                        return 1;
                    } 
                }
            }
        }
    }else if (tk == TKDo){
        char st_place[MAX_CODE], st_code[MAX_CODE];
        getToken();
        if (statement(st_place, st_code)){
            if (tk == TKWhile){
                getToken();
                if (tk == TKAbrePar){
                    getToken();
                    if (expression(e_place, e_code)){
                        if (tk == TKFechaPar){
                            getToken();
                            if (tk == TKPontoEVirgula){
                                getToken();
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    } 
    
    else error("'if' (iteration)");

    return 0;
}

int selection_statement(char place[MAX_CODE], char code[MAX_CODE]){ 
    if (DEBUG_MODE) printf("Selection statement\n");
    char e_place[MAX_CODE], e_code[MAX_CODE];
    if (tk == TKIf){
        getToken();
        if (tk == TKAbrePar){
            getToken();
            if(expression(e_place, e_code)){
                if (tk == TKFechaPar){
                    getToken();
                    char st_place[MAX_CODE], st_code[MAX_CODE];
                    if (statement(st_place, st_code)){
                        return 1;
                    }
                }else error("')' (selection)");
            }else error("')' (selection)");
        }else error("'(' (selection)");
    }else error("'if' (selection)");

    return 0;
}

int declaration_specifiers(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Declaration Specifiers \n");
    if (tk == TKVoid || tk == TKInt || tk == TKFloat || tk == TKChar || tk == TKDouble){ //type_specifier
        getToken();
        return 1;
    }else if (tk == TKConst){ //type_qualifier
        getToken();
        declaration_specifiers(place, code);
        return 1;
    }else if (tk == TKStatic){ //storage_class_specifier
        getToken();
        declaration_specifiers(place, code);
        return 1;
    }

    return 0;
}

int declarator(char place[MAX_CODE], char code[MAX_CODE]);

int direct_declarator(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Direct Declarator\n");
    int retorno = 0;
    if (tk == TKId){
        strcpy(last_decl_id, lex);
        getToken();
        retorno = 1;
        // return 1;
    }else if (tk == TKAbrePar){
        getToken();
        if (declarator(place, code)){
            if (tk == TKFechaPar){
                getToken();
                retorno = 1;
                // return 1;
            }else error(")");
        }else error("Declaration");
    }

    char e_place[MAX_CODE], e_code[MAX_CODE];
    while (1){ //enquanto conseguir encontrar novas combinações
        if (tk == TKAbreColchete){ //vetores
            getToken();
            if (tk == TKFechaColchete){
                getToken();
                retorno = 1;
            }else if (postfix_expression(e_place, e_code)){ //teoricamente tinha que passar por umas 15 recursões antes de chegar aqui, mas também sou filho de Deus
                if (tk == TKFechaColchete){
                    getToken();
                    retorno = 1;
                }
            }
        } else if (tk == TKAbrePar){ //funçoes
            getToken();
            if (tk == TKFechaPar){
                getToken();
                retorno = 1;
            } 

            if (declaration_specifiers(place, code)){
                if (declarator(place, code)) {
                    retorno = 1;

                    // continua analisando possíveis parâmetros adicionais
                    while (tk == TKVirgula){
                        getToken();
                        if (declaration_specifiers(place, code)){
                            if (!declarator(place, code)) {
                                error("param declarator");
                                return 0;
                            }
                        } else{
                            error("param specifier");
                            return 0;
                        }
                    }

                    if (tk == TKFechaPar){
                        getToken();
                    } else {
                        error("')' na lista de parâmetros");
                        return 0;
                    }

                    compound_statement(place, code);
                } else restauraPosToken(); // declarator falhou
            } else restauraPosToken(); // specifier falhou 
        }
        else break;
    }

    return retorno;
}

int declarator(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Declarator\n");
    if (tk == TKProd){ //*
        getToken();
        while(tk == TKProd) getToken();
        if (direct_declarator(place, code)) return 1;
    }else if (direct_declarator(place, code)) return 1;

    return 0;
}
int init_declarator(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Init Declarator\n");
    char e_place[MAX_CODE], e_code[MAX_CODE];
    if (declarator(place, code)){
        if (tk == TKAtrib){
            getToken();
            if (expression(e_place, e_code)){
                char code[MAX_CODE];
                sprintf(code, "%s:=%s", last_decl_id, e_place); //inicializa com 0 se não tiver inicializador
                fprintf(arq_intermediario, code);
                printf("%s", code);
                return 1; //initializer
            } 
        }       

        return 1;
    }
   
    return 0;
}

int init_declarator_list(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Init Declarator List\n");
    int declared = 0;
    while (init_declarator(place, code)){
        declared = 1;
        if(tk == TKVirgula){
            getToken();
        } else break; //se não for virgula, não poderia continuar declarando
    }

    return declared;
}

int declaration(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Declaration\n");
    if(declaration_specifiers(place, code)){
        if (tk == TKPontoEVirgula){getToken();return 1;}
        else if (init_declarator_list(place, code)){
            if (tk == TKPontoEVirgula){
                getToken();
                return 1;
            } 
        }
    }

    return 0;
}

int statement(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("Statement\n");
    if (tk == TKFechaChaves) {
        // se for o fim de um bloco, não é um statement e pode retornar imediatamente. Não precisa consumir no entanto, outra chamada irá cuidar disso. Essa verificação é só para evitar erros propagados
        return 0;
    }
    else if(tk == TKIf){
        if (selection_statement(place, code)){
            return 1;
        }
    }else if(tk == TKFor || tk == TKWhile || tk == TKDo){
        if (iteration_statement(place, code)){
            return 1;
        }
    }else if (tk == TKAbreChaves){
        if (compound_statement(place, code)){
            return 1;
        }
    }  
    
    // marcaPosToken();
    if (declaration(place, code)){
        return 1;
    }
    // restauraPosToken();

    // marcaPosToken();
    if (expression_statement(place, code)){
        return 1;
    }
    // restauraPosToken();
    
    
    return 0;
}


int statement_list(char place[MAX_CODE], char code[MAX_CODE]){
    if (DEBUG_MODE) printf("statement list");

    int success = 0;

    char s_place[MAX_CODE], s_code[MAX_CODE];
    strcpy(s_code, "");
    while (tk != TKEOF && statement(s_place, s_code)) {
        if (strcmp(code,"") != 0 || strcmp(s_code,"") != 0)
            sprintf(code, "%s\n%s", code, s_code);
        success = 1; // reconheceu pelo menos um statement
    }

    if (errored) return 0;
    return success;
}

int main()
{
	arqin=fopen("Fonte.cpp","rb");
	if (!arqin) {
        fprintf(arqout, "Erro na abertura do fonte.\n");
		printf("Erro na abertura do fonte.\n");
		return 0;
	}
    arq_intermediario=fopen("Cod_Intermediario.pvm","wt");
	proxC(); // l� primeiro caracter do arquivo
	getToken(); // ja comeca com primeiro token
	/*while (c!=-1)
	{
        printf("%s identified in '%s', at line %d and column %d\n", tokens[tk-1], lex, lin, curr_col);
        fprintf(arqout, "%s identified in '%s', at line %d and column %d\n", tokens[tk-1], lex, lin, curr_col);
        setColor(7);
        // curr_col = col;
		getToken();
	}*/
    // if (P())
    char place[MAX_CODE];
    char code[MAX_CODE];
    strcpy(code, "");
    strcpy(place, "");
    if (statement_list(place, code)){
        printf("Compilado com sucesso!");
        fprintf(arq_intermediario, code);
    }
    // else printf("Faca os ajustes no token %s (%s) e tente compilar novamente", tokens[tk-1], lex);

    fclose(arq_intermediario);
}