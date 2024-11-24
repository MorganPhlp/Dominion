CXX=g++
CXXFLAGS=-Wall -Wextra -Werror
LDFLAGS = -lncurses

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

exec: Carte.o CarteAction.o CarteVictoire.o CarteTresor.o Plateau.o Joueur.o Jeu.o test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: run clean

run: exec
	./$<

clean:
	rm -f *.o *~ *.core
