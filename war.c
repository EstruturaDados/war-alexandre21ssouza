// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//


// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Protótipos das Funções ---


//     FUNÇÃO: CADASTRAR
void cadastrarTerritorios(struct Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n=== Cadastro do Territorio %d ===\n", i + 1);

        printf("Nome do territorio: ");
        scanf(" %29[^\n]", mapa[i].nome);   // aceita espaços

        printf("Cor do exercito: ");
        scanf(" %9[^\n]", mapa[i].cor);     // aceita espaços

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);

        if (mapa[i].tropas < 0)
            mapa[i].tropas = 0;
    }
}

//     FUNÇÃO: EXIBIR

void exibirTerritorios(struct Territorio* mapa, int qtd) {
    printf("\n\n=== TERRITORIOS ATUAIS ===\n");

    for (int i = 0; i < qtd; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}
    

//Função: atacar
   
void atacar(struct Territorio* atacante, struct Territorio* defensor) {

    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("\nRolagem de dados -> Atacante: %d | Defensor: %d\n",
           dadoAtq, dadoDef);

    if (dadoAtq > dadoDef) {
        printf("ATACANTE VENCEU!\n");

        strcpy(defensor->cor, atacante->cor);

        int transferencia = atacante->tropas / 2;
        defensor->tropas = transferencia;
        atacante->tropas -= transferencia;

        if (atacante->tropas < 0) atacante->tropas = 0;

        printf("Defensor agora pertence a '%s' com %d tropas.\n",
               defensor->cor, defensor->tropas);
    }
    else {
        printf("ATACANTE PERDEU OU EMPATOU. Perde 1 tropa.\n");
        atacante->tropas--;

        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Função: liberarMemoria

void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {

    // inicializa o gerador de números aleatórios
   srand(time(NULL)); 

    int qtd;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &qtd);

    struct Territorio* mapa = (struct Territorio*) calloc(qtd, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }
   
    // Cadastro e exibição inicial
    cadastrarTerritorios(mapa, qtd);
    exibirTerritorios(mapa, qtd);

    // Escolha de ataque
    int atk, def;
    printf("\nEscolha o numero do territorio atacante: ");
    scanf("%d", &atk);

    printf("Escolha o numero do territorio defensor: ");
    scanf("%d", &def);

    // Validações recomendadas
    if (atk < 0 || atk >= qtd || def < 0 || def >= qtd) {
        printf("\nErro: escolha fora do intervalo de territorios.\n");
    }
    else if (strcmp(mapa[atk].cor, mapa[def].cor) == 0) {
        printf("\nUm territorio nao pode atacar outro da mesma cor.\n");
    }
    else {
        atacar(&mapa[atk], &mapa[def]);
    }

    // Exibição pós-ataque
    exibirTerritorios(mapa, qtd);

    liberarMemoria(mapa);
    return 0;
}

