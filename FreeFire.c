#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COMPONENTES 10

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Funções de ordenação
void bubbleSortNome(Componente componentes[], int n, long long int *comparacoes);
void insertionSortTipo(Componente componentes[], int n, long long int *comparacoes);
void selectionSortPrioridade(Componente componentes[], int n, long long int *comparacoes);

// Função de busca
int buscaBinariaPorNome(const Componente componentes[], int n, const char nomeBusca[]);

// Funções de interface e gerenciamento
void menuPrincipal(int n, int max, int ordenadoFlag);
void adicionarComponente(Componente componentes[], int *n, int *ordenadoFlag);
void descartarComponente(Componente componentes[], int *n, int *ordenadoFlag);
void menuOrganizarMochila(Componente componentes[], int n, int *ordenadoFlag);
void mostrarComponentes(const Componente componentes[], int n, int mostrarIndices);
void limparBufferEntrada();

void menuPrincipal(int n, int max, int ordenadoFlag) {
    system("clear || cls"); 
    printf("=======================================================\n");
    printf("   PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("=======================================================\n");
    printf("Itens na Mochila: %d/%d\n", n, max);
    printf("Status da Ordenacao por Nome: %s\n", ordenadoFlag ? "ORDENADO" : "NAO ORDENADO");
    printf("-------------------------------------------------------\n");
    printf("1. Adicionar Componente\n");
    printf("2. Descartar Componente\n");
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por nome)\n");
    printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
    printf("-------------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void adicionarComponente(Componente componentes[], int *n, int *ordenadoFlag) {
    if (*n >= MAX_COMPONENTES) {
        printf("\n!! A mochila esta cheia! Descarte um item antes de adicionar outro. !!\n");
        return;
    }

    printf("\n--- Adicionando novo componente ---\n");
    printf("Nome: ");
    fgets(componentes[*n].nome, sizeof(componentes[*n].nome), stdin);
    componentes[*n].nome[strcspn(componentes[*n].nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(componentes[*n].tipo, sizeof(componentes[*n].tipo), stdin);
    componentes[*n].tipo[strcspn(componentes[*n].tipo, "\n")] = 0;

    printf("Prioridade (1 a 10): ");
    scanf("%d", &componentes[*n].prioridade);
    limparBufferEntrada();
    
    (*n)++;
    *ordenadoFlag = 0;
    printf("\nComponente adicionado com sucesso!\n");
}

void descartarComponente(Componente componentes[], int *n, int *ordenadoFlag) {
    if (*n == 0) {
        printf("\n!! A mochila ja esta vazia. !!\n");
        return;
    }
    
    printf("\n--- Descartar Componente ---\n");
    mostrarComponentes(componentes, *n, 1);
    
    printf("\nDigite o numero do item que deseja descartar (ou -1 para cancelar): ");
    int indice;
    scanf("%d", &indice);
    limparBufferEntrada();

    if (indice < 0 || indice >= *n) {
        printf("\nSelecao invalida ou cancelada.\n");
        return;
    }

    for (int i = indice; i < *n - 1; i++) {
        componentes[i] = componentes[i + 1];
    }

    (*n)--;
    *ordenadoFlag = 0;
    printf("\nItem descartado com sucesso!\n");
}

void menuOrganizarMochila(Componente componentes[], int n, int *ordenadoFlag) {
    if (n == 0) {
        printf("\n!! Mochila vazia. Nao ha nada para organizar. !!\n");
        return;
    }

    int opcao;
    printf("\n--- Organizar Mochila ---\n");
    printf("1. Por Nome (Bubble Sort)\n");
    printf("2. Por Tipo (Insertion Sort)\n");
    printf("3. Por Prioridade (Selection Sort)\n");
    printf("0. Voltar ao menu principal\n");
    printf("Escolha o criterio de ordenacao: ");
    scanf("%d", &opcao);
    limparBufferEntrada();

    Componente copia_mochila[MAX_COMPONENTES];
    memcpy(copia_mochila, componentes, n * sizeof(Componente));

    long long int comparacoes = 0;
    clock_t inicio, fim;
    double tempoGasto;

    switch(opcao) {
        case 1:
            inicio = clock();
            bubbleSortNome(copia_mochila, n, &comparacoes);
            fim = clock();
            *ordenadoFlag = 1;
            printf("\n--- Mochila Organizada por NOME ---\n");
            break;
        case 2:
            inicio = clock();
            insertionSortTipo(copia_mochila, n, &comparacoes);
            fim = clock();
            *ordenadoFlag = 0;
            printf("\n--- Mochila Organizada por TIPO ---\n");
            break;
        case 3:
            inicio = clock();
            selectionSortPrioridade(copia_mochila, n, &comparacoes);
            fim = clock();
            *ordenadoFlag = 0;
            printf("\n--- Mochila Organizada por PRIORIDADE ---\n");
            break;
        case 0:
            return;
        default:
            printf("\nOpcao de ordenacao invalida.\n");
            return;
    }
    
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    mostrarComponentes(copia_mochila, n, 0);
    printf("\n--- Analise de Desempenho ---\n");
    printf("Numero de comparacoes: %lld\n", comparacoes);
    printf("Tempo de execucao: %f segundos\n", tempoGasto);
}

void mostrarComponentes(const Componente componentes[], int n, int mostrarIndices) {
    if (n == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }
    printf("--------------------------------------------------------------------\n");
    if(mostrarIndices) printf("%-5s | ", "No.");
    printf("%-30s | %-20s | %s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        if(mostrarIndices) printf("[%d]   | ", i);
        printf("%-30s | %-20s | %d\n", componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("--------------------------------------------------------------------\n");
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void bubbleSortNome(Componente c[], int n, long long int *comp) {
    *comp = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*comp)++;
            if (strcmp(c[j].nome, c[j + 1].nome) > 0) {
                Componente temp = c[j]; c[j] = c[j + 1]; c[j + 1] = temp;
            }
        }
    }
}

void insertionSortTipo(Componente c[], int n, long long int *comp) {
    *comp = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = c[i]; int j = i - 1;
        while (j >= 0) {
             (*comp)++;
            if(strcmp(c[j].tipo, chave.tipo) > 0) {
                c[j + 1] = c[j]; j = j - 1;
            } else break;
        }
        c[j + 1] = chave;
    }
}

void selectionSortPrioridade(Componente c[], int n, long long int *comp) {
    *comp = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comp)++;
            if (c[j].prioridade < c[min_idx].prioridade) min_idx = j;
        }
        if (min_idx != i) {
            Componente temp = c[min_idx]; c[min_idx] = c[i]; c[i] = temp;
        }
    }
}

int buscaBinariaPorNome(const Componente c[], int n, const char nomeBusca[]) {
    int esq = 0, dir = n - 1;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        int res = strcmp(c[meio].nome, nomeBusca);
        if (res == 0) return meio;
        if (res < 0) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}

int main() {
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
     Componente mochila[MAX_COMPONENTES];
    int numComponentes = 0;
    int opcao;
    
    int ordenadoPorNome = 0; // 0 para NAO ORDENADO, 1 para ORDENADO

    do {
        menuPrincipal(numComponentes, MAX_COMPONENTES, ordenadoPorNome);
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                adicionarComponente(mochila, &numComponentes, &ordenadoPorNome);
                break;
            case 2:
                descartarComponente(mochila, &numComponentes, &ordenadoPorNome);
                break;
            case 3:
                printf("\n--- INVENTARIO DA MOCHILA ---\n");
                mostrarComponentes(mochila, numComponentes, 0);
                break;
            case 4:
                menuOrganizarMochila(mochila, numComponentes, &ordenadoPorNome);
                break;
            case 5:
                if (numComponentes == 0) {
                    printf("\nALERTA: A mochila esta vazia. Nao ha o que buscar.\n");
                } else if (!ordenadoPorNome) {
                    // ======================================================================
                    // AQUI ESTÁ A MUDANÇA QUE VOCÊ PEDIU
                    // ======================================================================
                    printf("\nALERTA: A busca binaria requer que a mochila esteja ordenada por NOME.\nUse a Opcao 4 para organizar a mochila primeiro.\n");

                } else {
                    Componente mochila_ordenada[MAX_COMPONENTES];
                    memcpy(mochila_ordenada, mochila, numComponentes * sizeof(Componente));
                    long long int comps_nao_usadas = 0;
                    bubbleSortNome(mochila_ordenada, numComponentes, &comps_nao_usadas);

                    char nomeBusca[30];
                    printf("\nDigite o nome do componente-chave para buscar: ");
                    fgets(nomeBusca, sizeof(nomeBusca), stdin);
                    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                    
                    int indice = buscaBinariaPorNome(mochila_ordenada, numComponentes, nomeBusca);

                    if (indice != -1) {
                        printf("\n>>> SUCESSO! Componente-chave '%s' encontrado. A torre pode ser ativada! <<<\n", nomeBusca);
                    } else {
                        printf("\n>>> FALHA! Componente-chave '%s' nao encontrado na mochila. <<<\n", nomeBusca);
                    }
                }
                break;
            case 0:
                printf("\n>>> Tentando ativar a torre de fuga... Adeus, ilha! <<<\n");
                break;
            default:
                printf("\n!! Opcao invalida. Tente novamente. !!\n");
                break;
        }
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
