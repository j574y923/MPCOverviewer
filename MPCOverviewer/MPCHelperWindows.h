
//#include "SFML/Graphics.hpp"

#include "copypaste.h"
#include "tab.h"

#include <fstream>
#include <istream>
//#include <iostream>
//#include <string>
//#include <vector>

#ifndef MPC_HELPER_WINDOWS_H
#define MPC_HELPER_WINDOWS_H

class MPCHelperWindows{

public:

	//bool exit because return true if chose to save or not, false if chose to cancel or exit; depending on bool value, tab will be closed
	void saveprompt(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as), tab (&tab_to_save), bool (&exit));

	void menu_1savewindow(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as), bool rename);

	void menu_1loadwindow(std::string(&directory_string), sf::Font(&font), sf::Vector2i(&position), sf::Text(&title));

	void menu_2tablist(sf::Font(&font), sf::Vector2i(&position), std::vector<tab>(&tab_list), int(&current_tab));

	//SHOULD TAKE IN ALL SELECTIONS (I.E. A VECTOR OF ARRAYS THAT CONTAIN CHAR VECTORS)
	//NEED ALL THE SELECTION LENGTHS AS WELL
	void replace(sf::Vector2i(&position), std::vector<std::vector<char>*> &clip_selections_vec, std::vector<int> &selectionlengths_vec, copypaste &copypastetool);

	//note that new_tab is not taken by reference
	void special_filter(sf::Vector2i(&position), std::string &specialmeasure_string, tab new_tab, tab &tab, int* currently_filtered_notes);

	//This probably still needs work in the area of replacing (e.g. crashing) but has all the functionality intended at the moment
	void find_and_replace(sf::Vector2i(&position), sf::Font(&font), tab new_tab, tab &tab, copypaste &copypastetool, sf::RenderWindow &window, sf::View &view_0, sf::Sprite &bg, int &current_x, int&current_y);
};

#endif