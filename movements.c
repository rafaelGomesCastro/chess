#include "movements.h"

int check_house_status(Piece *p, Piece *fp, int line, int col) {
    Piece *ap = fp;

    if (line < 0 || col < 0 || line > 7 || col > 7) return BLOCK;

    if (p->type != PAWN) {
        while (ap) {
            if (ap->state && ap->line == line && ap->col == col) {
                if (ap->color != p->color) return CAPTURE;
                return BLOCK;
            }
            ap = ap->next;
        }
    }
    else {
        while (ap) {
            if (ap->state && ap->line == line && ap->col == col) {
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

void get_pawn_move(Piece *p, int **possible_moves, Piece *fp) {
  int count = 0;

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
