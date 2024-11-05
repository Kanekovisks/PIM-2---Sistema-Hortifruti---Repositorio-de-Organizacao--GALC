#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <dirent.h>

#define MAX 100
#define FILE_NAME "estoque.txt"
#define FLUXO_CAIXA "fluxo_caixa.txt"
#define NOME_MAX 16  // 15 caracteres + 1 para o terminador nulo

typedef struct {
    int id;
    char nome[NOME_MAX];
    int quantidade;
    float preco;
} Produto;

// Estrutura para armazenar o fluxo de caixa
typedef struct {
    char data[20];
    float entrada;
    float saida;
} FluxoCaixa;

// Funções de Estoque
void salvarProdutosNoArquivo(Produto *produtos, int count);
void carregarProdutosDoArquivo(Produto *produtos, int *count);
void adicionarProduto(Produto *produtos, int *count);
void listarProdutos(Produto *produtos, int count);
void buscarProduto(Produto *produtos, int count);
void removerProduto(Produto *produtos, int *count);

// Funções de Venda e Fluxo de Caixa
void realizarVenda(Produto *produtos, int *count);
void salvarHistoricoVenda(int id, const char *nome, int quantidade, float preco, float subtotal, float total);
void buscarHistoricoVenda();
void listarHistoricoVendas();
int numerarVenda();
void registrarFluxoCaixa(const char *tipo, float valor);
void registrarCompra();

// Funções de Relatório de Fluxo de Caixa
void menuFluxoCaixa();
void exibirExtrato();


void menu(Produto *produtos, int *count, int *venda_id);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Produto produtos[MAX];
    int count = 0, venda_id = 1;
    carregarProdutosDoArquivo(produtos, &count);
    menu(produtos, &count, &venda_id);
    return 0;
}

// Funções de estoque
void salvarProdutosNoArquivo(Produto *produtos, int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %d %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }
    fclose(fp);
}

void carregarProdutosDoArquivo(Produto *produtos, int *count) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("Arquivo não encontrado. Iniciando com lista vazia.\n");
        *count = 0;
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // Ignora a linha de cabeçalho (ID,QUANT,TIPO,PREÇO)

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
    printf("\n--- Adicionar Produto ---\n");
    Produto novoProduto;
    novoProduto.id = *count + 1;
    printf("Digite o nome do produto: ");
    scanf("%s", novoProduto.nome);
    printf("Digite a quantidade: ");
    scanf("%d", &novoProduto.quantidade);
    printf("Digite o preço: ");
    scanf("%f", &novoProduto.preco);

    produtos[*count] = novoProduto;
    (*count)++;
    salvarProdutosNoArquivo(produtos, *count);
    printf("Produto adicionado com sucesso.\n");
}

void listarProdutos(Produto *produtos, int count) {
    printf("\n--- Lista de Produtos ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Nome: %s | Quantidade: %d | Preço: R$ %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }
}

void buscarProduto(Produto *produtos, int count) {
    int id;
    printf("Digite o ID do produto que deseja buscar: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (produtos[i].id == id) {
            printf("Produto encontrado: ID: %d | Nome: %s | Quantidade: %d | Preço: R$ %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
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


// Realiza uma venda
void realizarVenda(Produto *produtos, int *count) {
    int id, quantidade;
    float totalVenda = 0.0;

    printf("\n--- Realizar Venda ---\n");

    while (1) {
        printf("Digite o ID do produto (ou 0 para finalizar): ");
        scanf("%d", &id);

        if (id == 0) break;

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

    registrarFluxoCaixa("ENTRADA", totalVenda);
}

// Numera arquivo de venda
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

// Salva venda no arquivo
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


void listarHistoricoVendas() {
    FILE *fp = fopen("historico_vendas.txt", "r");
    if (fp == NULL) {
        printf("Nenhum histórico de vendas encontrado.\n");
        return;
    }
    char linha[200];
    while (fgets(linha, sizeof(linha), fp)) {
        printf("%s", linha);
    }
    fclose(fp);
}


// Funções de relatório de fluxo de caixa
void exibirRelatorioDiario() {
    // Implementação para exibir relatório diário
}

void exibirRelatorioSemanal() {
    // Implementação para exibir relatório semanal
}

void exibirRelatorioMensal() {
    // Implementação para exibir relatório mensal
}

void menuFluxoCaixa() {
    int opcao;
    do {
        printf("\n--- Menu de Fluxo de Caixa ---\n");
        printf("1. Exibir Relatório\n");
        printf("4. Registrar Compra\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirExtrato();
                break;
            case 4:
                registrarCompra();
                break;
            case 5:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);
}

// Função de menu principal
void menu(Produto *produtos, int *count, int *venda_id) {
    int opcao;
    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("5. Realizar Venda\n");
        printf("6. Listar Histórico de Vendas\n");
        printf("7. Menu de Fluxo de Caixa\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
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
                realizarVenda(produtos, count);
                break;
            case 6:
                listarHistoricoVendas();
                break;
            case 7:
                menuFluxoCaixa();
                break;
            case 8:
                printf("Saindo do Sistema...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        if (opcao != 8) {
            printf("\nPressione qualquer tecla para continuar.\n");
            getche();
            system("cls");
        }
    } while (opcao != 8);
}



// Função para registrar uma transação de entrada ou saída
void registrarCompra() {
    float valorCompra;
    printf("Digite o valor da compra: ");
    scanf("%f", &valorCompra);

    registrarFluxoCaixa("SAÍDA", valorCompra);

    printf("Compra registrada com sucesso como saída de R$ %.2f no fluxo de caixa.\n", valorCompra);
}

// Função para registrar fluxo de caixa
void registrarFluxoCaixa(const char *tipo, float valor) {
    FILE *fp = fopen(FLUXO_CAIXA, "a");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de fluxo de caixa.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(fp, "%s: R$ %.2f DATA: %02d/%02d/%d %02d:%02d\n", tipo, valor, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    fclose(fp);
}



// Função para exibir o extrato diário
void exibirExtrato() {
    FILE *fp = fopen(FLUXO_CAIXA, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de fluxo de caixa.\n");
        return;
    }

    float totalEntrada = 0.0, totalSaida = 0.0;
    char linha[100];

    printf("\n--- Extrato ---\n");
    while (fgets(linha, sizeof(linha), fp)) {
        FluxoCaixa fluxo;

        // Alteração: usamos ; como delimitador na leitura
        sscanf(linha, "%f%f", &fluxo.entrada, &fluxo.saida);

        totalEntrada += fluxo.entrada;
        totalSaida += fluxo.saida;

        printf("Entrada: R$ %.2f | Saída: R$ %.2f | Data: %szn",&fluxo.entrada, &fluxo.saida, &fluxo.data);
    }
    fclose(fp);

    printf("---------------\n");
    printf("Total Entrada: R$ %.2f\n", totalEntrada);
    printf("Total Saída: R$ %.2f\n", totalSaida);
    printf("Saldo: R$ %.2f\n", totalEntrada - totalSaida);
}
