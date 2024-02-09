#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

#define ROW 15+4+3
#define COL 10

bool debugbool = false;
bool debugbool2 = false;
bool run = true;
bool spawn = true;

typedef struct{
    bool control;
    bool solid;
    bool freeze;
    int identity;
    char draw;
}Box;

Box playfield[ROW][COL];
Box ori;

void setplay()
{
    for(int i=3; i<ROW; i++){
        for(int j=0; j<COL; j++){
            playfield[i][j].control = false;
            playfield[i][j].solid = false;
            playfield[i][j].freeze = false;
            playfield[i][j].identity = 0;
            playfield[i][j].draw = ' ';
        }
    }

    for(int i=0; i<3; i++){
        for(int j=0; j<COL; j++){
            playfield[i][j].control = false;
            playfield[i][j].solid = false;
            playfield[i][j].freeze = true;
            playfield[i][j].identity = -1;
            playfield[i][j].draw = '=';
        }
    }

    playfield[0][2].draw = 'T';
    playfield[0][3].draw = 'E';
    playfield[0][4].draw = 'T';
    playfield[0][5].draw = 'R';
    playfield[0][6].draw = 'I';
    playfield[0][7].draw = 'S';

    ori.control = false;
    ori.solid = false;
    ori.freeze = false;
    ori.identity = 0;
    ori.draw = ' ';
}

void visualize()
{
    for(int i=ROW-5; i>-1; i--){
        for(int j=0; j<COL; j++){
            printw("%c", playfield[i][j].draw);
        }
    printw("\n");
    }
}

void assvalue(char symbol, int id, int r1, int c1, int r2, int c2, int r3, int c3, int r4, int c4)
{
    playfield[r1][c1].draw = symbol; playfield[r2][c2].draw = symbol; playfield[r3][c3].draw = symbol; playfield[r4][c4].draw = symbol;
    playfield[r1][c1].control = true; playfield[r2][c2].control = true; playfield[r3][c3].control = true; playfield[r4][c4].control = true;
    playfield[r1][c1].solid = true; playfield[r2][c2].solid = true; playfield[r3][c3].solid = true; playfield[r4][c4].solid = true;
    playfield[r1][c1].identity = id; playfield[r2][c2].identity = id; playfield[r3][c3].identity = id; playfield[r4][c4].identity = id;
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
        case 1: assvalue('L', 1, i,j, i,j+1, i,j+2, i+1,j+2);
                break;
        case 2: assvalue('L', 2, i,j, i,j+1, i,j+2, i+1,j);
                break;
        case 3: assvalue('Z', 3, i+1,j, i+1,j+1, i,j+1, i,j+2);
                break;
        case 4: assvalue('Z', 4, i,j, i,j+1, i+1,j+1, i+1,j+2);
                break;
        case 5: assvalue('A', 5, i,j, i,j+1, i,j+2, i+1,j+1);
                break;
        case 6: assvalue('I', 6, i,j, i,j+1, i,j+2, i,j+3);
                break;
        case 8: assvalue('Z', 8, i+2,j, i+1,j, i+1,j+1, i,j+1);
                break;
        case 9: assvalue('Z', 9, i+2,j+1, i+1,j+1, i+1,j, i,j);
                break;
        case 10: assvalue('I', 10, i,j, i+1,j, i+2,j, i+3,j);
                 break;
        case 11: assvalue('L', 11, i+2,j, i+1,j, i,j, i,j+1);
                 break;
        case 12: assvalue('L', 12, i,j, i+1,j, i+2,j, i+2,j+1);
                 break;
        case 13: assvalue('L', 13, i,j, i+1,j, i+1,j+1, i+1,j+2);
                 break;
        case 14: assvalue('L', 14, i+1,j, i+1,j+1, i+1,j+2, i,j+2);
                 break;
        case 15: assvalue('L', 15, i+2,j, i+2,j+1, i+1,j+1, i,j+1);
                 break;
        case 16: assvalue('L', 16, i,j, i,j+1, i+1,j+1, i+2,j+1);
                 break;
        case 17: assvalue('A', 17, i,j, i+1,j, i+2,j, i+1,j+1);
                 break;
        case 18: assvalue('A', 18, i+1,j, i+1,j+1, i+1,j+2, i,j+1);
                 break;
        case 19: assvalue('A', 19, i,j+1, i+1,j+1, i+2,j+1, i+1,j);
                 break;
        case 20: assvalue('I', 20, i,j+2, i+1,j+2, i+2,j+2, i+3,j+2);
                 break;
        default: return;
    }
}

void moveleft()
{
    bool adjust = true;

    for(int i=3; i<ROW; i++){
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
        for(int i=3; i<ROW; i++){
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

    for(int i=3; i<ROW; i++){
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
        for(int i=3; i<ROW; i++){
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
    for(int i=3; i<ROW; i++){
        for(int j=0; j<COL; j++){
            if(playfield[i][j].control){
                switch(playfield[i][j].identity){
                    case 1: updatepiece(15, i, j); return;
                    case 2: updatepiece(16, i, j); return;
                    case 3: updatepiece(9, i, j); return;
                    case 4: updatepiece(8, i, j+1); return;
                    case 5: updatepiece(19, i, j); return;
                    case 6: updatepiece(20, i, j+2); return;
                    case 8: updatepiece(4, i, j-2); return;
                    case 9: updatepiece(3, i, j-1); return;
                    case 10: updatepiece(6, i, j-1); return;
                    case 11: updatepiece(1, i, j-1); return;
                    case 12: updatepiece(2, i, j-1); return;
                    case 13: updatepiece(11, i, j+1); return;
                    case 14: updatepiece(12, i, j-1); return;
                    case 15: updatepiece(13, i, j-1); return;
                    case 16: updatepiece(14, i, j); return;
                    case 17: updatepiece(5, i, j-1); return;
                    case 18: updatepiece(17, i, j); return;
                    case 19: updatepiece(18, i, j-1); return;
                    case 20: updatepiece(10, i, j-1); return;
                    default: return;
                }
            }
        }
    }
}

void rotateright()
{
    for(int i=3; i<ROW; i++){
        for(int j=0; j<COL; j++){
            if(playfield[i][j].control){
                switch(playfield[i][j].identity){
                    case 1: updatepiece(11, i, j+1); return;
                    case 2: updatepiece(12, i, j+1); return;
                    case 3: updatepiece(9, i, j); return;
                    case 4: updatepiece(8, i, j+1); return;
                    case 5: updatepiece(17, i, j+1); return;
                    case 6: updatepiece(10, i, j+1); return;
                    case 8: updatepiece(4, i, j-2); return;
                    case 9: updatepiece(3, i, j-1); return;
                    case 10: updatepiece(20, i, j+1); return;
                    case 11: updatepiece(13, i, j-1); return;
                    case 12: updatepiece(14, i, j-1); return;
                    case 13: updatepiece(15, i, j); return;
                    case 14: updatepiece(16, i, j-2); return;
                    case 15: updatepiece(1, i, j-1); return;
                    case 16: updatepiece(2, i, j); return;
                    case 17: updatepiece(18, i, j-1); return;
                    case 18: updatepiece(19, i, j-1); return;
                    case 19: updatepiece(5, i, j-1); return;
                    case 20: updatepiece(6, i, j-2); return;
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

    for(int i=3; i<ROW; i++){
        for(int j=0; j<COL; j++){

            if(playfield[i][j].control)
                spawnctr++;

            /*if(playfield[0][j].solid){
                playfield[0][j].control = false;
                playfield[0][j].freeze = true;
            }*/

            if(playfield[i][j].solid && !playfield[i][j].freeze){
                for(int k=0; k<COL; k++){
                    if(playfield[i-1][k].freeze || playfield[i][k].freeze){
                        for(int k=0; k<COL; k++){
                            if(playfield[i][k].control && playfield[i-1][k].freeze){
                                freezer = true;
                            }
                            else if(playfield[i+1][k].control && playfield[i][k].freeze){
                                freezer = true;
                            }
                        }
                        if(freezer){
                            for(int l=-3; l<4; l++){
                                for(int k=0; k<COL; k++){
                                    if(playfield[i+l][k].control){
                                        playfield[i+l][k].control = false;
                                        playfield[i+l][k].freeze = true;
                                        //debugbool2 = true;
                                    }
                                }
                            }
                            freezer = false;
                        }
                        /*else{
                            playfield[i-1][j] = playfield[i][j];
                            playfield[i][j] = ori;
                        }*/
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

        }

        if(clearctr==COL){
            for(int j=0; j<COL; j++){
                playfield[i][j] = ori;
                for(int k=i; k<ROW; k++){
                    playfield[k][j].freeze = false;
                }
            }
        }
        clearctr = 0;
    }

    /*for(int a=0; a<5; a++){
        if(playfield[0][a].solid){
            playfield[0][a].freeze = true;
            playfield[0][a].control = false;
            clearctr++;

            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    if(playfield[i][j].control){
                        playfield[i][j].freeze = true;
                        playfield[i][j].control = false;
                    }
                }
            }
        }
    }*/

    //visualize();

    /*if(clearctr==5){
        for(int b=0; b<5; b++){
            playfield[0][b] = ori;
            for(int a=0; a<12; a++){
                playfield[a][b].freeze = false;
            }
        }
    }*/

    if(spawnctr == 0)
        spawn = true;
}

void generator()
{
    int choice;

    srand(time(NULL));

    choice = rand()%7;

    switch(choice){
        case 0: assvalue('L', 1, 18,4, 18,5, 18,6, 19,6);
                break;
        case 1: assvalue('L', 2, 18,4, 18,5, 18,6, 19,4);
                break;
        case 2: assvalue('Z', 3, 19,5, 19,6, 18,6, 18,7);
                break;
        case 3: assvalue('Z', 4, 18,5, 18,6, 19,6, 19,7);
                break;
        case 4: assvalue('A', 5, 18,5, 18,6, 18,7, 19,6);
                break;
        case 5: assvalue('I', 6, 18,5, 19,5, 20,5, 21,5);
                break;
        case 6: assvalue('O', 7, 18,5, 18,6, 19,5, 19,6);
                break;
    }
}

void debug()
{
    for(int i=11; i>-1; i--){
        for(int j=0; j<5; j++){
            printw("[%d][%d]: S%d F%d C%d\t", i, j, playfield[i][j].solid, playfield[i][j].freeze, playfield[i][j].control);
        }
    printw("\n");
    }
    printw("Bool: %d %d\n", debugbool, debugbool2);
}

int main()
{
    initscr();
    timeout(333);
    setplay();

    do{
        if(spawn){
            generator();
            spawn = false;
        }

        input();
        //visualize();
        //debug();
        track();
        visualize();
        clear();
        visualize();
        //debug();
        //debugbool = false;
        //debugbool2 = false;

    }while(run);

    endwin();

    return 0;
}
