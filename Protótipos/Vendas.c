#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <dirent.h> // Biblioteca para manipulação de diretórios

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
void realizarVenda(Produto *produtos, int *count, int *venda_id);
void salvarHistoricoVenda(int venda_id, int id, const char *nome, int quantidade, float preco, float subtotal, float total);
void mostrarHistoricoVendaEspecifica();
void listarHistoricoVendas(); // Nova função para listar todos os arquivos de vendas

void menu(Produto *produtos, int *count, int *venda_id);

int main() {
    setlocale(LC_ALL, "Portuguese");
    Produto produtos[MAX];
    int count = 0, venda_id = 1;
    carregarProdutosDoArquivo(produtos, &count);
    menu(produtos, &count, &venda_id);
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
void realizarVenda(Produto *produtos, int *count, int *venda_id) {
    int id, quantidade;
    float totalVenda = 0.0;

    printf("\n--- Realizar Venda ---\n");

    while (1) {
        printf("\nDigite o ID do produto (ou 0 para finalizar): ");
        scanf("%d", &id);

        if (id == 0) break;

        int produtoEncontrado = 0;
        for (int i = 0; i < *count; i++) {
            if (produtos[i].id == id) {
                produtoEncontrado = 1;
                printf("\nDigite a quantidade que deseja vender de %s: ", produtos[i].nome);
                scanf("%d", &quantidade);

                if (quantidade <= produtos[i].quantidade) {
                    float subtotal = quantidade * produtos[i].preco;
                    totalVenda += subtotal;
                    produtos[i].quantidade -= quantidade;

                    salvarHistoricoVenda(*venda_id, produtos[i].id, produtos[i].nome, quantidade, produtos[i].preco, subtotal, totalVenda);
                    printf("\n\nVenda registrada: %d unidades de %s - Total: R$ %.2f\n", quantidade, produtos[i].nome, subtotal);
                } else {
                    printf("\nQuantidade indisponível em estoque.\n");
                }
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("\nProduto com ID %d não encontrado.\n", id);
        }
    }

    printf("\n\n\tValor total da venda: R$ %.2f\n", totalVenda);
    salvarProdutosNoArquivo(produtos, *count);
    (*venda_id)++; // Incrementa o ID da próxima venda
}

void salvarHistoricoVenda(int venda_id, int id, const char *nome, int quantidade, float preco, float subtotal, float total) {
    char filename[20];
    sprintf(filename, "venda_%d.txt", venda_id); // Cria um nome único para cada arquivo de venda

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
    int venda_id;
    printf("\nDigite o ID da venda que deseja visualizar: ");
    scanf("%d", &venda_id);

    char filename[20];
    sprintf(filename, "venda_%d.txt", venda_id); // Nome do arquivo de venda

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\nHistórico de venda não encontrado para o ID %d.\n", venda_id);
        return;
    }

    printf("\n--- Histórico da Venda ID %d ---\n", venda_id);
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s\n", line);
    }
    printf("--------------------------------\n");
    fclose(fp);
}

// Função para listar todos os arquivos de histórico de vendas
void listarHistoricoVendas() {
    DIR *d;
    struct dirent *dir;
    d = opendir("."); // Abre o diretório atual
    if (d) {
        printf("\n--- Histórico de Vendas Disponíveis ---\n");
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, "venda_", 6) == 0 && strstr(dir->d_name, ".txt") != NULL) {
                printf("%s\n", dir->d_name);
            }
        }
        printf("---------------------------------------\n");
        closedir(d);
    } else {
        printf("Erro ao acessar o diretório.\n");
    }
}

// Menu principal com opção de visualizar histórico de venda específica e listar todas as vendas
void menu(Produto *produtos, int *count, int *venda_id) {
    int opcao;
    do {
        printf("\033[0;32m"); //Set the text to the color green
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); //Resets the text to default color

        printf("===============================\n");
        printf("CONTROLE DE ESTOQUE\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("5. Realizar Venda\n");
        printf("6. Mostrar Histórico de Venda Específica\n");
        printf("7. Listar Todas as Vendas\n");
        printf("8. Sair\n");
        printf("-------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                adicionarProduto(produtos, count);
                break;
            case 2:
                listarProdutos(produtos, *count);
                break;
            case 3:
                buscarProduto(produtos, *count);
                break;
            case 4:
                removerProduto(produtos, count);
                break;
            case 5:
                realizarVenda(produtos, count, venda_id);
                break;
            case 6:
                mostrarHistoricoVendaEspecifica();
                break;
            case 7:
                listarHistoricoVendas();
                break;
            case 8:
                puts("Saindo do Sistema");
                break;
            default:
                break;
        }

                puts("\n\n\tPressione qualquer tecla para continuar.");
                getche();
                system("cls");

    } while(opcao != 8);
}
