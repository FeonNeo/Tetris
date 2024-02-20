#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#define ROW 15+4+3+1
#define COL 10

bool run = true;
bool spawn = true;

typedef struct{
    bool control;
    bool solid;
    bool freeze;
    int identity;
    char draw[3];
}Box;

Box playfield[ROW][COL];
Box ori;

void setplay()
{
    for(int i=1; i<ROW-3; i++){
        for(int j=0; j<COL; j++){
            playfield[i][j].control = false;
            playfield[i][j].solid = false;
            playfield[i][j].freeze = false;
            playfield[i][j].identity = 0;
            strcpy(playfield[i][j].draw, "[ ]");
        }
    }

    for(int i=ROW-3; i<ROW; i++){
        for(int j=0; j<COL; j++){
            playfield[i][j].control = false;
            playfield[i][j].solid = false;
            playfield[i][j].freeze = true;
            playfield[i][j].identity = -1;
            strcpy(playfield[i][j].draw, "   ");
        }
    }

    for(int j=0; j<COL; j++){
        playfield[0][j].control = false;
        playfield[0][j].solid = false;
        playfield[0][j].freeze = true;
        playfield[0][j].identity = -1;
        strcpy(playfield[i][j].draw, "   ");
    }

    strcpy(playfield[0][2].draw, " T ");
    strcpy(playfield[0][3].draw, " E ");
    strcpy(playfield[0][4].draw, " T ");
    strcpy(playfield[0][5].draw, " R ");
    strcpy(playfield[0][6].draw, " I ");
    strcpy(playfield[0][7].draw, " S ");

    ori.control = false;
    ori.solid = false;
    ori.freeze = false;
    ori.identity = 0;
    strcpy(ori.draw, "[ ]");
}

void visualizenextpiece(int row, int choice)
{
    switch(row){
        case ROW-6: printw("\tNext Piece:"); break;
        case ROW-7: printw("\t********"); break;
        case ROW-8: switch(choice){
                        case 0: printw("\t*   #  *"); return;
                        case 1: printw("\t* #    *"); return;
                        case 2: printw("\t* ##   *"); return;
                        case 3: printw("\t*  ##  *"); return;
                        case 4: printw("\t*  #   *"); return;
                        case 5: printw("\t*      *");return;
                        case 6: printw("\t*  ##  *"); return;
                    }
        case ROW-9: switch(choice){
                        case 0: printw("\t* ###  *"); return;
                        case 1: printw("\t* ###  *"); return;
                        case 2: printw("\t*  ##  *"); return;
                        case 3: printw("\t* ##   *"); return;
                        case 4: printw("\t* ###  *"); return;
                        case 5: printw("\t* #### *"); return;
                        case 6: printw("\t*  ##  *"); return;
                    }
        case ROW-10:printw("\t********"); break;
    }
}

void visualize(int choice)
{
    for(int i=ROW-5; i>1; i--){
        for(int j=0; j<COL; j++){
            printw("%s", playfield[i][j].draw);
        }
        if(i>ROW-11){
            visualizenextpiece(i, choice);
        }
    printw("\n");
    }
}

void assvalue(char* drawing, int id, int r1, int c1, int r2, int c2, int r3, int c3, int r4, int c4)
{
    strcpy(playfield[r1][c1].draw, drawing);
    strcpy(playfield[r2][c2].draw, drawing);
    strcpy(playfield[r3][c3].draw, drawing);
    strcpy(playfield[r4][c4].draw, drawing);

    playfield[r1][c1].control = true;
    playfield[r2][c2].control = true;
    playfield[r3][c3].control = true;
    playfield[r4][c4].control = true;

    playfield[r1][c1].solid = true;
    playfield[r2][c2].solid = true;
    playfield[r3][c3].solid = true;
    playfield[r4][c4].solid = true;

    playfield[r1][c1].identity = id;
    playfield[r2][c2].identity = id;
    playfield[r3][c3].identity = id;
    playfield[r4][c4].identity = id;
}

void updatepiece(int id, int i, int j)
{

    for(int m=3; m<ROW; m++){
        for(int n=0; n<COL; n++){
            if(playfield[m][n].control){
                playfield[m][n] = ori;
            }
        }
    }

    switch(id){
        case 1: assvalue("[#]", 1, i,j, i,j+1, i,j+2, i+1,j+2);
                break;
        case 2: assvalue("[#]", 2, i,j, i,j+1, i,j+2, i+1,j);
                break;
        case 3: assvalue("[#]", 3, i+1,j, i+1,j+1, i,j+1, i,j+2);
                break;
        case 4: assvalue("[#]", 4, i,j, i,j+1, i+1,j+1, i+1,j+2);
                break;
        case 5: assvalue("[#]", 5, i,j, i,j+1, i,j+2, i+1,j+1);
                break;
        case 6: assvalue("[#]", 6, i,j, i+1,j, i+2,j, i+3,j);
                break;
        case 8: assvalue("[#]", 8, i+2,j, i+1,j, i+1,j+1, i,j+1);
                break;
        case 9: assvalue("[#]", 9, i+2,j+1, i+1,j+1, i+1,j, i,j);
                break;
        case 10: assvalue("[#]", 10, i,j-1, i,j, i,j+1, i,j+2);
                 break;
        case 11: assvalue("[#]", 11, i+2,j, i+1,j, i,j, i,j+1);
                 break;
        case 12: assvalue("[#]", 12, i,j, i+1,j, i+2,j, i+2,j+1);
                 break;
        case 13: assvalue("[#]", 13, i,j, i+1,j, i+1,j+1, i+1,j+2);
                 break;
        case 14: assvalue("[#]", 14, i+1,j, i+1,j+1, i+1,j+2, i,j+2);
                 break;
        case 15: assvalue("[#]", 15, i+2,j, i+2,j+1, i+1,j+1, i,j+1);
                 break;
        case 16: assvalue("[#]", 16, i,j, i,j+1, i+1,j+1, i+2,j+1);
                 break;
        case 17: assvalue("[#]", 17, i,j, i+1,j, i+2,j, i+1,j+1);
                 break;
        case 18: assvalue("[#]", 18, i+1,j, i+1,j+1, i+1,j+2, i,j+1);
                 break;
        case 19: assvalue("[#]", 19, i,j+1, i+1,j+1, i+2,j+1, i+1,j);
                 break;
        case 20: assvalue("[#]", 20, i,j-2, i,j-1, i,j, i,j+1);
                 break;
        default: return;
    }
}

void moveleft()
{
    bool adjust = true;

    for(int i=1; i<ROW-3; i++){
        for(int j=0; j<COL; j++){
            if(playfield[i][j].control){
                if(j==0){
                    adjust = false;
                }
                else if(playfield[i][j-1].solid && !playfield[i][j-1].control){
                    adjust = false;
                }
            }
        }
    }

    if(adjust){
        for(int i=1; i<ROW-3; i++){
            for(int j=1; j<COL; j++){
                if(playfield[i][j].control){
                    playfield[i][j-1] = playfield[i][j];
                    playfield[i][j] = ori;
                }
            }
        }
    }
}

void moveright()
{
    bool adjust = true;

    for(int i=1; i<ROW-3; i++){
        for(int j=0; j<COL; j++){
            if(playfield[i][j].control){
                if(j==COL-1){
                    adjust = false;
                }
                else if(playfield[i][j+1].solid && !playfield[i][j+1].control){
                    adjust = false;
                }
            }
        }
    }

    if(adjust){
        for(int i=1; i<ROW-3; i++){
            for(int j=COL-2; j>-1; j--){
                if(playfield[i][j].control){
                    playfield[i][j+1] = playfield[i][j];
                    playfield[i][j] = ori;
                }
            }
        }
    }
}

void rotateleft()
{
    for(int i=1; i<ROW-3; i++){
        for(int j=0; j<COL; j++){
            if(playfield[i][j].control){
                switch(playfield[i][j].identity){
                    case 1: updatepiece(15, i, j); return;
                    case 2: updatepiece(16, i, j); return;
                    case 3: updatepiece(9, i, j); return;
                    case 4: updatepiece(8, i, j+1); return;
                    case 5: updatepiece(19, i, j); return;
                    case 6: if(j < 2 || j+1 == COL){
                                return;
                            }else{
                                updatepiece(20, i, j);
                                return;
                            }
                    case 8: updatepiece(4, i, j-2); return;
                    case 9: updatepiece(3, i, j-1); return;
                    case 10: updatepiece(6, i, j+1); return;
                    case 11: if(j==0){
                                return;
                             }else{
                                updatepiece(1, i, j-1);
                                return;
                             }
                    case 12: if(j==0){
                                return;
                             }else{
                                updatepiece(2, i, j-1);
                                return;
                             }
                    case 13: updatepiece(11, i, j+1); return;
                    case 14: updatepiece(12, i, j-1); return;
                    case 15: updatepiece(13, i, j-1); return;
                    case 16: updatepiece(14, i, j); return;
                    case 17: updatepiece(5, i, j-1); return;
                    case 18: updatepiece(17, i, j); return;
                    case 19: updatepiece(18, i, j-1); return;
                    case 20: updatepiece(6, i, j+1); return;
                    default: return;
                }
            }
        }
    }
}

void rotateright()
{
    for(int i=1; i<ROW-3; i++){
        for(int j=0; j<COL; j++){
            if(playfield[i][j].control){
                switch(playfield[i][j].identity){
                    case 1: updatepiece(11, i, j+1); return;
                    case 2: updatepiece(12, i, j+1); return;
                    case 3: updatepiece(9, i, j); return;
                    case 4: updatepiece(8, i, j+1); return;
                    case 5: updatepiece(17, i, j+1); return;
                    case 6: if(j == 0 || j > COL-3){
                                return;
                            }else{
                                updatepiece(10, i, j);
                                return;
                            }
                    case 8: updatepiece(4, i, j-2); return;
                    case 9: updatepiece(3, i, j-1); return;
                    case 10: updatepiece(6, i, j+2); return;
                    case 11: updatepiece(13, i, j-1); return;
                    case 12: updatepiece(14, i, j-1); return;
                    case 13: updatepiece(15, i, j); return;
                    case 14: updatepiece(16, i, j-2); return;
                    case 15: if(j==COL-1){
                                return;
                             }else{
                                updatepiece(1, i, j-1);
                                return;
                             }
                    case 16: if(j==COL-1){
                                return;
                             }else{
                                updatepiece(2, i, j);
                                return;
                             }
                    case 17: updatepiece(18, i, j-1); return;
                    case 18: updatepiece(19, i, j-1); return;
                    case 19: updatepiece(5, i, j-1); return;
                    case 20: updatepiece(6, i, j+2); return;
                    default: return;
                }
            }
        }
    }
}

void input()
{
    char keypress;
    keypress = getch();

    switch(keypress){
        case 'a': moveleft(); break;
        case 'd': moveright(); break;
        case 'q': rotateleft(); break;
        case 'e': rotateright(); break;
        default: return;
    }
}

void track()
{
    bool freezer = false;
    int spawnctr = 0, clearctr = 0;

    for(int i=1; i<ROW-3; i++){
        for(int j=0; j<COL; j++){

            if(playfield[i][j].control)
                spawnctr++;

            if(playfield[i][j].solid && !playfield[i][j].freeze){
                for(int l=i+3; l>i-1; l--){
                    for(int k=0; k<COL; k++){
                        if(playfield[l][k].control && playfield[l-1][k].freeze){
                            for(int a=0; a<4; a++){
                                for(int b=0; b<COL; b++){
                                    if(playfield[l-a][b].control){
                                        playfield[l-a][b].control = false;
                                        playfield[l-a][b].freeze = true;
                                    }
                                }
                            }
                            l=0;
                            k=COL;
                        }
                    }
                }
            }

            if(playfield[i-1][j].identity != -1 && !playfield[i-1][j].freeze && !playfield[i][j].freeze){
                playfield[i-1][j] = playfield[i][j];
                playfield[i][j] = ori;
            }

            if(playfield[i-1][j].freeze && playfield[i][j].solid){
                playfield[i][j].control = false;
                playfield[i][j].freeze = true;
            }

            if(playfield[i][j].freeze)
                clearctr++;

        }//j loop closed

        if(clearctr==COL){
            for(int j=0; j<COL; j++){
                playfield[i][j] = ori;
                for(int k=i; k<ROW; k++){
                    playfield[k][j].freeze = false;
                }
            }
        }
        clearctr = 0;
    }//i loop closed

    if(spawnctr == 0)
        spawn = true;
}

void spawnpiece(int choice)
{
    switch(choice){
        case 0: assvalue("[#]", 1, 18,4, 18,5, 18,6, 19,6);
                break;
        case 1: assvalue("[#]", 2, 18,4, 18,5, 18,6, 19,4);
                break;
        case 2: assvalue("[#]", 3, 19,5, 19,6, 18,6, 18,7);
                break;
        case 3: assvalue("[#]", 4, 18,5, 18,6, 19,6, 19,7);
                break;
        case 4: assvalue("[#]", 5, 18,5, 18,6, 18,7, 19,6);
                break;
        case 5: assvalue("[#]", 6, 18,5, 19,5, 20,5, 21,5);
                break;
        case 6: assvalue("[#]", 7, 18,5, 18,6, 19,5, 19,6);
                break;
    }
}

void enddetect()
{
    for(int j=0; j<COL; j++){
        if(playfield[18][j].freeze){
            run = false;
        }
    }

    if(!run){
        printw("\nYOU DIED SUCKER");
        refresh();
        sleep(6);
    }

}

int main()
{
    initscr();
    timeout(333);
    setplay();

    srand(time(NULL));

    int spawnchoice = rand()%7;
    int nextspawnchoice;

    do{
        if(spawn){
            nextspawnchoice = rand()%7;
            spawnpiece(spawnchoice);
            spawnchoice = nextspawnchoice;
            spawn = false;
        }

        input();
        track();
        clear();
        refresh();
        visualize(nextspawnchoice);
        enddetect();

    }while(run);

    endwin();

    return 0;
}
