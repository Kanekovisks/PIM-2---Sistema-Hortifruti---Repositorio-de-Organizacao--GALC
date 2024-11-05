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


// Struct de Usuario Contas
typedef struct {
    char nome[NOME_MAX];
    char senha[SENHA_MAX];
    int is_admin; // 1 - ADM / 0 - B�sico
} Usuario;


// Declara��o das fun��es
int login(Usuario *usuario);
void carregarUsuarios(Usuario *usuarios, int *user_count);
void salvarUsuarios(Usuario *usuarios, int user_count);
void gerenciarUsuarios(Usuario *usuarios, int *user_count);
void removerUsuario(Usuario *usuarios, int *user_count);
void adicionarUsuario(Usuario *usuarios, int *user_count);

void menuPrincipal(int *user_count, Usuario *usuarios);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Usuario usuarios[USER_MAX];
    int user_count = 0;
    carregarUsuarios(usuarios, &user_count);
    Usuario usuario_atual;
    if (!login(&usuario_atual)) {
        printf("\nLogin falhou. Encerrando o programa.\n");
        return 1;
    }

    if (usuario_atual.is_admin) {
        menuPrincipal(&user_count, usuarios);
        return 0;
    }
    puts("N�o � Admin");
    return 0;
}

// Fun��o do Menu Principal
void menuPrincipal(int *user_count, Usuario *usuarios) {
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
        if (usuarios->is_admin) {
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
                break;

            case 2:
                break;

            case 3:
                break;

            case 4:
                if (usuarios->is_admin) {
                    gerenciarUsuarios(usuarios, user_count);
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
        printf("3. Sair do Gerenciamento de Usu�rios\n");
        printf("4. Remover Usu�rios\n");
        printf("---------------------------------\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch(opcao) {

        case 1:
            adicionarUsuario(usuarios, user_count);
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            fflush(stdin);
            system("cls");
            break;

        case 2:
            printf("\n--- Lista de Usu�rios ---\n");
            for (int i = 0; i < *user_count; i++) {
                printf("Nome: %s, Tipo: %s\n", usuarios[i].nome, usuarios[i].is_admin ? "Admin" : "Normal");
            }
            printf("-------------------------\n");
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            system("cls");
            break;

        case 3:
            printf("Saindo do Gerenciamento.\n");
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            system("cls");
            break;

        case 4:
            removerUsuario(usuarios, user_count);
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            system("cls");
            break;

        default:
            printf("Op��o inv�lida. Tente novamente.\n");
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            system("cls");
            break;

        }
    } while (opcao != 3);
}

void adicionarUsuario(Usuario *usuarios, int *user_count){
    if (*user_count >= USER_MAX) {
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
