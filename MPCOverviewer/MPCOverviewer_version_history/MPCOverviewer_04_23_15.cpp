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

sf::Vector2f arrow_whichindex(unsigned int mousepositionx, unsigned int mousepositiony, unsigned int current_x, unsigned int current_y){
	
	mousepositionx = mousepositionx + current_x - 75;
	if (mousepositionx < 0)
		mousepositionx = 0;
	else if (mousepositionx > 1279)
		mousepositionx = 1279;

	mousepositiony = mousepositiony + current_y - 130;
	
	sf::Vector2f indexposition((mousepositionx / 40) * 40 + 106, (mousepositiony / 230) * 230 + 154);
	return indexposition;
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
	//	Song files in directory read from MarioPaintSongList.txt and stored in directory_string
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
	//	Eraser, region selector and move toggle variables
	//
	bool eraser = false;
	bool region_selector = false;
	bool begin_selecting = false;
	bool move = false;
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
	//	Arrow image to indicate where to move a selection
	//
	sf::Texture texture6;
	texture6.loadFromFile("sprites/softicons_dot_com_slash_web-icons_slash_marker-style-icons-by-icondock_slash_arrow-down-icon.png");
	sf::Sprite arrow(texture6);
	arrow.setOrigin(sf::Vector2f(24, 24));

	//
	//	Tab list
	//
	std::vector<tab> tab_list;
	tab new_tab = tab();
	tab_list.push_back(new_tab);
	int current_tab = 0;
	int tab_offset = 0;
	std::vector<sf::Text> tab_text_list;
	tab_text_list.reserve(16);
	sf::Text new_tab_text;
	new_tab_text.setFont(font);
	new_tab_text.setColor(sf::Color::Black);
	new_tab_text.setCharacterSize(10);
	new_tab_text.setOrigin(sf::Vector2f(-700, -50));
	new_tab_text.setString("NewSong]MarioPaint");
	tab_text_list.push_back(new_tab_text);
	sf::RectangleShape rectangle0(sf::Vector2f(100, 20));
	rectangle0.setFillColor(sf::Color::Transparent);
	rectangle0.setOutlineThickness(2);
	rectangle0.setOutlineColor(sf::Color::Green);
	rectangle0.setOrigin(-700, -50);

	//
	//	Rectangle list for selecting multiple regions
	//
	std::vector<sf::RectangleShape> rectangle_list;
	sf::RectangleShape new_rectangle(sf::Vector2f(0, 0));
	new_rectangle.setFillColor(sf::Color(255, 255, 255, 128));
	new_rectangle.setOutlineThickness(1);
	new_rectangle.setOutlineColor(sf::Color::Red);
	new_rectangle.setOrigin(0, 0);

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
					if (current_y - 100 < 0)
						current_y = 0;
					else
						current_y -= 100;

					menu2_move = true;
					break;
				case sf::Keyboard::Down:
					if (current_y + 100 + window.getSize().y > 2892)
						current_y = 2892 - window.getSize().y;
					else
						current_y += 100;

					menu2_move = true;
					break;

				case sf::Keyboard::C://copy
					if (ctrl)
					{
						if (!rectangle_list.empty())
						{
							//copy whats in the region
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
									currently_filtered_notes[note_pos + 1] = note_pos;
								else
									currently_filtered_notes[note_pos + 1] = 19;
							}
							else if (note_pos == 16)
							{
								if (currently_filtered_notes[note_pos - 1] == 19)
									currently_filtered_notes[note_pos - 1] = note_pos;
								else
									currently_filtered_notes[note_pos - 1] = 19;
							}
							else if (currently_filtered_notes[note_pos] == 19)
								currently_filtered_notes[note_pos] = note_pos;
							else
								currently_filtered_notes[note_pos] = 19;
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
						tab_list.push_back(new_tab);
						if (tab_text_list.size() + 1 <= 16)
						{
							tab_text_list.push_back(new_tab_text);
							int temp_x = (tab_text_list.size() - 1) / 4;
							int temp_y = (tab_text_list.size() - 1) % 4;
							tab_text_list.at(tab_text_list.size() - 1).setOrigin(-700 - 100 * temp_x, -50 - 20 * temp_y);
							current_tab = tab_text_list.size() - 1;
							rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
							title.setString(new_tab_text.getString());
							title.setPosition(sf::Vector2f(928 - title.getGlobalBounds().width, 10));
							save_as.setString(directory.getString() + "\\" + title.getString());
						}
					}
					break;

				case sf::Keyboard::O://open/load file//NEED SAVE PROMPT BEFORE OPENING
					if (ctrl)
					{
						ctrl = false;//need this because will not detect if ctrl released when window opened
						helperwindowstool.menu_1loadwindow(directory_string, font, position, title);
						save_as.setString(directory.getString() + "\\" + title.getString());
						tab_text_list.at(current_tab).setString(title.getString());
						tab_list.at(current_tab + tab_offset).setm_filename(title.getString());
						tab_list.at(current_tab + tab_offset).populatecomposition(directory.getString());
					}
					break;

				case sf::Keyboard::R://region selector and replace
					if (shift)//region selector
					{
						menu2_toolselector(note_pos, eraser, region_selector, 2);
					}
					else//replace
					{
						if (!rectangle_list.empty())
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
							tab_text_list.at(current_tab).setString(title.getString());
							tab_list.at(current_tab + tab_offset).setm_filename(title.getString());
							tab_list.at(current_tab + tab_offset).populatecomposition(directory.getString());
						}
					}
					else
					{
						if (current_y + 100 + window.getSize().y > 2892)
							current_y = 2892 - window.getSize().y;
						else
							current_y += 100;

						menu2_move = true;
					}
					break;

				case sf::Keyboard::V://paste
					if (ctrl)
					{

					}
					break;

				case sf::Keyboard::W://close tab//check case sf::Keyboard::Up:
					if (ctrl)
					{
						//note: this code gets a little more abstract than typical
						//save prompt here
						tab_list.erase(tab_list.begin() + current_tab + tab_offset);
						if (tab_list.empty())//insert new tab
						{
							tab_list.push_back(new_tab);
							tab_text_list.clear();
							tab_text_list.push_back(new_tab_text);
							title.setString(new_tab_text.getString());
							title.setPosition(sf::Vector2f(928 - title.getGlobalBounds().width, 10));
							save_as.setString(directory.getString() + "\\" + title.getString());
						}
						else if (tab_list.size() % 4 == 0 && tab_list.size() >= 16
							&& (tab_offset + 12) == tab_list.size())//move tab list over when erasing last tab in tab_list that is > 16 and looking at that part
						{
							tab_offset -= 4;
							//other stuff happens
							for (int i = 0; i < tab_text_list.size(); i++)
							{
								//move to right
								tab_text_list.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) + 1) * -100, tab_text_list.at(i).getOrigin().y));
							}
							//erase all that are too far right, i.e. the last four or whatever the last few starting at 12 are
							tab_text_list.erase(tab_text_list.begin() + 12, tab_text_list.end());
							for (int i = 0; i < 4; i++)
							{
								//insert previous 4 tabs' texts at the beginning
								tab_text_list.insert(tab_text_list.begin() + i, new_tab_text);
								tab_text_list.at(i).setString(tab_list.at(tab_offset + i).getm_filename());
								tab_text_list.at(i).setOrigin(sf::Vector2f(-700, -50 + (i % 4) * -20));
							}
							if (current_tab == 12)
								current_tab = 15;
							else if (current_tab < 12)
								current_tab += 4;
							rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
							title.setString(tab_text_list.at(current_tab).getString());
							title.setPosition(928 - title.getGlobalBounds().width, 10);
							save_as.setString(directory.getString() + "\\" + title.getString());
						}
						else if (current_tab + tab_offset == tab_list.size())//iffy: erased last tab and need to move current_tab back one
						{
							tab_text_list.erase(tab_text_list.begin() + current_tab);
							current_tab--;
							rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
							title.setString(tab_text_list.at(current_tab).getString());
							title.setPosition(928 - title.getGlobalBounds().width, 10);
							save_as.setString(directory.getString() + "\\" + title.getString());
						}
						else//erase tab that wasn't last so shift everything over
						{
							int temp_tab_list_size;
							if (tab_list.size() - tab_offset < 16)
								temp_tab_list_size = tab_list.size() - tab_offset;
							else
								temp_tab_list_size = 16;

							for (int i = current_tab; i < temp_tab_list_size; i++)
							{
								tab_text_list.at(i).setString(tab_list.at(tab_offset + i).getm_filename());
							}
							if (tab_list.size() - tab_offset < 16)
							{
								tab_text_list.pop_back();
							}
						}
					}
					else
					{
						if (current_y - 100 < 0)
							current_y = 0;
						else
							current_y -= 100;

						menu2_move = true;
					}
					break;

				case sf::Keyboard::X://cut
					if (ctrl)
					{
						if (!rectangle_list.empty())
						{
							//copy and delete whats in the region
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

					}
					break;

				case sf::Keyboard::BackSpace: case sf::Keyboard::Delete://delete//with shift modifier, if everything on staff is deleted then move all notes back 1 (i.e. the opposite of insert)
					if (!rectangle_list.empty())
					{
						//delete whats in the region
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
							if (!rectangle_list.empty() && !move)
							{
								for (int i = 0; i < rectangle_list.size(); i++)
								{
									if (-rectangle_list.at(i).getOrigin().x <= position.x + current_x && position.x + current_x <= -rectangle_list.at(i).getOrigin().x + rectangle_list.at(i).getGlobalBounds().width
										&& -rectangle_list.at(i).getOrigin().y <= position.y + current_y && position.y + current_y <= -rectangle_list.at(i).getOrigin().y + rectangle_list.at(i).getGlobalBounds().height)
									{
										move = true;
										i = rectangle_list.size();
									}
								}
							}
							if (!move)
							{
								std::vector<sf::RectangleShape> rectangle_list_trash;
								rectangle_list.swap(rectangle_list_trash);
							}
						}
						if (region_selector && !move)
						{
							if (begin_selecting == false)
							{
								begin_selecting = true;

								//selecting additional regions
								rectangle_list.push_back(new_rectangle);
								rectangle_list.back().setOrigin(-position.x - current_x, -position.y - current_y);
							}
						}
					}
					break;

				case sf::Mouse::Right://for erasing with right click
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
						rectangle_list.back().setSize(sf::Vector2f(position.x + current_x + rectangle_list.back().getOrigin().x, position.y + current_y + rectangle_list.back().getOrigin().y));						
					}
					else if (move)
					{
						move = false;
						int temp_move_x = position.x + current_x + rectangle_list.at(0).getOrigin().x;//note origin are coords are neg, end - start = move amount
						int temp_move_y = position.y + current_y + rectangle_list.at(0).getOrigin().y;
						for (int i = 0; i < rectangle_list.size(); i++)
						{
							rectangle_list.at(i).setOrigin(sf::Vector2f(rectangle_list.at(i).getOrigin().x - temp_move_x, rectangle_list.at(i).getOrigin().y - temp_move_y));
						}
					}
					else if (unsigned(position.y - 7 + y_offset) <= unsigned(30 - 7))
					{
						if (10 <= position.x + current_x && position.x + current_x <= 125)
							helperwindowstool.menu_1savewindow(font, position, directory, title, save_as);
						else if (138 <= position.x + current_x && position.x + current_x <= 253)
							helperwindowstool.menu_1loadwindow(directory_string, font, position, title);

						save_as.setString(directory.getString() + "\\" + title.getString());
						tab_text_list.at(current_tab).setString(title.getString());
						tab_list.at(current_tab + tab_offset).setm_filename(title.getString());
						tab_list.at(current_tab + tab_offset).populatecomposition(directory.getString());
					}
					else if (unsigned(position.y - 55 + y_offset) <= unsigned(82 - 55) && position.x + current_x < 680)
					{
						menu2_toolselector(note_pos, eraser, region_selector, 0);
						//http://www.cplusplus.com/forum/beginner/4967/
						//https://www.youtube.com/watch?v=ma5QWDYsmBg
						std::cout << menu_2whichnote(position.x + current_x);
						note_pos = (int)menu_2whichnote(position.x + current_x) - 97;//have to set up all sprites onto one image and crop positions in order to give effect of update
						if (note_pos == 23)
							menu2_toolselector(note_pos, eraser, region_selector, 1);
						else if (note_pos == 24)
							menu2_toolselector(note_pos, eraser, region_selector, 2);
					}
					else if (unsigned(position.y - 50 + y_offset) <= unsigned(130 - 50) &&
						unsigned(position.x + current_x - 700) <= unsigned(1100 - 700))
					{
						if (menu2_whichtab(position.x, position.y, current_x, current_y) < tab_text_list.size())
							current_tab = menu2_whichtab(position.x, position.y, current_x, current_y);

						rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
						title.setString(tab_text_list.at(current_tab).getString());
						title.setPosition(928 - title.getGlobalBounds().width, 10);
						save_as.setString(directory.getString() + "\\" + title.getString());
						//NOTE NEED TO LOAD ALL CHANGES THAT COME WITH THE SELECTED TAB AS WELL(this will go between window.clear() and window.display())
					}
					else if (unsigned(position.y - 90 + y_offset) <= unsigned(110 - 90))
					{
						if (unsigned(position.x + current_x - 680) <= unsigned(699 - 680))
						{
							if (tab_offset > 0)
							{
								tab_offset -= 4;
								//other stuff happens
								for (unsigned int i = 0; i < tab_text_list.size(); ++i)
								{
									//move to right
									tab_text_list.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) + 1) * -100, tab_text_list.at(i).getOrigin().y));
								}
								//erase all that are too far right, i.e. the last four or whatever the last few starting at 12 are
								tab_text_list.erase(tab_text_list.begin() + 12, tab_text_list.end());
								for (unsigned int i = 0; i < 4; ++i)
								{
									//insert previous 4 tabs' texts at the beginning
									tab_text_list.insert(tab_text_list.begin() + i, new_tab_text);
									tab_text_list.at(i).setString(tab_list.at(tab_offset + i).getm_filename());
									tab_text_list.at(i).setOrigin(sf::Vector2f(-700, -50 + (i % 4) * -20));
								}
								if (current_tab < 12)
									current_tab += 4;
								rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
							}
						}
						else if (unsigned(position.x + current_x - 1101) <= unsigned(1120 - 1101))
						{
							if (tab_list.size() - tab_offset > 16)
							{
								tab_offset += 4;
								for (unsigned int i = 0; i < tab_text_list.size(); ++i)
								{
									//move to left
									tab_text_list.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) - 1) * -100, tab_text_list.at(i).getOrigin().y));
								}
								//erase all that are too far left, i.e. the first four
								tab_text_list.erase(tab_text_list.begin(), tab_text_list.begin() + 4);
								//check if at the end of tab_list and there aren't four tabs to load
								int temp_end = 3;
								if (tab_offset + 16 > tab_list.size())
									temp_end = tab_list.size() - 1;

								for (unsigned int i = 0; i <= temp_end % 4; ++i)
								{
									//insert (can use push_back here) subsequent 4 tabs' texts at the end (or however many there are toward the end)
									tab_text_list.push_back(new_tab_text);
									std::cout << tab_offset + i + 12 << "," << tab_list.size() << "," << tab_offset << std::endl;
									tab_text_list.at(i + 12).setString(tab_list.at(tab_offset + i + 12).getm_filename());
									tab_text_list.at(i + 12).setOrigin(sf::Vector2f(-1000, -50 + (i % 4) * -20));
								}
								if (current_tab > 3)
									current_tab -= 4;
								rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
							}
						}
						else if (unsigned(position.x + current_x - 1130) <= unsigned(1150 - 1130))//ERASE TAB
						{
							//note: this code gets a little more abstract than typical
							//save prompt here
							tab_list.erase(tab_list.begin() + current_tab + tab_offset);
							if (tab_list.empty())//insert new tab
							{
								tab_list.push_back(new_tab);
								tab_text_list.clear();
								tab_text_list.push_back(new_tab_text);
								title.setString(new_tab_text.getString());
								title.setPosition(sf::Vector2f(928 - title.getGlobalBounds().width, 10));
								save_as.setString(directory.getString() + "\\" + title.getString());
							}
							else if (tab_list.size() % 4 == 0 && tab_list.size() >= 16
								&& (tab_offset + 12) == tab_list.size())//move tab list over when erasing last tab in tab_list that is > 16 and looking at that part
							{
								tab_offset -= 4;
								//other stuff happens
								for (unsigned int i = 0; i < tab_text_list.size(); ++i)
								{
									//move to right
									tab_text_list.at(i).setOrigin(sf::Vector2f(-700 + ((i / 4) + 1) * -100, tab_text_list.at(i).getOrigin().y));
								}
								//erase all that are too far right, i.e. the last four or whatever the last few starting at 12 are
								tab_text_list.erase(tab_text_list.begin() + 12, tab_text_list.end());
								for (unsigned int i = 0; i < 4; ++i)
								{
									//insert previous 4 tabs' texts at the beginning
									tab_text_list.insert(tab_text_list.begin() + i, new_tab_text);
									tab_text_list.at(i).setString(tab_list.at(tab_offset + i).getm_filename());
									tab_text_list.at(i).setOrigin(sf::Vector2f(-700, -50 + (i % 4) * -20));
								}
								if (current_tab == 12)
									current_tab = 15;
								else if (current_tab < 12)
									current_tab += 4;
								rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
								title.setString(tab_text_list.at(current_tab).getString());
								title.setPosition(928 - title.getGlobalBounds().width, 10);
								save_as.setString(directory.getString() + "\\" + title.getString());
							}
							else if (current_tab + tab_offset == tab_list.size())//iffy: erased last tab and need to move current_tab back one
							{
								tab_text_list.erase(tab_text_list.begin() + current_tab);
								current_tab--;
								rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
								title.setString(tab_text_list.at(current_tab).getString());
								title.setPosition(928 - title.getGlobalBounds().width, 10);
								save_as.setString(directory.getString() + "\\" + title.getString());
							}
							else//erase tab that wasn't last so shift everything over
							{
								int temp_tab_list_size;
								if (tab_list.size() - tab_offset < 16)
									temp_tab_list_size = tab_list.size() - tab_offset;
								else
									temp_tab_list_size = 16;

								for (int i = current_tab; i < temp_tab_list_size; i++)
								{
									tab_text_list.at(i).setString(tab_list.at(tab_offset + i).getm_filename());
								}
								if (tab_list.size() - tab_offset < 16)
								{
									tab_text_list.pop_back();
								}
							}
						}
					}
					else if (unsigned(position.x + current_x - 1130) <= unsigned(1150 - 1130) &&
						unsigned(position.y - 50 + y_offset) <= unsigned(70 - 50))//ADD TAB
					{
						tab_list.push_back(new_tab);
						if (tab_text_list.size() + 1 <= 16)
						{
							tab_text_list.push_back(new_tab_text);
							int temp_x = (tab_text_list.size() - 1) / 4;
							int temp_y = (tab_text_list.size() - 1) % 4;
							tab_text_list.at(tab_text_list.size() - 1).setOrigin(-700 - 100 * temp_x, -50 - 20 * temp_y);
							current_tab = tab_text_list.size() - 1;
							rectangle0.setOrigin(sf::Vector2f(tab_text_list.at(current_tab).getOrigin().x, tab_text_list.at(current_tab).getOrigin().y));
							title.setString(new_tab_text.getString());
							title.setPosition(sf::Vector2f(928 - title.getGlobalBounds().width, 10));
							save_as.setString(directory.getString() + "\\" + title.getString());
						}
					}
					break;

				}//<-switch ends here
				std::cout << position.x << "," << position.y << std::endl;
			}
				break;

			case sf::Event::MouseMoved:

				position = sf::Mouse::getPosition(window);

				if (move)
				{
					arrow.setPosition(arrow_whichindex(position.x, position.y, current_x, current_y));
				}
				else if (!move)
				{
					arrow.setPosition(sf::Vector2f(0, 0));
				}
				break;

			case sf::Event::MouseWheelMoved:
				if (event_0.mouseWheel.delta < 0)
				{
					if (current_y + 100 + window.getSize().y > 2892)
						current_y = 2892 - window.getSize().y;
					else
						current_y += 100;
				}
				else
				{
					if (current_y - 100 < 0)
						current_y = 0;
					else
						current_y -= 100;
				}
				menu2_move = true;
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
					for (int i = 0; i < tab_text_list.size(); i++)
					{
						tab_text_list.at(i).setPosition(sf::Vector2f(0, current_y - 50));
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
					for (int i = 0; i < tab_text_list.size(); i++)
					{
						tab_text_list.at(i).setPosition(sf::Vector2f(0, 0));
					}
					rectangle0.setPosition(sf::Vector2f(0, 0));
				}
				menu2_move = false;
			}
		}
		
		window.clear();

		window.draw(bg);
		tab_list.at(current_tab + tab_offset).drawcomposition(false, window);
		for (int i = 0; i < rectangle_list.size(); ++i)
		{
			window.draw(rectangle_list.at(i));
		}
		window.draw(arrow);
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

		for (int i = 0; i < tab_text_list.size(); ++i)
		{
			window.draw(tab_text_list.at(i));
		}
		window.draw(rectangle0);

		window.setFramerateLimit(30);//NOTE: NEED THIS TO REDUCE CPU USAGE; 30 doesnt seem to be much of difference from 60

		window.display();
	}
	return (0);
}