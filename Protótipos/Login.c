#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#define USER_MAX 10
#define USERS_FILE "usuarios.txt"
#define NOME_MAX 16  // 15 caracteres + 1 para o terminador nulo
#define SENHA_MAX 16


typedef struct {
    char nome[NOME_MAX];
    char senha[SENHA_MAX];
    int is_admin;
} Usuario;


int login(Usuario *usuario);
void carregarUsuarios(Usuario *usuarios, int *user_count);
void salvarUsuarios(Usuario *usuarios, int user_count);
void gerenciarUsuarios(Usuario *usuarios, int *user_count);
void removerUsuario(Usuario *usuarios, int *user_count);
void adicionarUsuario(Usuario *usuarios, int *user_count);


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
        gerenciarUsuarios(usuarios, &user_count);
        return 0;
    }
    puts("Não é Admin");
    return 0;
}


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

int login(Usuario *usuario) {
    Usuario usuarios[USER_MAX];
    int user_count;
    carregarUsuarios(usuarios, &user_count);

    char nome[NOME_MAX], senha[SENHA_MAX];
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

void gerenciarUsuarios(Usuario *usuarios, int *user_count) {
    int opcao;
    do {
        printf("\033[0;32m"); //Set the text to the color green
        printf("\n--- HortiFruti GALC ---\n");
        printf("\033[0m"); //Resets the text to default color
        printf("\n--- Gerenciamento de Usuários ---\n");
        printf("1. Adicionar Usuário\n");
        printf("2. Listar Usuários\n");
        printf("3. Sair do Gerenciamento de Usuários\n");
        printf("4. Remover Usuários\n");
        printf("---------------------------------\n");
        printf("Escolha uma opção: ");
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
            printf("\n--- Lista de Usuários ---\n");
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
            printf("Opção inválida. Tente novamente.\n");
            puts("\n\n\tPressione qualquer tecla para continuar.");
            getche();
            system("cls");
            break;

        }
    } while (opcao != 3);
}

void adicionarUsuario(Usuario *usuarios, int *user_count){
    if (*user_count >= USER_MAX) {
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
