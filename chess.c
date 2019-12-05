#include <stdio.h>
#include <stdlib.h>

#define PAWN   1
#define TOWER  2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN  5
#define KING   6

#define BLACK  1
#define WHITE  0

typedef struct piece {
    int type;
    int color;
    int line;
    int col;

    int in_table;
} Piece;

Piece *initialize(int table[][8]) {

    Piece *pieces = (Piece*) malloc (32 * sizeof(Piece));

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            table[i][j] = 0;
        }
    }

    // Creating Pawns
    for (int i = 0; i < 16; ++i) {
        pieces[i].type     = PAWN;
        pieces[i].color    = i/8;
        pieces[i].line     = (i/8)*5 + 1;
        pieces[i].col      = i % 8;
        pieces[i].in_table = 1;
    }

    // Creating Towers
    pieces[16].type     = TOWER;
    pieces[16].color    = WHITE;
    pieces[16].line     = 0;
    pieces[16].col      = 0;
    pieces[16].in_table = 1;

    pieces[17].type     = TOWER;
    pieces[17].color    = BLACK;
    pieces[17].line     = 7;
    pieces[17].col      = 0;
    pieces[17].in_table = 1;

    pieces[18].type     = TOWER;
    pieces[18].color    = WHITE;
    pieces[18].line     = 0;
    pieces[18].col      = 7;
    pieces[18].in_table = 1;

    pieces[19].type     = TOWER;
    pieces[19].color    = BLACK;
    pieces[19].line     = 7;
    pieces[19].col      = 7;
    pieces[19].in_table = 1;

    // Creating Knights
    pieces[20].type     = KNIGHT;
    pieces[20].color    = WHITE;
    pieces[20].line     = 0;
    pieces[20].col      = 1;
    pieces[20].in_table = 1;

    pieces[21].type     = KNIGHT;
    pieces[21].color    = BLACK;
    pieces[21].line     = 7;
    pieces[21].col      = 1;
    pieces[21].in_table = 1;

    pieces[22].type     = KNIGHT;
    pieces[22].color    = WHITE;
    pieces[22].line     = 0;
    pieces[22].col      = 6;
    pieces[22].in_table = 1;

    pieces[23].type     = KNIGHT;
    pieces[23].color    = BLACK;
    pieces[23].line     = 7;
    pieces[23].col      = 6;
    pieces[23].in_table = 1;

    // Creating Bishops
    pieces[24].type     = BISHOP;
    pieces[24].color    = WHITE;
    pieces[24].line     = 0;
    pieces[24].col      = 2;
    pieces[24].in_table = 1;

    pieces[25].type     = BISHOP;
    pieces[25].color    = BLACK;
    pieces[25].line     = 7;
    pieces[25].col      = 2;
    pieces[25].in_table = 1;

    pieces[26].type     = BISHOP;
    pieces[26].color    = WHITE;
    pieces[26].line     = 0;
    pieces[26].col      = 5;
    pieces[26].in_table = 1;

    pieces[27].type     = BISHOP;
    pieces[27].color    = BLACK;
    pieces[27].line     = 7;
    pieces[27].col      = 5;
    pieces[27].in_table = 1;

    // Creating Queens
    pieces[28].type     = QUEEN;
    pieces[28].color    = WHITE;
    pieces[28].line     = 0;
    pieces[28].col      = 3;
    pieces[28].in_table = 1;

    pieces[29].type     = QUEEN;
    pieces[29].color    = BLACK;
    pieces[29].line     = 7;
    pieces[29].col      = 3;
    pieces[29].in_table = 1;

    // Creating Kings
    pieces[30].type     = KING;
    pieces[30].color    = WHITE;
    pieces[30].line     = 0;
    pieces[30].col      = 4;
    pieces[30].in_table = 1;

    pieces[31].type     = KING;
    pieces[31].color    = BLACK;
    pieces[31].line     = 7;
    pieces[31].col      = 4;
    pieces[31].in_table = 1;

    return pieces;

}

void print_table(int table[][8], Piece *pieces) {

    for (int i = 0; i < 32; ++i) {
        if (pieces[i].in_table == 1) {
            table[pieces[i].line][pieces[i].col] = pieces[i].type;
        }
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }

}

int main() {

    int table[8][8];
    Piece *pieces = initialize(table);
    print_table(table, pieces);

    return 0;

}
