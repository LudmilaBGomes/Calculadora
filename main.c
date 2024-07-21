#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

typedef struct {
    int itens[MAX];
    int topo;
} Pilha;


void inicializaPilha(Pilha *p) {
    p->topo = -1;
}


int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}


int pilhaCheia(Pilha *p) {
    return p->topo == MAX - 1;
}

void empilha(Pilha *p, int valor) {
    if (!pilhaCheia(p)) {
        p->itens[++(p->topo)] = valor;
    }
}


int desempilha(Pilha *p) {
    if (!pilhaVazia(p)) {
        return p->itens[(p->topo)--];
    }
    return -1; 
}


int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}


int realizandoOperacao(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}


int calculandoExpressao(char *expressao) {
    Pilha *pilhaDeOperando = (Pilha *)malloc(sizeof(Pilha));
    Pilha *pilhaDeOperador = (Pilha *)malloc(sizeof(Pilha));
    inicializaPilha(pilhaDeOperando);
    inicializaPilha(pilhaDeOperador);

    for (int i = 0; i < strlen(expressao); i++) {
        if (isspace(expressao[i])) continue;

        if (isdigit(expressao[i]) || (expressao[i] == '_' && isdigit(expressao[i + 1]))) {
            int ehNegativo = 0;
            if (expressao[i] == '_') {
                ehNegativo = 1;
                i++;
            }

            int num = 0;
            while (isdigit(expressao[i])) {
                num = num * 10 + (expressao[i] - '0');
                i++;
            }
            i--;

            if (ehNegativo) num = -num;
            empilha(pilhaDeOperando, num);
        } else if (expressao[i] == '(') {
            empilha(pilhaDeOperador, expressao[i]);
        } else if (expressao[i] == ')') {
            while (!pilhaVazia(pilhaDeOperador) && pilhaDeOperador->itens[pilhaDeOperador->topo] != '(') {
                int b = desempilha(pilhaDeOperando);
                int a = desempilha(pilhaDeOperando);
                char op = desempilha(pilhaDeOperador);
                empilha(pilhaDeOperando, realizandoOperacao(a, b, op));
            }
            desempilha(pilhaDeOperador); 
        } else if (strchr("+-*/", expressao[i])) {
            while (!pilhaVazia(pilhaDeOperador) && precedencia(pilhaDeOperador->itens[pilhaDeOperador->topo]) >= precedencia(expressao[i])) {
                int b = desempilha(pilhaDeOperando);
                int a = desempilha(pilhaDeOperando);
                char op = desempilha(pilhaDeOperador);
                empilha(pilhaDeOperando, realizandoOperacao(a, b, op));
            }
            empilha(pilhaDeOperador, expressao[i]);
        } else if (expressao[i] == '=') {
            while (!pilhaVazia(pilhaDeOperador)) {
                int b = desempilha(pilhaDeOperando);
                int a = desempilha(pilhaDeOperando);
                char op = desempilha(pilhaDeOperador);
                empilha(pilhaDeOperando, realizandoOperacao(a, b, op));
            }
            int resultado = desempilha(pilhaDeOperando);
            free(pilhaDeOperando);
            free(pilhaDeOperador);
            return resultado;
        }
    }
    free(pilhaDeOperando);
    free(pilhaDeOperador);
    return 0; 
}


int main() {
    char expressao[MAX];
    while (1) {
        printf("Qual expressao quer calcular?: ");
        fgets(expressao, MAX, stdin);
        if (expressao[0] == '#') break;
        int resultado = calculandoExpressao(expressao);
        printf("Resultado: %d\n", resultado);
    }
    return 0;
}
