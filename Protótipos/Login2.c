#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <time.h>

#define FILE_NAME "estoque.txt"
#define USERS_FILE "usuarios.txt"
#define CASHFLOW_FILE "fluxo_caixa.txt"
#define CASH_FILE "vendas.txt"

#define MAX 100
#define USER_MAX 10
#define NOME_MAX 16
#define SENHA_MAX 16


// Structs

// Struct de Produto Estoque
typedef struct {
    int id;
    char nome[NOME_MAX];
    int quantidade;
    float preco;
} Produto;

// Struct de Venda
typedef struct {
    int id;
    char produto[NOME_MAX];
    float valor;
    int quantidade;
    float total;
} Venda;

// Struct de Transacao Fluxo de Caixa
typedef struct {
    char descricao[50];
    float valor;
    char tipo; // 'E' para entrada, 'S' para sa�da
    time_t data; // Timestamp da transa��o
} Transacao;

// Struct de Usuario Contas
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
void menuVendas();
void registrarVendas();
void visualizarVendas();

// Prot�tipos das Fun��es de Fluxo de Caixa
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
void menuPrincipal(Produto *produtos, int *count, int *user_count, Usuario *usuario);



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
    menuPrincipal(produtos, &count, &user_count, &usuario_atual);
    return 0;
}



// Fun��o do Menu Principal
void menuPrincipal(Produto *produtos, int *count, int *user_count, Usuario *usuario) {
    int opcao;

    do {
        printf("\033[0;32m"); //Set the text to the color green
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); //Resets the text to default color

        printf("===============================\n");
        printf("MENU PRINCIPAL\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Controle de Estoque\n");
        printf("2. Controle de Vendas\n");
        if (usuario->is_admin) {
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
                break;

            case 3:/*
                if (usuario->is_admin) {
                    menuCaixa();
                } else {
                    printf("\nOp��o inv�lida. Tente novamente.\n");
                    break;
                }*/
                break;

            case 4:
                if (usuario->is_admin) {
                    gerenciarUsuarios(usuario, user_count);
                } else {
                    printf("\nOp��o inv�lida. Tente novamente.\n");
                    puts("\n\n\tPressione qualquer tecla para continuar.");
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
        printf("\033[0;32m"); //Set the text to the color green
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); //Resets the text to default color

        printf("===============================\n");
        printf("ESTOQUE\n");
        printf("===============================\n\n");

        printf("-------------------------------\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Remover Produto\n");
        printf("0. Voltar\n");
        printf("----------------------------\n");
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
void salvarProdutosNoArquivo(Produto *produtos, int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("\nErro ao abrir o arquivo de estoque.\n");
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
        *count = 0;
        return;
    }

    *count = 0;
    while (fscanf(fp, "%d %s %d %f", &produtos[*count].id, produtos[*count].nome, &produtos[*count].quantidade, &produtos[*count].preco) == 4) {
        (*count)++;
    }
    fclose(fp);
}

void adicionarProduto(Produto *produtos, int *count) {
    printf("ID do produto: ");
    scanf("%d", &produtos[*count].id);
    printf("Nome do produto: ");
    scanf("%s", produtos[*count].nome);
    printf("Quantidade do produto: ");
    scanf("%d", &produtos[*count].quantidade);
    printf("Pre�o do produto: ");
    scanf("%f", &produtos[*count].preco);
    (*count)++;
    salvarProdutosNoArquivo(produtos, *count);
    printf("\nProduto adicionado com sucesso.\n");
}

void listarProdutos(Produto *produtos, int count) {
    printf("\n----- LISTA DE PRODUTOS -----\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Nome: %s | Quantidade: %d | Pre�o: %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }
    printf("------------------------------\n");
}

void buscarProduto(Produto *produtos, int count) {
    int id;
    printf("ID do produto a buscar: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (produtos[i].id == id) {
            printf("\nProduto encontrado: ID: %d | Nome: %s | Quantidade: %d | Pre�o: %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
            return;
        }
    }
    printf("\nProduto n�o encontrado.\n");
}

void removerProduto(Produto *produtos, int *count) {
    int id;
    printf("ID do produto a remover: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < *count; i++) {
        if (produtos[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nProduto n�o encontrado.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        produtos[i] = produtos[i + 1];
    }
    (*count)--;
    salvarProdutosNoArquivo(produtos, *count);
    printf("\nProduto removido com sucesso.\n");
}


// Fun��es de Gerenciamento de Contas
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

int login(Usuario *usuario) {
    Usuario usuarios[USER_MAX];
    int user_count;
    carregarUsuarios(usuarios, &user_count);

    char nome[NOME_MAX], senha[SENHA_MAX];

        printf("\033[0;32m"); //Set the text to the color green
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); //Resets the text to default color

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

// Fun��o Menu de Contas

void gerenciarUsuarios(Usuario *usuarios, int *user_count) {
    int opcao;
    do {
        printf("\033[0;32m"); // Set the text to the color green
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); // Resets the text to default color

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
