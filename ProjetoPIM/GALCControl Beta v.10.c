/*
##################################################
UNIP 2024
Projeto Aplicativo para Hortifruti - GALC - GALCControl Beta v.10
PIM Curso ADS - UNIP Campus Dutra

Grupo Respons�vel:
Gabriel Roxo | 2� Semestre
Camila Pereira | 1� Semestre
Arreginaldo Aparecido | 2� Semestre
L�via Borges | 2� Semestre

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

// Limita��es de strings
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
    char tipo; // 'E' para entrada, 'S' para sa�da
    time_t data; // Timestamp da transa��o
} Transacao;

// Struct de Usuarios
typedef struct {
    char nome[NOME_MAX];
    char senha[SENHA_MAX];
    int is_admin; // 1 - ADM / 0 - B�sico
} Usuario;


// Prot�tipos de Fun��es
// Prot�tipos de Estoque
void salvarProdutosNoArquivo(Produto *produtos, int count);
void carregarProdutosDoArquivo(Produto *produtos, int *count);
void adicionarProduto(Produto *produtos, int *count);
void listarProdutos(Produto *produtos, int count);
void buscarProduto(Produto *produtos, int count);
void removerProduto(Produto *produtos, int *count);
void menuEstoque(Produto *produtos, int *count);

// Prot�tipos de Venda
void realizarVenda(Produto *produtos, int *count);
void salvarHistoricoVenda(int id, const char *nome, int quantidade, float preco, float subtotal, float total);
void buscarHistoricoVenda();
void listarHistoricoVendas();
int numerarVenda();
void menuVenda(Produto *produtos, int *count);

// Prot�tipos de Fluxo de Caixa
void registrarTransacao();
void visualizarExtratoDiario();
void visualizarExtratoSemanal();
void visualizarExtratoMensal();
void menuCaixa();

// Prot�tipos de Login e Gerenciamento de Usu�rios
int login(Usuario *usuario);
void carregarUsuarios(Usuario *usuarios, int *user_count);
void salvarUsuarios(Usuario *usuarios, int user_count);
void gerenciarUsuarios(Usuario *usuarios, int *user_count);
void adicionarUsuario(Usuario *usuarios, int *user_count);
void removerUsuario(Usuario *usuarios, int *user_count);

// Prot�tipo do Menu Principal
void menuPrincipal(Produto *produtos, int *count, int *user_count, Usuario *usuarios, Usuario *usuario_atual);



// Fun��o Main
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



// Fun��o do Menu Principal do programa
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
        printf("Escolha uma op��o: ");
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
                    printf("\nOp��o inv�lida. Tente novamente.\n");
                    break;
                }
                break;

            case 4:
                if (usuario_atual->is_admin) {
                    gerenciarUsuarios(usuarios, user_count);
                } else {
                    printf("\nOp��o inv�lida. Tente novamente.\n");
                    break;
                }
                break;

            case 0:
                printf("\nSaindo do sistema.\n");
                return;
            default:
                printf("\nOp��o inv�lida. Tente novamente.\n");
                break;
        }

        puts("\n\n\tPressione qualquer tecla para continuar.");
        getche();
        fflush(stdin);
        system("cls");

    } while(opcao != 0);
}



// Fun��es de Menu do estoque
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
        printf("Escolha uma op��o: ");
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
                printf("\nOp��o inv�lida. Tente novamente.\n");
                break;
        }

        puts("\n\n\tPressione qualquer tecla para continuar.");
        getche();
        fflush(stdin);
        system("cls");

    } while (opcao != 0);
}

// Fun��es auxiliares do estoque
// Salva produtos na lista
void salvarProdutosNoArquivo(Produto *produtos, int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return;
    }

    fprintf(fp, "ID\tNome           \tQuantidade\tPre�o\n");
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
        printf("Arquivo n�o encontrado. Iniciando com lista vazia.\n");
        *count = 0;
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // Ignora a linha de cabe�alho (ID,QUANT,TIPO,PRE�O)

    *count = 0;
    while (fscanf(fp, "%d\t%15[^\t]\t%d\t%f\n", &produtos[*count].id, produtos[*count].nome, &produtos[*count].quantidade, &produtos[*count].preco) == 4) {
        produtos[*count].nome[15] = '\0'; // Garante que o nome tenha no m�ximo 15 caracteres
        (*count)++;
        if (*count >= MAX) break;
    }

    fclose(fp);
    printf("Dados carregados do arquivo %s\n", FILE_NAME);
}

// Adiciona produto na lista
void adicionarProduto(Produto *produtos, int *count) {
    if (*count >= MAX) {
        printf("Limite m�ximo de produtos atingido.\n");
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
    printf("Digite o pre�o: ");
    scanf("%f", &produtos[*count].preco);
    fflush(stdin);

    (*count)++;
    salvarProdutosNoArquivo(produtos, *count);
    printf("Produto adicionado com sucesso!\n");
}

// Mostra todos os produtos da lista
void listarProdutos(Produto *produtos, int count) {
    printf("\n----------------- Lista de Produtos -----------------\n\n");
    printf("ID\tNome           \tQuantidade\tPre�o\n");
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
            printf("ID: %d\nNome: %s\nQuantidade: %d\nPre�o: %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
            return;
        }
    }

    printf("Produto com ID %d n�o encontrado.\n", id);
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

    printf("Produto com ID %d n�o encontrado.\n", id);
}



// Fun��es de Menu de Vendas
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
        printf("3. Buscar Hist�rico de Venda\n");
        printf("4. Listar Todas as Vendas\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
        printf("Escolha uma op��o: ");

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

// Fun��es auxiliares de Vendas
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
                    printf("Quantidade indispon�vel em estoque.\n");
                }
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("Produto com ID %d n�o encontrado.\n", id);
        }
    }

    printf("Valor total da venda: R$ %.2f\n", totalVenda);

     FILE *fp = fopen(CASHFLOW_FILE, "a");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de transa��es.\n");
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
        perror("Erro ao abrir o diret�rio.");
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
    return num_max + 1; // Retorna o pr�ximo n�mero de arquivo
}

// Salva venda no arquivo
void salvarHistoricoVenda(int id, const char *nome, int quantidade, float preco, float subtotal, float total) {
    int num_max = numerarVenda();
    char filename[20];
    sprintf(filename, "venda_%d.txt", num_max); // Cria um nome �nico para cada arquivo de venda

    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de hist�rico de vendas.\n");
        return;
    }

    fprintf(fp, "ID: %d, Nome: %s, Quantidade: %d, Pre�o unit�rio: %.2f, Subtotal: %.2f\n", id, nome, quantidade, preco, subtotal);
    fprintf(fp, "Valor total da venda: %.2f\n", total);
    fclose(fp);
}

// Mostra o hist�rico de uma venda espec�fica
void buscarHistoricoVenda() {
    int num_max = numerarVenda();
    printf("\nDigite o ID da venda que deseja visualizar: ");
    scanf("%d", &num_max);

    char filename[20];
    sprintf(filename, "venda_%d.txt", num_max); // Nome do arquivo de venda

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\nHist�rico de venda n�o encontrado para o ID %d.\n", num_max);
        return;
    }

    printf("\n--- Hist�rico da Venda ID %d ---\n", num_max);
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s\n", line);
    }
    printf("-------------------------------\n");
    fclose(fp);
}

// Lista todos os arquivos de hist�rico de vendas
void listarHistoricoVendas() {
    DIR *d;
    struct dirent *dir;
    d = opendir("."); // Abre o diret�rio atual
    if (d) {
        printf("\n--- Hist�rico de Vendas Dispon�veis ---\n");
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, "venda_", 6) == 0 && strstr(dir->d_name, ".txt") != NULL) {
                printf("%s\n", dir->d_name);
            }
        }
        printf("---------------------------------------\n");
        closedir(d);
    } else {
        printf("Erro ao acessar o diret�rio.\n");
    }
}



// Fun��es de Menu do Fluxo de Caixa
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
        printf("1. Registrar Transa��o\n");
        printf("2. Extrato Di�rio\n");
        printf("3. Extrato Semanal\n");
        printf("4. Extrato Mensal\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
        printf("Escolha uma op��o: ");

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
                printf("\nOp��o inv�lida. Tente novamente.\n");
                break;
        }

        printf("\nPressione qualquer tecla para continuar...");
        getche();
        fflush(stdin);
        system("cls");

    } while (opcao != 0);
}

// Registra uma nova transa��o
void registrarTransacao() {
    Transacao transacao;
    printf("Descri��o da transa��o: ");
    scanf(" %[^\n]s", transacao.descricao);
    printf("Valor da transa��o: ");
    scanf("%f", &transacao.valor);
    printf("Tipo (E para entrada, S para sa�da): ");
    scanf(" %c", &transacao.tipo);
    transacao.data = time(NULL); // Data e hora atual

    FILE *fp = fopen(CASHFLOW_FILE, "a");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de transa��es.\n");
        return;
    }
    fprintf(fp, "%s %.2f %c %ld\n", transacao.descricao, transacao.valor, transacao.tipo, transacao.data);
    fclose(fp);

    printf("\nTransa��o registrada com sucesso.\n");
}

// Visualiza extrato di�rio
void visualizarExtratoDiario() {
    FILE *fp = fopen(CASHFLOW_FILE, "r");
    if (fp == NULL) {
        printf("\nNenhuma transa��o registrada.\n");
        return;
    }

    Transacao transacao;
    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);
    float total = 0.0;

    printf("\n--- Extrato Di�rio ---\n");
    while (fscanf(fp, "%49s %f %c %ld", transacao.descricao, &transacao.valor, &transacao.tipo, &transacao.data) == 4) {
        struct tm *dataTransacao = localtime(&transacao.data);
        if (dataTransacao->tm_year == dataAtual->tm_year &&
            dataTransacao->tm_mon == dataAtual->tm_mon &&
            dataTransacao->tm_mday == dataAtual->tm_mday) {
            printf("Descri��o: %s | Valor: %.2f | Tipo: %c\n", transacao.descricao, transacao.valor, transacao.tipo);
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
        printf("\nNenhuma transa��o registrada.\n");
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
            printf("Descri��o: %s | Valor: %.2f | Tipo: %c | Data: %02d/%02d/%04d\n",
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
        printf("\nNenhuma transa��o registrada.\n");
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
            printf("Descri��o: %s | Valor: %.2f | Tipo: %c | Data: %02d/%02d/%04d\n",
                   transacao.descricao, transacao.valor, transacao.tipo,
                   dataTransacao->tm_mday, dataTransacao->tm_mon + 1, dataTransacao->tm_year + 1900);
            total += (transacao.tipo == 'E' ? transacao.valor : -transacao.valor);
        }
    }
    printf("Saldo do m�s: %.2f\n", total);
    fclose(fp);
}



// Fun��es de Menu de Contas
void gerenciarUsuarios(Usuario *usuarios, int *user_count) {
    int opcao;
    do {
        printf("\033[0;32m");
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m");

        printf("===============================\n");
        printf("GERENCIAMENTO DE USU�RIOS\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Adicionar Usu�rio\n");
        printf("2. Listar Usu�rios\n");
        printf("3. Remover Usu�rios\n");
        printf("0. Voltar\n");
        printf("-------------------------------\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch(opcao) {

            case 1:
                adicionarUsuario(usuarios, user_count);
                break;

            case 2:
                printf("\n--- Lista de Usu�rios ---\n");
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
                printf("Op��o inv�lida. Tente novamente.\n");
                break;

        }

        puts("\n\n\tPressione qualquer tecla para continuar.");
        getche();
        fflush(stdin);
        system("cls");

    } while (opcao != 0);
}

// Fun��es auxiliares de Gerenciamento de Contas
// Carrega a lista de Usu�rios
void carregarUsuarios(Usuario *usuarios, int *user_count) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (fp == NULL) {
        printf("\nNenhum arquivo de usu�rios encontrado. Criando usu�rio administrador padr�o.\n");

        // Criar usu�rio padr�o "ADM" com senha "1234" e permiss�o de administrador
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

// Salva a lista de usu�rios
void salvarUsuarios(Usuario *usuarios, int user_count) {
    FILE *fp = fopen(USERS_FILE, "w");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de usu�rios.\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(fp, "%s %s %d\n", usuarios[i].nome, usuarios[i].senha, usuarios[i].is_admin);
    }

    fclose(fp);
}

// Menu de login/autentica��o
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

        printf("Nome de usu�rio: ");
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

    printf("Usu�rio ou senha incorretos.\n");
    getche();
    return 0;
}

// Adiciona usu�rio na lista
void adicionarUsuario(Usuario *usuarios, int *user_count){
    if (*user_count > USER_MAX) {
        printf("N�mero m�ximo de usu�rios atingido.\n");
        return;
    }
    printf("Nome do novo usu�rio: ");
    scanf("%s", usuarios[*user_count].nome);
    printf("Senha do novo usu�rio: ");
    scanf("%s", usuarios[*user_count].senha);
    printf("� administrador? (1 para Sim, 0 para N�o): ");
    scanf("%d", &usuarios[*user_count].is_admin);
    (*user_count)++;
    salvarUsuarios(usuarios, *user_count);
    printf("Usu�rio adicionado com sucesso.\n");
}

// Remove usu�rio da lista
void removerUsuario(Usuario *usuarios, int *user_count) {
    char nome[NOME_MAX];
    printf("Digite o nome do usu�rio que deseja remover: ");
    scanf("%s", nome);

    int index = -1;
    for (int i = 0; i < *user_count; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nUsu�rio n�o encontrado.\n");
        return;
    }

    for (int i = index; i < *user_count - 1; i++) {
        usuarios[i] = usuarios[i + 1];
    }
    (*user_count)--;

    salvarUsuarios(usuarios, *user_count);
    printf("\nUsu�rio removido com sucesso.\n");
}
