module model;

import <algorithm>;

using std::vector, std::string, std::map, std::sort;

Model::Model(int w, int h)
: rules{{YOU, {}}}, innerWidth{w}, innerHeight{h}, level_number{0}, level_name{""}, 
    entities{vector<Entity>()} {}

void directionSort(vector<Entity *> &vec, char dir) {
    sort(vec.begin(), vec.end(), [dir](auto &a, auto &b) {
        bool is_x = (dir == 'l' || dir == 'r');
        int av = is_x ? a->loc.x : a->loc.y;
        int bv = is_x ? b->loc.x : b->loc.y;
        if (dir == 'd' || dir == 'r') {
            return av > bv;
        } else {
            return av < bv;
        }
    });
}

void Model::addEntity(int x, int y, Names name) {
    entities.push_back(Entity(Posn(2 * x + 1, 2 * y + 1), name));
}

void Model::addWord(int x, int y, Type type) {
    words.push_back(Word(Posn(2 * x + 1, 2 * y + 1), type));
}

void Model::move(char dir) {
    vector<Names> moving_names = rules[MOVE];
    vector<Entity *> moves;
    for (auto &e : entities) {
        if (std::find(moving_names.begin(), moving_names.end(), e.name) != moving_names.end()) {
            moves.push_back(&e);
        }
    }

    for (auto e : moves) {
        tryMove(*e, e->facing);
    }

    if (dir == ' ') {
        return;
    }

    vector<Names> names = rules[YOU];
    vector<Entity *> yous;
    for (auto &e : entities) {
        if (std::find(names.begin(), names.end(), e.name) != names.end()) {
            yous.push_back(&e);
        }
    }
    directionSort(yous, dir);

    for (auto e : yous) {
        tryMove(*e, dir);
    }
}

void Model::reset() {
    entities.clear();
    words.clear();
    rules.clear();
}

void Model::updateBoard() {
    vector<size_t> to_remove;
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i; j < entities.size(); ++j) {
            if (entities[i].loc == entities[j].loc) {
                char result = checkCollisions(entities[i], entities[j]);
                if (result == 'a') {
                    to_remove.push_back(i);
                } else if (result == 'b') {
                    to_remove.push_back(j);
                } else if (result == 'c') {
                    to_remove.push_back(j);
                    to_remove.push_back(i);
                }
            }
        }
    }

    sort(to_remove.begin(), to_remove.end(), std::greater<size_t>());
    to_remove.erase(unique(to_remove.begin(), to_remove.end()), to_remove.end());
    for (auto idx : to_remove) {
        entities.erase(entities.begin() + idx);
    }

    updateBoardRules();

    vector<Entity *> you_entities;
    for (auto &e : entities) {
        if (std::find(rules[YOU].begin(), rules[YOU].end(), e.name) != rules[YOU].end()) {
            you_entities.push_back(&e);
        }
    }

    if (rules[YOU].empty() || you_entities.empty()) {
        fail = true;
    }

    sort(entities.begin(), entities.end(), [this](auto &a, auto &b) {
        if ((std::find(rules[YOU].begin(), rules[YOU].end(), a.name) != rules[YOU].end() &&
            std::find(rules[YOU].begin(), rules[YOU].end(), b.name) == rules[YOU].end()) ||
            (!getProperties(a.name).empty() && getProperties(b.name).empty())) {
            return false;
        } else {
            return true;
        }
    });
}

void Model::updateBoardRules() {
    rules.clear();
    vector<Word> is_words;
    for (auto &w : words) {
        if (w.type == IS) {
            is_words.push_back(w);
        }
    }
    for (auto &is_word : is_words) {
        Posn loc = is_word.loc;
        Posn left = loc + Posn(- 2, 0);
        Posn right = loc + Posn(2, 0);
        Posn up = loc + Posn(0, - 2);
        Posn down = loc + Posn(0, 2); 
        Word *left_word = nullptr;
        Word *right_word = nullptr;
        Word *up_word = nullptr;
        Word *down_word = nullptr;
        for (auto &w : words) {
            if (w.loc == left) {
                left_word = &w;
            } else if (w.loc == right) {
                right_word = &w;
            } else if (w.loc == up) {
                up_word = &w;
            } else if (w.loc == down) {
                down_word = &w;
            }
        }
        if (left_word != nullptr && right_word != nullptr) {
            parse(left_word, right_word);
        }
        if (up_word != nullptr && down_word != nullptr) {
            parse(up_word, down_word);
        }
    }
}

void Model::parse(Word *left_word, Word *right_word) {
    WordType lt = left_word->wordType;
    WordType rt = right_word->wordType;
    if (lt == NOUN && rt == ADJECTIVE) {
        Names n = word_to_name[left_word->type];
        Properties p = word_to_property[right_word->type];
        if (rules.find(p) == rules.end()) {
            rules[p] = vector<Names>();
        }
        if (std::find(rules[p].begin(), rules[p].end(), n) == rules[p].end()) {
            rules[p].push_back(n);
        }
    } else if (lt == NOUN && rt == NOUN) {
        Names n1 = word_to_name[left_word->type];
        Names n2 = word_to_name[right_word->type];
        for (auto &e : entities) {
            if (e.name == n1) {
                e.name = n2;
            }
        }
    }
}

char Model::checkCollisions(Entity &a, Entity &b) {
    vector<Properties> a_properties = getProperties(a.name);
    vector<Properties> b_properties = getProperties(b.name);
    checkWin(a_properties, b_properties);
    bool remove_a = checkKill(a_properties, b_properties) || checkMelt(a_properties, b_properties);
    bool remove_b = checkKill(b_properties, a_properties) || checkMelt(b_properties, a_properties);
    if (checkOpen(a_properties, b_properties) || (remove_a && remove_b)) {
        return 'c';
    } else if (remove_a) {
        return 'a';
    } else if (remove_b) {
        return 'b';
    } else {
        return 'n';
    }
}

void Model::checkWin(vector<Properties> &a, vector<Properties> &b) {
    if ((std::find(a.begin(), a.end(), YOU) != a.end()) &&
        (std::find(b.begin(), b.end(), WIN) != b.end())) {
        completed = true;
    } else if ((std::find(b.begin(), b.end(), YOU) != b.end()) &&
               (std::find(a.begin(), a.end(), WIN) != a.end())) {
        completed = true;
    }
}

bool Model::checkKill(vector<Properties> &a, vector<Properties> &b) {
    if ((std::find(a.begin(), a.end(), YOU) != a.end()) &&
        (std::find(b.begin(), b.end(), KILL) != b.end())) {
        return true;
    }
    return false;
}

bool Model::checkMelt(vector<Properties> &a, vector<Properties> &b) {
    if ((std::find(a.begin(), a.end(), MELT) != a.end()) &&
        (std::find(b.begin(), b.end(), HOT) != b.end())) {
        return true;
    }
    return false;
}

bool Model::checkOpen(vector<Properties> &a, vector<Properties> &b) {
    if (((std::find(a.begin(), a.end(), OPEN) != a.end()) &&
        (std::find(b.begin(), b.end(), SHUT) != b.end())) ||
        ((std::find(b.begin(), b.end(), OPEN) != b.end()) &&
        (std::find(a.begin(), a.end(), SHUT) != a.end()))) {
        return true;
    }
    return false;
}


vector<Properties> Model::getProperties(Names name) {
    vector<Properties> props;
    for (auto &[prop, names] : rules) {
        if (std::find(names.begin(), names.end(), name) != names.end()) {
            props.push_back(prop);
        }
    }
    return props;
}

bool Model::tryMove(Word &mover, char dir) {
    Posn newloc = mover.loc;
    if (dir == 'u') newloc = newloc + Posn(0, -2);
    else if (dir == 'd') newloc = newloc + Posn(0, 2);
    else if (dir == 'l') newloc = newloc + Posn(-2, 0);
    else newloc = newloc + Posn(2, 0);

    if (newloc.x < 1 || newloc.y < 1 || newloc.x >= innerWidth || newloc.y >= innerHeight)
        return false;

    // Collect everything at the new location
    vector<Entity*> others;
    for (auto &e : entities)
        if (e.loc == newloc) others.push_back(&e);

    for (auto &other : others) {
        // STOP entities block words
        if (std::find(rules[STOP].begin(), rules[STOP].end(), other->name) != rules[STOP].end())
            return false;

        // PUSH entities attempt recursive push
        if (std::find(rules[PUSH].begin(), rules[PUSH].end(), other->name) != rules[PUSH].end()) {
            if (!tryMove(*other, dir)) return false;
        }

        // YOU entities block words (optional depending on your rules)
        if (std::find(rules[YOU].begin(), rules[YOU].end(), other->name) != rules[YOU].end())
            return false;
    }

    // Check for other words
    for (auto &w : words) {
        if (w.loc == newloc) {
            if (!tryMove(w, dir)) return false;
        }
    }

    // Move the word
    mover.loc = newloc;
    return true;
}


bool Model::tryMove(Entity &mover, char dir) {
    Posn newloc = mover.loc;
    mover.facing = dir;
    if (dir == 'u') {
        newloc = newloc + Posn(0, -2);
    } else if (dir == 'd') {
        newloc = newloc + Posn(0, 2);
    } else if (dir == 'l') {
        newloc = newloc + Posn(-2, 0);
    } else {
        newloc = newloc + Posn(2, 0);
    }

    bool is_move = (std::find(rules[MOVE].begin(), rules[MOVE].end(), mover.name) != rules[MOVE].end());

    if ((newloc.x < 1 || newloc.y < 1 || newloc.x >= innerWidth || newloc.y >= innerHeight) 
        && !is_move) {
        return false;
    } 

    vector<Entity *> others;
    for (auto &e : entities) {
        if (newloc == e.loc) {
            others.push_back(&e);
        }
    }

    if (is_move) {
        bool bounce = false;
        if (newloc.x < 1 || newloc.y < 1 || newloc.x >= innerWidth || newloc.y >= innerHeight) {
            bounce = true;
        }
        for (auto &other : others) {
            if (std::find(rules[STOP].begin(), rules[STOP].end(), other->name) != rules[STOP].end()) {
                bounce = true;
            }
            if (std::find(rules[PUSH].begin(), rules[PUSH].end(), other->name) != rules[PUSH].end()) {
                if (!tryMove(*other, dir)) {
                    bounce = true;
                } else {
                    mover.move(newloc);
                    return true;
                }
            }
        }
        for (auto &w : words) {
            if (newloc == w.loc) {
                if (!tryMove(w, dir)) {
                    bounce = true;
                } else {
                    mover.move(newloc);
                    return true;
                }
            }
        }
        if (bounce) {
            if (dir == 'u') {
                mover.facing = 'd';
                newloc = mover.loc + Posn(0, 2);
            } else if (dir == 'd') {
                mover.facing = 'u';
                newloc = mover.loc + Posn(0, -2);
            } else if (dir == 'l') {
                mover.facing = 'r';
                newloc = mover.loc + Posn(2, 0);
            } else {
                mover.facing = 'l';
                newloc = mover.loc + Posn(-2, 0);
            }
            others.clear();
            for (auto &e : entities) {
                if (newloc == e.loc) {
                    others.push_back(&e);
                }
            }
        }
    }

    for (auto &other : others) {
        if (std::find(rules[STOP].begin(), rules[STOP].end(), other->name) != rules[STOP].end()
            || (!is_move && std::find(rules[YOU].begin(), rules[YOU].end(), other->name) != rules[YOU].end())) {
            return false;
        }
        if (std::find(rules[PUSH].begin(), rules[PUSH].end(), other->name) != rules[PUSH].end()) {
            if (!tryMove(*other, dir)) {
                return false;
            }
        }
    }

    for (auto &w : words) {
        if (newloc == w.loc) {
            if (!tryMove(w, dir)) {
                return false;
            }
        }
    }

    mover.move(newloc);
    return true;
}
