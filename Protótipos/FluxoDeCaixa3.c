#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define FLUXO_CAIXA_FILE "fluxo_caixa.txt"

// Estrutura para armazenar o fluxo de caixa
typedef struct {
    char data[20];
    float entrada;
    float saida;
} FluxoCaixa;

// Fun��o para registrar uma transa��o de entrada ou sa�da
void registrarFluxo(float entrada, float saida) {
    FILE *fp = fopen(FLUXO_CAIXA_FILE, "a");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de fluxo de caixa.\n");
        return;
    }

    // Obter a data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Salvando dados com delimitadores claros usando ';'
    fprintf(fp, "%02d-%02d-%d;%0.2f;%0.2f\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, entrada, saida);

    fclose(fp);
}

// Fun��o para exibir o extrato di�rio
void exibirExtratoDiario() {
    FILE *fp = fopen(FLUXO_CAIXA_FILE, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de fluxo de caixa.\n");
        return;
    }

    float totalEntrada = 0.0, totalSaida = 0.0;
    char linha[100];

    printf("\n--- Extrato Di�rio ---\n");
    while (fgets(linha, sizeof(linha), fp)) {
        FluxoCaixa fluxo;

        // Leitura dos dados com delimitador ';' e verifica��o de leitura
        if (sscanf(linha, "%[^;];%f;%f", fluxo.data, &fluxo.entrada, &fluxo.saida) == 3) {
            totalEntrada += fluxo.entrada;  // Soma o valor de entrada
            totalSaida += fluxo.saida;      // Soma o valor de sa�da

            printf("Data: %s | Entrada: R$ %.2f | Sa�da: R$ %.2f\n", fluxo.data, fluxo.entrada, fluxo.saida);
        }
    }
    fclose(fp);

    printf("-----------------------\n");
    printf("Total Entrada: R$ %.2f\n", totalEntrada);
    printf("Total Sa�da: R$ %.2f\n", totalSaida);
    printf("Saldo: R$ %.2f\n", totalEntrada - totalSaida);
}

// Fun��o de teste para registrar e exibir o extrato
int main() {
    setlocale(LC_ALL,"Portuguese");

    // Exibe o extrato di�rio
    exibirExtratoDiario();

    return 0;
}
