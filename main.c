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

typedef struct Movement{
  char key;
  bool key_pressed;
  enum{
    UP='w',
    DOWN='s',//115
    LEFT='a',
    RIGHT='d',
    QUIT='q'
  }direction;
}Movement;

typedef struct Player{
  uint8_t x;
  uint8_t y;
}Player;

typedef struct Game{
  Player player_pos; /** player position based on its x,y coordinate*/
  uint8_t top_pipe_pos[2][2];
  uint8_t bottom_pipe_pos[2][2];
  uint8_t pipe_size;
  uint8_t pipe_gap;
  uint8_t state;
  bool input_state;
  Movement *movement;
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
  g->player_pos.x=3;
  g->player_pos.y=5;
  g->top_pipe_pos[0][0]=0;
  g->top_pipe_pos[1][1]=0;
  g->bottom_pipe_pos[0][0]=0;
  g->bottom_pipe_pos[1][1]=0;
  g->pipe_size=3;
  g->pipe_gap=5;
  g->state=1;
  g->input_state=0;

  g->movement=calloc(1,sizeof(Movement));
  g->movement->key=0;
  g->movement->key_pressed=false;

  return g;
}

//flush stdin
void flush_stdin(){
  char t=fgetc(stdin);
  while(t!='\n' && t!=EOF){
    t=fgetc(stdin);
  }
}

//create a thread for keypresses
void* key_press(void *game){
  Game *g=(Game*)game;
  char *key;
  while(g->state!=0){
    scanf("%1s",key);
    g->movement->key=getchar();
    flush_stdin();
    printf("read: %c\n",g->movement->key);
    g->input_state=true;
    g->movement->direction=g->movement->key;
  }
  pthread_exit(NULL);
}

char key_handler(Game **game){
  int ret=0; (*game)->movement->direction=(*game)->movement->key;
  switch((*game)->movement->direction){
    case UP:    ret=(*game)->movement->direction;  printf("up key 'w'\n");
                if((*game)->player_pos.y<h && (*game)->player_pos.y>0){ //TODO: use lvl bounds
                  (*game)->player_pos.y-=1;
                }
                break;
    case DOWN:  ret=(*game)->movement->direction;  printf("down key 's'\n");
                //TODO: add checks here for bounds before attempting to move player
                //TODO: moving wrong axis
                if((*game)->player_pos.y<h && (*game)->player_pos.y>0){ //TODO: use lvl bounds
                  (*game)->player_pos.y+=1;
                }
                break;
    case RIGHT: ret=(*game)->movement->direction;  printf("right key 'd'\n");
                if((*game)->player_pos.x<w && (*game)->player_pos.x>0){ //TODO: use lvl bounds
                  (*game)->player_pos.x+=1;
                }
                break;
    case LEFT: ret=(*game)->movement->direction;  printf("right key 'a'\n");
               if((*game)->player_pos.x<w && (*game)->player_pos.x>0){ //TODO: use lvl bounds
                 (*game)->player_pos.x-=1;
               }
               break;
    case QUIT:  ret=(*game)->state=0;             printf("quit key 'q'\n"); break;
    default:    ret=(*game)->movement->direction;  printf("invalid key\n"); break;
  }
  return ret;
}

/**
 * Runner
 * run game loop
 */
void run(char *(**lvl), Game **game){
  print_lvl(*lvl);
  while((*game)->state!=0){
    if((*game)->input_state!=0){
      //[x] - TODO: update player position to [x,y] data
      //TODO: refresh the level or update the previous location
      //TODO: game exit is broken
      //TODO: x,y is backwards

      //lets place the player in the level
      //swapped x and y for now. because of the way 2D arrays work
      (*lvl)[(*game)->player_pos.y][(*game)->player_pos.x]='$';
      //TODO: this may not be handled correctly
      print_lvl(*lvl);
      //lets update the position of the player
      printf("input state: %d\n",(*game)->input_state);
      printf("key pressed: %d %d\n",(*game)->movement->key,(*game)->movement->direction);
      key_handler(*&(game));
      printf("pos: %d,%d",(*game)->player_pos.x,(*game)->player_pos.y);
      (*game)->movement->key_pressed=false;
      (*game)->input_state=false;
    }
  }
}

int main(){
  char **lvl=init_lvl();
  //init player within level
  Game *game=init_game();
  pthread_create(&t,NULL,key_press,(void*)(game));//(void*)&(*game));
  run(&lvl,&game); //engine loop
  pthread_join(t,NULL); //ensure thread is done

  return 0;
}
