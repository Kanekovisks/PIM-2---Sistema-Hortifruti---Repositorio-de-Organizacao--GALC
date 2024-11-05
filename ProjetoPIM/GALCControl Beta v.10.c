/*
##################################################
UNIP 2024
Projeto Aplicativo para Hortifruti - GALC - GALCControl Beta v.10
PIM Curso ADS - UNIP Campus Dutra

Grupo Responsável:
Gabriel Roxo | 2º Semestre
Camila Pereira | 1º Semestre
Arreginaldo Aparecido | 2º Semestre
Lívia Borges | 2º Semestre

Turma: DS2R48
##################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <dirent.h>

// Arquivos de dados
#define FILE_NAME "estoque.txt"
#define USERS_FILE "usuarios.txt"
#define CASHFLOW_FILE "fluxo_caixa.txt"
#define CASH_FILE "vendas.txt"

// Limitações de strings
#define MAX 100
#define USER_MAX 10
#define NOME_MAX 16
#define SENHA_MAX 16


// Structs
// Struct de Produto
typedef struct {
    int id;
    char nome[NOME_MAX];
    int quantidade;
    float preco;
} Produto;

// Struct de Fluxo de Caixa
typedef struct {
    char descricao[50];
    float valor;
    char tipo; // 'E' para entrada, 'S' para saída
    time_t data; // Timestamp da transação
} Transacao;

// Struct de Usuarios
typedef struct {
    char nome[NOME_MAX];
    char senha[SENHA_MAX];
    int is_admin; // 1 - ADM / 0 - Básico
} Usuario;


// Protótipos de Funções
// Protótipos de Estoque
void salvarProdutosNoArquivo(Produto *produtos, int count);
void carregarProdutosDoArquivo(Produto *produtos, int *count);
void adicionarProduto(Produto *produtos, int *count);
void listarProdutos(Produto *produtos, int count);
void buscarProduto(Produto *produtos, int count);
void removerProduto(Produto *produtos, int *count);
void menuEstoque(Produto *produtos, int *count);

// Protótipos de Venda
void realizarVenda(Produto *produtos, int *count);
void salvarHistoricoVenda(int id, const char *nome, int quantidade, float preco, float subtotal, float total);
void buscarHistoricoVenda();
void listarHistoricoVendas();
int numerarVenda();
void menuVenda(Produto *produtos, int *count);

// Protótipos de Fluxo de Caixa
void registrarTransacao();
void visualizarExtratoDiario();
void visualizarExtratoSemanal();
void visualizarExtratoMensal();
void menuCaixa();

// Protótipos de Login e Gerenciamento de Usuários
int login(Usuario *usuario);
void carregarUsuarios(Usuario *usuarios, int *user_count);
void salvarUsuarios(Usuario *usuarios, int user_count);
void gerenciarUsuarios(Usuario *usuarios, int *user_count);
void adicionarUsuario(Usuario *usuarios, int *user_count);
void removerUsuario(Usuario *usuarios, int *user_count);

// Protótipo do Menu Principal
void menuPrincipal(Produto *produtos, int *count, int *user_count, Usuario *usuarios, Usuario *usuario_atual);



// Função Main
int main() {
    setlocale(LC_ALL, "Portuguese");

    Produto produtos[MAX];
    int count = 0;
    carregarProdutosDoArquivo(produtos, &count);

    Usuario usuarios[USER_MAX];
    int user_count = 0;
    carregarUsuarios(usuarios, &user_count);
    Usuario usuario_atual;
    if (!login(&usuario_atual)) {
        printf("\nLogin falhou. Encerrando o programa.\n");
        return 1;
    }


    system("cls");
    menuPrincipal(produtos, &count, &user_count, usuarios, &usuario_atual);
    return 0;
}



// Função do Menu Principal do programa
void menuPrincipal(Produto *produtos, int *count, int *user_count, Usuario *usuarios, Usuario *usuario_atual) {
    int opcao;

    do {
        printf("\033[0;32m");
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m");

        printf("===============================\n");
        printf("MENU PRINCIPAL\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Controle de Estoque\n");
        printf("2. Controle de Vendas\n");
        if (usuario_atual->is_admin) {
            printf("3. Fluxo de Caixa\n");
            printf("4. Gerenciamento de Contas\n");
        }
        printf("0. Sair\n");
        printf("-------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        system("cls");

        switch(opcao) {

            case 1:
                menuEstoque(produtos, count);
                break;

            case 2:
                menuVenda(produtos, count);
                break;

            case 3:
                if (usuario_atual->is_admin) {
                    menuCaixa();
                } else {
                    printf("\nOpção inválida. Tente novamente.\n");
                    break;
                }
                break;

            case 4:
                if (usuario_atual->is_admin) {
                    gerenciarUsuarios(usuarios, user_count);
                } else {
                    printf("\nOpção inválida. Tente novamente.\n");
                    break;
                }
                break;

            case 0:
                printf("\nSaindo do sistema.\n");
                return;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        puts("\n\n\tPressione qualquer tecla para continuar.");
        getche();
        fflush(stdin);
        system("cls");

    } while(opcao != 0);
}



// Funções de Menu do estoque
void menuEstoque(Produto *produtos, int *count) {
    int opcao;

    do {
        printf("\033[0;32m"); // Linha para colorir o texto para verde
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); // Retorna o texto a cor normal

        printf("===============================\n");
        printf("ESTOQUE\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
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

            case 0:
                return;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        puts("\n\n\tPressione qualquer tecla para continuar.");
        getche();
        fflush(stdin);
        system("cls");

    } while (opcao != 0);
}

// Funções auxiliares do estoque
// Salva produtos na lista
void salvarProdutosNoArquivo(Produto *produtos, int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return;
    }

    fprintf(fp, "ID\tNome           \tQuantidade\tPreço\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d\t%-15s\t%d\t\t%.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }

    fclose(fp);
    printf("Dados salvos no arquivo %s\n", FILE_NAME);
}

// Carrega a lista de produtos
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

// Adiciona produto na lista
void adicionarProduto(Produto *produtos, int *count) {
    if (*count >= MAX) {
        printf("Limite máximo de produtos atingido.\n");
        return;
    }

    printf("Digite o ID do produto: ");
    scanf("%d", &produtos[*count].id);
    fflush(stdin);
    printf("Digite o nome do produto (max 15 caracteres): ");
    getchar();
    fgets(produtos[*count].nome, NOME_MAX, stdin);
    produtos[*count].nome[strcspn(produtos[*count].nome, "\n")] = '\0'; // Remover newline
    if (strlen(produtos[*count].nome) > 15) {
        produtos[*count].nome[15] = '\0'; // 15 caracteres
    }
    printf("Digite a quantidade: ");
    scanf("%d", &produtos[*count].quantidade);
    fflush(stdin);
    printf("Digite o preço: ");
    scanf("%f", &produtos[*count].preco);
    fflush(stdin);

    (*count)++;
    salvarProdutosNoArquivo(produtos, *count);
    printf("Produto adicionado com sucesso!\n");
}

// Mostra todos os produtos da lista
void listarProdutos(Produto *produtos, int count) {
    printf("\n----------------- Lista de Produtos -----------------\n\n");
    printf("ID\tNome           \tQuantidade\tPreço\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%-15s\t%d\t\t%.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }
    printf("-----------------------------------------------------\n");
}

// Procura produto na lista
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

// Remove produto da lista
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



// Funções de Menu de Vendas
void menuVenda(Produto *produtos, int *count){
    int opcao_venda;

    do {
        printf("\033[0;32m");
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m");

        printf("===============================\n");
        printf("CONTROLE DE VENDAS\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Listar Produtos\n");
        printf("2. Realizar Venda\n");
        printf("3. Buscar Histórico de Venda\n");
        printf("4. Listar Todas as Vendas\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
        printf("Escolha uma opção: ");

        scanf("%d", &opcao_venda);

        switch(opcao_venda) {

            case 1:
                listarProdutos(produtos, *count);
                break;
            case 2:
                realizarVenda(produtos, count);
                break;
            case 3:
                buscarHistoricoVenda();
                break;
            case 4:
                listarHistoricoVendas();
                break;
            case 0:
                return;
            default:
                break;
        }

                puts("\n\n\tPressione qualquer tecla para continuar.");
                getche();
                system("cls");

    } while(opcao_venda != 0);
}

// Funções auxiliares de Vendas
// Realiza uma venda
void realizarVenda(Produto *produtos, int *count) {
    Transacao transacao;
    transacao.data = time(NULL); // Data e hora atual

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

     FILE *fp = fopen(CASHFLOW_FILE, "a");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de transações.\n");
        return;
    }
    fprintf(fp, "Venda %.2f E %ld\n", totalVenda, transacao.data);
    fclose(fp);

    salvarProdutosNoArquivo(produtos, *count);
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

// Mostra o histórico de uma venda específica
void buscarHistoricoVenda() {
    int num_max = numerarVenda();
    printf("\nDigite o ID da venda que deseja visualizar: ");
    scanf("%d", &num_max);

    char filename[20];
    sprintf(filename, "venda_%d.txt", num_max); // Nome do arquivo de venda

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\nHistórico de venda não encontrado para o ID %d.\n", num_max);
        return;
    }

    printf("\n--- Histórico da Venda ID %d ---\n", num_max);
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s\n", line);
    }
    printf("-------------------------------\n");
    fclose(fp);
}

// Lista todos os arquivos de histórico de vendas
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



// Funções de Menu do Fluxo de Caixa
void menuCaixa() {
    int opcao;

    do {
        printf("\033[0;32m");
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m");

        printf("===============================\n");
        printf("FLUXO DE CAIXA\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Registrar Transação\n");
        printf("2. Extrato Diário\n");
        printf("3. Extrato Semanal\n");
        printf("4. Extrato Mensal\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
        printf("Escolha uma opção: ");

        scanf("%d", &opcao);

        switch(opcao) {

            case 1:
                registrarTransacao();
                break;

            case 2:
                visualizarExtratoDiario();
                break;

            case 3:
                visualizarExtratoSemanal();
                break;

            case 4:
                visualizarExtratoMensal();
                break;

            case 0:
                return;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        printf("\nPressione qualquer tecla para continuar...");
        getche();
        fflush(stdin);
        system("cls");

    } while (opcao != 0);
}

// Registra uma nova transação
void registrarTransacao() {
    Transacao transacao;
    printf("Descrição da transação: ");
    scanf(" %[^\n]s", transacao.descricao);
    printf("Valor da transação: ");
    scanf("%f", &transacao.valor);
    printf("Tipo (E para entrada, S para saída): ");
    scanf(" %c", &transacao.tipo);
    transacao.data = time(NULL); // Data e hora atual

    FILE *fp = fopen(CASHFLOW_FILE, "a");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de transações.\n");
        return;
    }
    fprintf(fp, "%s %.2f %c %ld\n", transacao.descricao, transacao.valor, transacao.tipo, transacao.data);
    fclose(fp);

    printf("\nTransação registrada com sucesso.\n");
}

// Visualiza extrato diário
void visualizarExtratoDiario() {
    FILE *fp = fopen(CASHFLOW_FILE, "r");
    if (fp == NULL) {
        printf("\nNenhuma transação registrada.\n");
        return;
    }

    Transacao transacao;
    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);
    float total = 0.0;

    printf("\n--- Extrato Diário ---\n");
    while (fscanf(fp, "%49s %f %c %ld", transacao.descricao, &transacao.valor, &transacao.tipo, &transacao.data) == 4) {
        struct tm *dataTransacao = localtime(&transacao.data);
        if (dataTransacao->tm_year == dataAtual->tm_year &&
            dataTransacao->tm_mon == dataAtual->tm_mon &&
            dataTransacao->tm_mday == dataAtual->tm_mday) {
            printf("Descrição: %s | Valor: %.2f | Tipo: %c\n", transacao.descricao, transacao.valor, transacao.tipo);
            total += (transacao.tipo == 'E' ? transacao.valor : -transacao.valor);
        }
    }
    printf("Saldo do dia: %.2f\n", total);
    fclose(fp);
}

// Visualiza extrato semanal
void visualizarExtratoSemanal() {
    FILE *fp = fopen(CASHFLOW_FILE, "r");
    if (fp == NULL) {
        printf("\nNenhuma transação registrada.\n");
        return;
    }

    Transacao transacao;
    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);
    time_t inicioSemana = agora - (dataAtual->tm_wday * 24 * 3600); // Primeira hora do domingo
    float total = 0.0;

    printf("\n--- Extrato Semanal ---\n");
    while (fscanf(fp, "%49s %f %c %ld", transacao.descricao, &transacao.valor, &transacao.tipo, &transacao.data) == 4) {
        if (transacao.data >= inicioSemana && transacao.data <= agora) {
            struct tm *dataTransacao = localtime(&transacao.data);
            printf("Descrição: %s | Valor: %.2f | Tipo: %c | Data: %02d/%02d/%04d\n",
                   transacao.descricao, transacao.valor, transacao.tipo,
                   dataTransacao->tm_mday, dataTransacao->tm_mon + 1, dataTransacao->tm_year + 1900);
            total += (transacao.tipo == 'E' ? transacao.valor : -transacao.valor);
        }
    }
    printf("Saldo da semana: %.2f\n", total);
    fclose(fp);
}

// Visualiza extrato mensal
void visualizarExtratoMensal() {
    FILE *fp = fopen(CASHFLOW_FILE, "r");
    if (fp == NULL) {
        printf("\nNenhuma transação registrada.\n");
        return;
    }

    Transacao transacao;
    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);
    float total = 0.0;

    printf("\n--- Extrato Mensal ---\n");
    while (fscanf(fp, "%49s %f %c %ld", transacao.descricao, &transacao.valor, &transacao.tipo, &transacao.data) == 4) {
        struct tm *dataTransacao = localtime(&transacao.data);
        if (dataTransacao->tm_year == dataAtual->tm_year &&
            dataTransacao->tm_mon == dataAtual->tm_mon) {
            printf("Descrição: %s | Valor: %.2f | Tipo: %c | Data: %02d/%02d/%04d\n",
                   transacao.descricao, transacao.valor, transacao.tipo,
                   dataTransacao->tm_mday, dataTransacao->tm_mon + 1, dataTransacao->tm_year + 1900);
            total += (transacao.tipo == 'E' ? transacao.valor : -transacao.valor);
        }
    }
    printf("Saldo do mês: %.2f\n", total);
    fclose(fp);
}



// Funções de Menu de Contas
void gerenciarUsuarios(Usuario *usuarios, int *user_count) {
    int opcao;
    do {
        printf("\033[0;32m");
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m");

        printf("===============================\n");
        printf("GERENCIAMENTO DE USUÁRIOS\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Adicionar Usuário\n");
        printf("2. Listar Usuários\n");
        printf("3. Remover Usuários\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {

            case 1:
                adicionarUsuario(usuarios, user_count);
                break;

            case 2:
                printf("\n--- Lista de Usuários ---\n");
                for (int i = 0; i < *user_count; i++) {
                    printf("Nome: %s, Tipo: %s\n", usuarios[i].nome, usuarios[i].is_admin ? "Admin" : "Normal");
                }
                printf("-------------------------\n");
                break;

            case 3:
                removerUsuario(usuarios, user_count);
                break;

            case 0:
                return;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;

        }

        puts("\n\n\tPressione qualquer tecla para continuar.");
        getche();
        fflush(stdin);
        system("cls");

    } while (opcao != 0);
}

// Funções auxiliares de Gerenciamento de Contas
// Carrega a lista de Usuários
void carregarUsuarios(Usuario *usuarios, int *user_count) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (fp == NULL) {
        printf("\nNenhum arquivo de usuários encontrado. Criando usuário administrador padrão.\n");

        // Criar usuário padrão "ADM" com senha "1234" e permissão de administrador
        strcpy(usuarios[0].nome, "ADM");
        strcpy(usuarios[0].senha, "1234");
        usuarios[0].is_admin = 1;
        *user_count = 1;
        salvarUsuarios(usuarios, *user_count);
        return;
    }

    *user_count = 0;
    while (fscanf(fp, "%15s %15s %d", usuarios[*user_count].nome, usuarios[*user_count].senha, &usuarios[*user_count].is_admin) == 3) {
        (*user_count)++;
        if (*user_count >= USER_MAX) break;

    }

    fclose(fp);
}

// Salva a lista de usuários
void salvarUsuarios(Usuario *usuarios, int user_count) {
    FILE *fp = fopen(USERS_FILE, "w");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de usuários.\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(fp, "%s %s %d\n", usuarios[i].nome, usuarios[i].senha, usuarios[i].is_admin);
    }

    fclose(fp);
}

// Menu de login/autenticação
int login(Usuario *usuario) {
    Usuario usuarios[USER_MAX];
    int user_count;
    carregarUsuarios(usuarios, &user_count);

    char nome[NOME_MAX], senha[SENHA_MAX];

        printf("\033[0;32m");
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m");

        printf("===============================\n");
        printf("LOGIN\n");
        printf("===============================\n\n");

        printf("Nome de usuário: ");
        scanf("%s", nome);
        printf("Senha: ");
        scanf("%s", senha);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            *usuario = usuarios[i];
            printf("Login bem-sucedido!\n");
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            system("cls");
            return 1;
        }
    }

    printf("Usuário ou senha incorretos.\n");
    getche();
    return 0;
}

// Adiciona usuário na lista
void adicionarUsuario(Usuario *usuarios, int *user_count){
    if (*user_count > USER_MAX) {
        printf("Número máximo de usuários atingido.\n");
        return;
    }
    printf("Nome do novo usuário: ");
    scanf("%s", usuarios[*user_count].nome);
    printf("Senha do novo usuário: ");
    scanf("%s", usuarios[*user_count].senha);
    printf("É administrador? (1 para Sim, 0 para Não): ");
    scanf("%d", &usuarios[*user_count].is_admin);
    (*user_count)++;
    salvarUsuarios(usuarios, *user_count);
    printf("Usuário adicionado com sucesso.\n");
}

// Remove usuário da lista
void removerUsuario(Usuario *usuarios, int *user_count) {
    char nome[NOME_MAX];
    printf("Digite o nome do usuário que deseja remover: ");
    scanf("%s", nome);

    int index = -1;
    for (int i = 0; i < *user_count; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nUsuário não encontrado.\n");
        return;
    }

    for (int i = index; i < *user_count - 1; i++) {
        usuarios[i] = usuarios[i + 1];
    }
    (*user_count)--;

    salvarUsuarios(usuarios, *user_count);
    printf("\nUsuário removido com sucesso.\n");
}
