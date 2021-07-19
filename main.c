#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct TransactionT {
    int tempo;
    int identificador;
    char operacao;
    char atributo;
} TransactionT;


TransactionT *criaTransacao(int tempo, int identificador, char operacao, char atributo)
{
    TransactionT *transacao = (TransactionT *)malloc(sizeof(TransactionT) * 50);

    transacao->tempo = tempo - 1;
    transacao->identificador = identificador;
    transacao->operacao = operacao;
    transacao->atributo = atributo;

    return transacao;
}

void mostraTransacao(TransactionT *transacao)
{
    printf("tempo = %d ", transacao->tempo);
    printf("identificador = %d ", transacao->identificador);
    printf("operacao = %c ", transacao->operacao);
    printf("atributo = %c\n", transacao->atributo);
}

TransactionT **leitura_arquivo(int *limiar) 
{
    TransactionT **transacoes;
    transacoes = (TransactionT **)malloc((*limiar) * sizeof(TransactionT *));

    int tempo;
    int identificador;
    char operacao;
    char atributo;
    
    while(scanf("%d", &tempo) != EOF) {
        fscanf(stdin, "%d %c %c", &identificador, &operacao, &atributo);
        
        if (tempo >= (*limiar)) {
            (*limiar) *= 2;
            transacoes = (TransactionT **)realloc(transacoes,(*limiar) * sizeof(TransactionT *));

            if (!transacoes) {
                printf("Problema ao realocar a memória\n");
                exit(0);
            }
        }
        transacoes[tempo - 1] = criaTransacao(tempo, identificador, operacao, atributo);
    }

    for (int i = 0; i < tempo; i++) {
        mostraTransacao(transacoes[i]);
    }

    (*limiar) = tempo;
    return transacoes;
}
int main() {
    int limiar = 10;
    TransactionT **transacoes = leitura_arquivo(&limiar);
}