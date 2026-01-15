export module word;

import posn;
import entity;

export enum WordType { NOUN, ADJECTIVE, OPERATOR };
export enum Type { IS, AND, NOT, 
                    BRAD_WORD, SHAI_WORD, GORD_WORD, WALL_WORD, BOX_WORD, BUSH_WORD, 
                    LAVA_WORD, H2O_WORD, FLAG_WORD, KEY_WORD, DOOR_WORD, PLUM_WORD, 
                    YOU_WORD, WIN_WORD, STOP_WORD, PUSH_WORD, KILL_WORD, MOVE_WORD, 
                    HOT_WORD, MELT_WORD, OPEN_WORD, SHUT_WORD };

export class Word {
  public:
    Posn loc;
    Type type;
    WordType wordType;
  public:
    Word(Posn loc, Type type) : loc{loc}, type{type} {
        if (type == IS || type == AND || type == NOT) {
            wordType = OPERATOR;
        } else if (type >= BRAD_WORD && type <= PLUM_WORD) {
            wordType = NOUN;
        } else {
            wordType = ADJECTIVE;
        }
    };
    void move(Posn &newloc) {
        loc = newloc;
    }
};
