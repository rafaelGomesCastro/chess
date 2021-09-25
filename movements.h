#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

//#include <ncurses.h>
#include <ctype.h>

#include <termios.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")

enum Type {
  NONE = 0,
  PAWN,
  ROOK,
  KNIGHT,
  BISHOP,
  QUEEN,
  KING,
};

enum Color {
  WHITE = 0,
  BLACK,
};

enum State {
  BLOCK = 0,
  FREE,
  CAPTURE,
  PROMOTE,
};

#define EMPTY         0x2003

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1B[37m"
#define COLOR_RESET   "\x1b[0m"

#define UP            72
#define DOWN          80
#define LEFT          77
#define RIGHT         75
#define ENTER         10
#define ESC           27

struct piece {
    int type;
    int color;
    int line;
    int col;

    int initial_line;
    int initial_col;

    int state;

    wchar_t unicode;

    struct piece *next;
};
typedef struct piece Piece;

int check_house_status(Piece*, Piece*, int, int);
void get_pawn_move(Piece*, int**, Piece*);
