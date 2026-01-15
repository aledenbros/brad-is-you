# Edit the dependency/link order here (list object files in the order you want them linked).
# Put your files in the desired order in `DEP_ORDER` — no new target needed.
DEP_ORDER = posn.o entity.o entity-impl.o word.o model.o model-impl.o view.o view-impl.o controller.o controller-impl.o main.o

all: $(DEP_ORDER) game

game: $(DEP_ORDER) gcm.cache
	g++-14 -std=c++20 -fmodules-ts -Wall -g $(DEP_ORDER) -lncurses -o game


# Auto-generate per-object prerequisites from `DEP_ORDER`.
# Each object depends on its source (`%.cc`), all objects listed before it in `DEP_ORDER`,
# and `gcm.cache`. We rely on the generic `%.o` rule below to provide the compilation recipe.
SEEN :=
$(foreach o,$(DEP_ORDER),$(eval $(o): $(o:.o=.cc) $(SEEN) gcm.cache)$(eval SEEN := $(SEEN) $(o)))

%.o: %.cc gcm.cache
	g++-14 -std=c++20 -fmodules-ts -Wall -g -c $<

gcm.cache:
	g++-14 -std=c++20 -fmodules-ts -c -x c++-system-header chrono thread string vector cstddef new map algorithm

.PHONY: clean
clean:
	rm -f *.o game
	rm -rf gcm.cache
