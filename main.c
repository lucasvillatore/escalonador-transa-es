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

TransactionT **leituraArquivo(int *limiar, int *numero_vertices) 
{
    TransactionT **transacoes;
    transacoes = (TransactionT **)malloc((*limiar) * sizeof(TransactionT *));

    int tempo;
    int identificador;
    char operacao;
    char atributo;
    
    while(scanf("%d", &tempo) != EOF) {
        fscanf(stdin, "%d %c %c", &identificador, &operacao, &atributo);
        if (identificador > (*numero_vertices)) {
            (*numero_vertices) = identificador;
        }
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

    // for (int i = 0; i < tempo; i++) {
    //     mostraTransacao(transacoes[i]);
    // }

    (*limiar) = tempo;
    return transacoes;
}

void imprime_grafo(int **grafo, int tamanho)
{
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            printf("%d \t", grafo[i][j]);
        }
        printf("\n");
    }
}

int operacao_escrita(TransactionT *transacao)
{
    return transacao->operacao == 'R' ? 1 : 0;
}

int operacao_leitura(TransactionT *transacao)
{
    return transacao->operacao == 'W' ? 1 : 0;
}

int operacao_commit(TransactionT *transacao)
{
    return transacao->operacao == 'C' ? 1 : 0;
}


int mesmo_atributo(TransactionT *transacao_a, TransactionT *transacao_b)
{
    return transacao_a->atributo == transacao_b->atributo ? 1 : 0;
}

int mesmo_identificador(TransactionT *transacao_a, TransactionT *transacao_b)
{
    return transacao_a->identificador == transacao_b->identificador ? 1 : 0;
}

int **aloca_grafo(int tamanho, int numero_vertices, TransactionT **transacoes)
{
    int **grafo = (int **)calloc(numero_vertices, sizeof(int *));
    for (int i = 0; i < numero_vertices; i++) {
        grafo[i] = (int *)calloc(numero_vertices, sizeof(int));

        for (int j = 0; j < numero_vertices; j++) {
            grafo[i][j] = 0;
        }
    }

    for (int i = 0; i < tamanho; i++) {
        TransactionT *transacao_a = transacoes[i];
        int escrita_leitura_a = operacao_escrita(transacao_a) || operacao_leitura(transacao_a) ? 1 : 0;
        int commit_encontrado = 0;

        for (int j = i + 1; j < tamanho; j++) {
            TransactionT *transacao_b = transacoes[j];
            int escrita_leitura_b = operacao_escrita(transacao_b) || operacao_leitura(transacao_b) ? 1 : 0;

            if (escrita_leitura_a || escrita_leitura_b) {
                
                if (mesmo_atributo(transacao_a, transacao_b) && !mesmo_identificador(transacao_a, transacao_b) && !commit_encontrado) {
                    mostraTransacao(transacao_a);
                    mostraTransacao(transacao_b);
                    grafo[transacao_a->identificador - 1][transacao_b->identificador - 1] = 1;
                }
            }

            if (operacao_commit(transacao_b)) {
                commit_encontrado = 1;
            }
        }
    }

    return grafo;
    
}
int main() {
    int tamanho = 10;
    int numero_vertices = 0;
    int **grafo;
    TransactionT **transacoes = leituraArquivo(&tamanho, &numero_vertices);

    grafo = aloca_grafo(tamanho, numero_vertices, transacoes);

    imprime_grafo(grafo, numero_vertices);
}