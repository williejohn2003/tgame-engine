#include <stdio.h>

#define h 10
#define w 50

void make_pipes(){//maybe use a game struct
}

void print_lvl(int lvl[h][w]){
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            printf("%d",lvl[i][j]);
        }
        printf("\n");
    }
}

int main(){
    int lvl[h][w]={{0},{0}};
    print_lvl(lvl);
    return 0;
}
