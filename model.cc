export module model;

import <vector>;
import <string>;
import <map>;
import posn;
import entity;
import word;

using std::vector, std::string, std::map;

export class Model {
  public:
	map<Properties, vector<Names>> rules;
	static inline map<Type, Names> word_to_name{
		{ BRAD_WORD, BRAD },
		{ SHAI_WORD, SHAI },
		{ GORD_WORD, GORD },
		{ WALL_WORD, WALL },
		{ BOX_WORD, BOX },
		{ BUSH_WORD, BUSH },
		{ LAVA_WORD, LAVA },
		{ H2O_WORD, H2O },
		{ FLAG_WORD, FLAG },
		{ KEY_WORD, KEY },
		{ DOOR_WORD, DOOR },
		{ PLUM_WORD, PLUM },
	};
	static inline map<Type, Properties> word_to_property{
		{ YOU_WORD, YOU },
		{ WIN_WORD, WIN },
		{ STOP_WORD, STOP },
		{ PUSH_WORD, PUSH },
		{ KILL_WORD, KILL },
		{ MOVE_WORD, MOVE },
		{ HOT_WORD, HOT },
		{ MELT_WORD, MELT },
		{ OPEN_WORD, OPEN },
		{ SHUT_WORD, SHUT }
	};
	bool completed = false;
	bool fail = false;
    int innerWidth, innerHeight;
	int level_number;
	string level_name;
	vector<Entity> entities;
	vector<Word> words;

    Model(int w = 80, int h = 28);
	void addEntity(int x, int y, Names name);
	void addWord(int x, int y, Type type);
	void move(char dir);
	void updateBoard();
	void reset();
  private:
	void updateBoardRules();
	void parse(Word *left_word, Word *right_word);
  	char checkCollisions(Entity &a, Entity &b);
	void checkWin(vector<Properties> &a, vector<Properties> &b);
	bool checkKill(vector<Properties> &a, vector<Properties> &b);
	bool checkMelt(vector<Properties> &a, vector<Properties> &b);
	bool checkOpen(vector<Properties> &a, vector<Properties> &b);
	vector<Properties> getProperties(Names name);
	bool tryMove(Word &mover, char dir);
	bool tryMove(Entity &mover, char dir);
};
