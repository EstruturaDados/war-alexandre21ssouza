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


// protótipos das novas funções de missão
// -----------------------------------------
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao_eliminarCor(const char* missao, struct Territorio* mapa, int tamanho);


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


//função: atribuir missões
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    if (destino == NULL || missoes == NULL || totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}


//exibe a missão
void exibirMissao(const char* missao) {
    if (missao == NULL) return;
    printf("\n=== MISSÃO SORTEADA (mostrada 1 vez) ===\n%s\n", missao);
}


//verificação da missão eliminar, retorna 1 se a missão estiver cumprida
int verificarMissao_eliminarCor(const char* missao, struct Territorio* mapa, int tamanho) {
    if (missao == NULL || mapa == NULL || tamanho <= 0) return 0;

    // Formato esperado  
    const char* prefix = "Eliminar todas as tropas da cor ";
    size_t prefix_len = strlen(prefix);

     // considerar não cumprida
    if (strncmp(missao, prefix, prefix_len) != 0) {
       
        return 0;
    }

    // extrai a cor da missão 
    const char* corAlvo = missao + prefix_len;
    if (corAlvo[0] == '\0') return 0; // sem cor -> inválido

    // ainda existe tropa da cor alvo
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corAlvo) == 0 && mapa[i].tropas > 0) {
            return 0; 
        }
    }

    // nenhuma tropa encontrada da cor alvo -> missão cumprida
    return 1;
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


    //cria lista de cores únicas existentes no mapa
    char* coresUnicas[qtd];
    int coresCount = 0;
    for (int i = 0; i < qtd; i++) {
        int jaTem = 0;
        for (int j = 0; j < coresCount; j++) {
            if (strcmp(coresUnicas[j], mapa[i].cor) == 0) {
                jaTem = 1;
                break;
            }
        }
        if (!jaTem) {
            coresUnicas[coresCount++] = mapa[i].cor; // guarda ponteiro para a string existente
        }
    }

        // se não houver cores (caso estranho), adiciona fallback
    char corFallback[] = "Vermelha";
    if (coresCount == 0) {
        coresUnicas[0] = corFallback;
        coresCount = 1;
    }



    // escolhe aleatoriamente uma cor alvo da lista
    int idxCorSorteada = rand() % coresCount;
    char* corSorteada = coresUnicas[idxCorSorteada];

    // monta a string da missão e armazena dinamicamente (malloc) 
    char textoMissaoTemp[120];
    snprintf(textoMissaoTemp, sizeof(textoMissaoTemp), "Eliminar todas as tropas da cor %s", corSorteada);

    // aloca dinamicamente a missão do jogador (malloc)
    char* missaoJogador = (char*) malloc(120 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memoria para missao.\n");
        liberarMemoria(mapa);
        return 1;
    }

    // atribuir missão
    
    strcpy(missaoJogador, textoMissaoTemp);


    // Exibe a missão apenas uma vez (no início)
    exibirMissao(missaoJogador);


    // Loop de turnos: jogador realiza ataques até cumprir a missão ou escolher sair
    // ----------------------------
    int opcao = -1;
    int terminou = 0;
    while (!terminou) {
        printf("\n--- Menu (digite a opcao) ---\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar um ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            // limpar entrada inválida
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        if (opcao == 0) {
            printf("Encerrando o jogo. Obrigado por jogar.\n");
            break;
        } else if (opcao == 1) {
            exibirTerritorios(mapa, qtd);
            continue;
        } else if (opcao == 2) {
            // escolher atacante e defensor 
            int atk_input, def_input;
            printf("Escolha o numero do territorio atacante (1 a %d): ", qtd);
            scanf("%d", &atk_input);
            printf("Escolha o numero do territorio defensor (1 a %d): ", qtd);
            scanf("%d", &def_input);

            // converte para índice 
            int atk = atk_input - 1;
            int def = def_input - 1;

            // valida índices
            if (atk < 0 || atk >= qtd || def < 0 || def >= qtd) {
                printf("Escolha invalida de territorios. Tente novamente.\n");
                continue;
            }

            // valida que não atacante ataque território da mesma cor
            if (strcmp(mapa[atk].cor, mapa[def].cor) == 0) {
                printf("Erro: um territorio nao pode atacar outro da mesma cor.\n");
                continue;
            }

            // executa ataque 
            atacar(&mapa[atk], &mapa[def]);

            // Exibição pós-ataque 
            exibirTerritorios(mapa, qtd);

            // Após o ataque, verifica a missão silenciosamente
            if (verificarMissao_eliminarCor(missaoJogador, mapa, qtd)) {
                printf("\n>>> MISSÃO CUMPRIDA: %s\n", missaoJogador);
                printf(">>> PARABÉNS! Você venceu o jogo.\n");
                terminou = 1;
                break;
            } else {
                printf("Missão ainda não cumprida. Continue atacando.\n");
            }

        } else {
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
    } 



     // ---------- Limpeza de memória ----------
    free(missaoJogador); 
    liberarMemoria(mapa);
    return 0;
}

