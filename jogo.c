#include <stdio.h>
#include <stdlib.h>

#define LINHAS 200
#define COLUNAS 200

int main(){

    int linhas = 0;
    FILE *arquivo;
    char mapa[LINHAS][COLUNAS];

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
           
        }
    }
     fclose(arquivo);
    for(int i = 0;i < linhas; i++){
        
        printf("%s", mapa[i]);
    }
}