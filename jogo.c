#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LINHAS 200
#define COLUNAS 200

void pop_mapa();
void exibir_mapa();
void movimentar(char entrada);

char mapa[LINHAS][COLUNAS];
int jogadorx, jogadory, linhas = 0, colunas = 0;

int main(){

    
    char entrada;
    pop_mapa();
    printf("\n\n%d\n\n", colunas);
    while(1){
    exibir_mapa();
    scanf(" %c", &entrada);
    movimentar(entrada);
    system("clear");
    }



}

void pop_mapa(){

    FILE *arquivo;  

    char car;

    arquivo = fopen("mapa.txt", "r");

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
    
}

void exibir_mapa(){

    
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