#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <dirent.h>

#define MAX 100
#define FILE_NAME "estoque.txt"
#define NOME_MAX 16  // 15 caracteres + 1 para o terminador nulo

typedef struct {
    int id;
    char nome[NOME_MAX];
    int quantidade;
    float preco;
} Produto;

// Funções de Estoque
void salvarProdutosNoArquivo(Produto *produtos, int count);
void carregarProdutosDoArquivo(Produto *produtos, int *count);
void adicionarProduto(Produto *produtos, int *count);
void listarProdutos(Produto *produtos, int count);
void buscarProduto(Produto *produtos, int count);
void removerProduto(Produto *produtos, int *count);

// Funções de Venda
void realizarVenda(Produto *produtos, int *count);
void salvarHistoricoVenda( int id, const char *nome, int quantidade, float preco, float subtotal, float total);
void mostrarHistoricoVendaEspecifica(); // Nova função para exibir o histórico de uma venda específica
int numerarVenda();

void menu(Produto *produtos, int *count);

int main() {
    setlocale(LC_ALL, "Portuguese");
    Produto produtos[MAX];
    int count = 0;
    carregarProdutosDoArquivo(produtos, &count);
    menu(produtos, &count);
    return 0;
}

void salvarProdutosNoArquivo(Produto *produtos, int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(fp, "ID\tNome           \tQuantidade\tPreço\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d\t%-15s\t%d\t\t%.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }

    fclose(fp);
    printf("Dados salvos no arquivo %s\n", FILE_NAME);
}

void carregarProdutosDoArquivo(Produto *produtos, int *count) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("Arquivo não encontrado. Iniciando com lista vazia.\n");
        *count = 0;
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // Pular a linha de cabeçalho

    *count = 0;
    while (fscanf(fp, "%d\t%15[^\t]\t%d\t%f\n", &produtos[*count].id, produtos[*count].nome, &produtos[*count].quantidade, &produtos[*count].preco) == 4) {
        produtos[*count].nome[15] = '\0'; // Garante que o nome tenha no máximo 15 caracteres
        (*count)++;
        if (*count >= MAX) break;
    }

    fclose(fp);
    printf("Dados carregados do arquivo %s\n", FILE_NAME);
}

void adicionarProduto(Produto *produtos, int *count) {
    if (*count >= MAX) {
        printf("Limite máximo de produtos atingido.\n");
        return;
    }

    printf("Digite o ID do produto: ");
    scanf("%d", &produtos[*count].id);
    printf("Digite o nome do produto (max 15 caracteres): ");
    getchar(); // Limpar o buffer
    fgets(produtos[*count].nome, NOME_MAX, stdin);
    produtos[*count].nome[strcspn(produtos[*count].nome, "\n")] = '\0'; // Remover newline
    if (strlen(produtos[*count].nome) > 15) {
        produtos[*count].nome[15] = '\0'; // Truncar para 15 caracteres
    }
    printf("Digite a quantidade: ");
    scanf("%d", &produtos[*count].quantidade);
    printf("Digite o preço: ");
    scanf("%f", &produtos[*count].preco);

    (*count)++;
    salvarProdutosNoArquivo(produtos, *count);
    printf("Produto adicionado com sucesso!\n");
}

void listarProdutos(Produto *produtos, int count) {
    printf("\n--- Lista de Produtos ---\n");
    printf("ID\tNome           \tQuantidade\tPreço\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%-15s\t%d\t\t%.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }
    printf("-----------------------------------------------------\n");
}

void buscarProduto(Produto *produtos, int count) {
    int id;
    printf("Digite o ID do produto que deseja buscar: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (produtos[i].id == id) {
            printf("\nProduto encontrado:\n");
            printf("ID: %d\nNome: %s\nQuantidade: %d\nPreço: %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
            return;
        }
    }

    printf("Produto com ID %d não encontrado.\n", id);
}

void removerProduto(Produto *produtos, int *count) {
    int id;
    printf("Digite o ID do produto que deseja remover: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++) {
        if (produtos[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            (*count)--;
            salvarProdutosNoArquivo(produtos, *count);
            printf("Produto removido com sucesso.\n");
            return;
        }
    }

    printf("Produto com ID %d não encontrado.\n", id);
}

// Função para realizar uma venda
void realizarVenda(Produto *produtos, int *count) {
    int id, quantidade;
    float totalVenda = 0.0;

    printf("\n--- Realizar Venda ---\n");

    while (1) {
        printf("Digite o ID do produto (ou -1 para finalizar): ");
        scanf("%d", &id);

        if (id == -1) break;

        int produtoEncontrado = 0;
        for (int i = 0; i < *count; i++) {
            if (produtos[i].id == id) {
                produtoEncontrado = 1;
                printf("Digite a quantidade que deseja vender de %s: ", produtos[i].nome);
                scanf("%d", &quantidade);

                if (quantidade <= produtos[i].quantidade) {
                    float subtotal = quantidade * produtos[i].preco;
                    totalVenda += subtotal;
                    produtos[i].quantidade -= quantidade;

                    salvarHistoricoVenda(produtos[i].id, produtos[i].nome, quantidade, produtos[i].preco, subtotal, totalVenda);
                    printf("Venda registrada: %d unidades de %s - Total: R$ %.2f\n", quantidade, produtos[i].nome, subtotal);
                } else {
                    printf("Quantidade indisponível em estoque.\n");
                }
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("Produto com ID %d não encontrado.\n", id);
        }
    }

    printf("Valor total da venda: R$ %.2f\n", totalVenda);
    salvarProdutosNoArquivo(produtos, *count);
}

int numerarVenda() {
    int num_max = 0;
    struct dirent *entrada;
    DIR *dp = opendir(".");

    if (dp == NULL) {
        perror("Erro ao abrir o diretório.");
        return 1;
    }

    while ((entrada = readdir(dp))) {
        if (strncmp(entrada->d_name, "venda_", 6) == 0) {
            int num;
            if (sscanf(entrada->d_name, "venda_%d.txt", &num) == 1) {
                if (num > num_max) {
                    num_max = num;
                }
            }
        }
    }
    closedir(dp);
    return num_max + 1; // Retorna o próximo número de arquivo
}

void salvarHistoricoVenda(int id, const char *nome, int quantidade, float preco, float subtotal, float total) {
    int num_max = numerarVenda();
    char filename[20];
    sprintf(filename, "venda_%d.txt", num_max); // Cria um nome único para cada arquivo de venda

    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de histórico de vendas.\n");
        return;
    }

    fprintf(fp, "ID: %d, Nome: %s, Quantidade: %d, Preço unitário: %.2f, Subtotal: %.2f\n", id, nome, quantidade, preco, subtotal);
    fprintf(fp, "Valor total da venda: %.2f\n", total);
    fclose(fp);
}

// Função para mostrar o histórico de uma venda específica
void mostrarHistoricoVendaEspecifica() {
    int num_max = numerarVenda();
    printf("Digite o ID da venda que deseja visualizar: ");
    scanf("%d", &num_max);

    char filename[20];
    sprintf(filename, "venda_%d.txt", num_max); // Nome do arquivo de venda

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Histórico de venda não encontrado para o ID %d.\n", num_max);
        return;
    }

    printf("\n--- Histórico da Venda ID %d ---\n", num_max);
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
}

// Menu principal com opção de visualizar histórico de venda específica
void menu(Produto *produtos, int *count) {
    int opcao;
    do {
        printf("\033[0;32m"); // Set the text to the color green
        printf("\n--- HortiFruti GALC ---\n");
        printf("\033[0m"); // Resets the text to default color
        printf("\n--- Controle de Estoque ---\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("5. Realizar Venda\n");
        printf("6. Mostrar Histórico de Venda Específica\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                adicionarProduto(produtos, count);
                getche();
                system("cls");
                break;
            case 2:
                listarProdutos(produtos, *count);
                puts("\n\n\tPressione qualquer tecla para continuar.");
                getche();
                system("cls");
                break;
            case 3:
                buscarProduto(produtos, *count);
                break;
            case 4:
                removerProduto(produtos, count);
                getche();
                system("cls");
                break;
            case 5:
                realizarVenda(produtos, count);
                getche();
                system("cls");
                break;
            case 6:
                mostrarHistoricoVendaEspecifica();
                puts("\n\n\tPressione qualquer tecla para continuar.");
                getche();
                system("cls");
                break;
            case 7:
                printf("Saindo do sistema.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                getche();
                system("cls");
        }
    } while(opcao != 7);
}
