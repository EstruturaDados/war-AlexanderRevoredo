/*
 * PROJETO: Jogo WAR Estruturado (Desafio Final)
 * DESCRICAO: Implementa cadastro, ataque e sistema de missoes
 * com alocacao dinamica de memoria e ponteiros.
 * ARQUITETURA: Monolitica (tudo em um unico arquivo).
 */

// --- 1. Bibliotecas Necessarias ---
#include <stdio.h>   // Para printf, scanf
#include <string.h>  // Para strcpy (copiar strings), strcmp (comparar strings)
#include <stdlib.h>  // Para alocacao dinamica (malloc, calloc, free) e numeros aleatorios (rand, srand)
#include <time.h>    // Para time (usado em srand para garantir aleatoriedade)

// --- 2. Definicao da Struct ---
// REQUISITO: Struct Territorio com nome, cor, tropas
struct Territory {
    char name[30];
    char color[10];
    int troops;
};

// --- 3. Constantes e Banco de Missoes ---
#define TOTAL_MISSIONS 5 // Define o numero de missoes no banco

// REQUISITO: Vetor de strings (ponteiros de char) com as missoes
const char* missionsBank[TOTAL_MISSIONS] = {
    "Conquistar 3 territorios",
    "Eliminar todas as tropas da cor Vermelha",
    "Eliminar todas as tropas da cor Azul",
    "Ter 10 tropas em um unico territorio",
    "Conquistar o territorio 'Asia'"
    // Voce pode adicionar mais missoes aqui se quiser
};


// --- 4. Prototipos das Funcoes (Boa Pratica) ---
// Isso informa ao compilador que essas funcoes existem
// antes de ele as encontrar no codigo.
void registerTerritories(struct Territory* map, int size);
void displayMap(struct Territory* map, int size);
void assignMission(char* destination, const char* missions[], int totalMissions);
int checkMission(char* mission, struct Territory* map, int size, const char* playerColor);
void attack(struct Territory* attacker, struct Territory* defender);
void freeMemory(struct Territory* map, char* m1, char* m2);


// --- 5. Funcao Principal (main) ---
// Ponto de entrada do programa
int main() {
    
    // REQUISITO: Use srand(time(NULL)) para garantir aleatoriedade
    // Isso "embaralha" o gerador de numeros aleatorios
    srand(time(NULL));

    int numTerritories = 0;
    struct Territory* map = NULL; // PONTEIRO para o vetor de territorios
    char* missionPlayer1 = NULL;    // PONTEIRO para a string da missao 1
    char* missionPlayer2 = NULL;    // PONTEIRO para a string da missao 2
    int option;
    int gameActive = 1; // Controla o loop principal do menu

    // --- ALOCACAO DE MEMORIA ---
    
    // REQUISITO: Pedir ao usuario o numero total de territorios
    printf("--- CONFIGURACAO INICIAL DO JOGO WAR ---\n");
    printf("Quantos territorios tera o seu mapa? ");
    scanf("%d", &numTerritories);

    // REQUISITO: Alocar dinamicamente o vetor de territorios (usando malloc agora para variacao)
    // malloc(quantidade * tamanho_de_cada_item)
    // Diferente do calloc, nao inicializa com zero, mas funciona aqui.
    map = (struct Territory*) malloc(numTerritories * sizeof(struct Territory));

    // Verificacao de memoria (boa pratica)
    if (map == NULL) {
        printf("Erro fatal ao alocar memoria para o mapa!\n");
        return 1; // Encerra o programa com codigo de erro
    }

    // REQUISITO: Alocar dinamicamente espaco para a string da missao
    // (Vamos assumir 150 chars como tamanho maximo para a descricao da missao)
    missionPlayer1 = (char*) malloc(150 * sizeof(char));
    missionPlayer2 = (char*) malloc(150 * sizeof(char));

    if (missionPlayer1 == NULL || missionPlayer2 == NULL) {
        printf("Erro fatal ao alocar memoria para missoes!\n");
        free(map); // Libera o que ja foi alocado
        return 1;   // Encerra
    }

    // --- INICIO DO JOGO ---
    
    // REQUISITO: Sortear e atribuir a missao
    assignMission(missionPlayer1, missionsBank, TOTAL_MISSIONS);
    assignMission(missionPlayer2, missionsBank, TOTAL_MISSIONS);

    printf("\nJogo iniciado! (Assumindo 2 jogadores: 'Azul' e 'Vermelha')\n");
    // REQUISITO: Exibir missao uma vez no inicio
    printf("Missao do Jogador 1 (Azul): %s\n", missionPlayer1);
    printf("Missao do Jogador 2 (Vermelha): %s\n", missionPlayer2);

    // --- LOOP PRINCIPAL DO JOGO (MENU) ---
    // REQUISITO: Modularizacao (main controla o fluxo)
    do {
        printf("\n\n---------------------------------");
        printf("\n     MENU PRINCIPAL WAR");
        printf("\n---------------------------------");
        printf("\n1. Cadastrar/Recadastrar Territorios");
        printf("\n2. Exibir Mapa Completo");
        printf("\n3. Realizar Ataque");
        printf("\n4. Ver Minhas Missoes");
        printf("\n0. Sair do Jogo");
        printf("\n\nEscolha sua opcao: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                registerTerritories(map, numTerritories);
                break;

            case 2:
                displayMap(map, numTerritories);
                break;
            
            case 3:
                // --- Logica de Ataque ---
                printf("\n--- PREPARAR ATAQUE ---\n");
                displayMap(map, numTerritories); // Mostra o mapa para escolher
                
                int attackerIdx, defenderIdx;
                
                printf("Digite o ID do territorio ATACANTE: ");
                scanf("%d", &attackerIdx);
                printf("Digite o ID do territorio DEFENSOR: ");
                scanf("%d", &defenderIdx);

                // --- Validacoes ---
                // Verifica se os indices estao dentro dos limites do vetor
                if (attackerIdx < 0 || attackerIdx >= numTerritories || defenderIdx < 0 || defenderIdx >= numTerritories) {
                    printf("Erro: ID de territorio invalido.\n");
                
                } else if (attackerIdx == defenderIdx) {
                    printf("Erro: Um territorio nao pode atacar a si mesmo!\n");
                
                // REQUISITO: Validar para nao atacar propria cor
                // strcmp retorna 0 se forem iguais
                } else if (strcmp((map + attackerIdx)->color, (map + defenderIdx)->color) == 0) {
                    printf("Erro: Voce nao pode atacar um territorio da sua propria cor!\n");
                
                // Regra do War: precisa deixar pelo menos 1 tropa
                } else if ((map + attackerIdx)->troops <= 1) {
                    printf("Erro: Voce precisa ter mais de 1 tropa no territorio para atacar!\n");
                
                } else {
                    // Se tudo estiver OK, chama a funcao atacar
                    // REQUISITO: Passa os PONTEIROS exatos dos territorios
                    attack(map + attackerIdx, map + defenderIdx);
                }
                break;

            case 4:
                // Apenas exibe as missoes novamente
                printf("\n--- MISSOES ATUAIS ---\n");
                printf("Missao do Jogador 1 (Azul): %s\n", missionPlayer1);
                printf("Missao do Jogador 2 (Vermelha): %s\n", missionPlayer2);
                break;

            case 0:
                gameActive = 0; // Sinaliza para o 'while' parar
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

        // --- VERIFICACAO DE VITORIA (Apos cada acao) ---
        if (gameActive == 1) { // So verifica se o jogo nao tiver sido encerrado
            
            // REQUISITO: Verificar missao ao final de cada "turno" (acao)
            if (checkMission(missionPlayer1, map, numTerritories, "Azul")) {
                printf("\n\n!!! VITORIA DO JOGADOR 1 (AZUL) !!!\n");
                printf("Missao cumprida: %s\n", missionPlayer1);
                gameActive = 0; // Termina o loop
            
            } else if (checkMission(missionPlayer2, map, numTerritories, "Vermelha")) {
                printf("\n\n!!! VITORIA DO JOGADOR 2 (VERMELHA) !!!\n");
                printf("Missao cumprida: %s\n", missionPlayer2);
                gameActive = 0; // Termina o loop
            }
        }

    } while (gameActive == 1); // Continua enquanto 'gameActive' for verdadeiro


    // --- FIM DO PROGRAMA ---
    // REQUISITO: Liberar toda memoria alocada
    freeMemory(map, missionPlayer1, missionPlayer2);
    
    printf("Jogo encerrado. Memoria liberada.\n");
    return 0; // Indica que o programa terminou com sucesso
}


// --- 6. Implementacao das Funcoes ---

/*
 * Funcao: registerTerritories
 * Descricao: Preenche o vetor dinamico de territorios com dados do usuario
 * Parametros: map (ponteiro para o inicio do vetor), size (int)
 */
void registerTerritories(struct Territory* map, int size) {
    printf("\n--- CADASTRO DE TERRITORIOS ---\n");
    printf("Cores sugeridas para os jogadores: Azul, Vermelha\n");
    
    for (int i = 0; i < size; i++) {
        // Usamos o indice [i] para o usuario (ID do territorio)
        printf("\n--- Cadastrando Territorio ID [%d] ---\n", i); 

        printf("Digite o nome do territorio (ex: Asia): ");
        // REQUISITO: Acesso por ponteiros.
        // (map + i) -> E o endereco do i-esimo territorio
        // -> E o operador para acessar um campo de um ponteiro de struct
        // A sintaxe map[i].name tambem funcionaria e e mais limpa,
        // mas (map + i)->name demonstra o uso de ponteiros.
        scanf(" %29[^\n]", (map + i)->name);

        printf("Digite a cor do exercito: ");
        scanf(" %9[^\n]", (map + i)->color);

        printf("Digite a quantidade de tropas: ");
        // &(map + i)->troops e o "endereco de (map[i].troops)"
        scanf("%d", &(map + i)->troops);
    }
    printf("\nCadastro concluido!\n");
}


/*
 * Funcao: displayMap
 * Descricao: Mostra os dados de todos os territorios cadastrados
 * Parametros: map (ponteiro), size (int)
 */
void displayMap(struct Territory* map, int size) {
    printf("\n\n==================== MAPA ATUAL ====================\n");
    printf("(ID) | NOME \t\t | COR \t\t | TROPAS\n");
    printf("----------------------------------------------------\n");
    
    for (int i = 0; i < size; i++) {
        // Exibe o indice (ID) para o usuario poder escolher no ataque
        // %-15s significa "string alinhada a esquerda com 15 espacos"
        printf("[%d]  | %-15s | %-10s | %d\n",
            i,
            (map + i)->name,
            (map + i)->color,
            (map + i)->troops
        );
    }
    printf("====================================================\n");
}


/*
 * Funcao: assignMission
 * REQUISITO: Sorteia uma missao e copia para o destino
 * Parametros: destination (ponteiro para onde a string sera copiada)
 * missions (vetor de strings do banco)
 * totalMissions (tamanho do banco)
 */
void assignMission(char* destination, const char* missions[], int totalMissions) {
    // Sorteia um indice aleatorio de 0 ate (totalMissions - 1)
    int drawnIndex = rand() % totalMissions; 
    
    // REQUISITO: Copia a string sorteada para a variavel de destino do jogador
    strcpy(destination, missions[drawnIndex]);
}


/*
 * Funcao: checkMission
 * REQUISITO: Verifica se uma missao foi cumprida (logica simples)
 * Retorna: 1 se a missao foi cumprida, 0 caso contrario
 */
int checkMission(char* mission, struct Territory* map, int size, const char* playerColor) {
    // Lembrete: strcmp retorna 0 se as strings forem IGUAIS

    // Logica da Missao 1: "Conquistar 3 territorios"
    if (strcmp(mission, "Conquistar 3 territorios") == 0) {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (strcmp((map + i)->color, playerColor) == 0) {
                count++;
            }
        }
        if (count >= 3) return 1; // Cumpriu!
    }

    // Logica da Missao 2: "Eliminar todas as tropas da cor Vermelha"
    if (strcmp(mission, "Eliminar todas as tropas da cor Vermelha") == 0) {
        int troops = 0;
        for (int i = 0; i < size; i++) {
            if (strcmp((map + i)->color, "Vermelha") == 0) {
                troops += (map + i)->troops;
            }
        }
        if (troops == 0) return 1; // Cumpriu!
    }

    // Logica da Missao 3: "Eliminar todas as tropas da cor Azul"
    if (strcmp(mission, "Eliminar todas as tropas da cor Azul") == 0) {
        int troops = 0;
        for (int i = 0; i < size; i++) {
            if (strcmp((map + i)->color, "Azul") == 0) {
                troops += (map + i)->troops;
            }
        }
        if (troops == 0) return 1; // Cumpriu!
    }
    
    // Logica da Missao 4: "Ter 10 tropas em um unico territorio"
    if (strcmp(mission, "Ter 10 tropas em um unico territorio") == 0) {
        for (int i = 0; i < size; i++) {
            // Verifica se a cor bate E se as tropas sao >= 10
            if (strcmp((map + i)->color, playerColor) == 0 && (map + i)->troops >= 10) {
                return 1; // Cumpriu!
            }
        }
    }

    // Logica da Missao 5: "Conquistar o territorio 'Asia'"
    if (strcmp(mission, "Conquistar o territorio 'Asia'") == 0) {
        for (int i = 0; i < size; i++) {
            // Verifica se o nome e "Asia" E se a cor e do jogador
            if (strcmp((map + i)->name, "Asia") == 0 && strcmp((map + i)->color, playerColor) == 0) {
                return 1; // Cumpriu!
            }
        }
    }

    // Se nenhuma missao foi cumprida, retorna 0
    return 0; 
}


/*
 * Funcao: attack
 * REQUISITO: Simula um ataque usando ponteiros para os territorios
 * Parametros: attacker (ponteiro para o struct do atacante)
 * defender (ponteiro para o struct do defensor)
 */
void attack(struct Territory* attacker, struct Territory* defender) {
    
    // REQUISITO: Simula rolagem de dados (1 a 6)
    int attackDice = (rand() % 6) + 1;
    int defenseDice = (rand() % 6) + 1;

    printf("\n--- BATALHA --- \n");
    printf("Atacante (%s) rolou: %d\n", attacker->name, attackDice);
    printf("Defensor (%s) rolou: %d\n", defender->name, defenseDice);
    printf("-----------------\n");

    // REQUISITO: Atualizacao de campos
    // (A rolagem de ataque deve ser ESTRITAMENTE maior que a defesa)
    if (attackDice > defenseDice) {
        printf("VITORIA DO ATACANTE! O territorio %s foi conquistado!\n", defender->name);
        
        // 1. REQUISITO: Transfere a cor
        strcpy(defender->color, attacker->color);
        
        // 2. REQUISITO: Transfere metade das tropas
        // (O atacante move metade das suas tropas para o novo territorio)
        int movedTroops = attacker->troops / 2;
        if (movedTroops == 0) movedTroops = 1; // Garante mover pelo menos 1

        defender->troops = movedTroops; // Territorio conquistado recebe as tropas
        attacker->troops = attacker->troops - movedTroops; // Atacante perde as tropas

        printf("%s agora pertence a cor %s e tem %d tropas.\n", defender->name, defender->color, defender->troops);
        printf("%s agora tem %d tropas.\n", attacker->name, attacker->troops);

    } else {
        printf("VITORIA DO DEFENSOR! O territorio %s se defendeu.\n", defender->name);
        
        // REQUISITO: Se perder, o atacante perde uma tropa
        // (So perde se tiver mais de 1, para nao zerar)
        if (attacker->troops > 1) {
            attacker->troops--;
            printf("O atacante %s perdeu 1 tropa e agora tem %d.\n", attacker->name, attacker->troops);
        } else {
            printf("O atacante %s nao perdeu tropas (minimo de 1).\n", attacker->name);
        }
    }
}


/*
 * Funcao: freeMemory
 * REQUISITO: Libera toda a memoria alocada dinamicamente
 * Parametros: ponteiros para tudo que foi alocado
 */
void freeMemory(struct Territory* map, char* m1, char* m2) {
    printf("\n--- Encerrando e Liberando Memoria ---\n");
    
    // Libera cada ponteiro que foi alocado com malloc/calloc
    free(map);        
    free(m1);          
    free(m2);

    // Boa pratica: definir ponteiros como NULL apos liberar
    // para evitar "dangling pointers" (ponteiros pendurados)
    map = NULL;
    m1 = NULL;
    m2 = NULL;  
    
}