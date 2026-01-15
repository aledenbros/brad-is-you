module view;

import model;
import entity;
import word;

import <string>;
import <chrono>;
import <thread>;

using std::string;

void View::drawBorders(int w, int h) {
    int border_w = w + 2;
    int border_h = h + 2;

    mvaddch(0, 0,'+');
    mvaddch(0, border_w - 1, '+');
    mvaddch(border_h - 1, 0, '+');
    mvaddch(border_h - 1, border_w - 1, '+');

    for (int x = 1; x < border_w - 1; ++x) {
        mvaddch(0, x, '-');
        mvaddch(border_h - 1, x, '-');
    }
    for (int y = 1; y < border_h - 1; ++y) {
        mvaddch(y, 0, '|');
        mvaddch(y, border_w - 1, '|');
    }
}

void View::drawTitle(const Model& m) {
    string title = "Level " + std::to_string(m.level_number) + ": " + m.level_name;
    mvaddstr(m.innerHeight + 2, 0, title.c_str());
}

void View::drawObjects(const Model& m) {
    for (size_t i = 0; i < m.entities.size(); ++i) {
        const auto &e = m.entities[i];
        attron(COLOR_PAIR(colours[e.name][0]));
        mvaddch(e.loc.y, e.loc.x, sprites[e.name][0]);
        attroff(COLOR_PAIR(colours[e.name][0]));

        attron(COLOR_PAIR(colours[e.name][1]));
        mvaddch(e.loc.y, e.loc.x + 1, sprites[e.name][1]);
        attroff(COLOR_PAIR(colours[e.name][1]));

        attron(COLOR_PAIR(colours[e.name][2]));
        mvaddch(e.loc.y + 1, e.loc.x, sprites[e.name][2]);
        attroff(COLOR_PAIR(colours[e.name][2]));

        attron(COLOR_PAIR(colours[e.name][3]));
        mvaddch(e.loc.y + 1, e.loc.x + 1, sprites[e.name][3]);
        attroff(COLOR_PAIR(colours[e.name][3]));
    }
    for (size_t i = 0; i < m.words.size(); ++i) {
        const auto &w = m.words[i];
        attron(COLOR_PAIR(word_colours[w.type]));
        mvaddch(w.loc.y, w.loc.x, words[w.type][0]);
        mvaddch(w.loc.y, w.loc.x + 1, words[w.type][1]);
        mvaddch(w.loc.y + 1, w.loc.x, words[w.type][2]);
        mvaddch(w.loc.y + 1, w.loc.x + 1, words[w.type][3]);
        attroff(COLOR_PAIR(word_colours[w.type]));
    }
}

void View::restart(const Model& m) {
    render(m);
    string msg = "Restarting...";
    mvaddstr(m.innerHeight / 2, (m.innerWidth - msg.size()) / 2, msg.c_str());
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void View::fail(const Model& m) {
    render(m);
    string msg = "Nothing is YOU. Restarting...";
    attron(COLOR_PAIR(14));
    mvaddstr(m.innerHeight / 2, (m.innerWidth - msg.size()) / 2, msg.c_str());
    attroff(COLOR_PAIR(14));
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void View::completed(const Model& m) {
    render(m);
    string msg = "Level Completed!";
    attron(COLOR_PAIR(6));
    mvaddstr(m.innerHeight / 2, (m.innerWidth - msg.size()) / 2, msg.c_str());
    attroff(COLOR_PAIR(6));
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void View::render(const Model& m) {
    clear();
    drawBorders(m.innerWidth, m.innerHeight);
    drawTitle(m);
    drawObjects(m);
    refresh();
}
