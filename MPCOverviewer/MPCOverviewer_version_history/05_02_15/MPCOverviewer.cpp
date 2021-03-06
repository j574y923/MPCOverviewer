// MPCOverviewer.cpp : Defines the entry point for the console application.
//
//#include "SFML/Graphics.hpp"//included in TileMapComposition.hpp?

//#include "copypaste.h"//included in MPCHelperWindows.h
#include "MPCHelperWindows.h"
//#include "tab.h"//included in MPCHelperWindows.h
#include "tchar.h"
#include "TileMap.hpp"

//#include <fstream>//all //included in MPCHelperWindows.h
//#include <istream>
//#include <iostream>
//#include <string>
//#include <vector>

//GET RID OF THIS
void populaterecreatedcomposition(std::vector<char> (&composition)[384]){//NOTE NEED TO CHANGE '1' TO 'q'

	try
	{
		std::ifstream infile("changethislater.txt");//this will be specified by user in gui
		std::string file_contents;
		infile >> file_contents;

		int i = 0;
		int j = 4;//note content starts at index 4 (position 5) right after time signature
		while (i < 384)
		{
			//http://stackoverflow.com/questions/9438209/for-every-character-in-string
			char c = file_contents.at(j);
			j++;
			if (c == ':')
			{
				i++;
			}
			else
			{
				composition[i].push_back(c);
			}
		}
	}
	catch (std::exception & obj)
	{

	}
	for (int i = 0; i < 384; i++)
	{
		for (int j = 0; j < composition[i].size(); j++)
		{
			std::cout << composition[i].at(j);
		}
	}
}

char menu_2whichnote(unsigned int mouseposition){

	if (52 <= mouseposition && mouseposition <= 581)
	{
		if (472 <= mouseposition && mouseposition <= 497) 
			return 'q';//pirahna
		if (500 <= mouseposition && mouseposition <= 525) 
			return 'p';//coin
		if ((unsigned)((mouseposition - 52) - ((unsigned)(mouseposition - 52) / 28) * 2) % 26 != 0 && (unsigned)((mouseposition - 52) - ((unsigned)(mouseposition - 52) / 28)) % 27 != 0)
		{
			mouseposition = (mouseposition - 52) / 28;//should get an integer between 0 and 18
			return (char)(mouseposition + 97);//add 97 to get ascii values of chars
		}
		else//clicked in the margins between one of the acceptable instrument buttons so return nothing
			return 't';//let t be the indicator for nothing
	}
	else if (593 <= mouseposition && mouseposition <= 622)//eraser
		return 'x';
	else if (635 <= mouseposition && mouseposition <= 664)//region selector
		return 'y';
	else//nothing
		return 't';
}

int menu2_whichtab(unsigned int mousepositionx, unsigned int mousepositiony, unsigned int current_x, unsigned int current_y){

	mousepositionx = mousepositionx + current_x - 700;
	if (current_y < 50)
		mousepositiony = mousepositiony + current_y - 50;
	
	return (mousepositionx / 100) * 4 + (mousepositiony / 20);
}

void menu2_loadtabchanges(tab &loadtab, sf::Text &directory, sf::Text &title, sf::Text &save_as, int &current_x, int &current_y, int &note_pos, int (&currently_filtered_notes)[19]){

	title.setString(loadtab.getm_filename());
	title.setPosition(928 - title.getGlobalBounds().width, 10);
	save_as.setString(directory.getString() + "\\" + title.getString());
	//current_x = loadtab.getcurrent_x();
	//current_y = loadtab.getcurrent_y();
	note_pos = loadtab.getm_note_pos();

	std::string filtered_notes_string = loadtab.getm_filtered_notes();
	for (unsigned int i = 0; i < 19; ++i)
	{
		currently_filtered_notes[i] = 19;
	}
	for (unsigned int i = 0; i < filtered_notes_string.size(); ++i)
	{
		currently_filtered_notes[(int)filtered_notes_string.at(i) - 97] = (int)filtered_notes_string.at(i) - 97;
	}
}

void menu2_toolselector(int (&note_pos), bool (&eraser), bool (&region_selector), int select){

	if (select == 0)//instrument
	{
		eraser = false; region_selector = false;
	}
	else if (select == 1)//toggle eraser
	{
		note_pos = 19; eraser = !eraser; region_selector = false;
	}
	else//toggle region selector
	{
		note_pos = 19; eraser = false; region_selector = !region_selector;
	}
}

sf::Vector2f arrowdown_whichindex(int mousepositionx, unsigned int mousepositiony, unsigned int current_x, unsigned int current_y){
	
	mousepositionx = mousepositionx + current_x - 75;
	if (mousepositionx < 0)
		mousepositionx = 0;
	else if (mousepositionx > 1279)
		mousepositionx = 1279;

	mousepositiony = mousepositiony + current_y - 130;
	
	sf::Vector2f indexposition(((unsigned)mousepositionx / 40) * 40 + 106, (mousepositiony / 230) * 230 + 154);
	return indexposition;
}

int arrowright_whichindex(unsigned int mousepositiony, unsigned int current_y){

	mousepositiony = mousepositiony + current_y - 130;
	unsigned int level = mousepositiony / 230;
	unsigned int test = mousepositiony % 230;
	if (test < 24)
		return level * 230 + 143;//note: level * 230 + 24 + 130 - 13 (- 13 to account for texture height)
	else if (test > 185)
		return level * 230 + 303;
	else
	{
		test = test - 19;
		return level * 230 + (test / 10) * 10 + 143;
	}
}

std::vector<int> region_whichbounds(const sf::RectangleShape &rectangleregion){

	//essentially the left bound
	int beginningindex_j = -rectangleregion.getOrigin().x - 75;
	if (beginningindex_j < 0)
		beginningindex_j = 0;
	//else if (beginningindex_j > 1279)//note origin should not be above 1335 and beginningindex_j should not be above 1260 and this should be checked before passing in the rectangle
	//	beginningindex_j = 1279;
	if ((unsigned)beginningindex_j % 40 > 20)
		beginningindex_j = ((unsigned)beginningindex_j / 40) + 1;
	else
		beginningindex_j = ((unsigned)beginningindex_j / 40);
	//essentially the right bound
	int endingindex_j = -rectangleregion.getOrigin().x + rectangleregion.getGlobalBounds().width - 75;
	if (endingindex_j < 0)
		endingindex_j = 0;
	if ((unsigned)endingindex_j % 40 < 20)
		endingindex_j = ((unsigned)endingindex_j / 40) - 1;
	else
		endingindex_j = ((unsigned)endingindex_j / 40);
	//essentially the upper bound
	unsigned int beginningindex_i = -rectangleregion.getOrigin().y - 130;
	beginningindex_i = (beginningindex_i / 230);
	//essentially the lower bound
	unsigned int endingindex_i = -rectangleregion.getOrigin().y + rectangleregion.getGlobalBounds().height - 130;
	unsigned int totallevels = (endingindex_i / 230) - beginningindex_i + 1;

	std::vector<int> bounds_vec;
	for (unsigned int i = 0; i < totallevels; ++i)
	{
		int staffend = ((unsigned)endingindex_i % 230) - 24;
		if (staffend >= 0 || i != totallevels - 1)//this would effectively mean that this level (the final one) is useless as it is a bound before the first possible note 'a'
		{
			//int bounds[4];//bounds[0] is beginningindex, bounds[1] is endingindex, bounds[2] is staffbeg, bounds[3] is staffend
			bounds_vec.push_back(beginningindex_i * 32 + i * 32 + beginningindex_j);//bounds[0] = beginningindex_i * 32 + i * 32 + beginningindex_j; 
			bounds_vec.push_back(beginningindex_i * 32 + i * 32 + endingindex_j);//bounds[1] = beginningindex_i * 32 + i * 32 + endingindex_j;
			if (i == 0)
			{
				int staffbeg = -rectangleregion.getOrigin().y - 130;
				staffbeg = ((unsigned)staffbeg % 230) - 24;
				if (staffbeg < 0)
					bounds_vec.push_back(97);//bounds[2] = 97;
				else if (staffbeg > 160)
					bounds_vec.push_back(113);//bounds[2] = 113;
				else
				{
					if ((unsigned)staffbeg % 10 > 0)
						bounds_vec.push_back(((unsigned)staffbeg / 10) + 97 + 1);//bounds[2] = ((unsigned)staffbeg / 10) + 97 + 1;
					else
						bounds_vec.push_back(((unsigned)staffbeg / 10) + 97);//bounds[2] = ((unsigned)staffbeg / 10) + 97;
				}
			}
			else
				bounds_vec.push_back(97);//bounds[2] = 97;
			if (i == totallevels - 1)
			{
				//if (staffend < 0)//not possible
				//	bounds[3] = 97;
				if (staffend > 160)
					bounds_vec.push_back(113);// bounds[3] = 113;
				else
					bounds_vec.push_back(((unsigned)staffend / 10) + 97);// bounds[3] = ((unsigned)staffend / 10) + 97;
			}
			else
				bounds_vec.push_back(113);// bounds[3] = 113;

			//bounds_vec.push_back(bounds[0]); bounds_vec.push_back(bounds[1]);
			//bounds_vec.push_back(bounds[2]); bounds_vec.push_back(bounds[3]);
		}
	}

	return bounds_vec;
}

template <typename selectionStruct>
void define_clipboard(tab &tab, copypaste &copypastetool, std::vector<selectionStruct> &selections_vec, std::vector<std::vector<char>*> &clipboard, std::vector<int> &clipboardlengths_vec){//works?

	clear_clipboard(clipboard);
	clipboardlengths_vec.clear();
	unsigned int i, j, beginningindex, endingindex = 0;
	for (i = 0; i < selections_vec.size(); ++i)
	{
		beginningindex = selections_vec.at(i).index;
		for (j = i; j < selections_vec.size(); ++j)
		{
			if (j == selections_vec.size() - 1)
			{
				endingindex = selections_vec.at(j).index;
				clipboard.push_back(copypastetool.copy(tab.getm_composition(), beginningindex, endingindex, selections_vec.at(j).staffbeg, selections_vec.at(j).staffend, tab.getm_filtered_notes(), endingindex - beginningindex + 1));
				clipboardlengths_vec.push_back(endingindex - beginningindex + 1);
				break;
			}
			if (selections_vec.at(j).index != selections_vec.at(j + 1).index - 1
				|| selections_vec.at(j).staffbeg != selections_vec.at(j + 1).staffbeg
				|| selections_vec.at(j).staffend != selections_vec.at(j + 1).staffend)
			{
				endingindex = selections_vec.at(j).index;
				clipboard.push_back(copypastetool.copy(tab.getm_composition(), beginningindex, endingindex, selections_vec.at(j).staffbeg, selections_vec.at(j).staffend, tab.getm_filtered_notes(), endingindex - beginningindex + 1));
				clipboardlengths_vec.push_back(endingindex - beginningindex + 1);
				break;
			}
		}
		i = j;
	}
}

void clear_clipboard(std::vector<std::vector<char>*> &clipboard){//works?

	for (unsigned int i = 0; i < clipboard.size(); ++i)
	{
		delete[] clipboard.at(i);
	}
	clipboard.clear();
}

template <typename selectionStruct>
void remove_selection(tab &tab, copypaste &copypastetool, std::vector<selectionStruct> &selections_vec){//works?

	unsigned int i, j, beginningindex, endingindex = 0;
	for (i = 0; i < selections_vec.size(); ++i)
	{
		beginningindex = selections_vec.at(i).index;
		for (j = i; j < selections_vec.size(); ++j)
		{
			if (j == selections_vec.size() - 1)
			{
				endingindex = selections_vec.at(j).index;
				copypastetool.remove(tab.getm_composition(), beginningindex, endingindex, selections_vec.at(j).staffbeg, selections_vec.at(j).staffend, tab.getm_filtered_notes());
				break;
			}
			if (selections_vec.at(j).index != selections_vec.at(j + 1).index - 1
				|| selections_vec.at(j).staffbeg != selections_vec.at(j + 1).staffbeg
				|| selections_vec.at(j).staffend != selections_vec.at(j + 1).staffend)
			{
				endingindex = selections_vec.at(j).index;
				copypastetool.remove(tab.getm_composition(), beginningindex, endingindex, selections_vec.at(j).staffbeg, selections_vec.at(j).staffend, tab.getm_filtered_notes());
				break;
			}
		}
		i = j;
	}
}

void paste_selection(tab &tab, copypaste &copypastetool, std::vector<std::vector<char>*> &clipboard, int pastingindex, std::vector<int> &clipboardlengths_vec){

	for (unsigned int i = 0; i < clipboard.size(); ++i)
	{
		copypastetool.paste(tab.getm_composition(), pastingindex, clipboard.at(i), clipboardlengths_vec.at(i));
	}
}

int _tmain(int argc, _TCHAR** argv){

	//http://stackoverflow.com/questions/18159820/how-to-initialize-an-array-of-vectorint-in-c-with-predefined-counts
	//std::array<std::vector<char>, 384> composition;
	std::vector<char> composition[384];

	//http://stackoverflow.com/questions/5724171/passing-an-array-by-reference
	//void foo(int (&x)[100]);
		//This only accepts arrays of 100 integers
	populaterecreatedcomposition(composition);

	//test area
	//https://www.youtube.com/watch?v=FLpD54gx_5w
	
	MPCHelperWindows helperwindowstool;
	copypaste copypastetool;

	//
	//	Main window
	//
	sf::RenderWindow window(sf::VideoMode(1200, 750), "MPCOverviewer");
	sf::View view_0;
	view_0.reset(sf::FloatRect(0, 0, 1200, 750));
	view_0.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view_0);

	//
	//	Position of view in main window
	//
	int current_x = 0;
	int current_y = 0;
	//
	//	Position of mouse in main window
	//
	sf::Vector2i position;

	//
	//	Background image (overview of all staves and measures)
	//
	sf::Texture texture0;
	texture0.loadFromFile("bg.png");
	sf::Sprite bg(texture0);
	bg.setOrigin(sf::Vector2f(0, -130));

	//
	//	Load menu
	//
	sf::Texture texture1;
	texture1.loadFromFile("menu_1.png");
	sf::Sprite menu_1(texture1);
	menu_1.setTextureRect(sf::IntRect(0, 0, 1360, 50));
	//
	//	Directory
	//
	sf::Font font;
	font.loadFromFile("resources/sansation.ttf");
	sf::Text directory;
	directory.setFont(font);
	directory.setColor(sf::Color::Black);
	directory.setCharacterSize(15);
	directory.setPosition(sf::Vector2f(265, 10));
	//
	//	Song files in directory read from MarioPaintSongvec.txt and stored in directory_string
	//
	std::string directory_string;
	try
	{
		std::ifstream infile("config.txt");
		std::string file_contents;

		getline(infile, file_contents);
		file_contents.erase(file_contents.begin(), file_contents.begin() + 10);
		directory.setString(file_contents);
		directory_string = file_contents;
	}
	catch (std::exception & obj)
	{

	}
	//
	//	Title of song file
	//
	sf::Text title;
	title.setString("NewSong]MarioPaint");
	title.setFont(font);
	title.setColor(sf::Color::Black);
	title.setCharacterSize(15);
	title.setPosition(928 - title.getLocalBounds().width, 10);
	//
	//	Save as
	//
	sf::Text save_as;
	save_as.setString(directory.getString() + "\\" + title.getString());//set title's string through tab first
	save_as.setFont(font);
	save_as.setColor(sf::Color::Black);
	save_as.setCharacterSize(10);
	save_as.setPosition(sf::Vector2f(345, 30));

	//
	//	Tool menu
	//
	sf::Texture texture2;
	texture2.loadFromFile("menu_2.png");
	sf::Sprite menu_2(texture2);
	menu_2.setOrigin(sf::Vector2f(0, -50));
	menu_2.setTextureRect(sf::IntRect(0, 0, 1360, 80));
	//
	//	Selected instrument that appears on the uppermost film square
	//
	sf::Texture texture3;
	texture3.loadFromFile("sprites/large/instruments_transparent.gif");
	sf::Sprite current_note(texture3);
	current_note.setOrigin(sf::Vector2f(-12, -50));
	int note_pos = 19;
	//
	//	Selected instruments to be filtered that appear on filter bar
	//
	TileMap filtered_notes;
	int currently_filtered_notes[19] = { 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19 };
	if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
	{
		return -1;
	}
	filtered_notes.setOrigin(sf::Vector2f(-54, -91));
	//
	//	Hotkey bindings for num1-5 to be bound to desired instruments
	//
	int hotkey_notes[5] = { 20, 20, 20, 20, 20 };
	//
	//	Eraser, region selector and drag toggle variables
	//
	bool eraser = false;
	bool region_selector = false;
	bool begin_selecting = false;
	bool drag = false;
	bool fade_rectangle_vec = false; 
	bool menu2_move = false;
	//
	//	Eraser and region selector buttons that appear when turned on
	//
	sf::Texture texture4;
	texture4.loadFromFile("sprites/ERASER.png");
	sf::Sprite eraser_on(texture4);
	eraser_on.setOrigin(sf::Vector2f(-593, -53));
	sf::Texture texture5;
	texture5.loadFromFile("sprites/REGIONSELECTOR.png");
	sf::Sprite region_selector_on(texture5);
	region_selector_on.setOrigin(sf::Vector2f(-635, -53));
	//
	//	Arrow image to indicate where the cursor's position is on the composition
	//
	sf::Texture texture6;
	texture6.loadFromFile("sprites/softicons_dot_com_slash_web-icons_slash_marker-style-icons-by-icondock_slash_arrow-down-icon.png");
	sf::Sprite arrowdown(texture6);
	arrowdown.setOrigin(sf::Vector2f(24, 24));
	sf::Sprite arrowright(texture6);
	arrowright.setOrigin(sf::Vector2f(24, 24));
	arrowright.rotate(-90);

	//
	//	Tab vec
	//
	std::vector<tab> tab_vec;
	tab new_tab = tab();
	tab_vec.push_back(new_tab);
	int current_tab = 0;
	int tab_offset = 0;
	std::vector<sf::Text> tab_text_vec;
	tab_text_vec.reserve(16);
	sf::Text new_tab_text;
	new_tab_text.setFont(font);
	new_tab_text.setColor(sf::Color::Black);
	new_tab_text.setCharacterSize(10);
	new_tab_text.setOrigin(sf::Vector2f(-700, -50));
	new_tab_text.setString("NewSong]MarioPaint");
	tab_text_vec.push_back(new_tab_text);
	sf::RectangleShape rectangle0(sf::Vector2f(100, 20));
	rectangle0.setFillColor(sf::Color::Transparent);
	rectangle0.setOutlineThickness(2);
	rectangle0.setOutlineColor(sf::Color::Green);
	rectangle0.setOrigin(-700, -50);

	//
	//	Rectangle vec for selecting multiple regions
	//
	std::vector<sf::RectangleShape> rectangle_vec;
	sf::RectangleShape new_rectangle(sf::Vector2f(0, 0));
	new_rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	new_rectangle.setOutlineThickness(1);
	new_rectangle.setOutlineColor(sf::Color::Red);
	new_rectangle.setOrigin(0, 0);
	//
	//	Fade clock used to increment a fade for rectangle vec after it has been dragged
	//
	sf::Clock fade_clock;
	fade_clock.restart();
	//http://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects
	//	Selection vec for selected regions
	//
	struct selectionStruct
	{
		int index;//index of the composition
		char staffbeg;
		char staffend;

		selectionStruct(int i, char _a, char _b) : index(i), staffbeg(_a), staffend(_b) {}

		bool operator < (const selectionStruct& str) const
		{
			return (index < str.index);
		}
	};
	std::vector < selectionStruct > selections_vec;
	//
	//	Clipboard
	//
	std::vector<std::vector<char>*> clipboard;//use std::array instead of pointer
	std::vector <int> clipboardlengths_vec;

	//
	//	Ctrl and Shift toggle variables for modifiers needed for certain shortcut keys
	//
	bool ctrl = false;
	bool shift = false;

	while (window.isOpen()){

		sf::Event event_0;

		while (window.pollEvent(event_0)){

			switch (event_0.type){

			case sf::Event::Closed://SAVE PROMPT, probably for all tabs, might need a devoted method for this
				window.close();
				break;
				
			case sf::Event::Resized:
				if (window.getSize().x > 1360)
					window.setSize(sf::Vector2u(1360, window.getSize().y));
				if (window.getSize().y > 2892)
					window.setSize(sf::Vector2u(window.getSize().x, 2892));
				
				current_x, current_y = 0;
				break;

			case sf::Event::KeyPressed:
				switch (event_0.key.code){

				case sf::Keyboard::Right: case sf::Keyboard::D:
					if (current_x + 100 + window.getSize().x > 1360)
						current_x = 1360 - window.getSize().x;
					else
						current_x += 100;

					menu2_move = true;
					break;
				case sf::Keyboard::Left: case sf::Keyboard::A:
					if (current_x - 100 < 0)
						current_x = 0;
					else
						current_x -= 100;

					menu2_move = true;
					break;
				case sf::Keyboard::Up:
				Up:
					if (current_y - 100 < 0)
						current_y = 0;
					else
						current_y -= 100;

					menu2_move = true;
					break;
				case sf::Keyboard::Down: case sf::Keyboard::Space:
				Down:
					if (current_y + 100 + window.getSize().y > 2892)
						current_y = 2892 - window.getSize().y;
					else
						current_y += 100;

					menu2_move = true;
					break;

				case sf::Keyboard::Num1:{menu2_toolselector(note_pos, eraser, region_selector, 0);
					position = sf::Mouse::getPosition(window); int y_offset; if (current_y > 50) { y_offset = 50; }else{ y_offset = current_y; }
					if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{ if ((int)menu_2whichnote(position.x + current_x) - 97 < 19){ hotkey_notes[0] = note_pos = (int)menu_2whichnote(position.x + current_x) - 97; } }//set binding for num1
					else if (hotkey_notes[0] != 20) { note_pos = hotkey_notes[0]; } tab_vec.at(current_tab + tab_offset).setcurrent_note((char)(note_pos + 97));//use num1
					}break;
				case sf::Keyboard::Num2:{menu2_toolselector(note_pos, eraser, region_selector, 0);
					position = sf::Mouse::getPosition(window); int y_offset; if (current_y > 50) { y_offset = 50; }else{ y_offset = current_y; }
					if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{ if ((int)menu_2whichnote(position.x + current_x) - 97 < 19){ hotkey_notes[1] = note_pos = (int)menu_2whichnote(position.x + current_x) - 97; } }//set binding for num2
					else if (hotkey_notes[1] != 20) { note_pos = hotkey_notes[1]; } tab_vec.at(current_tab + tab_offset).setcurrent_note((char)(note_pos + 97));//use num2
					}break;
				case sf::Keyboard::Num3:{menu2_toolselector(note_pos, eraser, region_selector, 0);
					position = sf::Mouse::getPosition(window); int y_offset; if (current_y > 50) { y_offset = 50; }else{ y_offset = current_y; }
					if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{ if ((int)menu_2whichnote(position.x + current_x) - 97 < 19){ hotkey_notes[2] = note_pos = (int)menu_2whichnote(position.x + current_x) - 97; } }//set binding for num3
					else if (hotkey_notes[2] != 20) { note_pos = hotkey_notes[2]; } tab_vec.at(current_tab + tab_offset).setcurrent_note((char)(note_pos + 97));//use num3
					}break;
				case sf::Keyboard::Num4:{menu2_toolselector(note_pos, eraser, region_selector, 0);
					position = sf::Mouse::getPosition(window); int y_offset; if (current_y > 50) { y_offset = 50; }else{ y_offset = current_y; }
					if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{ if ((int)menu_2whichnote(position.x + current_x) - 97 < 19){ hotkey_notes[3] = note_pos = (int)menu_2whichnote(position.x + current_x) - 97; } }//set binding for num4
					else if (hotkey_notes[3] != 20) { note_pos = hotkey_notes[3]; } tab_vec.at(current_tab + tab_offset).setcurrent_note((char)(note_pos + 97));//use num4
					}break;
				case sf::Keyboard::Num5:{menu2_toolselector(note_pos, eraser, region_selector, 0);
					position = sf::Mouse::getPosition(window); int y_offset; if (current_y > 50) { y_offset = 50; }else{ y_offset = current_y; }
					if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{ if ((int)menu_2whichnote(position.x + current_x) - 97 < 19){ hotkey_notes[4] = note_pos = (int)menu_2whichnote(position.x + current_x) - 97; } }//set binding for num5
					else if (hotkey_notes[4] != 20) { note_pos = hotkey_notes[4]; } tab_vec.at(current_tab + tab_offset).setcurrent_note((char)(note_pos + 97));//use num5
					}break;

				case sf::Keyboard::C://copy
					if (ctrl)
					{
						if (!selections_vec.empty())
						{
							//copy whats in the region
							define_clipboard(tab_vec.at(current_tab + tab_offset), copypastetool, selections_vec, clipboard, clipboardlengths_vec);
						}
					}
					break;

				case sf::Keyboard::E://eraser
					menu2_toolselector(note_pos, eraser, region_selector, 1);
					break;

				case sf::Keyboard::F:{

					position = sf::Mouse::getPosition(window);
					int temp_note_pos = note_pos;
					int y_offset;
					if (current_y > 50)
						y_offset = 50;
					else
						y_offset = current_y;
				
					if (55 - y_offset <= position.y && position.y <= 82 - y_offset && position.x + current_x < 680)
					{
						note_pos = (int)menu_2whichnote(position.x + current_x) - 97;
						if (note_pos <= 18)
						{
							if (note_pos == 15)
							{
								if (currently_filtered_notes[note_pos + 1] == 19)
								{
									currently_filtered_notes[note_pos + 1] = note_pos;
									tab_vec.at(current_tab + tab_offset).addontom_filtered_notes((char)(note_pos + 97));
								}
								else
								{
									currently_filtered_notes[note_pos + 1] = 19;
									tab_vec.at(current_tab + tab_offset).removefromm_filtered_notes((char)(note_pos + 97));
								}
							}
							else if (note_pos == 16)
							{
								if (currently_filtered_notes[note_pos - 1] == 19)
								{
									currently_filtered_notes[note_pos - 1] = note_pos;
									tab_vec.at(current_tab + tab_offset).addontom_filtered_notes((char)(note_pos + 97));
								}
								else
								{
									currently_filtered_notes[note_pos - 1] = 19;
									tab_vec.at(current_tab + tab_offset).removefromm_filtered_notes((char)(note_pos + 97));
								}
							}
							else if (currently_filtered_notes[note_pos] == 19)
							{
								currently_filtered_notes[note_pos] = note_pos;
								tab_vec.at(current_tab + tab_offset).addontom_filtered_notes((char)(note_pos + 97));
							}
							else
							{
								currently_filtered_notes[note_pos] = 19;
								tab_vec.at(current_tab + tab_offset).removefromm_filtered_notes((char)(note_pos + 97));
							}
						}
					}
					if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
						return -1;

					note_pos = temp_note_pos;
				}
					break;

				case sf::Keyboard::I://insert: hover over where you want to insert and press 'I', will insert either nothing (thus moving all notes over 1) or copied selection 
					break;

				case sf::Keyboard::N: case sf::Keyboard::T://new tab
					if (ctrl)
					{
					ctrlN_or_ctrlT:

						//tab_vec.at(current_tab + tab_offset).setcurrent_x(current_x);//do this in the future
						//tab_vec.at(current_tab + tab_offset).setcurrent_y(current_y);
						tab_vec.push_back(new_tab);
						if (tab_text_vec.size() + 1 <= 16)
						{
							tab_text_vec.push_back(new_tab_text);
							int temp_x = (tab_text_vec.size() - 1) / 4;
							int temp_y = (tab_text_vec.size() - 1) % 4;
							tab_text_vec.at(tab_text_vec.size() - 1).setOrigin(-700 - 100 * temp_x, -50 - 20 * temp_y);
							if (current_y > 50)
								tab_text_vec.at(tab_text_vec.size() - 1).setPosition(0, current_y - 50);
							current_tab = tab_text_vec.size() - 1;
							rectangle0.setOrigin(sf::Vector2f(tab_text_vec.at(current_tab).getOrigin().x, tab_text_vec.at(current_tab).getOrigin().y));
							//title.setString(new_tab_text.getString());
							//title.setPosition(sf::Vector2f(928 - title.getGlobalBounds().width, 10));
							//save_as.setString(directory.getString() + "\\" + title.getString());
							menu2_loadtabchanges(new_tab, directory, title, save_as, current_x, current_y, note_pos, currently_filtered_notes);
							if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
								return -1;
						}
					}
					break;

				case sf::Keyboard::O://open/load file//NEED SAVE PROMPT BEFORE OPENING
					if (ctrl)
					{
						ctrl = false;//need this because will not detect if ctrl released when window opened
						helperwindowstool.menu_1loadwindow(directory_string, font, position, title);
						save_as.setString(directory.getString() + "\\" + title.getString());
						tab_text_vec.at(current_tab).setString(title.getString());
						tab_vec.at(current_tab + tab_offset).setm_filename(title.getString());
						tab_vec.at(current_tab + tab_offset).populatecomposition(directory.getString());
					}
					break;

				case sf::Keyboard::R://region selector and replace
					if (shift)//region selector
					{
						menu2_toolselector(note_pos, eraser, region_selector, 2);
					}
					else//replace
					{
						if (!selections_vec.empty())
						{
							//replace whats in the region
							
						}
					}
					break;

				case sf::Keyboard::S://save//check case sf::Keyboard::Down:
					if (ctrl)
					{
						if (shift)
						{

						}
						else
						{
							ctrl = false;//need this because will not detect if ctrl released when window opened
							helperwindowstool.menu_1savewindow(font, position, directory, title, save_as);
							save_as.setString(directory.getString() + "\\" + title.getString());
							tab_text_vec.at(current_tab).setString(title.getString());
							tab_vec.at(current_tab + tab_offset).setm_filename(title.getString());
							tab_vec.at(current_tab + tab_offset).populatecomposition(directory.getString());
						}
					}
					else
						goto Down;
					break;

				case sf::Keyboard::V://paste
					if (ctrl)
					{
						if (!clipboard.empty())
						{
							position = sf::Mouse::getPosition(window);
							sf::Vector2f temp_2f_holder = arrowdown_whichindex(position.x, position.y, current_x, current_y);
							paste_selection(tab_vec.at(current_tab + tab_offset), copypastetool, clipboard, ((temp_2f_holder.y - 154) / 230) * 32 + (temp_2f_holder.x - 106) / 40, clipboardlengths_vec);
							tab_vec.at(current_tab + tab_offset).drawcomposition(true, window);
						}
					}
					break;

				case sf::Keyboard::W://close tab//check case sf::Keyboard::Up:
					if (ctrl)
					{
					ctrlW:

						//note: this code gets a little more abstract than typical
						//save prompt here
						tab_vec.erase(tab_vec.begin() + current_tab + tab_offset);
						if (tab_vec.empty())//insert new tab
						{
							tab_vec.push_back(new_tab);
							tab_text_vec.clear();
							tab_text_vec.push_back(new_tab_text);
							//title.setString(new_tab_text.getString());
							//title.setPosition(sf::Vector2f(928 - title.getGlobalBounds().width, 10));
							//save_as.setString(directory.getString() + "\\" + title.getString());
						}
						else if (tab_vec.size() % 4 == 0 && tab_vec.size() >= 16
							&& (tab_offset + 12) == tab_vec.size())//move tab vec over when erasing last tab in tab_vec that is > 16 and looking at that part
						{
							tab_offset -= 4;
							//other stuff happens
							for (int i = 0; i < tab_text_vec.size(); i++)
							{
								//move to right
								tab_text_vec.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) + 1) * -100, tab_text_vec.at(i).getOrigin().y));
							}
							//erase all that are too far right, i.e. the last four or whatever the last few starting at 12 are
							tab_text_vec.erase(tab_text_vec.begin() + 12, tab_text_vec.end());
							for (int i = 0; i < 4; i++)
							{
								//insert previous 4 tabs' texts at the beginning
								tab_text_vec.insert(tab_text_vec.begin() + i, new_tab_text);
								tab_text_vec.at(i).setString(tab_vec.at(tab_offset + i).getm_filename());
								tab_text_vec.at(i).setOrigin(sf::Vector2f(-700, -50 + (i % 4) * -20));
							}
							if (current_tab == 12)
								current_tab = 15;
							else if (current_tab < 12)
								current_tab += 4;
							rectangle0.setOrigin(sf::Vector2f(tab_text_vec.at(current_tab).getOrigin().x, tab_text_vec.at(current_tab).getOrigin().y));
							//title.setString(tab_text_vec.at(current_tab).getString());
							//title.setPosition(928 - title.getGlobalBounds().width, 10);
							//save_as.setString(directory.getString() + "\\" + title.getString());
						}
						else if (current_tab + tab_offset == tab_vec.size())//iffy: erased last tab and need to move current_tab back one
						{
							tab_text_vec.erase(tab_text_vec.begin() + current_tab);
							current_tab--;
							rectangle0.setOrigin(sf::Vector2f(tab_text_vec.at(current_tab).getOrigin().x, tab_text_vec.at(current_tab).getOrigin().y));
							//title.setString(tab_text_vec.at(current_tab).getString());
							//title.setPosition(928 - title.getGlobalBounds().width, 10);
							//save_as.setString(directory.getString() + "\\" + title.getString());
						}
						else//erase tab that wasn't last so shift everything over//everything meaning all the tabs in tab_vec
						{
							int temp_tab_vec_size;
							if (tab_vec.size() - tab_offset < 16)
								temp_tab_vec_size = tab_vec.size() - tab_offset;
							else
								temp_tab_vec_size = 16;

							for (int i = current_tab; i < temp_tab_vec_size; i++)
							{
								tab_text_vec.at(i).setString(tab_vec.at(tab_offset + i).getm_filename());
							}
							if (tab_vec.size() - tab_offset < 16)
							{
								tab_text_vec.pop_back();
							}
						}
						menu2_loadtabchanges(tab_vec.at(current_tab + tab_offset), directory, title, save_as, current_x, current_y, note_pos, currently_filtered_notes);
						if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
							return -1;
					}
					else
						goto Up;
					break;

				case sf::Keyboard::X://cut
					if (ctrl)
					{
						//copy and delete whats in the region
						if (!selections_vec.empty())
						{
							//copy whats in the region
							define_clipboard(tab_vec.at(current_tab + tab_offset), copypastetool, selections_vec, clipboard, clipboardlengths_vec);
							//delete whats in the region
							remove_selection(tab_vec.at(current_tab + tab_offset), copypastetool, selections_vec);
							tab_vec.at(current_tab + tab_offset).drawcomposition(true, window);
						}
					}
					break;

				case sf::Keyboard::Y://redo
					if (ctrl)
					{

					}
					break;

				case sf::Keyboard::Z://undo
					if (ctrl)
					{
						for (int i = 0; i < selections_vec.size(); i++)//PERSONAL DEBUG HELPER CODE
						{
							std::cout << selections_vec.at(i).index << "," << selections_vec.at(i).staffbeg << "," << selections_vec.at(i).staffend << std::endl;
						}
						define_clipboard(tab_vec.at(current_tab + tab_offset), copypastetool, selections_vec, clipboard, clipboardlengths_vec);
					}
					break;

				case sf::Keyboard::BackSpace: case sf::Keyboard::Delete://delete//with shift modifier, if everything on staff is deleted then move all notes back 1 (i.e. the opposite of insert)
					if (!selections_vec.empty())
					{
						//delete whats in the region
						remove_selection(tab_vec.at(current_tab + tab_offset), copypastetool, selections_vec);
						tab_vec.at(current_tab + tab_offset).drawcomposition(true, window);
					}
					break;

				case sf::Keyboard::Add: case sf::Keyboard::Equal:{//increase volume at cursor position
					position = sf::Mouse::getPosition(window);
					if (position.y + current_y > 130)
					{
						sf::Vector2f temp_2f_holder = arrowdown_whichindex(position.x, position.y, current_x, current_y);
						unsigned int temp_2f_holder_index = ((unsigned)(temp_2f_holder.y - 154) / 230) * 32 + (unsigned)(temp_2f_holder.x - 106) / 40;
						if (!tab_vec.at(current_tab + tab_offset).getm_composition()[temp_2f_holder_index].empty())
						{
							int next_volume_level = (int)tab_vec.at(current_tab + tab_offset).getm_composition()[temp_2f_holder_index].back() - 97 + 1;
							if (next_volume_level < 17)
								tab_vec.at(current_tab + tab_offset).updatecomposition(next_volume_level, temp_2f_holder_index, window);
						}
					}
				}
					break;
				case sf::Keyboard::Subtract: case sf::Keyboard::Dash:{//decrease volume at cursor position
					position = sf::Mouse::getPosition(window);
					if (position.y + current_y > 130)
					{
						sf::Vector2f temp_2f_holder = arrowdown_whichindex(position.x, position.y, current_x, current_y);
						unsigned int temp_2f_holder_index = ((unsigned)(temp_2f_holder.y - 154) / 230) * 32 + (unsigned)(temp_2f_holder.x - 106) / 40;
						if (!tab_vec.at(current_tab + tab_offset).getm_composition()[temp_2f_holder_index].empty())
						{
							int next_volume_level = (int)tab_vec.at(current_tab + tab_offset).getm_composition()[temp_2f_holder_index].back() - 97 - 1;
							if (next_volume_level >= 0)
								tab_vec.at(current_tab + tab_offset).updatecomposition(next_volume_level, temp_2f_holder_index, window);
						}
					}
				}
					break;

				case sf::Keyboard::LControl: case sf::Keyboard::RControl:
					ctrl = true;
					break;
				case sf::Keyboard::LShift: case sf::Keyboard::RShift:
					shift = true;
					break;
				}
				break;

			case sf::Event::KeyReleased:
				switch (event_0.key.code){

				case sf::Keyboard::LControl: case sf::Keyboard::RControl:
					ctrl = false;
					break;
				case sf::Keyboard::LShift: case sf::Keyboard::RShift:
					shift = false;
					break;
				}
				break;

			case sf::Event::MouseButtonPressed:{//for placing notes, region select and moving selections (as well as note/volume slider in the future)

				position = sf::Mouse::getPosition(window);
				int y_offset;
				if (current_y > 50)
					y_offset = 50;
				else
					y_offset = current_y;

				switch (event_0.mouseButton.button)
				{
				case sf::Mouse::Left:
					if (position.y > 130 - y_offset)
					{
						if (!ctrl || !region_selector)
						{
							//check if clicked in any of the rectangles; if so then MOVE all selections
							if (!rectangle_vec.empty() && !drag && !fade_rectangle_vec)
							{
								for (int i = 0; i < rectangle_vec.size(); i++)
								{
									if (unsigned(position.x + current_x + rectangle_vec.at(i).getOrigin().x) <= unsigned(rectangle_vec.at(i).getGlobalBounds().width) && 
										unsigned(position.y + current_y + rectangle_vec.at(i).getOrigin().y) <= unsigned(rectangle_vec.at(i).getGlobalBounds().height))
									{
										drag = true;
										i = rectangle_vec.size();
									}
								}
							}
							if (!drag)
							{
								fade_rectangle_vec = false;
								std::vector<sf::RectangleShape> rectangle_vec_trash;
								rectangle_vec.swap(rectangle_vec_trash);
								std::vector<selectionStruct> selections_vec_trash;
								selections_vec.swap(selections_vec_trash);

								if (eraser)
								{
									if ((unsigned)(position.y + current_y - 130) % 230 < 187)
										goto RightClick_Erase;
								}
								else if (!region_selector)
								{
									int note_or_volume = (unsigned)(position.y + current_y - 130) % 230;
									if (note_or_volume < 187 && note_pos < 19)//place a note
									{
										sf::Vector2f temp_2f_holder = arrowdown_whichindex(position.x, position.y, current_x, current_y);
										if (shift)//sharp
											tab_vec.at(current_tab + tab_offset).updatecomposition((char)((((unsigned)arrowright_whichindex(position.y, current_y) - 143) % 230) / 10 + 97),
											((temp_2f_holder.y - 154) / 230) * 32 + (temp_2f_holder.x - 106) / 40, false, '#', window);
										else if (ctrl)//flat
											tab_vec.at(current_tab + tab_offset).updatecomposition((char)((((unsigned)arrowright_whichindex(position.y, current_y) - 143) % 230) / 10 + 97),
											((temp_2f_holder.y - 154) / 230) * 32 + (temp_2f_holder.x - 106) / 40, false, ';', window);
										else
											tab_vec.at(current_tab + tab_offset).updatecomposition((char)((((unsigned)arrowright_whichindex(position.y, current_y) - 143) % 230) / 10 + 97),
											((temp_2f_holder.y - 154) / 230) * 32 + (temp_2f_holder.x - 106) / 40, false, 'n', window);
									}
									else if (note_or_volume > 186)//adjust volume
									{
										note_or_volume = 229 - note_or_volume - 2;//now it is between 0 and 40
										sf::Vector2f temp_2f_holder = arrowdown_whichindex(position.x, position.y, current_x, current_y);
										if (note_or_volume < 2)
											tab_vec.at(current_tab + tab_offset).updatecomposition(0, ((temp_2f_holder.y - 154) / 230) * 32 + (temp_2f_holder.x - 106) / 40, window);
										else
										{
											double volume_approx = (note_or_volume * 16) / 40;//find approximately what the volume level is
											if (volume_approx - (int)volume_approx < 0.5)//round the number to find exact level
												note_or_volume = (int)volume_approx;
											else
												note_or_volume = (int)volume_approx + 1;
											tab_vec.at(current_tab + tab_offset).updatecomposition(note_or_volume, ((temp_2f_holder.y - 154) / 230) * 32 + (temp_2f_holder.x - 106) / 40, window);
										}
									}
								}
							}
						}
						if (region_selector && !drag)
						{
							if (fade_rectangle_vec)
							{
								fade_rectangle_vec = false;
								std::vector<sf::RectangleShape> rectangle_vec_trash;
								rectangle_vec.swap(rectangle_vec_trash);
								std::vector<selectionStruct> selections_vec_trash;
								selections_vec.swap(selections_vec_trash);
							}
							if (!begin_selecting)
							{
								begin_selecting = true;
								//selecting additional regions
								rectangle_vec.push_back(new_rectangle);
								rectangle_vec.back().setOrigin(-position.x - current_x, -position.y - current_y);
							}
						}
					}
					break;

				case sf::Mouse::Right:{//for erasing with right click

				RightClick_Erase:
					sf::Vector2f temp_2f_holder = arrowdown_whichindex(position.x, position.y, current_x, current_y);
					unsigned int temp_2f_holder_x = temp_2f_holder.x; unsigned int temp_2f_holder_y = temp_2f_holder.y;
					tab_vec.at(current_tab + tab_offset).updatecomposition((char)((((unsigned)arrowright_whichindex(position.y, current_y) - 143) % 230) / 10 + 97),
						((temp_2f_holder_y - 154) / 230) * 32 + (temp_2f_holder_x - 106) / 40, true, 'n', window);
				}
					break;
				}
			}
				break;

			case sf::Event::MouseButtonReleased:{//turns out this way is slower->//should probably use a switch statement in here instead of if and else-if

				position = sf::Mouse::getPosition(window);
				int y_offset;
				if (current_y > 50)
					y_offset = 50;
				else
					y_offset = current_y;

				switch (event_0.mouseButton.button){

				case sf::Mouse::Left:
					if (region_selector && begin_selecting)
					{
						begin_selecting = false;
						//defining additional regions' heights and widths
						rectangle_vec.back().setSize(sf::Vector2f(position.x + current_x + rectangle_vec.back().getOrigin().x, position.y + current_y + rectangle_vec.back().getOrigin().y));						
						//mirroring the region if it is drawn backwards
						if (position.x + current_x < -rectangle_vec.back().getOrigin().x)
						{
							rectangle_vec.back().setOrigin(sf::Vector2f(-position.x - current_x, rectangle_vec.back().getOrigin().y));
							rectangle_vec.back().setSize(sf::Vector2f(-rectangle_vec.back().getSize().x, rectangle_vec.back().getSize().y));
						}
						if (position.y + current_y < -rectangle_vec.back().getOrigin().y)
						{
							rectangle_vec.back().setOrigin(sf::Vector2f(rectangle_vec.back().getOrigin().x, -position.y - current_y));
							rectangle_vec.back().setSize(sf::Vector2f(rectangle_vec.back().getSize().x, -rectangle_vec.back().getSize().y));
						}
						//putting region information into selections_vec
						std::vector<int> temp_bounds_holder = region_whichbounds(rectangle_vec.back());
						int sel_vec_index = 0;
						if (temp_bounds_holder.at(0) <= temp_bounds_holder.at(1) && temp_bounds_holder.at(2) <= temp_bounds_holder.at(3))//if there is anything in the region at all
						{
							for (unsigned int i = 0; i < temp_bounds_holder.size(); i += 4)//every 4th is the beginning index information
							{
								for (unsigned int j = temp_bounds_holder.at(i); j <= temp_bounds_holder.at(i + 1); ++j)//lower index to upper index
								{
									bool merge = false;
									for (sel_vec_index; sel_vec_index < selections_vec.size(); ++sel_vec_index)//check if index exists already
									{
										if (j == selections_vec.at(sel_vec_index).index)//index exists, break out of loop
										{
											merge = true;
											break;
										}
										else if (j < selections_vec.at(sel_vec_index).index)//index does not exist, found correct spot for the new index, break out of loop
										{
											--sel_vec_index; if (sel_vec_index < 0) { sel_vec_index = 0; }
											break;
										}
									}
									if (merge)//index exists, check if need to merge the two 
									{
										int merge_occurred_before = -1;//check if previous overlaps with current (would mean merge occurred before) with the sel_vec_index indicating the previous at which merge occurred
										for (sel_vec_index; sel_vec_index < selections_vec.size(); ++sel_vec_index)
										{
											if (selections_vec.at(sel_vec_index).index != j)//further instances of the index do not exist, break out of loop
											{
												--sel_vec_index;
												break;
											}

											if (selections_vec.at(sel_vec_index).staffend + 1 >= temp_bounds_holder.at(i + 2) && selections_vec.at(sel_vec_index).staffbeg - 1 <= temp_bounds_holder.at(i + 3))//check if staff ranges overlap or touch
											{
												if (selections_vec.at(sel_vec_index).staffbeg > temp_bounds_holder.at(i + 2))//if new bound is lower than the current low
													selections_vec.at(sel_vec_index).staffbeg = temp_bounds_holder.at(i + 2);
												if (selections_vec.at(sel_vec_index).staffend < temp_bounds_holder.at(i + 3))//if new bound is higher than the current high
													selections_vec.at(sel_vec_index).staffend = temp_bounds_holder.at(i + 3);

												if (merge_occurred_before != -1)
												{
													if (selections_vec.at(merge_occurred_before).staffbeg > selections_vec.at(sel_vec_index).staffbeg)//if new bound is lower than the current low
														selections_vec.at(merge_occurred_before).staffbeg = selections_vec.at(sel_vec_index).staffbeg;
													if (selections_vec.at(merge_occurred_before).staffend < selections_vec.at(sel_vec_index).staffend)//if new bound is higher than the current high
														selections_vec.at(merge_occurred_before).staffend = selections_vec.at(sel_vec_index).staffend;
													selections_vec.erase(selections_vec.begin() + sel_vec_index);//get rid of current instance of index as a former instance has the current's bounds
													--sel_vec_index;//pretend the current that was just erased never happened
												}
												merge_occurred_before = sel_vec_index;
											}
										}
										if (merge_occurred_before == -1)//ran out of indices equal to new index, did not merge so have to put bounds somewhere, just put it at the appropriate spot
											selections_vec.insert(selections_vec.begin() + sel_vec_index, selectionStruct(j, temp_bounds_holder.at(i + 2), temp_bounds_holder.at(i + 3)));
									}
									else//index does not exist, just put it at the appropriate spot
									{
										if (selections_vec.empty())//first index and its bounds for selections_vec
											selections_vec.push_back(selectionStruct(j, temp_bounds_holder.at(i + 2), temp_bounds_holder.at(i + 3)));
										else
										{
											selections_vec.insert(selections_vec.begin() + sel_vec_index, selectionStruct(j, temp_bounds_holder.at(i + 2), temp_bounds_holder.at(i + 3)));
											++sel_vec_index;
										}
									}
								}
							}
							std::sort(selections_vec.begin(), selections_vec.end());
						}
					}
					else if (drag)
					{
						drag = false;
						int temp_move_x = position.x + current_x + rectangle_vec.at(0).getOrigin().x;//note origin coords are neg, end - start = move amount
						int temp_move_y = position.y + current_y + rectangle_vec.at(0).getOrigin().y;
						for (int i = 0; i < rectangle_vec.size(); ++i)
						{
							rectangle_vec.at(i).setOrigin(sf::Vector2f(rectangle_vec.at(i).getOrigin().x - temp_move_x, rectangle_vec.at(i).getOrigin().y - temp_move_y));
						}
						fade_rectangle_vec = true; 
						fade_clock.restart();
						std::vector<selectionStruct> selections_vec_trash;
						selections_vec.swap(selections_vec_trash);
					}
					else if (unsigned(position.y - 7 + y_offset) <= unsigned(30 - 7))
					{
						if (unsigned(position.x + current_x - 10) <= unsigned(125 - 10))
							helperwindowstool.menu_1savewindow(font, position, directory, title, save_as);
						else if (unsigned(position.x + current_x - 138) <= unsigned(253 - 138))
							helperwindowstool.menu_1loadwindow(directory_string, font, position, title);

						save_as.setString(directory.getString() + "\\" + title.getString());
						tab_text_vec.at(current_tab).setString(title.getString());
						tab_vec.at(current_tab + tab_offset).setm_filename(title.getString());
						tab_vec.at(current_tab + tab_offset).populatecomposition(directory.getString());
					}
					else if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{
						menu2_toolselector(note_pos, eraser, region_selector, 0);//why is this here??...to turn off region_selector or eraser if one of them is on...
						//http://www.cplusplus.com/forum/beginner/4967/
						//https://www.youtube.com/watch?v=ma5QWDYsmBg
						std::cout << menu_2whichnote(position.x + current_x);
						note_pos = (int)menu_2whichnote(position.x + current_x) - 97;//have to set up all sprites onto one image and crop positions in order to give effect of update
						if (note_pos == 23)
							menu2_toolselector(note_pos, eraser, region_selector, 1);
						else if (note_pos == 24)
							menu2_toolselector(note_pos, eraser, region_selector, 2);
						tab_vec.at(current_tab + tab_offset).setcurrent_note((char)(note_pos + 97));
					}
					else if (unsigned(position.y - 50 + y_offset) <= unsigned(130 - 50) &&
						unsigned(position.x + current_x - 700) <= unsigned(1100 - 700))
					{
						if (menu2_whichtab(position.x, position.y, current_x, current_y) < tab_text_vec.size())
							current_tab = menu2_whichtab(position.x, position.y, current_x, current_y);

						rectangle0.setOrigin(sf::Vector2f(tab_text_vec.at(current_tab).getOrigin().x, tab_text_vec.at(current_tab).getOrigin().y));
						//title.setString(tab_text_vec.at(current_tab).getString());
						//title.setPosition(928 - title.getGlobalBounds().width, 10);
						//save_as.setString(directory.getString() + "\\" + title.getString());
						//NOTE NEED TO LOAD ALL CHANGES THAT COME WITH THE SELECTED TAB AS WELL(this will go between window.clear() and window.display())
						menu2_loadtabchanges(tab_vec.at(current_tab + tab_offset), directory, title, save_as, current_x, current_y, note_pos, currently_filtered_notes);
						if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
							return -1;
					}
					else if (unsigned(position.y - 90 + y_offset) <= unsigned(110 - 90))
					{
						if (unsigned(position.x + current_x - 680) <= unsigned(699 - 680))//SEE THE TABS TOWARD THE LEFT
						{
							if (tab_offset > 0)
							{
								tab_offset -= 4;
								//other stuff happens
								for (int i = 0; i < tab_text_vec.size(); ++i)//using unsigned int i screws these loops up for some reason???
								{
									//move to right
									tab_text_vec.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) + 1) * -100, tab_text_vec.at(i).getOrigin().y));
								}
								//erase all that are too far right, i.e. the last four or whatever the last few starting at 12 are
								tab_text_vec.erase(tab_text_vec.begin() + 12, tab_text_vec.end());
								for (int i = 0; i < 4; ++i)
								{
									//insert previous 4 tabs' texts at the beginning
									tab_text_vec.insert(tab_text_vec.begin() + i, new_tab_text);
									tab_text_vec.at(i).setString(tab_vec.at(tab_offset + i).getm_filename());
									tab_text_vec.at(i).setOrigin(sf::Vector2f(-700, -50 + (i % 4) * -20));									
								}
								//move previous 4 tabs to correct positions
								for (int i = 0; i < 4; i++)
								{
									tab_text_vec.at(i).setPosition(sf::Vector2f(0, current_y - y_offset));
								}
								if (current_tab < 12)
									current_tab += 4;
								rectangle0.setOrigin(sf::Vector2f(tab_text_vec.at(current_tab).getOrigin().x, tab_text_vec.at(current_tab).getOrigin().y));

								menu2_loadtabchanges(tab_vec.at(current_tab + tab_offset), directory, title, save_as, current_x, current_y, note_pos, currently_filtered_notes);
								if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
									return -1;
							}
						}
						else if (unsigned(position.x + current_x - 1101) <= unsigned(1120 - 1101))//SEE THE TABS TOWARD THE RIGHT
						{
							if (tab_vec.size() - tab_offset > 16)
							{
								tab_offset += 4;
								for (int i = 0; i < tab_text_vec.size(); ++i)
								{
									//move to left
									tab_text_vec.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) - 1) * -100, tab_text_vec.at(i).getOrigin().y));
								}
								//erase all that are too far left, i.e. the first four
								tab_text_vec.erase(tab_text_vec.begin(), tab_text_vec.begin() + 4);
								//check if at the end of tab_vec and there aren't four tabs to load
								int temp_end = 3;
								if (tab_offset + 16 > tab_vec.size())
									temp_end = tab_vec.size() - 1;

								for (int i = 0; i <= (unsigned)temp_end % 4; ++i)
								{
									//insert (can use push_back here) subsequent 4 tabs' texts at the end (or however many there are toward the end)
									tab_text_vec.push_back(new_tab_text);
									std::cout << tab_offset + i + 12 << "," << tab_vec.size() << "," << tab_offset << std::endl;
									tab_text_vec.at(i + 12).setString(tab_vec.at(tab_offset + i + 12).getm_filename());
									tab_text_vec.at(i + 12).setOrigin(sf::Vector2f(-1000, -50 + (i % 4) * -20));
								}
								//move next 4 tabs to correct positions
								for (int i = 12; i <= 12 + (unsigned)temp_end % 4; i++)
								{
									tab_text_vec.at(i).setPosition(sf::Vector2f(0, current_y - y_offset));
								}
								if (current_tab > 3)
									current_tab -= 4;
								rectangle0.setOrigin(sf::Vector2f(tab_text_vec.at(current_tab).getOrigin().x, tab_text_vec.at(current_tab).getOrigin().y));

								menu2_loadtabchanges(tab_vec.at(current_tab + tab_offset), directory, title, save_as, current_x, current_y, note_pos, currently_filtered_notes);
								if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
									return -1;
							}
						}
						else if (unsigned(position.x + current_x - 1130) <= unsigned(1150 - 1130))//ERASE TAB
						{
							goto ctrlW;
						}
					}
					else if (unsigned(position.x + current_x - 1130) <= unsigned(1150 - 1130) &&
						unsigned(position.y - 50 + y_offset) <= unsigned(70 - 50))//ADD TAB
					{
						goto ctrlN_or_ctrlT;
					}
					break;

				}//<-switch ends here
				std::cout << position.x << "," << position.y << std::endl;
			}
				break;

			case sf::Event::MouseMoved:

				position = sf::Mouse::getPosition(window);

				if (position.y + current_y > 130)
				{
					arrowdown.setPosition(arrowdown_whichindex(position.x, position.y, current_x, current_y));
					arrowright.setPosition(sf::Vector2f(current_x + 48, arrowright_whichindex(position.y, current_y)));
				}
				else
				{
					arrowdown.setPosition(sf::Vector2f(0, 0));
					arrowright.setPosition(sf::Vector2f(0, 0));
				}
				break;

			case sf::Event::MouseWheelMoved:
				if (event_0.mouseWheel.delta < 0)
					goto Down;
				else
					goto Up;
				break;
			}

			//
			//	Move window view accordingly
			//
			view_0.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
			window.setView(view_0);
			//
			//	Move menus accordingly, these operations should only occur after moving the program around so this will need to be changed
			//
			if (menu2_move)
			{
				if (current_y > 50)
				{
					menu_2.setPosition(sf::Vector2f(0, current_y - 50));
					current_note.setPosition(sf::Vector2f(0, current_y - 50));
					filtered_notes.setPosition(sf::Vector2f(0, current_y - 50));
					eraser_on.setPosition(sf::Vector2f(0, current_y - 50));
					region_selector_on.setPosition(sf::Vector2f(0, current_y - 50));
					for (int i = 0; i < tab_text_vec.size(); i++)
					{
						tab_text_vec.at(i).setPosition(sf::Vector2f(0, current_y - 50));
					}
					rectangle0.setPosition(sf::Vector2f(0, current_y - 50));
				}
				else
				{
					menu_2.setPosition(sf::Vector2f(0, 0));
					current_note.setPosition(sf::Vector2f(0, 0));
					filtered_notes.setPosition(sf::Vector2f(0, 0));
					eraser_on.setPosition(sf::Vector2f(0, 0));
					region_selector_on.setPosition(sf::Vector2f(0, 0));
					for (int i = 0; i < tab_text_vec.size(); i++)
					{
						tab_text_vec.at(i).setPosition(sf::Vector2f(0, 0));
					}
					rectangle0.setPosition(sf::Vector2f(0, 0));
				}
				menu2_move = false;
			}
		}
		
		window.clear();

		window.draw(bg);
		tab_vec.at(current_tab + tab_offset).drawcomposition(false, window);
		if (fade_rectangle_vec)
		{	
			int fade_amount = (int)(3 - fade_clock.getElapsedTime().asSeconds()) * (unsigned)128 / 3;
			for (int i = 0; i < rectangle_vec.size(); ++i)
			{
				rectangle_vec.at(i).setFillColor(sf::Color(255, 255, 255, fade_amount));
			}
			if (fade_amount == 0)
			{
				fade_rectangle_vec = false;
				std::vector<sf::RectangleShape> rectangle_vec_trash;
				rectangle_vec.swap(rectangle_vec_trash);
			}
		}
		for (int i = 0; i < rectangle_vec.size(); ++i)
		{
			window.draw(rectangle_vec.at(i));
		}
		window.draw(arrowdown);
		window.draw(arrowright);
		window.draw(menu_1);
		window.draw(menu_2);
		current_note.setTextureRect(sf::IntRect(32 * note_pos, 0, 32, 36));
		window.draw(current_note);
		window.draw(directory);
		window.draw(title);
		window.draw(save_as);

		window.draw(filtered_notes);

		if (eraser)
			window.draw(eraser_on);
		else if (region_selector)
			window.draw(region_selector_on);

		for (int i = 0; i < tab_text_vec.size(); ++i)
		{
			window.draw(tab_text_vec.at(i));
		}
		window.draw(rectangle0);

		window.setFramerateLimit(30);//NOTE: NEED THIS TO REDUCE CPU USAGE; 30 doesnt seem to be much of difference from 60

		window.display();
	}

	return (0);
}