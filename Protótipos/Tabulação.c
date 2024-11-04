#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#define MAX 100
#define FILE_NAME "estoque.txt"
#define NOME_MAX 16  // 15 caracteres + 1 para o terminador nulo

typedef struct {
    int id;
    char nome[NOME_MAX];
    int quantidade;
    float preco;
} Produto;

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

void menu(Produto *produtos, int *count) {
    int opcao;
    setlocale(LC_ALL, "Portuguese");
    do {
        printf("\033[0;32m"); // Set the text to the color green
        printf("\n--- HortiFruti GALC ---\n");
        printf("\033[0m"); // Resets the text to default color
        printf("\n--- Controle de Estoque ---\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("5. Sair\n");
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
                printf("Saindo do sistema.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                getche();
                system("cls");
        }
    } while(opcao != 5);
}

int main() {
     setlocale(LC_ALL, "Portuguese");
    Produto produtos[MAX];
    int count = 0;
    carregarProdutosDoArquivo(produtos, &count);
    menu(produtos, &count);
    return 0;
}
