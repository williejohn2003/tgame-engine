#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>


pthread_t t; // user entry thread

//max size of level 255 pixels total 2^8 256-1
#define h 10
#define w 50
#define pw 4

typedef struct Game{
    int8_t player_pos[1][2]; /** player position based on its x,y coordinate*/
    int8_t top_pipe_pos[2][2];
    int8_t bottom_pipe_pos[2][2];
    int8_t pipe_size;
    int8_t pipe_gap;
    int8_t state;
    bool input_state;
}Game;

void print_lvl(char **lvl){
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            printf("%c",(lvl)[i][j]);
        }
        printf("\n");
    }
}

void make_pipes(char **lvl){//maybe use a game struct
    int a=10;
    for (int i=a;i<pw;i++){
       lvl[4][i]='3';
    }
    
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            printf("%c",(*lvl)[i]);
        }
        printf("\n");
    }
}

char **init_lvl(){
    char **tmp=calloc(h,sizeof(char*));

    for(int i=0;i<h;i++){
        tmp[i]=calloc(w,sizeof(char));
    }

    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            tmp[i][j]=' ';
        }
    }

    return tmp;
}

Game *init_game(){
    Game *g=calloc(1,sizeof(Game));
    g->player_pos[0][0]=0;
    g->player_pos[0][1]=0;
    g->top_pipe_pos[0][0]=0;
    g->top_pipe_pos[1][1]=0;
    g->bottom_pipe_pos[0][0]=0;
    g->bottom_pipe_pos[1][1]=0;
    g->pipe_size=3;
    g->pipe_gap=5;
    g->state=0;
    
    return g;
}

//create a thread for keypresses
void* key_press(void *game){
    Game *g=(Game*)game;
    while(1){
        char n=fgetc(stdin);
        (*g).input_state=true;
    }
    //pthread_exit(NULL);
}

int main(){
    char **lvl=init_lvl();
    Game *game=init_game();
    pthread_create(&t,NULL,key_press, (void*)&(*game));

        print_lvl(lvl);
    while(1){
        //printf("no input\n"); 
        if(game->input_state!=0){
            printf("input state: %d\n",game->input_state);
            game->input_state=0;
        }
    }
        pthread_join(t,NULL); //ensure thread is done
    
    return 0;
}
