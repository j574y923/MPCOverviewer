
//#include "SFML/Graphics.hpp"

#include "copypaste.h"
#include "tab.h"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <vector>

#ifndef MPC_HELPER_WINDOWS_H
#define MPC_HELPER_WINDOWS_H

class MPCHelperWindows{

public:

	//bool exit because return true if chose to save or not, false if chose to cancel or exit; depending on bool value, tab will be closed
	void saveprompt(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as), tab (&tab_to_save), bool (&exit));

	void menu_1savewindow(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as));

	void menu_1loadwindow(std::string(&directory_string), sf::Font(&font), sf::Vector2i(&position), sf::Text(&title));

	void menu_2tablist(sf::Font(&font), sf::Vector2i(&position), std::vector<tab>(&tab_list), int(&current_tab));

	//SHOULD TAKE IN ALL SELECTIONS (I.E. A VECTOR OF ARRAYS THAT CONTAIN CHAR VECTORS)
	//NEED ALL THE SELECTION LENGTHS AS WELL
	//THIS MAY END UP WITH ERROR LNK2019 AND ERROR LNK1120 OR SOMETHING
	void replace(sf::Vector2i(&position), std::vector<std::vector<char>*> listofselections, std::vector<int> listofselectionlengths, copypaste(&copypastetool));
};

#endif