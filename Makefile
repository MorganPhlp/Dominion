CXX=g++
CXXFLAGS=-Wall -Wextra -Werror

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

exec: Carte.o CarteAction.o CarteVictoire.o CarteTresor.o Jeu.o test.o 
	$(CXX) $^ -o $@

.PHONY: run clean

run: exec
	./$<

clean:
	rm -f *.o *~ *.core 
