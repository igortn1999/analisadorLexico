#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Analisador lexico Inicial 4ECA
//Referencia da Apostila 2.1.1
typedef struct{
    int tipo;
    int valor;
}Token;
//Struct para Tabela de simbolos 
//Representa o no de uma lista encadeada
struct lista{
    int tipo;
    int valor;
    int linha;
    int coluna;
    struct lista* next; //ponteiro para proximo no
};
typedef struct lista Tabela;

Tabela* t;

#define FALSE 0
#define TRUE 1

#define TOK_NUM 0
#define TOK_OP 1  //operacoes aritmeticas
#define TOK_PONT 2 //sinais de pontuacao

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define openPAR 0
#define closePAR 1

const char *ops="+-*/";
char *codigo;
int tamanho;
int pos;

Tabela* cria(void){
    return NULL;
}

Tabela* add(Tabela* t,Token *tok,int linha,int coluna){
    Tabela* no = (Tabela*) malloc(sizeof(Tabela));
    no->tipo = tok->tipo;
    no->valor = tok->valor;
    no->linha = linha;
    no->coluna = coluna;
    no->next = t;
    return no;
}

void inicializa_analise(char *prog){
	//void inicializa_analise(entrada){
    //quando se passa un vetor como parametro para uma função só pelo nome, se passa o endereço do indice zero, que inicia o vetor
    codigo=prog;
    strlen(codigo);
    pos=0;
}

char le_caractere(void){
	char c;
	
	if(pos<tamanho){
		c=codigo[pos];//pega o caracter na posicao pos no buffer principal
		pos++;
	}else{
		c=-1;//A ver...
	}
	return c;
}

int operador(char c){
	int res;
	
	switch(c){
		case '+':
			res=ADD;
			break;
		
		case '-':
			res=SUB;
			break;
			
		case '*':
			res=MUL;
			break;
		
		case '/':
			res=DIV;
			break;
			
		default:
			return -1; //A ver...
			
	}
	return res;
}

int sinalPont(char c){
	int res;
	
	switch(c){
		case '(':
			res=openPAR;
			break;
		case ')':
			res=closePAR;
			break;
		default:
			return -1;//A ver...	
	}
	
	return res;
}

Token *proximo_token(Token *tok){
	char c;
	char valor[200];//vetor de apoio 
	int vpos = 0;
	
	c=le_caractere();
	
	while(isspace(c)){//descartar espacos vazios
		c=le_caractere();//ignorando espacos
	}
	
	if(isdigit(c)){
		//preencher o campo token com 'number' la na struct 
		tok->tipo=TOK_NUM;
		valor[vpos++] = c;//		valor[vpos] = c;		vpos++;
		c=le_caractere();
		while(isdigit(c)){//numero pode ter n casas
					valor[vpos++] = c;
					c=le_caractere();
		}
		pos--;
		valor[vpos]='\0';
		tok->valor=atoi(valor);

		t=add(t, tok, 2, 3);//t velho é passado como parametro e o t novo é a atribuição
		//TODO implementar contador de linhas e colunas
	}
	else if(strchr(ops, c)!=NULL){//procura o char c na string ops declarada anteriormente, se encontrar, se trata de uma operacao
		tok->tipo=TOK_OP;
		tok->valor=operador(c);
		t=add(t, tok, 2, 3);
	}
	else if(c=='(' || c==')' ){
		tok->tipo = TOK_PONT;
		tok->valor= sinalPont(c);
		t=add(t, tok, 2, 3);
	}
	else{
		tok= NULL;
	}
	
	return tok;	
	
}

char *operadorToString(int op){
	switch(op){
		case ADD:
			return "SOMA";
		case SUB:
			return "SUBTRACAO";
		case MUL:
			return "MULTIPLICACAO";
		case DIV:
			return "DIVISAO";
		default:
			return "NENHUM";
	}
}

char *pontuacaoToString(int op){
	switch(op){
		case openPAR:
			return "ABRE APARENTESES";
		case closePAR:
			return "FECHA PARENTESES";
		default:
			return "NENHUM";
	}
}

void imprime_token(Token *tok, FILE *saida){
    switch(tok->tipo){
		case TOK_NUM:
			fprintf(saida, "Numero\t\t - Valor: %d\n", tok->valor);
			break;

		case TOK_OP:
			fprintf(saida, "Operador\t\t - Valor: %s\n", operadorToString(tok->valor));
			break;
		
		case TOK_PONT:
			fprintf(saida, "Pontuacao\t\t - Valor: %s\n", pontuacaoToString(tok->valor));
			break;

		default:
			fprintf(saida, "Numero\t\t - Valor: %d\n", tok->valor);
			break;
	}
}

void print(Tabela *t, FILE *tabelaOutput){
	Tabela *tabelaAuxiliar;
	int index=0;
	for(tabelaAuxiliar = t; tabelaAuxiliar!=NULL;tabelaAuxiliar=tabelaAuxiliar->next){

		index++;

		switch(tabelaAuxiliar->tipo){
		case TOK_NUM:
			fprintf(tabelaOutput, "\nE%d\n TOK: NUM\tLex: %d\tOcor: L%d C%d\n", index, tabelaAuxiliar->valor ,tabelaAuxiliar->linha, tabelaAuxiliar->coluna);
			break;

		case TOK_OP:
			fprintf(tabelaOutput, "\nE%d\n TOK: OP\tLex: %s\tOcor: L%d C%d\n", index, operadorToString(tabelaAuxiliar->valor) ,tabelaAuxiliar->linha, tabelaAuxiliar->coluna);
			break;
		
		case TOK_PONT:
			fprintf(tabelaOutput, "\nE%d\n TOK: PONT\tLex: %s\tOcor: L%d C%d\n", index, pontuacaoToString(tabelaAuxiliar->valor) ,tabelaAuxiliar->linha, tabelaAuxiliar->coluna);
			break;

		default:
			fprintf(tabelaOutput, "Token Desconhecido");
			break;
	} 
	}
}


int main(void){
    FILE *entradaCod; //entrada.txt codigo fonte
    FILE *fluxoTokens;//saida.txt Fluxo de tokens
    FILE *tabela;//tabela.txt Tabela de símbolos

    if((entradaCod = fopen("entrada.txt", "r"))==NULL){
        puts("Nao foi possivel abrir\n\n");
        exit(1);
    }
    
    printf("Analisador Lexico do 4ECA\n\n");
    printf("Lendo arquivo...\n");

    fluxoTokens = fopen("saida.txt","w");
    tabela = fopen("tabela.txt","w");

    char entrada[200];//vetor de caracteres = buffer = "Memoria de transicao" ou memoria temporaria
    Token tok;
    t = cria();

    printf("\n\n****** PROCESSANDO ******\n\n");

    fgets(entrada, 200, entradaCod);

    inicializa_analise(entrada);

    proximo_token(entrada);
    while(proximo_token(&tok)!=NULL){
        imprime_token(&tok, fluxoTokens);
    }

	print(t, tabela);

	fclose(entradaCod);
	fclose(fluxoTokens);
	fclose(tabela);

	printf("\n\nAnalise Lexica finalizada com sucesso\\n\n");

	return 0;
}
