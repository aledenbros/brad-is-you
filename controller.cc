export module controller;

import model;
import view;

export class Controller {
  public:
    Model model;
    View view;

    Controller();
	  void load(int level);
    void run(int level);

  private:
    void handleInput(int ch);
};
