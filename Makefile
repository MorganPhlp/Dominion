CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -g
LDFLAGS = -lncurses

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

exec: Carte.o CarteAction.o CarteVictoire.o CarteTresor.o Plateau.o Joueur.o Jeu.o test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: run debug clean

run: exec
	./$<
debug: exec
	gdb ./$<

clean:
	rm -f *.o *~ *.core
