#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * club, diamond, heart, spade, no trump
 * C     D        H      S      N
 */

typedef struct _card {
    int point;
    char suit;
} card;

enum e {C, D, H, S};

int main(int argc, char *argv[]){
    int i, j;
    int foo;
    int ret;
    char buf[128];
    int deck[4][13];
    card played;

    FILE *board = fopen(argv[1], "r");
    FILE *steplog = fopen(argv[2], "r");

    int player;
    int game;
    int score[4];
    int stage;          // call: 0, play: 1
    card hand[13];
    card other[13];
    card contract;
    int declarer;
    int bridge[4];
    int round;

    // read board

    fscanf(board, " Player%d", &player);
    fscanf(board, " Game %d", &game);
    fscanf(board, " Score %d,%d,%d,%d", score, score + 1,
           score + 2, score + 3);
    fscanf(board, " Stage %s", buf);
    if (!strcmp(buf, "call")) {
        stage = 0;
        fscanf(board, " Hand %c-%d", &hand[0].suit, &hand[0].point);
        for (i = 1; i < 13; ++i) {
            fscanf(board, ",%c-%d", &hand[i].suit, &hand[i].point);
        }
    } else {
        stage = 1;
        ret = fscanf(board, " Hand %c-%d", &hand[0].suit, &hand[0].point);
        i = 1;
        while (ret) {
            ret = fscanf(board, ",%c-%d", &hand[i].suit, &hand[i].point);
            i++;
        }
        ret = fscanf(board, " Other %c-%d", &other[0].suit, &other[0].point);
        i = 1;
        while (ret) {
            ret = fscanf(board, ",%c-%d", &other[i].suit, &other[i].point);
            i++;
        }
        fscanf(board, " Contract %c-%d", &contract.suit, &contract.point);
        fscanf(board, " Declarer Player%d", &declarer);
        fscanf(board, " Bridge %d,%d,%d,%d", bridge, bridge + 1,
               bridge + 2, bridge + 3);
        fscanf(board, " Round %d", &round);
    }

    card call[4][25];

    // read steplog
    fscanf(steplog, "Stage call");
    i = 0;
    ret = 1;
    while (ret) {
        ret = fscanf(steplog, " Player%d call %s", foo, buf);
        if (strcmp(buf, "pass")) {
            call[i%4][i/4].suit = buf[0];
            call[i%4][i/4].point = atoi(buf[2]);
        } else {
            call[i%4][i/4].suit = 'P';
        }
        i++;
    }

    if (stage == 1) {
        fscanf(steplog, " Stage play");
        ret = fscanf(steplog, " Round %d", &foo);
        while (ret) {
            for (j = 0; j < 4; ++j) {
                fscanf(steplog, " Player%d play %c-%d", foo, &played.suit, &played.point);
                deck[played.suit][played.point] = 1;
            }
            ret = fscanf(steplog, " Round %d", &foo);
        }
    }

    fclose(board);
    fclose(steplog);

    // call

    // play

    return 0;
}
