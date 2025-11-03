#include <stdio.h>
#include <string.h>

#define MAX_TERRITORIOS 5

struct war {
    char nome[30];
    char cor[10];
    int tropas;
};

// funcao para limar o buffer de entrada
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct war territorio[MAX_TERRITORIOS];
    
    printf("==============================\n");
    printf("Bem-vindo ao jogo de guerra!\n");
    printf("==============================\n");

    printf("Vamos cadastrar os 5 territórios iniciais do nosso mundo.\n");

     for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        printf("Nome do Territorio: ");
        if (fgets(territorio[i].nome, sizeof(territorio[i].nome), stdin) == NULL) {
            printf("Erro na leitura do nome. Tente novamente.\n");
            i--;
            continue;
        }
        territorio[i].nome[strcspn(territorio[i].nome, "\n")] = '\0';

        printf("Cor do Territorio: ");
        if (fgets(territorio[i].cor, sizeof(territorio[i].cor), stdin) == NULL) {
            printf("Erro na leitura da cor. Tente novamente.\n");
            i--;
            continue;
        }
        territorio[i].cor[strcspn(territorio[i].cor, "\n")] = '\0';

        printf("Numero de Tropas: ");
        if (scanf("%d", &territorio[i].tropas) != 1) {
            printf("Entrada inválida para tropas. Tente novamente.\n");
            clearBuffer();
            i--;
            continue;
        }
           clearBuffer();
    }

    printf("\n--- MAPA DO MMUNDO ATUAL ---\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", territorio[i].nome);
        printf("Cor: %s\n", territorio[i].cor);
        printf("Tropas: %d\n", territorio[i].tropas);
        printf("--------------------------\n");
    }

    return 0;

}