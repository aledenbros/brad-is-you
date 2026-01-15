export module view;

import model;
import entity;
import word;
import <vector>;
import <map>;
#include <ncurses.h>

using std::map, std::vector;

export class View {
	map<Names, vector<char>> sprites;
	map<Names, vector<int>> colours;
	map<Type, vector<char>> words;
	map<Type, int> word_colours;
  public:
	View()
	: sprites{
        { BRAD, { '(', ')', '[', ']' } },
        { SHAI, { '(', ')', '[', ']' } },
        { GORD, { '(', ')', '[', ']' } },

        { WALL, { '=', '=', '=', '=' } },
        { BOX,  { 'X', 'X', 'X', 'X' } },
        { BUSH, { '@', '@', '@', '@' } },
        { LAVA, { '~', '~', '~', '~' } },
        { H2O,  { '~', '~', '~', '~' } },

        { FLAG, { '[', ']', '|', ' ' } },
        { KEY,  { '%', '>', '|', ' ' } },
        { DOOR, { 'T', 'T', '|', '|' } },
        { PLUM, { ' ', 'P', '{', '}' } }
    }, colours{
        { BRAD, { 1, 1, 2, 2 } },
        { SHAI, { 3, 3, 4, 4 } },
        { GORD, { 5, 5, 6, 6 } },

        { WALL, { 7, 7, 7, 7 } },
        { BOX,  { 8, 8, 8, 8 } },
        { BUSH, { 9, 9, 9, 9 } },
        { LAVA, { 10, 10, 10, 10 } },
        { H2O,  { 11, 11, 11, 11 } },

        { FLAG, { 12, 12, 12, 12 } },
        { KEY,  { 13, 13, 13, 13 } },
        { DOOR, { 15, 15, 15, 15 } },
        { PLUM, { 9, 9, 16, 16 } }
    }, words {
		{ BRAD_WORD, { 'B', 'R', 'A', 'D' } },
		{ SHAI_WORD, { 'S', 'H', 'A', 'I' } },
		{ GORD_WORD, { 'G', 'O', 'R', 'D' } },

		{ WALL_WORD, { 'W', 'A', 'L', 'L' } },
		{ BOX_WORD,  { 'B', 'O', 'X', ' ' } },
		{ BUSH_WORD, { 'B', 'U', 'S', 'H' } },
		{ LAVA_WORD, { 'L', 'A', 'V', 'A' } },
		{ H2O_WORD,  { 'H', '2', 'O', ' ' } },

		{ FLAG_WORD, { 'F', 'L', 'A', 'G' } },
		{ KEY_WORD,  { 'K', 'E', 'Y', ' ' } },
		{ DOOR_WORD, { 'D', 'O', 'O', 'R' } },
		{ PLUM_WORD, { 'P', 'L', 'U', 'M' } },

		{ YOU_WORD,  { 'Y', 'O', 'U', ' ' } },
		{ WIN_WORD,  { 'W', 'I', 'N', ' ' } },
		{ STOP_WORD, { 'S', 'T', 'O', 'P' } },
		{ PUSH_WORD, { 'P', 'U', 'S', 'H' } },
		{ KILL_WORD, { 'K', 'I', 'L', 'L' } },
		{ MOVE_WORD, { 'M', 'O', 'V', 'E' } },

		{ HOT_WORD,  { 'H', 'O', 'T', ' ' } },
		{ MELT_WORD, { 'M', 'E', 'L', 'T' } },
		{ OPEN_WORD, { 'O', 'P', 'E', 'N' } },
		{ SHUT_WORD, { 'S', 'H', 'U', 'T' } },

		{ IS, { 'I', ' ', ' ', 'S' } },
		{ AND, { 'A', ' ', 'N', 'D' } },
		{ NOT, { 'N', 'O', ' ', 'T'} }
	}, word_colours{
		{ BRAD_WORD, 1 },
		{ SHAI_WORD, 3 },
		{ GORD_WORD, 5 },
		{ WALL_WORD, 5 },
		{ BOX_WORD, 8 },
		{ BUSH_WORD, 9 },
		{ LAVA_WORD, 10 },
		{ H2O_WORD, 11 },
		{ FLAG_WORD, 12 },
		{ KEY_WORD, 13 },
		{ DOOR_WORD, 15 },
		{ PLUM_WORD, 9 },
		{ YOU_WORD, 16 },
		{ WIN_WORD, 17 },
		{ STOP_WORD, 18 },
		{ PUSH_WORD, 19 },
		{ KILL_WORD, 20 },
		{ MOVE_WORD, 21 },
		{ HOT_WORD, 22 },
		{ MELT_WORD, 23 },
		{ OPEN_WORD, 24 },
		{ SHUT_WORD, 25 },
		{ IS, 13 },
		{ AND, 13 },
		{ NOT, 14 }
	} {
		initscr();
		start_color();
		init_colours();
    	noecho();
    	cbreak();
    	keypad(stdscr, TRUE);
    	curs_set(0);
	}
	~View() {
		endwin();
	}
    void drawBorders(int w, int h);
    void drawObjects(const Model& m);
	void drawTitle(const Model& m);
    void render(const Model& m);
	void completed(const Model& m);
	void fail(const Model& m);
	void restart(const Model& m);
  private:
	void init_colours() {
	    init_pair(1,  COLOR_YELLOW,  COLOR_BLACK);   // BRAD ()
	    init_pair(2,  COLOR_BLUE,    COLOR_BLACK);   // BRAD []

	    init_pair(3,  COLOR_WHITE,   COLOR_BLACK);   // SHAI ()
	    init_pair(4,  COLOR_BLACK,   COLOR_BLACK);   // SHAI []

	    init_pair(5,  COLOR_WHITE,   COLOR_BLACK);   // GORD ()
	    init_pair(6,  COLOR_YELLOW,  COLOR_BLACK);   // GORD []

	    init_pair(7,  COLOR_BLACK,   COLOR_WHITE);   // WALL ===
	    init_pair(8,  COLOR_WHITE,  COLOR_YELLOW);   // BOX XX
	    init_pair(9,  COLOR_GREEN,   COLOR_BLACK);   // BUSH @@
	    init_pair(10, COLOR_YELLOW,    COLOR_RED);   // LAVA ~~
	    init_pair(11, COLOR_CYAN,     COLOR_BLUE);   // H2O ~~

	    init_pair(12, COLOR_YELLOW,  COLOR_BLACK);   // FLAG []|

	    init_pair(13, COLOR_WHITE,   COLOR_BLACK);   // KEY %>
	    init_pair(14, COLOR_RED,     COLOR_BLACK);   // DOOR TT
	    init_pair(15, COLOR_MAGENTA, COLOR_BLACK);   // PLUM {}

		init_pair(16, COLOR_BLACK, COLOR_MAGENTA); // YOU
		init_pair(17, COLOR_BLACK, COLOR_YELLOW); // WIN
		init_pair(18, COLOR_BLACK, COLOR_WHITE); // STOP
		init_pair(19, COLOR_BLACK, COLOR_YELLOW); // PUSH
		init_pair(20, COLOR_BLACK, COLOR_RED); // KILL
		init_pair(21, COLOR_BLACK, COLOR_CYAN); // MOVE
		init_pair(22, COLOR_BLACK, COLOR_RED); // HOT
		init_pair(23, COLOR_BLACK, COLOR_YELLOW); // MELT
		init_pair(24, COLOR_BLACK, COLOR_WHITE); // OPEN
		init_pair(25, COLOR_BLACK, COLOR_RED); // SHUT
	}
};
