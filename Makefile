CXX=g++
CXXFLAGS=-Wall -Wextra -Werror

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

exec: Carte.o Jeu.o PLateau.o
	$(CXX) $^ -o $@

.PHONY: run clean

run: exec
	./$<

clean:
	rm -f *.o *~ *.core 
