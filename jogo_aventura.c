/*

  Projeto 2 - Projetos e Analise de Algoritmos

- Nicholas dos Santos Leal              - RA: 10409210
- José Maike Ferreira Ferro             - RA: 10732694
- Bruno Matheus Garutti Pinto           - RA: 10736747
- Klaus Gustavo Castelein Ricieri Moura - RA: 10735808

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define TAM_NOME 80
#define TAM_DISCIPLINA 30
#define TAM_LINHA 200
#define ENTRADA "entrada.csv"
#define SAIDA "saida.csv"

typedef struct {
    int ano;
    char nome[TAM_NOME];
    char disciplina[TAM_DISCIPLINA];
    float nota;
} Aluno;

void menu(void);
int lerInteiro(void);
void gerarEntrada(int quantidade);
int carregarArquivo(Aluno **alunos);
void salvarArquivo(Aluno alunos[], int quantidade);
void bubbleSort(Aluno alunos[], int quantidade, long long *comparacoes);
void mergeSort(Aluno alunos[], int inicio, int fim, long long *comparacoes);
void merge(Aluno alunos[], int inicio, int meio, int fim, long long *comparacoes);
void executarOrdenacao(int tipo);
void limparEnter(void);
void removerEnter(char texto[]);

/*
 * Funcao principal do programa.
 * Mostra o menu e executa a opcao escolhida pelo usuario.
 */
int main(void) {
    int opcao;
    int quantidade;
        
    srand((unsigned int) time(NULL));

    do {
        menu();
        opcao = lerInteiro();

        if (opcao == 1) {
            printf("Informe a quantidade de alunos: ");
            quantidade = lerInteiro();

            if (quantidade > 0) {
                gerarEntrada(quantidade);
                printf("\nArquivo entrada.csv gerado com sucesso.\n\n");
            } else {
                printf("\nQuantidade invalida.\n\n");
            }
        } else if (opcao == 2) {
            executarOrdenacao(1);
        } else if (opcao == 3) {
            executarOrdenacao(2);
        } else if (opcao == 0) {
            printf("\nPrograma finalizado.\n");
        } else {
            printf("\nOpcao invalida.\n\n");
            limparEnter();
        }
    } while (opcao != 0);

    return 0;
}

/*
 * Exibe o menu principal do programa.
 */
void menu(void) {
    printf("========================================\n");
    printf(" HISTORICO DE NOTAS - PROF. CHARLES\n");
    printf("========================================\n");
    printf("1 - Gerar entrada aleatoria (entrada.csv)\n");
    printf("2 - Ordenar por nota (Bubble Sort)\n");
    printf("3 - Ordenar por nota (Merge Sort)\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

/*
 * Gera o arquivo entrada.csv com dados aleatorios.
 * Os dados seguem o formato pedido no enunciado.
 */
/*
 * Le um numero inteiro do teclado.
 * Se o usuario digitar letra ou outro valor invalido,
 * a funcao limpa o buffer e retorna -1.
 */
int lerInteiro(void) {
    int numero;

    if (scanf("%d", &numero) != 1) {
        limparEnter();
        return -1;
    }

    return numero;
}

void gerarEntrada(int quantidade) {
    FILE *arquivo;
    int i;
    int ano;
    int indiceNome;
    int indiceDisciplina;
    int numero;
    float nota;

    char nomes[][20] = {
        "Joao", "Maria", "Jose", "Ana", "Carlos",
        "Camila", "Pedro", "Julia", "Bruno", "Patricia"
    };

    char disciplinas[][20] = {
        "matematica", "portugues", "geografia"
    };

    /* Abre o arquivo entrada.csv para escrita. */
    arquivo = fopen(ENTRADA, "w");

    if (arquivo == NULL) {
        printf("\nErro ao criar o arquivo de entrada.\n");
        return;
    }

    /* Gera os dados aleatorios no formato pedido pelo enunciado. */
    for (i = 0; i < quantidade; i++) {
        /* Sorteia os dados do aluno. */
        ano = 2010 + rand() % 14;
        indiceNome = rand() % 10;
        indiceDisciplina = rand() % 3;
        numero = rand() % 10000;
        nota = (float)(rand() % 101) / 10.0f;

        fprintf(
            arquivo,
            "%d,%s%d,%s,%.1f\n",
            ano,
            nomes[indiceNome],
            numero,
            disciplinas[indiceDisciplina],
            nota
        );
    }

    fclose(arquivo);
}

/*
 * Carrega os dados do arquivo entrada.csv
 * para um vetor dinamico de alunos.
 */
int carregarArquivo(Aluno **alunos) {
    FILE *arquivo;
    char linha[TAM_LINHA];
    char *campo;
    int quantidade = 0;
    int capacidade = 100;

    /* Abre o arquivo entrada.csv para leitura. */
    arquivo = fopen(ENTRADA, "r");

    if (arquivo == NULL) {
        printf("\nErro: arquivo entrada.csv nao encontrado.\n");
        printf("Gere o arquivo antes de ordenar.\n\n");
        return 0;
    }

    /* Aloca memoria inicial para armazenar os alunos. */
    *alunos = (Aluno *) malloc(capacidade * sizeof(Aluno));

    if (*alunos == NULL) {
        printf("\nErro de memoria ao carregar os alunos.\n\n");
        fclose(arquivo);
        return 0;
    }

    /* Le cada linha do CSV e separa os campos usando virgula. */
    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        removerEnter(linha);

        if (quantidade == capacidade) {
            /* Dobra a capacidade do vetor caso fique cheio. */
            capacidade = capacidade * 2;
            *alunos = (Aluno *) realloc(*alunos, capacidade * sizeof(Aluno));

            if (*alunos == NULL) {
                printf("\nErro de memoria ao aumentar o vetor.\n\n");
                fclose(arquivo);
                return 0;
            }
        }

        /* Separa os campos da linha usando virgula. */
        campo = strtok(linha, ",");
        if (campo != NULL) {
            (*alunos)[quantidade].ano = atoi(campo);
        }

        campo = strtok(NULL, ",");
        if (campo != NULL) {
            strcpy((*alunos)[quantidade].nome, campo);
        }

        campo = strtok(NULL, ",");
        if (campo != NULL) {
            strcpy((*alunos)[quantidade].disciplina, campo);
        }

        campo = strtok(NULL, ",");
        if (campo != NULL) {
            (*alunos)[quantidade].nota = (float) atof(campo);
            quantidade++;
        }
    }

    fclose(arquivo);
    return quantidade;
}

/*
 * Salva os alunos ordenados no arquivo saida.csv.
 */
void salvarArquivo(Aluno alunos[], int quantidade) {
    FILE *arquivo;
    int i;

    /* Abre o arquivo saida.csv para escrita. */
    arquivo = fopen(SAIDA, "w");

    if (arquivo == NULL) {
        printf("\nErro ao criar o arquivo de saida.\n");
        return;
    }

    /* Salva o vetor ja ordenado no arquivo saida.csv. */
    for (i = 0; i < quantidade; i++) {
        fprintf(
            arquivo,
            "%d,%s,%s,%.1f\n",
            alunos[i].ano,
            alunos[i].nome,
            alunos[i].disciplina,
            alunos[i].nota
        );
    }

    fclose(arquivo);
}

/*
 * Ordena os alunos usando Bubble Sort.
 * Tambem conta quantas comparacoes foram feitas.
 */
void bubbleSort(Aluno alunos[], int quantidade, long long *comparacoes) {
    int i;
    int j;
    Aluno aux;

    /* Inicia o contador de comparacoes. */
    *comparacoes = 0;

    /* Compara alunos vizinhos e leva as maiores notas para o final. */
    for (i = 0; i < quantidade - 1; i++) {
        for (j = 0; j < quantidade - 1 - i; j++) {
            (*comparacoes)++;

            /* Troca os alunos de lugar se a nota atual for maior. */
            if (alunos[j].nota > alunos[j + 1].nota) {
                aux = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = aux;
            }
        }
    }
}

/*
 * Divide o vetor em partes menores ate chegar
 * em apenas um elemento.
 */
void mergeSort(Aluno alunos[], int inicio, int fim, long long *comparacoes) {
    int meio;

    if (inicio < fim) {
        /* Calcula o meio do vetor. */
        meio = (inicio + fim) / 2;

        mergeSort(alunos, inicio, meio, comparacoes);
        mergeSort(alunos, meio + 1, fim, comparacoes);
        merge(alunos, inicio, meio, fim, comparacoes);
    }
}

/*
 * Junta duas partes ja ordenadas do vetor.
 */
void merge(Aluno alunos[], int inicio, int meio, int fim, long long *comparacoes) {
    int i;
    int j;
    int k;
    int tamanhoEsquerda = meio - inicio + 1;
    int tamanhoDireita = fim - meio;

    /* Vetores auxiliares usados na intercalacao. */
    Aluno *esquerda = (Aluno *) malloc(tamanhoEsquerda * sizeof(Aluno));
    Aluno *direita = (Aluno *) malloc(tamanhoDireita * sizeof(Aluno));

    if (esquerda == NULL || direita == NULL) {
        printf("\nErro de memoria no Merge Sort.\n");
        /* Libera a memoria utilizada pelos vetores auxiliares. */
    free(esquerda);
        free(direita);
        return;
    }

    /* Copia as duas metades para vetores auxiliares. */
    for (i = 0; i < tamanhoEsquerda; i++) {
        esquerda[i] = alunos[inicio + i];
    }

    for (j = 0; j < tamanhoDireita; j++) {
        direita[j] = alunos[meio + 1 + j];
    }

    i = 0;
    j = 0;
    k = inicio;

    /* Junta as duas metades comparando as notas. */
    while (i < tamanhoEsquerda && j < tamanhoDireita) {
        (*comparacoes)++;

        /* Coloca no vetor o menor valor entre esquerda e direita. */
        if (esquerda[i].nota <= direita[j].nota) {
            alunos[k] = esquerda[i];
            i++;
        } else {
            alunos[k] = direita[j];
            j++;
        }

        k++;
    }

    while (i < tamanhoEsquerda) {
        alunos[k] = esquerda[i];
        i++;
        k++;
    }

    while (j < tamanhoDireita) {
        alunos[k] = direita[j];
        j++;
        k++;
    }

    free(esquerda);
    free(direita);
}

/*
 * Executa a ordenacao escolhida pelo usuario,
 * mede o tempo e mostra os resultados na tela.
 */
void executarOrdenacao(int tipo) {
    Aluno *alunos;
    int quantidade;
    long long comparacoes = 0;
    clock_t inicio;
    clock_t fim;
    double tempo;

    /* Carrega os alunos do arquivo entrada.csv. */
    quantidade = carregarArquivo(&alunos);

    if (quantidade <= 0) {
        return;
    }

    /* Mede somente o tempo da ordenacao, sem incluir leitura e escrita de arquivo. */
    /* Marca o inicio da ordenacao. */
    inicio = clock();

    if (tipo == 1) {
        bubbleSort(alunos, quantidade, &comparacoes);
    } else {
        mergeSort(alunos, 0, quantidade - 1, &comparacoes);
    }

    /* Marca o fim da ordenacao. */
    fim = clock();

    /* Calcula o tempo gasto em segundos. */
    tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    /* Salva os alunos ordenados no arquivo de saida. */
    salvarArquivo(alunos, quantidade);

    if (tipo == 1) {
        printf("\nAlgoritmo: Bubble Sort\n");
    } else {
        printf("\nAlgoritmo: Merge Sort\n");
    }

    printf("Tamanho Entrada: %d\n", quantidade);
    printf("Tempo execucao: %.6f segundos\n", tempo);
    printf("Comparacoes (passos): %lld\n", comparacoes);
    printf("Arquivo saida.csv gerado com sucesso.\n\n");

    /* Libera a memoria utilizada pelo vetor de alunos. */
    free(alunos);
}

/*
 * Limpa caracteres restantes do teclado.
 * Evita problemas em leituras futuras.
 */
void limparEnter(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/*
 * Remove o caractere de quebra de linha
 * lido pelo fgets.
 */
void removerEnter(char texto[]) {
    int tamanho = strlen(texto);

    if (tamanho > 0 && texto[tamanho - 1] == '\n') {
        texto[tamanho - 1] = '\0';
        tamanho--;
    }

    if (tamanho > 0 && texto[tamanho - 1] == '\r') {
        texto[tamanho - 1] = '\0';
    }
}
