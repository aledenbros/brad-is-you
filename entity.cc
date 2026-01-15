export module entity;

import <string>;
import <vector>;
import <new>;
import posn;

using std::string, std::vector;

export enum Names { BRAD, SHAI, GORD, WALL, BOX, BUSH, LAVA, H2O, FLAG, KEY, DOOR, PLUM };
export enum Properties { YOU, WIN, STOP, PUSH, KILL, MOVE, HOT, MELT, OPEN, SHUT };

export class Entity {
  public:
    Posn loc;
	Names name;
    char facing;
  public:
    Entity(Posn loc, Names name, char facing = 'r')
    : loc{loc}, name{name}, facing{facing} {}
    Entity(const Entity& other) = default;
    void move(Posn &newloc);
};
