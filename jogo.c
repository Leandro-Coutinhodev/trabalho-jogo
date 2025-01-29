#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define LINHAS 200
#define COLUNAS 200
#define INFINITO 99999

void pop_mapa();
void exibir_mapa();
void movimentar(char entrada);
void movimentar_inimigo();
int heuristica(int x1, int y1, int x2, int y2);

char mapa[LINHAS][COLUNAS];
int jogadorx, jogadory, linhas = 0, colunas = 0;
int inimigox, inimigoy, tesourox, tesouroy;
int dificuldade = 1;
int nivel = 1;
int hp;

typedef struct{

    int x, y;
    int custo_g, custo_h, custo_f;
    int pai_x, pai_y;
} Nodo;

int main(){

    
    char entrada;
    while(1){

    hp = 3;
    
    printf("\nEscolha a dificuldade desejada: \n");
    printf("1 - Fácil.");
    printf("\n2 - Médio.");
    printf("\n3 - Difícil.\n");
    scanf("%d", &dificuldade);
    pop_mapa();

    while(nivel < 10){
    while(1){
    exibir_mapa();
    scanf(" %c", &entrada);
    movimentar(entrada);
    if(jogadorx == tesourox && jogadory == tesouroy){
        printf("Você encontrou o tesouro. Parabéns!");
        nivel++;
        break;
    }
    movimentar_inimigo();

    if(jogadorx == inimigox && jogadory == inimigoy){
        if(hp > 0){
            printf("\nVocê foi encontrado pelo inimigo!Tente novamente!");
            hp--;
            pop_mapa();
            printf("\n\nVidas restantes = %d");
        }else{
            printf("\nVocê foi encontrado pelo inimigo!Fim de jogo!\n");
            break;
        }
        
    }
    system("clear");
    }
    }
    }



}

void pop_mapa(){

    char diretorio[50];
    sprintf(diretorio, "mapas/mapa%d.txt", nivel);

    for(int i = 0;i < linhas;i++){
        for(int j = 0;j < colunas;j++){
            mapa[i][j] = '\0';
        }
    }

    FILE *arquivo;  

    char car;

    arquivo = fopen(diretorio, "r");

    while (fgets(mapa[linhas], COLUNAS, arquivo) != NULL) {
        
        linhas++;
    
    }

    for(int i = 0;i < linhas;i++){
        for(int j = 0;j < COLUNAS; j++){

            if(fscanf(arquivo, " %c", &car) != EOF){
                printf("%c", mapa[i][j]);
                 mapa[i][j] = car;
            }

            if(mapa[i][j] == 'P'){
                    jogadorx = i;
                    jogadory = j;
                 }
           
        }
    }
     fclose(arquivo);

     int colmaior;
     for(int i = 0;i < linhas;i++){
        colmaior = 0;
        for(int j = 0;j < COLUNAS;j++){
        if(mapa[i][j] == '.' || mapa[i][j] == '#' || mapa[i][j] == 'P' || mapa[i][j] == 'V')
            colmaior++;
        }
        if(colunas < colmaior)
            colunas = colmaior;
        
     }

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

    srand(time(NULL));
    int jogador_pos = rand() % total_posicoes;
    int tesouro_pos = rand() % total_posicoes;

    while (tesouro_pos == jogador_pos) {
        tesouro_pos = rand() % total_posicoes;
    }

    if(dificuldade > 1){
        
        int inimigo_pos = rand() % total_posicoes;
        while (inimigo_pos == jogador_pos || inimigo_pos == tesouro_pos) {
        inimigo_pos = rand() % total_posicoes;
  
    }
        inimigox = posicoes_livres[inimigo_pos][0];
        inimigoy = posicoes_livres[inimigo_pos][1];
        mapa[inimigox][inimigoy] = 'I';
        
    }

    jogadorx = posicoes_livres[jogador_pos][0];
    jogadory = posicoes_livres[jogador_pos][1];
    mapa[jogadorx][jogadory] = 'P';

    tesourox = posicoes_livres[tesouro_pos][0];
    tesouroy = posicoes_livres[tesouro_pos][1];
    mapa[tesourox][tesouroy] = 'V';

    
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
    printf("\ns - Para baixo.");
}

void movimentar(char entrada){

    entrada = tolower(entrada);
    
    if(entrada == 'w' && mapa[jogadorx-1][jogadory] != '#' && jogadorx > 0){
        
        mapa[jogadorx][jogadory] = '.';
        jogadorx--;
        mapa[jogadorx][jogadory] = 'P';


    }else if(entrada == 's' && mapa[jogadorx+1][jogadory] != '#' && jogadorx < linhas){

        mapa[jogadorx][jogadory] = '.';
        jogadorx++;
        mapa[jogadorx][jogadory] = 'P';
    }else if(entrada == 'a' && mapa[jogadorx][jogadory-1] != '#' && jogadory > 0){

        mapa[jogadorx][jogadory] = '.';
        jogadory--;
        mapa[jogadorx][jogadory] = 'P';

    }else if(entrada == 'd' && mapa[jogadorx][jogadory+1] != '#' && jogadory < colunas){

        mapa[jogadorx][jogadory] = '.';
        jogadory++;
        mapa[jogadorx][jogadory] = 'P';
    }
}

void movimentar_inimigo() {

    if (dificuldade == 2) {
        
        mapa[inimigox][inimigoy] = '.';
        if (jogadorx < inimigox && mapa[inimigox - 1][inimigoy] != '#') {
            inimigox--;
        } else if (jogadorx > inimigox && mapa[inimigox + 1][inimigoy] != '#') {
            inimigox++;
        } else if (jogadory < inimigoy && mapa[inimigox][inimigoy - 1] != '#') {
            inimigoy--;
        } else if (jogadory > inimigoy && mapa[inimigox][inimigoy + 1] != '#') {
            inimigoy++;
        }
        mapa[inimigox][inimigoy] = 'I';
    }else if(dificuldade == 3){


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

      
            if (nx >= 0 && nx < linhas && ny >= 0 && ny < colunas && mapa[nx][ny] != '#' && !visitados[nx][ny]) {
                Nodo vizinho = {nx, ny, atual.custo_g + 1, heuristica(nx, ny, jogadorx, jogadory), 0, atual.x, atual.y};
                vizinho.custo_f = vizinho.custo_g + vizinho.custo_h;

                aberto[fim++] = vizinho;
            }
        }
    }


    Nodo passo = melhor;
    while (passo.pai_x != inimigox || passo.pai_y != inimigoy) {
        for (int i = inicio - 1; i >= 0; i--) {
            if (aberto[i].x == passo.pai_x && aberto[i].y == passo.pai_y) {
                passo = aberto[i];
                break;
            }
        }
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