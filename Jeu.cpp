#include "Jeu.h"

std::vector<CarteTresor> Jeu::listeCarteTresor;
std::vector<CarteAction> Jeu::listeCarteAction;
std::vector<CarteVictoire> Jeu::listeCarteVictoire;

Jeu::Jeu(int nb_joueurs) : m_nb_joueurs(nb_joueurs) {}

Jeu::~Jeu() {}

std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> Jeu::loadCard(){
	std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> data;
	std::ifstream file("ExcelActionCards.csv");
	std::string line;
	
	if(file.is_open()){
		std::getline(file, line); //Pour ignorer en-tête
		
		while(std::getline(file, line)){
			std::stringstream ss(line);
			std::string name, description, cardType, col;
			int cost, win_points = 0, coins = 0, draws = 0, actions = 0, buys = 0;
			bool attack = false, reaction = false;
			
			try{
				std::getline(ss, cardType, ',');
				std::getline(ss, name, ',');
				
				std::getline(ss, col, ',');
				cost = col.empty() ? 0 : std::stoi(col);
				
				bool entreGuillemets = false;
				char c;
				while(ss.get(c)){
					if(c == '"' && !entreGuillemets){
						entreGuillemets = true;
					}
					else if(c == '"' && entreGuillemets){
						entreGuillemets = false;
					}
					else if(c == ',' && !entreGuillemets){
						break;
					}
					else description.push_back(c);
				}
				if (description.front() > 'Z' || description.front() < 'A') { //Bug avec les guillemets qui rajoutent des caractères au début et à la fin de description
    					description.erase(0, 3);
    					description.erase(description.size() - 3, 3);
				}
				
				
				std::getline(ss, col, ',');
				win_points = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				coins = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				draws = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				actions = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				buys = col.empty() ? 0 : std::stoi(col);
				
				std::getline(ss, col, ',');
				attack = (col == "1");
				std::getline(ss, col, ',');
				reaction = (col == "1");
				
				std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool> t {cardType, name, cost, description, win_points, coins, draws, actions, buys, attack, reaction};
				data.push_back(t);
			} catch (const std::invalid_argument& e) {
                		std::cerr << "Erreur de conversion dans la ligne : " << line << "\n";
                		std::cerr << "Exception: " << e.what() << std::endl;
			} catch (const std::out_of_range& e) {
				std::cerr << "Valeur hors limite dans la ligne : " << line << "\n";
				std::cerr << "Exception: " << e.what() << std::endl;
			}
			
		}
		file.close();
	} else{
		std::cerr << "Impossible d'ouvrir le fichier donné" << std::endl;
	}
	return data;
}

void Jeu::remplirListeCarte(){
	std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> data = loadCard();
	for(const auto& t : data){
		if(std::get<0>(t) == "Trésor"){
			CarteTresor c = CarteTresor(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<5>(t));
			listeCarteTresor.push_back(c);
		}
		else if(std::get<0>(t) == "Victoire"){
			CarteVictoire c = CarteVictoire(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<4>(t));
			listeCarteVictoire.push_back(c);
		}
		else{
			CarteAction c = CarteAction(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<7>(t), std::get<8>(t), std::get<6>(t), std::get<5>(t), std::get<9>(t), std::get<10>(t));
			listeCarteAction.push_back(c);
		}
	}
}


void Jeu::printTotalCard(){
    std::cout << "========== CARTES TRESOR ==========" << std::endl;
    for (const CarteTresor& carte : listeCarteTresor) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
    }

    std::cout << "\n========== CARTES VICTOIRE ==========" << std::endl;
    for (const CarteVictoire& carte : listeCarteVictoire) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
    }

    std::cout << "\n========== CARTES ACTION ==========" << std::endl;
    for (const CarteAction& carte : listeCarteAction) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
    }
}

























