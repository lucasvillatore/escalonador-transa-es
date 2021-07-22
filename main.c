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

    transacao->tempo = tempo;
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
    return transacao->operacao == 'W' ? 1 : 0;
}

int operacao_leitura(TransactionT *transacao)
{
    return transacao->operacao == 'R' ? 1 : 0;
}

int operacao_commit(TransactionT *transacao)
{
    return transacao->operacao == 'C' ? 1 : 0;
}

TransactionT **leituraArquivo(int *limiar, EscalonadorT ***escalonador, int *numero_escalonacoes) 
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

    int indice = 0;
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
        TransactionT *transacao = criaTransacao(tempo, identificador, operacao, atributo);
        transacoes[indice] = transacao;

        if (operacao_commit(transacoes[indice]) == 1) {
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
        indice++;
    }

    (*numero_escalonacoes) = escalonacao;
    (*limiar) = indice;
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

int calcula_tamanho_array(int *array) 
{
    int i;
    for (i = 0; array[i] != -1; i++);

    return i;
}

int **aloca_grafo(int numero_vertices, int *tarefa_escalonada, int numero_transacoes, TransactionT **transacoes)
{
    int **grafo = (int **)calloc(numero_vertices, sizeof(int *));
    for (int i = 0; i < numero_vertices; i++) {
        grafo[i] = (int *)calloc(numero_vertices, sizeof(int));

        for (int j = 0; j < numero_vertices; j++) {
            grafo[i][j] = 0;
        }
    }

    for (int j = 0; j < numero_vertices; j++) {
        TransactionT *transacao_a = transacoes[j];
        if (!id_esta_no_array(tarefa_escalonada, transacao_a->identificador, numero_vertices) || operacao_commit(transacao_a)) {
            continue;
        } 

        for (int k = j + 1; k < numero_transacoes; k++) {
            TransactionT *transacao_b = transacoes[k];
            if (!id_esta_no_array(tarefa_escalonada, transacao_b->identificador, numero_vertices) || operacao_commit(transacao_b)) {
                continue;
            }

            if (
                transacao_a->atributo == transacao_b->atributo &&
                transacao_a->identificador != transacao_b->identificador &&
                (operacao_escrita(transacao_a) || operacao_escrita(transacao_b))

            ){
                grafo[transacao_a->identificador - 1][transacao_b->identificador - 1] = 1;
            }
        }
    }

    return grafo;
    
}

int conta_tamanho_array(int *array) 
{
    int j;
    for (j = 0; array[j] != -1; j++) {
    }

    return j;
}

int visao_equivalente(int **grafo, int *tarefa)
{
    return 0;
}

int vertice_visitado(int *nos_visitados, int vertice)
{
    for (int i = 0; nos_visitados[i] != -2; i++) {
        if (nos_visitados[i] == vertice) {
            return 1;
        }
    }
    
    return 0;
}

void adiciona_vertice_nos_visitados(int **nos_visitados, int vertice)
{
    int i;
    for (i = 0; (*nos_visitados)[i] != -1; i++) {
    }

    (*nos_visitados)[i] = vertice;
}

int *pega_vizinhos_vertices(int **grafo, int vertice, int tamanho_grafo, int *num_vizinhos)
{
    int *vizinhos = (int *)malloc(tamanho_grafo * sizeof(int));
    for (int i = 0; i < tamanho_grafo; i++) {
        if (grafo[vertice][i] > 0) {
            vizinhos[(*num_vizinhos)] = i;
            (*num_vizinhos)++;
        }
    }

    return vizinhos;
}

int tem_ciclo(int **grafo, int vertice, int tamanho_grafo, int **nos_visitados)
{
    if (vertice_visitado((*nos_visitados), vertice)) {
        return 1;
    }
    adiciona_vertice_nos_visitados(nos_visitados, vertice);

    int resultado = 0;
    int num_vizinhos = 0;
    int *vizinhos = pega_vizinhos_vertices(grafo, vertice, tamanho_grafo, &num_vizinhos);
    // para cada vizinho, chamo busca em profundidade
    for (int i = 0; i < num_vizinhos; i++) {
        resultado = tem_ciclo(grafo, vizinhos[i], tamanho_grafo, nos_visitados);
    }

    return resultado;
}

int *aloca_array_visitados(int tamanho)
{
    int *array = (int *)malloc((tamanho + 1) * sizeof(int));

    for (int i = 0; i < tamanho; i++) {
        array[i] = -1;
    }

    array[tamanho] = -2;

    return array;
}

int main() {
    int tamanho = 10;
    int numero_escalonacoes = 10;
    
    int **escalonadores = (int **)malloc(sizeof(int *) * numero_escalonacoes);

    int **grafo;

    TransactionT **transacoes = leituraArquivo(&tamanho, &escalonadores, &numero_escalonacoes);

    for (int i = 0; i < numero_escalonacoes; i++) {
        int *tarefa_escalonada = escalonadores[i];
        int tamanho_array = conta_tamanho_array(tarefa_escalonada);
        int k = 0;
        // printf("%d ", i + 1);
        for (k = 0; k < tamanho_array - 1; k++) {
            // printf("%d,", tarefa_escalonada[k]);
        }
        // printf("%d ", tarefa_escalonada[k]);

        grafo = aloca_grafo(tamanho_array, tarefa_escalonada, tamanho, transacoes);

        int *nos_visitados = aloca_array_visitados(tamanho_array);
        
        int ciclo = tem_ciclo(grafo, 0, tamanho_array, &nos_visitados);

        imprime_grafo(grafo, tamanho_array);
        if (ciclo) {
            printf("tem ciclo\n");
            // printf("NS ");
        }else{
            printf("não tem ciclo\n");
            // printf("SS ");
        }

        if (!ciclo || visao_equivalente(grafo, tarefa_escalonada)) {
            // printf("SV");
        }else {
            // printf("NV");
        }

        // printf("\n");
    }

}