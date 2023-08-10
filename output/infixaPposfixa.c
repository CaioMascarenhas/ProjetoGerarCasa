#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAM 100

// Função que verifica se o caractere é um operador
int eh_operador(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return 1;
    return 0;
}

// Função que verifica a precedência dos operadores
int precedencia(char c)
{
    if (c == '+' || c == '-')
        return 1;
    if (c == '*' || c == '/')
        return 2;
    return 0;
}

// Função que converte a expressão infixa para pósfixa
void infixa_para_posfixa(char infixa[], char posfixa[]) {
    char pilha[MAX_TAM];
    int topo = -1;
    int i, j, tam;
    int ultimo_operando = 0; // flag que indica se o último caractere adicionado foi um operando

    tam = strlen(infixa);

    // Percorre a expressão infixa
    for (i = 0, j = 0; i < tam; i++) {
        // Se o caractere atual é um operando, adiciona-o à expressão pósfixa
        if (!eh_operador(infixa[i]) && infixa[i] != '(' && infixa[i] != ')' && infixa[i] != ' ') {
            if (ultimo_operando) {
                posfixa[j++] = infixa[i];
            } else {
                posfixa[j++] = ' ';
                posfixa[j++] = infixa[i];
            }
            ultimo_operando = 1;
        }
        // Se o caractere atual é um parêntese de abertura, adiciona-o à pilha
        else if (infixa[i] == '(') {
            pilha[++topo] = infixa[i];
            ultimo_operando = 0;
        }
        // Se o caractere atual é um operador
        else if (eh_operador(infixa[i])) {
            // Remove os operadores com precedência maior ou igual ao operador atual da pilha e adiciona-os à expressão pósfixa
            while (topo >= 0 && pilha[topo] != '(' && precedencia(pilha[topo]) >= precedencia(infixa[i])) {
                posfixa[j++] = ' ';
                posfixa[j++] = pilha[topo--];
                ultimo_operando = 0;
            }
            // Adiciona o operador atual à pilha
            pilha[++topo] = infixa[i];
            ultimo_operando = 0;
        }
        // Se o caractere atual é um parêntese de fechamento
        else if (infixa[i] == ')') {
            // Remove os operadores da pilha e adiciona-os à expressão pósfixa até encontrar o parêntese de abertura correspondente
            while (topo >= 0 && pilha[topo] != '(') {
                posfixa[j++] = ' ';
                posfixa[j++] = pilha[topo--];
                ultimo_operando = 0;
            }
            // Remove o parêntese de abertura da pilha
            topo--;
            ultimo_operando = 0;
        }
    }

    // Remove os operadores restantes da pilha e adiciona-os à expressão pósfixa
    while (topo >= 0) {
        posfixa[j++] = ' ';
        posfixa[j++] = pilha[topo--];
        ultimo_operando = 0;
    }

    // Adiciona o caractere nulo ao final da expressão pósfixa
    posfixa[j] = '\0';

}
// Função principal
int main()
{
    char infixa[MAX_TAM], posfixa[MAX_TAM];

    // Lê a expressão infixa do usuário
    printf("Digite a expressao infixa: ");
    scanf("%[^\n]s", infixa);

    // Converte a expressão infixa para pósfixa
    infixa_para_posfixa(infixa, posfixa);

    // Exibe a expressão pósfixa
    printf("Expressao posfixa: %s\n", posfixa);

    return 0;
}