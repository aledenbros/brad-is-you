export module posn;

export class Posn {
  public:
	int x, y;
	Posn(int x, int y): x{x}, y{y} {}
	Posn operator+(Posn &&other) {
		return Posn(x + other.x, y + other.y);
	}
	bool operator==(const Posn &other) const {
		return x == other.x && y == other.y;
	}
};
