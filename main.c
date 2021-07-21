#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct TransactionT {
    int tempo;
    int identificador;
    char operacao;
    char atributo;
} TransactionT;

typedef int EscalonadorT;

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

int id_esta_no_array(int *array_ids, int id, int tamanho)
{
    for (int i = 0; i < tamanho; i++) {
        if (array_ids[i] == id) {
            return 1;
        }
    }
    return 0;
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

TransactionT **leituraArquivo(int *limiar, int *numero_vertices, EscalonadorT ***escalonador, int *numero_escalonacoes) 
{
    TransactionT **transacoes;
    transacoes = (TransactionT **)malloc((*limiar) * sizeof(TransactionT *));

    int tempo;
    int identificador;
    char operacao;
    char atributo;
    
    int tamanho_array = 10; // controla o tamanho total do array 
    int indice_array_atual = 0; // controla o indice atual do array
    int numero_commits = 0; //controla numero de commits
    int escalonacao = 0; //controla a escalonacao atual
    (*escalonador)[escalonacao] = (int *)malloc(sizeof(int) * tamanho_array);

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
        TransactionT *transacao = criaTransacao(tempo, identificador, operacao, atributo);
        transacoes[tempo - 1] = transacao;

        if (operacao_commit(transacoes[tempo - 1]) == 1) {
            numero_commits--;
        }
        if (!id_esta_no_array((*escalonador)[escalonacao], transacao->identificador, tamanho_array)) {
            if (indice_array_atual >= tamanho_array) {
                tamanho_array *= 2;
                (*escalonador)[escalonacao] = (int *)realloc(escalonador[escalonacao], (tamanho_array) * sizeof(int));
            }
            numero_commits++;
            (*escalonador)[escalonacao][indice_array_atual] = transacao->identificador;
            indice_array_atual++;
        }
        

        if (numero_commits == 0) {
            (*escalonador)[escalonacao][indice_array_atual] = -1;
            
            
            tamanho_array = 10; // controla o tamanho total do array 
            indice_array_atual = 0; // controla o indice atual do array
            numero_commits = 0; //controla numero de commits
            escalonacao++; //controla a escalonacao atual
            
            (*escalonador)[escalonacao] = (int *)malloc(sizeof(int) * tamanho_array);
        }
    }

    (*numero_escalonacoes) = escalonacao;
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
        int escrita_a = operacao_escrita(transacao_a);
        int leitura_a = operacao_leitura(transacao_a);
        int commit_encontrado = 0;

        for (int j = i + 1; j < tamanho; j++) {
            TransactionT *transacao_b = transacoes[j];
            int escrita_b = operacao_escrita(transacao_b);
            int leitura_b = operacao_leitura(transacao_b);

            if (escrita_a && leitura_b || escrita_b && leitura_a || escrita_a && escrita_b) {
                
                if (mesmo_atributo(transacao_a, transacao_b) && !mesmo_identificador(transacao_a, transacao_b) && !commit_encontrado) {
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
    int numero_escalonacoes = 10;
    
    int **escalonadores = (int **)malloc(sizeof(int *) * numero_escalonacoes);

    int **grafo;

    TransactionT **transacoes = leituraArquivo(&tamanho, &numero_vertices, &escalonadores, &numero_escalonacoes);

    // for (int i = 0; i < numero_escalonacoes; i++) {
    //     printf("Escalonação %d\n", i + 1);
    //     int *tarefa_escalonada = escalonadores[i];
    //     for (int j = 0; tarefa_escalonada[j] != -1; j++) {
    //         printf("id = %d\n", tarefa_escalonada[j]);
    //     }
    //     printf("\n");
    // }
    grafo = aloca_grafo(tamanho, numero_vertices, transacoes);

    imprime_grafo(grafo, numero_vertices);
}