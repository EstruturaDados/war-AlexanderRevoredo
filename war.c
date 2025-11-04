#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Função para limpar o buffer de entrada
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para cadastrar territórios
void cadastrarTerritorios(struct Territorio* mapa, int num_territorios) {
    for (int i = 0; i < num_territorios; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // Leitura do nome
        printf("Nome do Territorio: ");
        if (fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin) == NULL) {
            printf("Erro na leitura do nome. Tente novamente.\n");
            i--;
            continue;
        }
        size_t len_nome = strcspn(mapa[i].nome, "\n");
        if (len_nome == sizeof(mapa[i].nome) - 1) {
            printf("Nome muito longo (máximo 29 caracteres). Tente novamente.\n");
            clearBuffer();
            i--;
            continue;
        }
        mapa[i].nome[len_nome] = '\0';

        // Leitura da cor
        printf("Cor do Territorio: ");
        if (fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin) == NULL) {
            printf("Erro na leitura da cor. Tente novamente.\n");
            i--;
            continue;
        }
        size_t len_cor = strcspn(mapa[i].cor, "\n");
        if (len_cor == sizeof(mapa[i].cor) - 1) {
            printf("Cor muito longa (máximo 9 caracteres). Tente novamente.\n");
            clearBuffer();
            i--;
            continue;
        }
        mapa[i].cor[len_cor] = '\0';

        // Leitura das tropas
        printf("Numero de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 1) {
            printf("Entrada inválida para tropas (deve ser pelo menos 1). Tente novamente.\n");
            clearBuffer();
            i--;
            continue;
        }
        clearBuffer();  // Limpa o \n restante após leitura bem-sucedida
    }
}

// Função para exibir o mapa atual
void exibirMapa(struct Territorio* mapa, int num_territorios) {
    printf("\n--- MAPA DO MUNDO ATUAL ---\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
        printf("--------------------------\n");
    }
}

// Função para simular ataque
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Simula rolagem de dados (1 a 6)
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("Rolagem: Atacante rolou %d, Defensor rolou %d\n", dado_atacante, dado_defensor);

    if (dado_atacante > dado_defensor) {
        // Atacante vence: Transfere cor e metade das tropas do atacante para o defensor
        printf("Atacante vence! Transferindo controle e tropas.\n");
        strcpy(defensor->cor, atacante->cor);
        int tropas_transferidas = atacante->tropas / 2;
        defensor->tropas = tropas_transferidas;
        atacante->tropas -= tropas_transferidas;
    } else {
        // Defensor vence ou empata: Atacante perde uma tropa
        printf("Defensor resiste! Atacante perde 1 tropa.\n");
        if (atacante->tropas > 1) {
            atacante->tropas--;
        } else {
            atacante->tropas = 0;  // Não pode ficar negativo
        }
    }
}

// Função para liberar memória
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

int main() {
    srand(time(NULL));  // Inicializa gerador de números aleatórios

    printf("==============================\n");
    printf("Bem-vindo ao jogo de guerra!\n");
    printf("==============================\n");

    // Solicita número de territórios
    int num_territorios;
    printf("Informe o numero total de territorios: ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios < 2) {
        printf("Numero invalido (deve ser pelo menos 2). Encerrando.\n");
        return 1;
    }
    clearBuffer();

    // Alocação dinâmica de memória para o vetor de territórios
    struct Territorio* mapa = (struct Territorio*) malloc(num_territorios * sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro na alocacao de memoria.\n");
        return 1;
    }

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, num_territorios);

    // Exibe mapa inicial
    exibirMapa(mapa, num_territorios);

    // Loop para simular ataques até o usuário decidir parar
    char opcao;
    do {
        // Seleciona atacante e defensor
        int idx_atacante, idx_defensor;
        printf("\nSelecione o territorio atacante (1 a %d): ", num_territorios);
        if (scanf("%d", &idx_atacante) != 1 || idx_atacante < 1 || idx_atacante > num_territorios) {
            printf("Indice invalido.\n");
            clearBuffer();
            continue;
        }
        idx_atacante--;  // Ajusta para índice 0-based

        printf("Selecione o territorio defensor (1 a %d): ", num_territorios);
        if (scanf("%d", &idx_defensor) != 1 || idx_defensor < 1 || idx_defensor > num_territorios || idx_defensor == idx_atacante + 1) {
            printf("Indice invalido ou mesmo territorio.\n");
            clearBuffer();
            continue;
        }
        idx_defensor--;  // Ajusta para índice 0-based
        clearBuffer();

        // Valida: Não pode atacar território da mesma cor
        if (strcmp(mapa[idx_atacante].cor, mapa[idx_defensor].cor) == 0) {
            printf("Nao pode atacar territorio da mesma cor!\n");
            continue;
        }

        // Valida: Atacante deve ter pelo menos 2 tropas para atacar (uma fica no território)
        if (mapa[idx_atacante].tropas < 2) {
            printf("Atacante nao tem tropas suficientes (precisa de pelo menos 2).\n");
            continue;
        }

        // Realiza o ataque
        atacar(&mapa[idx_atacante], &mapa[idx_defensor]);

        // Exibe mapa atualizado
        exibirMapa(mapa, num_territorios);

        // Pergunta se deseja continuar
        printf("Deseja realizar outro ataque? (s/n): ");
        opcao = getchar();
        clearBuffer();
    } while (opcao == 's' || opcao == 'S');

    // Libera memória
    liberarMemoria(mapa);

    return 0;
}