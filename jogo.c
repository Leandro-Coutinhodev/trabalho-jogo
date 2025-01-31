#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define LINHAS 200
#define COLUNAS 200
#define INFINITO 99999

void pop_mapa();//Carrega os arquivos dos mapas e os adiciona na matriz mapa. Gera as posições iniciais dos personagens e do tesouro.
void exibir_mapa();//Exibe  o mapa atual com os personagens e o tesouro já adicionados.
void movimentar(char entrada);//Movimenta o jogador de acordo com o caractere de entrada.
void movimentar_inimigo();//De acordo com o nível de dificuldade, gera um movimento para perseguir o jogador(utiliza o algoritmo A-Estrela na dificuldade 3).
int heuristica(int x1, int y1, int x2, int y2);//utiliza a distancia Manhattan para calcular a menor distância de um nó iicial(inimigo) até o destino(jogador).Nós = índices da matriz.
void pontuar();//Calcula os pontos do jogador de acordo com a regra de pontuação presente na função.
void rankear();//Solicita a inicial do jogador e exibe o rank geral de todos os jogadores.
void ordRank();//Organiza o rank geral em ordem decrescente.

char mapa[LINHAS][COLUNAS];
int jogadorx, jogadory, linhas = 0, colunas = 0;
int inimigox, inimigoy, tesourox, tesouroy;
int dificuldade = 1;
int nivel;
int hp, l, passos;
char op;

typedef struct{

    int x, y;
    int custo_g, custo_h, custo_f;
    int pai_x, pai_y;
} Nodo;

struct Jogador{

    int pontos ;
    char inicial[3];
};

struct Jogador jogador[10] = {0};

int main(){

    srand(time(NULL));
    
    char entrada;
    l = 0;
    while(1){
    
    nivel = 1;
    hp = 3;
    
    printf("\nEscolha a dificuldade desejada: \n");
    printf("1 - Fácil.");
    printf("\n2 - Médio.");
    printf("\n3 - Difícil.\n");
    do{
    scanf("%d", &dificuldade);
    }while(dificuldade < 1 || dificuldade > 3); //Força uma entrada correta
    while(nivel <= 10){
    
        passos = 0;

        pop_mapa();
    while(1){
    exibir_mapa();
    do{
    scanf(" %c", &entrada);
    while(getchar() != '\n');
    entrada = tolower(entrada);
    }while(entrada != 'w' && entrada != 'a' && entrada != 's' && entrada != 'd');
    movimentar(entrada);
    if(jogadorx == tesourox && jogadory == tesouroy){
        mapa[jogadorx][jogadory] = 'V';
        exibir_mapa();
        printf("Você encontrou o tesouro. Parabéns!\n\n");
        pontuar();
        nivel++;
        break;
    }
    if(jogadorx == inimigox && jogadory == inimigoy){
        if(hp > 1){
            printf("\nVocê foi encontrado pelo inimigo!Tente novamente!");
            hp--;
            pop_mapa();
        }else{
            printf("\nVocê foi encontrado pelo inimigo!Fim de jogo!\n");
            hp--;
            break;
        }
        
    }
    movimentar_inimigo();

    if(jogadorx == inimigox && jogadory == inimigoy){
        if(hp > 1){
            printf("\nVocê foi encontrado pelo inimigo!Tente novamente!");
            hp--;
            pop_mapa();
        }else{
            printf("\nVocê foi encontrado pelo inimigo!Fim de jogo!\n");
            hp--;
            break;
        }
        
    }
    system("clear");
    if(hp == 0){
        break;
    }
    }
    if(hp == 0){
        break;
    }
    }
    rankear();
    l++;
    printf("\n\nDeseja continuar no jogo?(s/n) : \n");
    scanf(" %c", &op);
    if(tolower(op) == 'n')
        break;
    }

    printf("\n\nObrigado por jogar!");
}

void pop_mapa(){

    char diretorio[50];
    sprintf(diretorio, "mapas/mapa%d.txt", nivel);

    FILE *arquivo = fopen(diretorio, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo do mapa!\n");
        return;
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mapa[i][j] = '\0';
        }
    }

    // Lendo o mapa do arquivo
    linhas = 0;
    while (fgets(mapa[linhas], COLUNAS, arquivo)) {
        int len = 0;
        while (mapa[linhas][len] != '\0' && mapa[linhas][len] != '\n') {
            len++;
        }
        if (len > colunas) {
            colunas = len;
        }
        linhas++;
    }
    fclose(arquivo);

    
    int posicoes_livres[LINHAS * COLUNAS][2];
    int total_posicoes = 0;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (mapa[i][j] == '.') {
                posicoes_livres[total_posicoes][0] = i;
                posicoes_livres[total_posicoes][1] = j;
                total_posicoes++;
            }
        }
    }

    if (total_posicoes == 0) {
        printf("Erro: Nenhuma posição livre encontrada no mapa.\n");
        return;
    }

    


    int jogador_pos = rand() % total_posicoes;
    jogadorx = posicoes_livres[jogador_pos][0];
    jogadory = posicoes_livres[jogador_pos][1];
    mapa[jogadorx][jogadory] = 'P';

    int tesouro_pos;
    do {
        tesouro_pos = rand() % total_posicoes;
    } while (tesouro_pos == jogador_pos);

    tesourox = posicoes_livres[tesouro_pos][0];
    tesouroy = posicoes_livres[tesouro_pos][1];
    mapa[tesourox][tesouroy] = 'X';


    if (dificuldade > 1) {
        int inimigo_pos;
        do {
            inimigo_pos = rand() % total_posicoes;
        } while (inimigo_pos == jogador_pos || inimigo_pos == tesouro_pos);

        inimigox = posicoes_livres[inimigo_pos][0];
        inimigoy = posicoes_livres[inimigo_pos][1];
        mapa[inimigox][inimigoy] = 'I';
    }

    
}

void exibir_mapa(){

    
    printf("\t\t:::NÍVEL-%d:::\n", nivel);
    printf("\t\tVidas = %d\n\n\n", hp);
    for(int i = 0;i < linhas; i++){
        
        printf("%s", mapa[i]);
    }
    printf("\nDigite uma das entradas para movimentar-se no mapa: ");
    printf("\nw - Para cima.");
    printf("\na - Para a esquerda.");
    printf("\nd - Para a direita.");
    printf("\ns - Para baixo.\n");
}

void movimentar(char entrada) {
    passos++;

    
    if (entrada == 'w' && jogadorx > 0 && mapa[jogadorx - 1][jogadory] != '#' && jogadorx - 1 < LINHAS) {
        mapa[jogadorx][jogadory] = '.';
        jogadorx--;
        mapa[jogadorx][jogadory] = 'P';
    } 
    
    else if (entrada == 's' && jogadorx < linhas - 1 && mapa[jogadorx + 1][jogadory] != '#' && jogadorx + 1 < LINHAS) {
        mapa[jogadorx][jogadory] = '.';
        jogadorx++;
        mapa[jogadorx][jogadory] = 'P';
    } 
  
    else if (entrada == 'a' && jogadory > 0 && mapa[jogadorx][jogadory - 1] != '#' && jogadory - 1 < COLUNAS) {
        mapa[jogadorx][jogadory] = '.';
        jogadory--;
        mapa[jogadorx][jogadory] = 'P';
    } 
   
    else if (entrada == 'd' && jogadory < colunas - 1 && mapa[jogadorx][jogadory + 1] != '#' && jogadory + 1 < COLUNAS) {
        mapa[jogadorx][jogadory] = '.';
        jogadory++;
        mapa[jogadorx][jogadory] = 'P';
    }
}

void movimentar_inimigo() {

    if (dificuldade == 2) {
        mapa[inimigox][inimigoy] = '.';

        if (jogadorx < inimigox && mapa[inimigox - 1][inimigoy] != '#' && (inimigox - 1 != tesourox || inimigoy != tesouroy)) {
            inimigox--;
        } else if (jogadorx > inimigox && mapa[inimigox + 1][inimigoy] != '#' && (inimigox + 1 != tesourox || inimigoy != tesouroy)) {
            inimigox++;
        } else if (jogadory < inimigoy && mapa[inimigox][inimigoy - 1] != '#' && (inimigox != tesourox || inimigoy - 1 != tesouroy)) {
            inimigoy--;
        } else if (jogadory > inimigoy && mapa[inimigox][inimigoy + 1] != '#' && (inimigox != tesourox || inimigoy + 1 != tesouroy)) {
            inimigoy++;
        }

        mapa[inimigox][inimigoy] = 'I';

    } else if (dificuldade == 3) {
        int visitados[LINHAS][COLUNAS] = {0};
        Nodo aberto[LINHAS * COLUNAS];
        int inicio = 0, fim = 0;

        Nodo inicial = {inimigox, inimigoy, 0, heuristica(inimigox, inimigoy, jogadorx, jogadory), 0, -1, -1};
        inicial.custo_f = inicial.custo_g + inicial.custo_h;
        aberto[fim++] = inicial;

        Nodo melhor = inicial;

        while (inicio < fim) {
            int melhor_indice = inicio;
            for (int i = inicio + 1; i < fim; i++) {
                if (aberto[i].custo_f < aberto[melhor_indice].custo_f) {
                    melhor_indice = i;
                }
            }

            Nodo atual = aberto[melhor_indice];
            aberto[melhor_indice] = aberto[inicio];
            aberto[inicio++] = atual;

            visitados[atual.x][atual.y] = 1;

            if (atual.x == jogadorx && atual.y == jogadory) {
                melhor = atual;
                break;
            }

            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};

            for (int i = 0; i < 4; i++) {

                int nx = atual.x + dx[i];
                int ny = atual.y + dy[i];

                if (nx >= 0 && nx < LINHAS && ny >= 0 && ny < COLUNAS && 
                    mapa[nx][ny] != '#' && !visitados[nx][ny] && 
                    !(nx == tesourox && ny == tesouroy)) {

                    if (fim < LINHAS * COLUNAS) { // Evita estouro do array

                        Nodo vizinho = {nx, ny, atual.custo_g + 1, heuristica(nx, ny, jogadorx, jogadory), 0, atual.x, atual.y};
                        vizinho.custo_f = vizinho.custo_g + vizinho.custo_h;
                        aberto[fim++] = vizinho;
                    }
                }
            }
        }

        if (melhor.x == inimigox && melhor.y == inimigoy) {
            return;
        }

        Nodo passo = melhor;
        while (passo.pai_x != inimigox || passo.pai_y != inimigoy) {
            int encontrou = 0;
            for (int i = inicio - 1; i >= 0; i--) {
                if (aberto[i].x == passo.pai_x && aberto[i].y == passo.pai_y) {
                    passo = aberto[i];
                    encontrou = 1;
                    break;
                }
            }
            if (!encontrou) break;  // Evita loop infinito caso não encontre o pai
        }

        mapa[inimigox][inimigoy] = '.';
        inimigox = passo.x;
        inimigoy = passo.y;
        mapa[inimigox][inimigoy] = 'I';
    }
}

int heuristica(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);

}


void pontuar(){

    int base[10] = {65, 90, 115, 165, 215, 165, 215, 115, 215, 165};
    int difi[3] = {1, 2, 3};

    if(nivel >= 1 && nivel <= 10){

        if(passos <= base[nivel-1]){

            int pontos = (base[nivel-1] - passos) * difi[dificuldade-1];
            jogador[l].pontos += pontos + (10 * difi[dificuldade-1]);
        }else
            jogador[l].pontos += 10;

    }

}

void rankear() {
    
    printf("Digite as suas iniciais (3 caracteres): \n");
    scanf("%3s", jogador[l].inicial);
    getchar();

    printf("\n%s, sua pontuação foi: %d\n", jogador[l].inicial, jogador[l].pontos);

    ordRank();

    printf("\n\n\t\t::: RANKING GERAL :::\n");
    printf("\nPosição  |  Iniciais  |  Pontuação\n");
    printf("----------------------------------\n");

    for (int i = 0; i <= l; i++) {
        printf("#%d       |   %s       |  %d\n", i + 1, jogador[i].inicial, jogador[i].pontos);
    }
}

void ordRank() {
    for (int i = 0; i < l; i++) {
        for (int j = i + 1; j <= l; j++) {
            if (jogador[i].pontos < jogador[j].pontos) {
                struct Jogador temp = jogador[i];
                jogador[i] = jogador[j];
                jogador[j] = temp;
            }
        }
    }
}