#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

//#include <ncurses.h>
#include <ctype.h>

#include <termios.h>
#include <unistd.h>

#define PAWN          1
#define ROOK          2
#define KNIGHT        3
#define BISHOP        4
#define QUEEN         5
#define KING          6

#define BLACK         1
#define WHITE         0

#define BLOCK         0
#define FREE          1
#define CAPTURE       2

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

    int in_table;

    wchar_t unicode;

    struct piece *next;
};

typedef struct piece Piece;

int **calculate_possible_moves(Piece*, wchar_t[][8], int[2], Piece*);

int getch() {
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int **reset_possible_moves() {

  int **possible_moves = (int**) malloc (21 * sizeof(int*));
  for (int i = 0; i < 22; ++i) {
      possible_moves[i] = (int*) malloc (2 * sizeof(int));
  }
  possible_moves[0][0] = -1;

  return possible_moves;
}

Piece *initialize(wchar_t table[][8]) {

    Piece **pieces = (Piece**) malloc (32 * sizeof(Piece*));
    for (int i = 0; i < 32; ++i) {
        pieces[i] = (Piece*) malloc (sizeof(Piece));
    }
    //Piece *fp      = pieces[0];

    // Creating Table
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            table[i][j] = EMPTY;
        }
    }

    // Creating Pawns
    for (int i = 0; i < 16; ++i) {
        Piece *fp = pieces[i];

        fp->type         = PAWN;
        fp->color        = i/8;
        fp->line         = (i/8)*5 + 1;
        fp->col          = i % 8;
        fp->initial_line = fp->line;
        fp->initial_col  = fp->col;
        fp->in_table     = 1; // >>> here
        fp->unicode      = fp->color == BLACK ? 0x265F : 0x2659;
        fp->next         = NULL;

        if (i > 0) {
            Piece *ant = pieces[i-1];
            ant->next  = fp;
        }
    }

    // Creating ROOKs
    pieces[16]->type         = ROOK;
    pieces[16]->color        = WHITE;
    pieces[16]->line         = 0;
    pieces[16]->col          = 0;
    pieces[16]->initial_line = pieces[16]->line;
    pieces[16]->initial_col  = pieces[16]->col;
    pieces[16]->in_table     = 1;
    pieces[16]->unicode      = 0x2656;
    pieces[15]->next         = pieces[16];

    pieces[17]->type         = ROOK;
    pieces[17]->color        = BLACK;
    pieces[17]->line         = 7;
    pieces[17]->col          = 0;
    pieces[17]->initial_line = pieces[17]->line;
    pieces[17]->initial_col  = pieces[17]->col;
    pieces[17]->in_table     = 1;
    pieces[17]->unicode      = 0x265C;
    pieces[16]->next         = pieces[17];

    pieces[18]->type         = ROOK;
    pieces[18]->color        = WHITE;
    pieces[18]->line         = 0;
    pieces[18]->col          = 7;
    pieces[18]->initial_line = pieces[18]->line;
    pieces[18]->initial_col  = pieces[18]->col;
    pieces[18]->in_table     = 1;
    pieces[18]->unicode      = 0x2656;
    pieces[17]->next         = pieces[18];

    pieces[19]->type         = ROOK;
    pieces[19]->color        = BLACK;
    pieces[19]->line         = 7;
    pieces[19]->col          = 7;
    pieces[19]->initial_line = pieces[19]->line;
    pieces[19]->initial_col  = pieces[19]->col;
    pieces[19]->in_table     = 1;
    pieces[19]->unicode      = 0x265C;
    pieces[18]->next         = pieces[19];

    // Creating Knights
    pieces[20]->type         = KNIGHT;
    pieces[20]->color        = WHITE;
    pieces[20]->line         = 0;
    pieces[20]->col          = 1;
    pieces[20]->initial_line = pieces[20]->line;
    pieces[20]->initial_col  = pieces[20]->col;
    pieces[20]->in_table     = 1;
    pieces[20]->unicode      = 0x2658;
    pieces[19]->next         = pieces[20];

    pieces[21]->type         = KNIGHT;
    pieces[21]->color        = BLACK;
    pieces[21]->line         = 7;
    pieces[21]->col          = 1;
    pieces[21]->initial_line = pieces[21]->line;
    pieces[21]->initial_col  = pieces[21]->col;
    pieces[21]->in_table     = 1;
    pieces[21]->unicode      = 0x265E;
    pieces[20]->next         = pieces[21];

    pieces[22]->type         = KNIGHT;
    pieces[22]->color        = WHITE;
    pieces[22]->line         = 0;
    pieces[22]->col          = 6;
    pieces[22]->initial_line = pieces[22]->line;
    pieces[22]->initial_col  = pieces[22]->col;
    pieces[22]->in_table     = 1;
    pieces[22]->unicode      = 0x2658;
    pieces[21]->next         = pieces[22];

    pieces[23]->type         = KNIGHT;
    pieces[23]->color        = BLACK;
    pieces[23]->line         = 7;
    pieces[23]->col          = 6;
    pieces[23]->initial_line = pieces[23]->line;
    pieces[23]->initial_col  = pieces[23]->col;
    pieces[23]->in_table     = 1;
    pieces[23]->unicode      = 0x265E;
    pieces[22]->next         = pieces[23];

    // Creating Bishops
    pieces[24]->type         = BISHOP;
    pieces[24]->color        = WHITE;
    pieces[24]->line         = 0;
    pieces[24]->col          = 2;
    pieces[24]->initial_line = pieces[24]->line;
    pieces[24]->initial_col  = pieces[24]->col;
    pieces[24]->in_table     = 1;
    pieces[24]->unicode      = 0x2657;
    pieces[23]->next         = pieces[24];

    pieces[25]->type         = BISHOP;
    pieces[25]->color        = BLACK;
    pieces[25]->line         = 7;
    pieces[25]->col          = 2;
    pieces[25]->initial_line = pieces[25]->line;
    pieces[25]->initial_col  = pieces[25]->col;
    pieces[25]->in_table     = 1;
    pieces[25]->unicode      = 0x265D;
    pieces[24]->next         = pieces[25];

    pieces[26]->type         = BISHOP;
    pieces[26]->color        = WHITE;
    pieces[26]->line         = 0;
    pieces[26]->col          = 5;
    pieces[26]->initial_line = pieces[26]->line;
    pieces[26]->initial_col  = pieces[26]->col;
    pieces[26]->in_table     = 1;
    pieces[26]->unicode      = 0x2657;
    pieces[25]->next         = pieces[26];

    pieces[27]->type         = BISHOP;
    pieces[27]->color        = BLACK;
    pieces[27]->line         = 7;
    pieces[27]->col          = 5;
    pieces[27]->initial_line = pieces[27]->line;
    pieces[27]->initial_col  = pieces[27]->col;
    pieces[27]->in_table     = 1;
    pieces[27]->unicode      = 0x265D;
    pieces[26]->next         = pieces[27];

    // Creating Queens
    pieces[28]->type         = QUEEN;
    pieces[28]->color        = WHITE;
    pieces[28]->line         = 0;
    pieces[28]->col          = 3;
    pieces[28]->initial_line = pieces[28]->line;
    pieces[28]->initial_col  = pieces[28]->col;
    pieces[28]->in_table     = 1;
    pieces[28]->unicode      = 0x2655;
    pieces[27]->next         = pieces[28];

    pieces[29]->type         = QUEEN;
    pieces[29]->color        = BLACK;
    pieces[29]->line         = 7;
    pieces[29]->col          = 3;
    pieces[29]->initial_line = pieces[29]->line;
    pieces[29]->initial_col  = pieces[29]->col;
    pieces[29]->in_table     = 1;
    pieces[29]->unicode      = 0x265B;
    pieces[28]->next         = pieces[29];

    // Creating Kings
    pieces[30]->type         = KING;
    pieces[30]->color        = WHITE;
    pieces[30]->line         = 0;
    pieces[30]->col          = 4;
    pieces[30]->initial_line = pieces[30]->line;
    pieces[30]->initial_col  = pieces[30]->col;
    pieces[30]->in_table     = 1;
    pieces[30]->unicode      = 0x2654;
    pieces[29]->next         = pieces[30];

    pieces[31]->type         = KING;
    pieces[31]->color        = BLACK;
    pieces[31]->line         = 7;
    pieces[31]->col          = 4;
    pieces[31]->initial_line = pieces[31]->line;
    pieces[31]->initial_col  = pieces[31]->col;
    pieces[31]->in_table     = 1;
    pieces[31]->unicode      = 0x265A;
    pieces[30]->next         = pieces[31];

    Piece *fp = pieces[0];
    return fp;

}

void clear_table(wchar_t table[][8]) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            table[i][j] = EMPTY;
        }
    }
}

void print_table(wchar_t table[][8], Piece *fp, int curr_house[2], int **possible_moves) {

    while (system("clear"));
    setlocale(LC_CTYPE, "");
    clear_table(table);

    Piece *p = fp;

    while (p) {
        if (p->in_table == 1)
            table[p->line][p->col] = p->unicode;
        p = p->next;
    }

    int i = 0;
    while (i < 21 && possible_moves[i][0] != -1) {
        table[possible_moves[i][0]][possible_moves[i][1]] = 0x2605;
        ++i;
    }

    for (int i = 7; i > -1; --i) {
        for (int j = 0; j < 8; ++j) {
            if (curr_house[0] == i && curr_house[1] == j) {
                wprintf(COLOR_RED L"|%lc%lc%lc%lc|", 0x203E, 0x203E, 0x203E, 0x203E);
                continue;
            }
            if ((i+j) % 2 == 1) wprintf(COLOR_WHITE L"|%lc%lc%lc%lc|", 0x203E, 0x203E, 0x203E, 0x203E);
            else                wprintf(COLOR_GREEN L"|%lc%lc%lc%lc|", 0x203E, 0x203E, 0x203E, 0x203E);
        }
        wprintf(L"\n");

        for (int j = 0; j < 8; ++j) {
            if (curr_house[0] == i && curr_house[1] == j)
                wprintf(COLOR_RED L"| ", 0x203E, 0x203E, 0x203E, 0x203E);
            else if ((i+j) % 2 == 1) wprintf(COLOR_WHITE L"| ");
            else                     wprintf(COLOR_GREEN L"| ");

            wprintf(COLOR_RESET L"%lc", table[i][j]);

            if (curr_house[0] == i && curr_house[1] == j)
                wprintf(COLOR_RED L"  |");
            else if ((i+j) % 2 == 1) wprintf(COLOR_WHITE L"  |");
            else                wprintf(COLOR_GREEN L"  |");
        }
        wprintf(L"\n");

        for (int j = 0; j < 8; ++j) {
            if (curr_house[0] == i && curr_house[1] == j)
                wprintf(COLOR_RED L"|____|");
            else if ((i+j) % 2 == 1) wprintf(COLOR_WHITE L"|____|");
            else                wprintf(COLOR_GREEN L"|____|");
        }
        wprintf(L"\n");
    }
    /*
    for (int i = 7; i > -1; --i) {
        if (i%2 == 1) wprintf(COLOR_WHITE L" %lc%lc%lc%lc", 0x203E, 0x203E, 0x203E, 0x203E);
        else          wprintf(COLOR_GREEN L" %lc%lc%lc%lc", 0x203E, 0x203E, 0x203E, 0x203E);
    }
    wprintf(L"\n");
    */

}

int verify_check(Piece *p, wchar_t table[][8], Piece *fp) {
  int in_check = 0;

  int curr_house[2];
  curr_house[0] = p->line;
  curr_house[1] = p->col;
  int **possible_moves = calculate_possible_moves(p, table, curr_house, fp);

  int reverse_color = (p->color + 1) % 2;
  Piece *king = fp;
  while (!(king->type == KING && king->color == reverse_color)) king = king->next;

  int i = 0;
  while (possible_moves[i][0] != -1 && possible_moves[i][0] != king->line && possible_moves[i++][1] != king->col);
  if (possible_moves[i][0] == king->line && possible_moves[i++][1] == king->col) in_check = 1;

  return in_check;
}

int game_over(wchar_t table[][8]) {
    // Implement Check Mate verification

    return 0;
}

Piece *check_available_piece(wchar_t table[][8], Piece *fp, int turn, int curr_house[2], int in_check) {
  if (table[curr_house[0]][curr_house[1]] == EMPTY) return NULL;

  Piece *p = fp;
  while (p) {
      if (p->in_table                  &&
          p->line == curr_house[0]     &&
          p->col  == curr_house[1]) {
          break;
      }
      p = p->next;
  }

  if (p->color != turn) return NULL;

  // TODO: implement movement restriction when in check
  if (in_check) {
    Piece *king = fp;
    while (!(king->type == KING && king->color == turn)) king = king->next;
  }

  return p;
}

Piece *select_piece(wchar_t table[][8], Piece *fp, int turn, int curr_house[2], int in_check) {

    char str_turn[6] = "white";
    if (turn == BLACK) strcpy(str_turn,"black");

    char key = getch();
    while (key != ESC && key != ENTER) {
        if      (toupper(key) == 'W' && curr_house[0] < 7) curr_house[0] += 1;
        else if (toupper(key) == 'S' && curr_house[0] > 0) curr_house[0] -= 1;
        else if (toupper(key) == 'A' && curr_house[1] > 0) curr_house[1] -= 1;
        else if (toupper(key) == 'D' && curr_house[1] < 7) curr_house[1] += 1;

        print_table(table,fp,curr_house,reset_possible_moves());

        key = getch();
    }

    Piece *p = check_available_piece(table, fp, turn, curr_house, in_check);
    if (!p) return select_piece(table, fp, turn, curr_house, in_check);

    return p;
}

int check_house_status(Piece *p, Piece *fp, int line, int col) {
    Piece *ap = fp;

    if (line < 0 || col < 0 || line > 7 || col > 7) return BLOCK;

    if (p->type != PAWN) {
        while (ap) {
            if (ap->in_table && ap->line == line && ap->col == col) {
                if (ap->color != p->color) return CAPTURE;
                return BLOCK;
            }
            ap = ap->next;
        }
    }
    else {
        while (ap) {
            if (ap->in_table && ap->line == line && ap->col == col) {
                if (ap->color != p->color &&
                    (p->line - line == 1 || p->line - line == -1) &&
                    (p->col  - col  == 1 || p->col  - col  == -1)) return CAPTURE;
                return BLOCK;
            }
            ap = ap->next;
        }
    }

    return FREE;
}

int **calculate_possible_moves(Piece *p, wchar_t table[][8], int curr_house[2], Piece *fp) {

    int **possible_moves = reset_possible_moves();

    int count = 0;
    if (p->type == PAWN) {
        if (p->color == WHITE && p->line < 7) {
            int status_one_hop = check_house_status(p, fp, p->line + 1, p->col);
            if (status_one_hop != BLOCK) {
                possible_moves[count][0]   = p->line + 1;
                possible_moves[count++][1] = p->col;

                int status_two_hops = check_house_status(p, fp, p->line + 2, p->col);
                if (p->line == p->initial_line && status_two_hops != BLOCK) {
                    possible_moves[count][0]   = p->line + 2;
                    possible_moves[count++][1] = p->col;
                }
            }

            int status_capture = check_house_status(p, fp, p->line + 1, p->col + 1);
            if (status_capture == CAPTURE) {
                possible_moves[count][0]   = p->line + 1;
                possible_moves[count++][1] = p->col  + 1;
            }

            status_capture = check_house_status(p, fp, p->line + 1, p->col - 1);
            if (status_capture == CAPTURE) {
                possible_moves[count][0]   = p->line + 1;
                possible_moves[count++][1] = p->col  - 1;
            }
        }
        else if (p->color == BLACK && p->line > 0) {
            int status_one_hop = check_house_status(p, fp, p->line - 1, p->col);
            if (status_one_hop != BLOCK) {
                possible_moves[count][0]   = p->line - 1;
                possible_moves[count++][1] = p->col;

                int status_two_hops = check_house_status(p, fp, p->line - 2, p->col);
                if (p->line == p->initial_line && status_two_hops != BLOCK) {
                    possible_moves[count][0]   = p->line - 2;
                    possible_moves[count++][1] = p->col;
                }
            }

            int status_capture = check_house_status(p, fp, p->line - 1, p->col + 1);
            if (status_capture == CAPTURE) {
                possible_moves[count][0]   = p->line - 1;
                possible_moves[count++][1] = p->col  + 1;
            }

            status_capture = check_house_status(p, fp, p->line - 1, p->col - 1);
            if (status_capture == CAPTURE) {
                possible_moves[count][0]   = p->line - 1;
                possible_moves[count++][1] = p->col  - 1;
            }
        }
    }

    else if (p->type == ROOK) {
        int line = p->line - 1;
        while (line > -1) {
            int house_status = check_house_status(p, fp, line, p->col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = line--;
                possible_moves[count++][1] = p->col;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }

        line = p->line + 1;
        while (line < 8) {
            int house_status = check_house_status(p, fp, line, p->col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = line++;
                possible_moves[count++][1] = p->col;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }

        int col = p->col - 1;
        while (col > -1) {
            int house_status = check_house_status(p, fp, p->line, col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = p->line;
                possible_moves[count++][1] = col--;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }

        col = p->col + 1;
        while (col < 8) {
            int house_status = check_house_status(p, fp, p->line, col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = p->line;
                possible_moves[count++][1] = col++;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }
    }

    else if (p->type == KNIGHT) {
        int line = p->line + 2;
        int col  = p->col  + 1;
        int house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line + 2;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line + 1;
        col  = p->col  + 2;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line + 1;
        col  = p->col  - 2;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line - 2;
        col  = p->col  + 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line - 2;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line - 1;
        col  = p->col  + 2;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

        line = p->line - 1;
        col  = p->col  - 2;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0] = line;
            possible_moves[count++][1] = col;
        }

    }

    else if (p->type == BISHOP) {
        int line = p->line + 1;
        int col  = p->col  + 1;
        int house_status = check_house_status(p, fp, line, col);
        while (line < 8 && col < 8 && house_status != BLOCK) {
            possible_moves[count][0]   = line++;
            possible_moves[count++][1] = col++;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line + 1;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        while (line < 8 && col > -1 && house_status != BLOCK) {
            possible_moves[count][0]   = line++;
            possible_moves[count++][1] = col--;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line - 1;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        while (line > -1 && col > -1 && house_status != BLOCK) {
            possible_moves[count][0]   = line--;
            possible_moves[count++][1] = col--;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line - 1;
        col  = p->col  + 1;
        house_status = check_house_status(p, fp, line, col);
        while (line > -1 && col < 8 && house_status != BLOCK) {
            possible_moves[count][0]   = line--;
            possible_moves[count++][1] = col++;
            house_status = check_house_status(p, fp, line, col);
        }
    }

    else if (p->type == QUEEN) {
        int line = p->line + 1;
        int col  = p->col  + 1;
        int house_status = check_house_status(p, fp, line, col);
        while (line < 8 && col < 8 && house_status != BLOCK) {
            possible_moves[count][0]   = line++;
            possible_moves[count++][1] = col++;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line + 1;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        while (line < 8 && col > -1 && house_status != BLOCK) {
            possible_moves[count][0]   = line++;
            possible_moves[count++][1] = col--;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line - 1;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        while (line > -1 && col > -1 && house_status != BLOCK) {
            possible_moves[count][0]   = line--;
            possible_moves[count++][1] = col--;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line - 1;
        col  = p->col  + 1;
        house_status = check_house_status(p, fp, line, col);
        while (line > -1 && col < 8 && house_status != BLOCK) {
            possible_moves[count][0]   = line--;
            possible_moves[count++][1] = col++;
            house_status = check_house_status(p, fp, line, col);
        }

        line = p->line - 1;
        while (line > -1) {
            house_status = check_house_status(p, fp, line, p->col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = line--;
                possible_moves[count++][1] = p->col;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }

        line = p->line + 1;
        while (line < 8) {
            house_status = check_house_status(p, fp, line, p->col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = line++;
                possible_moves[count++][1] = p->col;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }

        col = p->col - 1;
        while (col > -1) {
            house_status = check_house_status(p, fp, p->line, col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = p->line;
                possible_moves[count++][1] = col--;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }

        col = p->col + 1;
        while (col < 8) {
            house_status = check_house_status(p, fp, p->line, col);
            if (house_status != BLOCK) {
                possible_moves[count][0] = p->line;
                possible_moves[count++][1] = col++;
            }

            if (house_status == BLOCK || house_status == CAPTURE) break;
        }
    }

    else if (p->type == KING) {
        int line = p->line - 1;
        int col  = p->col  - 1;
        int house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line - 1;
        col  = p->col;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line - 1;
        col  = p->col  + 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line;
        col  = p->col - 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line;
        col  = p->col + 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line + 1;
        col  = p->col  - 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line + 1;
        col  = p->col;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }

        line = p->line + 1;
        col  = p->col  + 1;
        house_status = check_house_status(p, fp, line, col);
        if (house_status != BLOCK) {
            possible_moves[count][0]   = line;
            possible_moves[count++][1] = col;
        }
    }

    possible_moves[count][0] = -1;
    possible_moves[count][1] = -1;

    //print_table(table, fp, curr_house, possible_moves);

    return possible_moves;

}

int move_piece(Piece *p, int **possible_moves, wchar_t table[][8], int curr_house[2], Piece *fp) {

    print_table(table,fp,curr_house,possible_moves);

    char key = getch();
    while (key != ESC && key != ENTER) {
        if      (toupper(key) == 'W' && curr_house[0] < 7) curr_house[0] += 1;
        else if (toupper(key) == 'S' && curr_house[0] > 0) curr_house[0] -= 1;
        else if (toupper(key) == 'A' && curr_house[1] > 0) curr_house[1] -= 1;
        else if (toupper(key) == 'D' && curr_house[1] < 7) curr_house[1] += 1;

        print_table(table,fp,curr_house,possible_moves);

        key = getch();
    }

    if (p->line == curr_house[0] && p->col == curr_house[1]) {
      return -2;
    }


    int ok = 0;
    int i = 0;
    while (i < 22 && possible_moves[i][0] != -1) {
        if (curr_house[0] == possible_moves[i][0] &&
            curr_house[1] == possible_moves[i][1]) {
            ok = 1;
            break;
        }
        ++i;
    }

    if (ok == 0) {
        // return move_piece(p, possible_moves, table, curr_house, fp);
        return 0;
    }

    int status_house = check_house_status(p, fp, curr_house[0], curr_house[1]);
    if (status_house == CAPTURE) {
        Piece *ap = fp;
        while (ap) {
            if (ap->in_table && ap->line == curr_house[0] && ap->col == curr_house[1]) {
                ap->in_table = 0;
                break;
            }
            ap = ap->next;
        }
    }

    p->line = curr_house[0];
    p->col  = curr_house[1];

    // print_table(table, fp, curr_house, reset_possible_moves());

    return 1;

}

int main() {
    int curr_house[2];
    curr_house[0] = 4;
    curr_house[1] = 4;

    wchar_t table[8][8];
    Piece *fp = initialize(table);
    print_table(table, fp, curr_house, reset_possible_moves());

    int in_check = 0;
    int turn = WHITE;
    while (!game_over(table)) {
      int ok = 0;
      Piece *selected_piece = NULL;
      while (ok != 1) {
        selected_piece = check_available_piece(table, fp, turn, curr_house);
        if (!selected_piece || ok == -2) selected_piece = select_piece(table, fp, turn, curr_house, in_check);
        int **possible_moves = calculate_possible_moves(selected_piece, table, curr_house, fp);
        ok = move_piece(selected_piece, possible_moves, table, curr_house, fp);

        print_table(table, fp, curr_house, reset_possible_moves());
      }
      in_check = verify_check(selected_piece, table, fp);
      turn = (turn + 1) % 2;
    }

    return 0;
}
