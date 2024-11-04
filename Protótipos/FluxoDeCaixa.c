#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define CASHFLOW_FILE "fluxo_caixa.txt"

// Struct de Transacao
typedef struct {
    char descricao[50];
    float valor;
    char tipo; // 'E' para entrada, 'S' para sa�da
    time_t data; // Timestamp da transa��o
} Transacao;

// Prot�tipos das Fun��es de Fluxo de Caixa
void registrarTransacao();
void visualizarExtratoDiario();
void visualizarExtratoSemanal();
void visualizarExtratoMensal();
void menuCaixa();

// Fun��o para registrar uma nova transa��o
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

// Fun��o para visualizar extrato di�rio
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

// Fun��o para visualizar extrato semanal
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

// Fun��o para visualizar extrato mensal
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

// Menu de Fluxo de Caixa
void menuCaixa() {
    int opcao;

    do {
        printf("\033[0;32m"); // Set the text to the color green
        printf("------- HORTIFRUTI GALC -------\n");
        printf("\033[0m"); // Resets the text to default color

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
        system("cls");

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
                break;
            default:
                printf("\nOp��o inv�lida. Tente novamente.\n");
                break;
        }

        printf("\nPressione qualquer tecla para continuar...");
        getche();
        system("cls");

    } while (opcao != 0);
}
