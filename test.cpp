//#include "Plateau.h"
#include "Jeu.h"

int main(){
	Plateau plat = Plateau(2);
	Joueur j1 = Joueur("Morgan");
	Joueur j2 = Joueur("Fefe");
	std::vector<Joueur> liste = {j1,j2};
	Jeu jeu = Jeu(plat, liste);
	jeu.jouerPartie();
	return 0;
}
