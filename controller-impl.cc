module controller;

import <new>;
import model;
import view;
import entity;
import word;
import posn;
#include <ncurses.h>

Controller::Controller() : model{80, 28} {}

void Controller::load(int level) {
    if (level == 1) {
        model.level_number = 1;
        model.level_name = "Where do I go?";

        model.addWord(3, 2, FLAG_WORD);

        model.addEntity(5, 2, FLAG);
        model.addWord(15, 1, IS);
        model.addWord(19, 2, WIN_WORD);

        model.addEntity(17, 3, FLAG);

        model.addEntity(20, 4, WALL);


        model.addWord(14, 9, WALL_WORD);
        model.addWord(15, 9, IS);
        model.addWord(16, 9, STOP_WORD);

        model.addEntity(19, 9, BRAD);

        model.addWord(3, 9,  BRAD_WORD);
        model.addWord(3, 10, IS);
        model.addWord(3, 11, YOU_WORD);

    } else if (level == 2) {
        model.level_number = 2;
        model.level_name = "Bring it to me!";
        
        model.addWord(3, 2, BUSH_WORD);
        model.addWord(4, 2, IS);
        model.addWord(5, 2, YOU_WORD);

        model.addEntity(10, 5, BUSH);

        model.addWord(15, 1, SHAI_WORD);
        model.addWord(16, 1, IS);
        model.addWord(17, 1, MOVE_WORD);

        model.addEntity(20, 8, SHAI);

        model.addWord(30, 5, BOX_WORD);
        model.addWord(31, 5, IS);
        model.addWord(32, 5, WIN_WORD);
        model.addWord(30, 6, IS);
        model.addWord(31, 7, PUSH_WORD);

        model.addEntity(35, 8, BOX);

        model.addWord(20, 10, WALL_WORD);
        model.addWord(21, 10, IS);
        model.addWord(22, 10, STOP_WORD);

        model.addEntity(18, 8, WALL);
        
    } else if (level == 3) {
        model.level_number = 3;
        model.level_name = "Hot stuff!";
        
        model.addWord(3, 2, LAVA_WORD);
        model.addWord(4, 2, IS);
        model.addWord(5, 2, YOU_WORD);

        model.addEntity(10, 5, LAVA);

        model.addWord(15, 1, GORD_WORD);
        model.addWord(16, 1, IS);
        model.addWord(17, 1, HOT_WORD);

        model.addEntity(20, 8, GORD);

        model.addWord(30, 5, PLUM_WORD);
        model.addWord(31, 5, IS);
        model.addWord(32, 5, WIN_WORD);
        model.addWord(30, 6, IS);
        model.addWord(31, 7, MELT_WORD);

        model.addEntity(35, 8, PLUM);
    } else if (level == 4) {
        model.level_number = 4;
        model.level_name = "Keys and Doors!";
        
        model.addWord(3, 2, DOOR_WORD);
        model.addWord(4, 2, IS);
        model.addWord(5, 2, YOU_WORD);

        model.addEntity(10, 5, DOOR);

        model.addWord(15, 1, KEY_WORD);
        model.addWord(16, 1, IS);
        model.addWord(17, 1, OPEN_WORD);

        model.addEntity(20, 8, KEY);

        model.addWord(30, 5, BOX_WORD);
        model.addWord(31, 5, IS);
        model.addWord(32, 5, WIN_WORD);
        model.addWord(30, 6, IS);
        model.addWord(31, 7, SHUT_WORD);

        model.addEntity(35, 8, BOX);
    } else if (level == 5) {
        model.level_number = 5;
        model.level_name = "Watch out!";
        model.addWord(3, 2, BRAD_WORD);
        model.addWord(4, 2, IS);
        model.addWord(5, 2, YOU_WORD);
        model.addEntity(10, 5, BRAD);
        model.addWord(15, 1, GORD_WORD);
        model.addWord(16, 1, IS);
        model.addWord(17, 1, KILL_WORD);
        model.addEntity(20, 8, GORD);
        model.addWord(30, 5, FLAG_WORD);
        model.addWord(31, 5, IS);
        model.addWord(32, 5, WIN_WORD);
        model.addEntity(35, 8, FLAG);
    } else {
        model.level_number = 1;
        model.level_name = "Where do I go?";
        // Default to level 1
    }
}

void Controller::run(int level) {
    load(level);
    model.updateBoard();
    view.render(model);

    int ch;
    while ((ch = getch()) != 'q') {
        handleInput(ch);
        
        model.updateBoard();

        view.render(model);

        if (model.fail) {
            view.fail(model);
            model.fail = false;
            model.reset();
            load(model.level_number);
            view.render(model);
        }
        if (model.completed) {
            view.completed(model);
            model.completed = false;
            model.reset();
            ++model.level_number;
            load(model.level_number);
            view.render(model);
        }
    }
}

void Controller::handleInput(int ch) {
    if (ch == 'r') {
        view.restart(model);
        model.reset();
        load(model.level_number);
    } else if (ch == KEY_UP || ch == 'w') {
        model.move('u');
    } else if (ch == KEY_DOWN || ch == 's') {
        model.move('d');
    } else if (ch == KEY_LEFT || ch == 'a') {
        model.move('l');
    } else if (ch == KEY_RIGHT || ch == 'd') {
        model.move('r');
    } else if (ch == ' ') {
        model.move(' ');
    }
}
