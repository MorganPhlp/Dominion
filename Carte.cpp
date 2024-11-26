#include <iostream>
#include <string>
#include "Carte.h"
#include <ncurses.h>

// Constructeur
Carte::Carte(std::string name, std::string description, int cost, TypeCarte type) : m_name(name), m_description(description), m_cost(cost), m_type(type) {}

// Destructeur
Carte::~Carte() = default;

//Getters
std::string Carte::getName() const{return m_name;}
std::string Carte::getDescription() const{return m_description;}
TypeCarte Carte::getType() const{return m_type;}
int Carte::getCost() const{return m_cost;}
