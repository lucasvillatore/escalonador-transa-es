# Trabalho 2 - Escalonamento de transações 
## Alunos: 
Lucas Block Villatore GRR20171677

Leonardo Bueno Nogueira Kruger GRR20180130
## Algoritmos e convenções utilizadas no teste de seriabilidade.
Manipulação do grafo: **Matriz de adjacencia.**

Verificação de ciclo: **Busca de profundidade.**

## Funções importantes
**leituraArquivo:** Faz a leitura do arquivo de entrada salvando as entradas e separando em escalonamentos (linhas para a saida).
```
TransactionT **leituraArquivo(int *limiar, EscalonadorT ***escalonador, int *numero_escalonacoes)
```
**aloca_grafo:** Aloca memoria e produz a matriz de adjacencia para manipulação do grafo, ocorre um for entre as operações de transação de modo a testar todas as possibilidades dois a dois de modo a verificar se ocorre uma operação de escrita e alguma outra operação no mesmo atributo adicionando uma aresta entre os vertices, caso não esteja ocorrendo uma escrita, atributos das operações serem diferentes ou operação de commit é ignorado e continua para proxima iteração até acabar.  

```
int **aloca_grafo(int numero_vertices, int *tarefa_escalonada, int numero_transacoes, TransactionT **transacoes)
```
**tem_ciclo:** Dentro desta função ocorre a busca de profundidade dentro do grafo enviado, são visitados todos os vertices e caso um vertice seja visitado mais de uma vez retorna o valor 1 que significa que existe ciclo no grafo e 0 caso não exista.

```
int tem_ciclo(int **grafo, int vertice, int tamanho_grafo, int **nos_visitados)
```

### Falar que implementamos o algoritmo 1


### Falar que não implementamos o algoritmo 2



