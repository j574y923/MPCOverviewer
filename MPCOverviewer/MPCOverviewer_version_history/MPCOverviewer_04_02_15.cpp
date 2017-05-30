// MPCOverviewer.cpp : Defines the entry point for the console application.
//
#include "SFML/Graphics.hpp"

#include "copypaste.h"
#include "tab.h"
#include "tchar.h"
#include "TileMap.hpp"
#include "undovector.h"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <vector>

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

char menu_2whichnote(int mouseposition){

	if (52 <= mouseposition && mouseposition <= 581)
	{
		if (472 <= mouseposition && mouseposition <= 497)//pirahna
		{
			return 'q';
		}
		if (500 <= mouseposition && mouseposition <= 525)//coin
		{
			return 'p';
		}
		if (((mouseposition - 52) - ((mouseposition - 52) / 28) * 2) % 26 != 0 && ((mouseposition - 52) - ((mouseposition - 52) / 28)) % 27 != 0)
		{
			mouseposition = (mouseposition - 52) / 28;//should get an integer between 0 and 18
			return (char)(mouseposition + 97);//add 97 to get ascii values of chars
		}
		else//clicked in the margins between one of the acceptable instrument buttons so return nothing
		{
			return 't';//let t be the indicator for nothing
		}

	}
	else if (593 <= mouseposition && mouseposition <= 622)//eraser
	{
		return 'x';
	}
	else if (635 <= mouseposition && mouseposition <= 664)//region selector
	{
		return 'y';
	}
	else//nothing
	{
		return 't';
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
	sf::RenderWindow window(sf::VideoMode(1200, 750), "MPCOverviewer");
	sf::View view;
	view.reset(sf::FloatRect(0, 0, 1200, 750));
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);

	int current_x = 0;
	int current_y = 0;
	sf::Vector2i position;

	sf::Texture texture0;
	if (!texture0.loadFromFile("bg.png"))
	{
		std::cout << "Error loading bg.png" << std::endl;
	}
	sf::Sprite bg(texture0);
	bg.setOrigin(sf::Vector2f(0, -130));
	sf::Texture texture1;
	if (!texture1.loadFromFile("menu_1.png"))
	{
		std::cout << "Error loading menu_1.png" << std::endl;
	}
	sf::Sprite menu_1(texture1);
	menu_1.setTextureRect(sf::IntRect(0, 0, 1360, 50));
	sf::Texture texture2;
	if (!texture2.loadFromFile("menu_2.png"))
	{
		std::cout << "Error loading menu_2.png" << std::endl;
	}
	sf::Sprite menu_2(texture2);
	menu_2.setOrigin(sf::Vector2f(0, -50));
	menu_2.setTextureRect(sf::IntRect(0, 0, 1360, 80));
	sf::Texture texture3;
	if (!texture3.loadFromFile("sprites/large/instruments_transparent.gif"))
	{
		std::cout << "Error loading sprites/large/instruments_transparent.gif" << std::endl;
	}
	sf::Sprite current_note(texture3);
	current_note.setOrigin(sf::Vector2f(-12, -50));
	int note_pos = 19;

	sf::Font font;
	font.loadFromFile("resources/sansation.ttf");
	sf::Text directory;
	directory.setFont(font);
	directory.setColor(sf::Color::Black);
	directory.setCharacterSize(15);
	directory.setPosition(sf::Vector2f(265, 10));
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
	sf::Text title;
	title.setString("");
	title.setFont(font);
	title.setColor(sf::Color::Black);
	title.setCharacterSize(15);
	title.setPosition(0, 0);

	TileMap filtered_notes;
	int currently_filtered_notes[] = { 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19 };
	if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
	{
		return -1;
	}
	filtered_notes.setOrigin(sf::Vector2f(-54, -91));
	/*for (int i = 0; i < 19; i++)
	{
		std::string instrument_sprite_numerical = std::to_string(i);
		sf::Texture load_instrument_texture;
		load_instrument_texture.loadFromFile("sprites/numerical/" + instrument_sprite_numerical + ".png");
		filtered_note_textures.push_back(load_instrument_texture);
		sf::Sprite loaded_instrument_sprite(filtered_note_textures.at(i));
		filtered_notes.push_back(loaded_instrument_sprite);
		filtered_notes.at(i).setPosition(52 + (28 * i), 91);
	}*/
	//initialize here

	std::vector<tab> tab_list;
	int current_tab = 0;
	std::vector<sf::Text> tab_text_list;

	bool eraser = false;
	bool region_selector = false;

	bool ctrl = false;
	bool shift = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
				
			case sf::Event::Resized:
				if (window.getSize().x > 1360)
				{
					window.setSize(sf::Vector2u(1360, window.getSize().y));
				}
				if (window.getSize().y > 2892)
				{
					window.setSize(sf::Vector2u(window.getSize().x, 2892));
				}
				view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
				window.setView(view);
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Right: case sf::Keyboard::D:
					if (current_x + 100 + window.getSize().x > 1360)
					{
						current_x = 1360 - window.getSize().x;
					}
					else
					{
						current_x += 100;
					}
					view.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
					window.setView(view);
					break;
				case sf::Keyboard::Left: case sf::Keyboard::A:
					if (current_x - 100 < 0)
					{
						current_x = 0;
					}
					else
					{
						current_x -= 100;
					}
					view.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
					window.setView(view);
					break;
				case sf::Keyboard::Up: case sf::Keyboard::W:
					if (current_y - 100 < 0)
					{
						current_y = 0;
					}
					else
					{
						current_y -= 100;
					}
					view.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
					window.setView(view);
					if (current_y > 50)
					{
						menu_2.setPosition(sf::Vector2f(0, current_y - 50));
						current_note.setPosition(sf::Vector2f(0, current_y - 50));
						filtered_notes.setPosition(sf::Vector2f(0, current_y - 50));
					}
					else
					{
						menu_2.setPosition(sf::Vector2f(0, 0));
						current_note.setPosition(sf::Vector2f(0, 0));
						filtered_notes.setPosition(sf::Vector2f(0, 0));
					}
					break;
				case sf::Keyboard::Down: case sf::Keyboard::S:
					if (current_y + 100 + window.getSize().y > 2892)
					{
						current_y = 2892 - window.getSize().y;
					}
					else
					{
						current_y += 100;
					}
					view.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
					window.setView(view);
					if (current_y > 50)
					{
						menu_2.setPosition(sf::Vector2f(0, current_y - 50));
						current_note.setPosition(sf::Vector2f(0, current_y - 50));
						filtered_notes.setPosition(sf::Vector2f(0, current_y - 50));
					}
					else
					{
						menu_2.setPosition(sf::Vector2f(0, 0));
						current_note.setPosition(sf::Vector2f(0, 0));
						filtered_notes.setPosition(sf::Vector2f(0, 0));
					}
					break;

				case sf::Keyboard::C://copy
					if (ctrl)
					{

					}
					break;

				case sf::Keyboard::E://eraser
					note_pos = 19;
					eraser = true;
					break;

				case sf::Keyboard::F:
				{
					position = sf::Mouse::getPosition(window);
					int temp_note_pos = note_pos;
					if (current_y > 50)
					{
						if (5 <= position.y && position.y <= 32 && position.x + current_x < 680)
						{
							note_pos = (int)menu_2whichnote(position.x + current_x) - 97;
							if (note_pos <= 18)
							{
								if (note_pos == 15)
								{
									if (currently_filtered_notes[note_pos + 1] == 19)
									{
										currently_filtered_notes[note_pos + 1] = note_pos;
									}
									else
									{
										currently_filtered_notes[note_pos + 1] = 19;
									}
								}
								else if (note_pos == 16)
								{
									if (currently_filtered_notes[note_pos - 1] == 19)
									{
										currently_filtered_notes[note_pos - 1] = note_pos;
									}
									else
									{
										currently_filtered_notes[note_pos - 1] = 19;
									}
								}
								else if (currently_filtered_notes[note_pos] == 19)
								{
									currently_filtered_notes[note_pos] = note_pos;
								}
								else
								{
									currently_filtered_notes[note_pos] = 19;
								}
							}
						}
					}
					else
					{
						if (55 - current_y <= position.y && position.y <= 82 - current_y && position.x + current_x < 680)
						{
							note_pos = (int)menu_2whichnote(position.x + current_x) - 97;
							if (note_pos <= 18)
							{
								if (note_pos == 15)
								{
									if (currently_filtered_notes[note_pos + 1] == 19)
									{
										currently_filtered_notes[note_pos + 1] = note_pos;
									}
									else
									{
										currently_filtered_notes[note_pos + 1] = 19;
									}
								}
								else if (note_pos == 16)
								{
									if (currently_filtered_notes[note_pos - 1] == 19)
									{
										currently_filtered_notes[note_pos - 1] = note_pos;
									}
									else
									{
										currently_filtered_notes[note_pos - 1] = 19;
									}
								}
								else if (currently_filtered_notes[note_pos] == 19)
								{
									currently_filtered_notes[note_pos] = note_pos;
								}
								else
								{
									currently_filtered_notes[note_pos] = 19;
								}
							}
						}
					}
					if (!filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4))
					{
						return -1;
					}
					note_pos = temp_note_pos;
				}
					break;

				case sf::Keyboard::O://open/load file
					if (ctrl)
					{

					}
					break;

				case sf::Keyboard::R://region selector and replace
					if (shift)//region selector
					{
						note_pos = 19;
						eraser = false;
						region_selector = true;//make a helper method that turns tools on and off
					}
					else//replace
					{

					}
					break;

				//case sf::Keyboard::S://save//check case sf::Keyboard::Down:

				case sf::Keyboard::T://new tab
					if (ctrl)
					{

					}
					break;

				case sf::Keyboard::V://paste
					if (ctrl)
					{

					}
					break;

				case sf::Keyboard::X://cut
					if (ctrl)
					{

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

				case sf::Keyboard::Delete://delete
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
				switch (event.key.code)
				{
				case sf::Keyboard::LControl: case sf::Keyboard::RControl:
					ctrl = false;
					break;

				case sf::Keyboard::LShift: case sf::Keyboard::RShift:
					shift = false;
					break;
				}
				break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(window);
				if (current_y > 50)
				{
					if (5 <= position.y && position.y <= 32 && position.x + current_x < 680)
					{
						//http://www.cplusplus.com/forum/beginner/4967/
						//https://www.youtube.com/watch?v=ma5QWDYsmBg
						std::cout << menu_2whichnote(position.x + current_x);
						note_pos = (int)menu_2whichnote(position.x + current_x) - 97;//have to set up all sprites onto one image and crop positions in order to give effect of update
					}
				}
				else
				{
					if (55 - current_y <= position.y && position.y <= 82 - current_y && position.x + current_x < 680)
					{
						std::cout << menu_2whichnote(position.x + current_x);
						note_pos = (int)menu_2whichnote(position.x + current_x) - 97;
					}
					else if (7 - current_y <= position.y && position.y <= 30 - current_y 
						&& 138 - current_x <= position.x && position.x <= 253 - current_x)
					{
						std::cout << "read list of files in directory" << std::endl;//create new window with list of files
						sf::RenderWindow load_file_window(sf::VideoMode(280, 350), "Load file from MarioPaintSongList", sf::Style::Titlebar | sf::Style::Close);
						std::vector<sf::Text> file_list;
						
						sf::Texture texture4;
						if (!texture4.loadFromFile("load_menu_bg.png"))
						{
							std::cout << "Error loading load_menu_bg.png" << std::endl;
						}
						sf::Sprite load_menu_bg(texture4);
						load_menu_bg.setTextureRect(sf::IntRect(0, 0, 280, 350));
						sf::Texture texture5;
						if (!texture5.loadFromFile("load_menu.png"))
						{
							std::cout << "Error loading load_menu.png" << std::endl;
						}
						sf::Sprite load_menu(texture5);

						int current_y_2 = 0;

						std::string loaded_file = "";

						sf::RectangleShape rectangle0(sf::Vector2f(280, 15));
						rectangle0.setFillColor(sf::Color::Transparent);
						rectangle0.setOutlineThickness(2);
						rectangle0.setOutlineColor(sf::Color::Green);
						rectangle0.setPosition(0, 0);

						try
						{
							//load file from prefs directory			
							std::ifstream infile(directory_string + "/MarioPaintSongList.txt");
							std::string file_contents;
							getline(infile, file_contents);
							std::string temp_store_filename = "";

							std::cout << file_contents << std::endl;//create new window with list of files

							int file_count = 0;
							for (int i = 0; i < file_contents.size(); i++)
							{
								if (file_contents.at(i) == '*')
								{
									sf::Text push_back_filename;
									push_back_filename.setFont(font);
									push_back_filename.setColor(sf::Color::Black);
									push_back_filename.setCharacterSize(15);
									push_back_filename.setPosition(sf::Vector2f(0, 15 * file_count + 30));
									push_back_filename.setString(temp_store_filename);
									file_list.push_back(push_back_filename);
									temp_store_filename = "";

									file_count++;
									if (15 * file_count + 30 > 350)
									{
										load_menu_bg.setTextureRect(sf::IntRect(0, 0, 280, 15 * file_count + 30));
									}
								}
								else
								{
									temp_store_filename = temp_store_filename + file_contents.at(i);
								}
							}

						}
						catch (std::exception & obj)
						{

						}

						while (load_file_window.isOpen())
						{
							sf::Event event_2;

							sf::View view_2;
							view_2.reset(sf::FloatRect(0, 0, 280, 350));
							view_2.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

							while (load_file_window.pollEvent(event_2))
							{
								switch (event_2.type)
								{
								case sf::Event::Closed:
									load_file_window.close();
									break;

								case sf::Event::KeyPressed:
									switch (event_2.key.code)
									{
									case sf::Keyboard::Up: case sf::Keyboard::W:
										if (current_y_2 - 100 < 0)
										{
											current_y_2 = 0;
										}
										else
										{
											current_y_2 -= 100;
										}
										load_menu.setPosition(sf::Vector2f(0, current_y_2));
										view_2.reset(sf::FloatRect(0, current_y_2, 280, 350));
										load_file_window.setView(view_2);
										break;
									case sf::Keyboard::Down: case sf::Keyboard::S:
										if (current_y_2 + 100 + 350 > load_menu_bg.getTextureRect().height)
										{
											current_y_2 = load_menu_bg.getTextureRect().height - 350;
										}
										else
										{
											current_y_2 += 100;
										}
										load_menu.setPosition(sf::Vector2f(0, current_y_2));
										view_2.reset(sf::FloatRect(0, current_y_2, 280, 350));
										load_file_window.setView(view_2);
										break;
									}
									break;

								case sf::Event::MouseButtonReleased:
									position = sf::Mouse::getPosition(load_file_window);
									int which_file;
									if (2 <= position.x && position.x <= 117
										&& 1 <= position.y && position.y <= 24)
									{
										if (!loaded_file.empty())
										{
											title.setString(loaded_file);
											title.setPosition(928 - title.getLocalBounds().width, 10);
											//put into populatecomposition()
										}
									}
									else if (position.y > 30)
									{
										int file_pos = (position.y + current_y_2 - 30) / 15;
										loaded_file = file_list.at(file_pos).getString();
										rectangle0.setPosition(0, file_pos * 15 + 30);
									}
									break;

								case sf::Event::MouseWheelMoved:
									if (event_2.mouseWheel.delta < 0)
									{
										if (current_y_2 + 100 + 350 > load_menu_bg.getTextureRect().height)
										{
											current_y_2 = load_menu_bg.getTextureRect().height - 350;
										}
										else
										{
											current_y_2 += 100;
										}
									}
									else
									{
										if (current_y_2 - 100 < 0)
										{
											current_y_2 = 0;
										}
										else
										{
											current_y_2 -= 100;
										}
									}
									load_menu.setPosition(sf::Vector2f(0, current_y_2));
									view_2.reset(sf::FloatRect(0, current_y_2, 280, 350));
									load_file_window.setView(view_2);
									break;
								}
							}

							load_file_window.draw(load_menu_bg);
							for (int i = 0; i < file_list.size(); i++)
							{
								load_file_window.draw(file_list.at(i));
							}
							load_file_window.draw(rectangle0);
							load_file_window.draw(load_menu);

							load_file_window.setFramerateLimit(15);

							load_file_window.display();
						}
					}
				}
				std::cout << position.x << "," << position.y << std::endl;
				break;

			case sf::Event::MouseWheelMoved:
				if (event.mouseWheel.delta < 0)
				{
					if (current_y + 100 + window.getSize().y > 2892)
					{
						current_y = 2892 - window.getSize().y;
					}
					else
					{
						current_y += 100;
					}
				}
				else
				{
					if (current_y - 100 < 0)
					{
						current_y = 0;
					}
					else
					{
						current_y -= 100;
					}
				}
				view.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
				window.setView(view);
				if (current_y > 50)
				{
					menu_2.setPosition(sf::Vector2f(0, current_y - 50));
					current_note.setPosition(sf::Vector2f(0, current_y - 50));
					filtered_notes.setPosition(sf::Vector2f(0, current_y - 50));
				}
				else
				{
					menu_2.setPosition(sf::Vector2f(0, 0));
					current_note.setPosition(sf::Vector2f(0, 0));
					filtered_notes.setPosition(sf::Vector2f(0, 0));
				}
				break;
			}
		}
		
		window.clear();

		window.draw(bg);
		window.draw(menu_1);
		window.draw(menu_2);
		current_note.setTextureRect(sf::IntRect(32 * note_pos, 0, 32, 36));
		window.draw(current_note);
		window.draw(directory);
		window.draw(title);

		window.draw(filtered_notes);

		window.setFramerateLimit(60);//NOTE: NEED THIS TO REDUCE CPU USAGE

		window.display();
	}
	return (0);
}

