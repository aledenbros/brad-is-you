import controller;
import <string>;

int main(int argc, char** argv) {
    Controller game;
    if (argc > 1) {
        int level = std::stoi(argv[1]);
        game.run(level);
    } else {
        game.run(1);
    }
}
