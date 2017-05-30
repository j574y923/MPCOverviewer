
//#include "SFML/Graphics.hpp"

#include "TileMapComposition.hpp"
#include "undovector.h"

#ifndef TAB_H
#define TAB_H

class tab{

public:
	//sets all private members to null, etc.
	tab();

	//deallocates m_composition, etc. when tab closed
	//~tab();

	//draw m_composition
	//if changes then change the tile map
	void drawcomposition(bool changes, sf::RenderWindow (&window));

	//update changes from m_undovector then call drawcomposition to draw updated composition
	void updatecomposition(bool undo, sf::RenderWindow(&window));

	//update changes from where user clicked with m_current_note then call drawcomposition to draw updated composition
	void updatecomposition(char staffindex, int measureindex, bool remove, char flatsharpnat, sf::RenderWindow(&window));

	//update changes from where user clicked then call drawcomposition to draw updated composition
	void updatecomposition(int volumelevel, int measureindex, sf::RenderWindow(&window));

	//main passes in directory; directory is added to m_filename string and txt file is read and populates m_composition
	//main calls this whenever a new file is loaded into the tab
	void populatecomposition(std::string directory);

	//return m_composition when need to output txt file
	//also return as reference when need to alter m_composition through copypastetool
	std::vector<char>* getm_composition();
	void outputcomposition();
	//DONT DO THIS, WILL NOT BE ABLE TO DEFINE REGION//void copypastecommand(int command_num);//command_num: 0 for copy, 1 for remove, 2 for paste

	//return m_filename (this is referred to as the 'title' in main)
	std::string getm_filename();
	//changes m_composition to loaded file and populate m_composition
	void setm_filename(std::string filename);

	//return m_current_x to update position of bg and menu_1 in current tab rendered in window
	int getcurrent_x();
	void setcurrent_x(int current_x);

	//return m_current_y to update position of bg and menu_1 in current tab rendered in window
	int getcurrent_y();
	void setcurrent_y(int current_y);

	//used to set m_current_note, main will detect when the instrument bar has been clicked and will pass in the clicked instrument
	void setcurrent_note(char current_note);
	//get m_note_pos
	int getm_note_pos();

	//add onto string of filtered notes
	void addontom_filtered_notes(char note_to_filter);
	//remove from string of filtered notes
	void removefromm_filtered_notes(char note_to_remove);

	//return string of filtered notes to display on gui
	std::string getm_filtered_notes();
private:
	std::vector<char> m_composition[384];
	TileMapComposition m_tile_map_comp;
	//TileMapComposition m_tile_map_FS;//way slower than one tilemap
	//std::vector<sf::RectangleShape> m_comp_volume;//way slower than one tilemap
	//std::vector<sf::Text> m_comp_volume_char;//way slower than one tilemap
	//std::string m_directory;//[private member for current directory of file here]//belongs in main
	std::string m_filename = "NewSong]MarioPaint";//[private member for file name here]
	int m_tempo = 358;
	int m_current_x, m_current_y;
	int m_note_pos;
	//sf::Texture m_instruments_transparent;
	//sf::Sprite m_current_note;
	std::string m_filtered_notes;
	undovector m_undovector = undovector(m_composition);
	//DONT DO THIS, WILL NOT BE ABLE TO DEFINE REGION//copypaste copypastetool;
};

//#include "tab.hpp"

#endif