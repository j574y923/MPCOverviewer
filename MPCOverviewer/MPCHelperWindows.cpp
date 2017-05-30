
#include "MPCHelperWindows.h"
#include "tchar.h"
#include <iostream>

void MPCHelperWindows::saveprompt(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as), tab(&tab_to_save), bool (&exit)){

	sf::RenderWindow save_prompt_window(sf::VideoMode(420, 140), "Save work?", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture texture0;
	texture0.loadFromFile("save_prompt_menu.png");
	sf::Sprite save_prompt_bg(texture0);
	save_prompt_bg.setTextureRect(sf::IntRect(0, 0, 420, 140));

	sf::Text save_title;
	save_title.setFont(font);
	save_title.setColor(sf::Color::Black);
	save_title.setCharacterSize(10);
	save_title.setPosition(sf::Vector2f(0, 40));
	save_title.setString(save_as.getString());

	while (save_prompt_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 420, 140));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		while (save_prompt_window.pollEvent(event_1)){

			switch (event_1.type){

			case sf::Event::Closed://cancel
				exit = false;
				save_prompt_window.close();
				break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(save_prompt_window);
				if (70 <= position.y && position.y <= 93)
				{
					if (10 <= position.x && position.x <= 125)//save as
					{
						menu_1savewindow(font, position, directory, title, save_as, false);
						tab_to_save.setm_filename(title.getString());
						tab_to_save.outputcomposition(directory.getString());//NOT DONE?
						exit = true;
						save_prompt_window.close();
					}
					else if (138 <= position.x && position.x <= 253)//dont save
					{
						exit = true;
						save_prompt_window.close();
					}
					else if (267 <= position.x && position.x <= 382)//cancel
					{
						exit = false;
						save_prompt_window.close();
					}
				}
				else if (106 <= position.y && position.y <= 129 && 10 <= position.x && position.x <= 125)//just save
				{
					tab_to_save.outputcomposition(directory.getString()); // NOT DONE?
					exit = true;
					save_prompt_window.close();
				}
				break;
			}

			//
			//	Update name file will save under accordingly
			//
			save_title.setString(save_as.getString());
		}
		save_prompt_window.clear();

		save_prompt_window.draw(save_prompt_bg);
		save_prompt_window.draw(save_title);

		save_prompt_window.setFramerateLimit(15);

		save_prompt_window.display();
	}
}

//note: this does everything it is intended to do but has some incomplete qualities
void MPCHelperWindows::menu_1savewindow(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as), bool rename){

	sf::RenderWindow save_as_window(sf::VideoMode(350, 210), "Save song as...", sf::Style::Titlebar | sf::Style::Close);
	if (rename)
		save_as_window.setTitle("Rename song as...");
	sf::Texture texture0;
	texture0.loadFromFile("load_menu_bg.png");
	sf::Sprite save_as_bg(texture0);
	save_as_bg.setTextureRect(sf::IntRect(0, 0, 350, 210));

	std::string save_under = title.getString();
	std::vector<int> length_of_characters;
	for (int i = 0; i < save_under.size(); i++)
	{
		sf::Text temp_storage_character;
		temp_storage_character.setFont(font);
		temp_storage_character.setColor(sf::Color::Black);
		temp_storage_character.setCharacterSize(15);
		temp_storage_character.setPosition(sf::Vector2f(0, 0));
		temp_storage_character.setString(save_under.at(i));
		length_of_characters.push_back(temp_storage_character.getGlobalBounds().width);
	}
	int title_pos = 0;
	sf::Text new_title;
	new_title.setFont(font);
	new_title.setColor(sf::Color::Black);
	new_title.setCharacterSize(15);
	new_title.setPosition(sf::Vector2f(0, 0));
	new_title.setString(save_under);

	int current_x_2 = 0;

	sf::Text place_cursor;
	place_cursor.setFont(font);
	place_cursor.setColor(sf::Color::Green);
	place_cursor.setCharacterSize(15);
	place_cursor.setPosition(sf::Vector2f(new_title.getGlobalBounds().width, 0));
	place_cursor.setString("|");

	while (save_as_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 350, 210));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		int i = 0;
		int total_length = 0;

		while (save_as_window.pollEvent(event_1)){

			switch (event_1.type){

			case sf::Event::Closed://ALSO NEED TO SAVE
				save_as_window.close();
				break;

			case sf::Event::TextEntered:
				if (32 <= event_1.text.unicode && event_1.text.unicode < 127)
				{
					if (event_1.text.unicode == 34 || event_1.text.unicode == 42 || event_1.text.unicode == 47
						|| event_1.text.unicode == 58 || event_1.text.unicode == 60 || event_1.text.unicode == 62
						|| event_1.text.unicode == 63 || event_1.text.unicode == 92 || event_1.text.unicode == 124)
						break;

					save_under.insert(title_pos, 1, static_cast<char>(event_1.text.unicode));
					sf::Text temp_storage_character;
					temp_storage_character.setFont(font);
					temp_storage_character.setColor(sf::Color::Black);
					temp_storage_character.setCharacterSize(15);
					temp_storage_character.setPosition(sf::Vector2f(0, 0));
					temp_storage_character.setString(save_under.at(title_pos));
					length_of_characters.insert(length_of_characters.begin() + title_pos, temp_storage_character.getGlobalBounds().width);
					title_pos++;
				}
				break;

			case sf::Event::KeyPressed:
				switch (event_1.key.code){

				case sf::Keyboard::Up:
					if (current_x_2 - 100 < 0)
						current_x_2 = 0;
					else
						current_x_2 -= 100;

					view_1.reset(sf::FloatRect(current_x_2, 0, 350, 210));
					save_as_window.setView(view_1);
					break;
				case sf::Keyboard::Down:
					if (current_x_2 + 100 + 350 > save_as_bg.getTextureRect().width)
						current_x_2 = save_as_bg.getTextureRect().width - 350;
					else
						current_x_2 += 100;

					view_1.reset(sf::FloatRect(current_x_2, 0, 350, 210));
					save_as_window.setView(view_1);
					break;
				case sf::Keyboard::Left:
					if (title_pos - 1 >= 0)
					{
						title_pos--;
					}
					break;
				case sf::Keyboard::Right:
					if (title_pos + 1 <= save_under.size())
					{
						title_pos++;
					}
					break;

				case sf::Keyboard::BackSpace:
					if (title_pos - 1 >= 0)
					{
						title_pos--;
						save_under.erase(save_under.begin() + title_pos);
						length_of_characters.erase(length_of_characters.begin() + title_pos);
					}
					break;
				}
				break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(save_as_window);

				i = 0;
				total_length = length_of_characters.at(i);
				if (position.x + current_x_2 > new_title.getGlobalBounds().width)
					title_pos = save_under.size();
				else
				{
					while (total_length < position.x + current_x_2)
					{
						i++;
						total_length = total_length + length_of_characters.at(i);
					}
					title_pos = i;
				}
				break;

			case sf::Event::MouseWheelMoved:
				if (event_1.mouseWheel.delta < 0)
				{
					if (current_x_2 + 100 + 350 > save_as_bg.getTextureRect().width)
						current_x_2 = save_as_bg.getTextureRect().width - 350;
					else
						current_x_2 += 100;
				}
				else
				{
					if (current_x_2 - 100 < 0)
						current_x_2 = 0;
					else
						current_x_2 -= 100;
				}

				view_1.reset(sf::FloatRect(current_x_2, 0, 350, 210));
				save_as_window.setView(view_1);
				break;
			}

			//
			//	Adjust place_cursor
			//
			total_length = 0;
			for (i = 0; i < title_pos; i++)
			{
				total_length = total_length + length_of_characters.at(i);
			}
			place_cursor.setPosition(sf::Vector2f(total_length, 0));
			if (total_length > current_x_2 + 350)
			{
				view_1.reset(sf::FloatRect(total_length - 350 + current_x_2, 0, 350, 210));
				save_as_window.setView(view_1);
			}

			//
			//	Adjust name which file saves as and the window accordingly
			//
			new_title.setString(save_under);
			if (new_title.getGlobalBounds().width > 350)
				save_as_bg.setTextureRect(sf::IntRect(0, 0, new_title.getGlobalBounds().width, 210));
			title.setString(new_title.getString());
			title.setPosition(928 - title.getLocalBounds().width, 10);
			save_as.setString(directory.getString() + "\\" + title.getString());
		}

		save_as_window.clear();

		save_as_window.draw(save_as_bg);
		save_as_window.draw(new_title);

		save_as_window.draw(place_cursor);

		save_as_window.setFramerateLimit(15);

		save_as_window.display();
	}
}

void MPCHelperWindows::menu_1loadwindow(std::string(&directory_string), sf::Font(&font), sf::Vector2i(&position), sf::Text(&title)){

	sf::RenderWindow load_file_window(sf::VideoMode(280, 350), "Load file from MarioPaintSongList", sf::Style::Titlebar | sf::Style::Close);
	std::vector<sf::Text> file_text_list;

	sf::Texture texture0;
	texture0.loadFromFile("load_menu_bg.png");
	sf::Sprite load_menu_bg(texture0);
	load_menu_bg.setTextureRect(sf::IntRect(0, 0, 280, 350));
	sf::Texture texture1;
	texture1.loadFromFile("load_menu.png");
	sf::Sprite load_menu(texture1);

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

		//std::cout << file_contents << std::endl;//create new window with list of files

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
				file_text_list.push_back(push_back_filename);
				temp_store_filename = "";

				file_count++;
				if (15 * file_count + 30 > 350)
					load_menu_bg.setTextureRect(sf::IntRect(0, 0, 280, 15 * file_count + 30));
			}
			else
				temp_store_filename = temp_store_filename + file_contents.at(i);
		}

	}
	catch (std::exception & obj)
	{

	}

	while (load_file_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 280, 350));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		while (load_file_window.pollEvent(event_1)){

			switch (event_1.type){

			case sf::Event::Closed:
				load_file_window.close();
				break;

			case sf::Event::KeyPressed:
				switch (event_1.key.code){

				case sf::Keyboard::Up: case sf::Keyboard::W:
					if (current_y_2 - 100 < 0)
						current_y_2 = 0;
					else
						current_y_2 -= 100;

					load_menu.setPosition(sf::Vector2f(0, current_y_2));
					view_1.reset(sf::FloatRect(0, current_y_2, 280, 350));
					load_file_window.setView(view_1);
					break;
				case sf::Keyboard::Down: case sf::Keyboard::S:
					if (current_y_2 + 100 + 350 > load_menu_bg.getTextureRect().height)
						current_y_2 = load_menu_bg.getTextureRect().height - 350;
					else
						current_y_2 += 100;

					load_menu.setPosition(sf::Vector2f(0, current_y_2));
					view_1.reset(sf::FloatRect(0, current_y_2, 280, 350));
					load_file_window.setView(view_1);
					break;
				}
				break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(load_file_window);

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
					if (file_pos < file_text_list.size())
					{
						loaded_file = file_text_list.at(file_pos).getString();
						rectangle0.setPosition(0, file_pos * 15 + 30);
					}
				}
				break;

			case sf::Event::MouseWheelMoved:
				if (event_1.mouseWheel.delta < 0)
				{
					if (current_y_2 + 100 + 350 > load_menu_bg.getTextureRect().height)
						current_y_2 = load_menu_bg.getTextureRect().height - 350;
					else
						current_y_2 += 100;
				}
				else
				{
					if (current_y_2 - 100 < 0)
						current_y_2 = 0;
					else
						current_y_2 -= 100;
				}

				load_menu.setPosition(sf::Vector2f(0, current_y_2));
				view_1.reset(sf::FloatRect(0, current_y_2, 280, 350));
				load_file_window.setView(view_1);
				break;
			}
		}

		load_file_window.clear();

		load_file_window.draw(load_menu_bg);
		for (int i = 0; i < file_text_list.size(); i++)
		{
			load_file_window.draw(file_text_list.at(i));
		}
		load_file_window.draw(rectangle0);
		load_file_window.draw(load_menu);

		load_file_window.setFramerateLimit(15);

		load_file_window.display();
	}
}

void MPCHelperWindows::menu_2tablist(sf::Font(&font), sf::Vector2i(&position), std::vector<tab>(&tab_list), int(&current_tab)){

	sf::RenderWindow tab_list_window(sf::VideoMode(280, 350), "Load file from MarioPaintSongList", sf::Style::Titlebar | sf::Style::Close);
	std::vector<sf::Text> tab_text_list;
	for (int i = 0; i < tab_list.size(); i++)
	{
		sf::Text push_back_tab_name;
		push_back_tab_name.setFont(font);
		push_back_tab_name.setColor(sf::Color::Black);
		push_back_tab_name.setCharacterSize(15);
		push_back_tab_name.setPosition(sf::Vector2f(0, 15 * i));
		//push_back_tab_name.setString(tab_list.at(i).getfilename());//NOT DONE
		tab_text_list.push_back(push_back_tab_name);
	}

	sf::Texture texture0;
	texture0.loadFromFile("load_menu_bg.png");
	sf::Sprite tab_list_bg(texture0);
	tab_list_bg.setTextureRect(sf::IntRect(0, 0, 280, 350));

	int current_y_2 = 0;

	sf::RectangleShape rectangle0(sf::Vector2f(280, 15));
	rectangle0.setFillColor(sf::Color::Transparent);
	rectangle0.setOutlineThickness(2);
	rectangle0.setOutlineColor(sf::Color::Green);
	rectangle0.setPosition(0, 0);

	while (tab_list_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 280, 350));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		while (tab_list_window.pollEvent(event_1)){

			switch (event_1.type){

			case sf::Event::Closed:
				tab_list_window.close();
				break;

			case sf::Event::KeyPressed:
				switch (event_1.key.code){

				case sf::Keyboard::Up: case sf::Keyboard::W:
					if (current_y_2 - 100 < 0)
						current_y_2 = 0;
					else
						current_y_2 -= 100;

					view_1.reset(sf::FloatRect(0, current_y_2, 280, 350));
					tab_list_window.setView(view_1);
					break;
				case sf::Keyboard::Down: case sf::Keyboard::S:
					if (current_y_2 + 100 + 350 > tab_list_bg.getTextureRect().height)
						current_y_2 = tab_list_bg.getTextureRect().height - 350;
					else
						current_y_2 += 100;

					view_1.reset(sf::FloatRect(0, current_y_2, 280, 350));
					tab_list_window.setView(view_1);
					break;
				}
				break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(tab_list_window);

				current_tab = (position.y + current_y_2) / 15;//CHANGE THIS
				rectangle0.setPosition(0, current_tab * 15);
				break;

			case sf::Event::MouseWheelMoved:
				if (event_1.mouseWheel.delta < 0)
				{
					if (current_y_2 + 100 + 350 > tab_list_bg.getTextureRect().height)
						current_y_2 = tab_list_bg.getTextureRect().height - 350;
					else
						current_y_2 += 100;
				}
				else
				{
					if (current_y_2 - 100 < 0)
						current_y_2 = 0;
					else
						current_y_2 -= 100;
				}

				view_1.reset(sf::FloatRect(0, current_y_2, 280, 350));
				tab_list_window.setView(view_1);
				break;
			}
		}

		tab_list_window.clear();

		tab_list_window.draw(tab_list_bg);
		for (int i = 0; i < tab_text_list.size(); i++)
		{
			tab_list_window.draw(tab_text_list.at(i));
		}
		tab_list_window.draw(rectangle0);
		
		tab_list_window.setFramerateLimit(15);

		tab_list_window.display();
	}
}

//----------------------
//	Replace
//----------------------
char _menu_2whichnote(int mouseposition){

	if (52 <= mouseposition && mouseposition <= 581)
	{
		if (472 <= mouseposition && mouseposition <= 497)
			return 'q';//pirahna
		if (500 <= mouseposition && mouseposition <= 525)
			return 'p';//coin
		if (((mouseposition - 52) - ((mouseposition - 52) / 28) * 2) % 26 != 0 && ((mouseposition - 52) - ((mouseposition - 52) / 28)) % 27 != 0)
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

void MPCHelperWindows::replace(sf::Vector2i(&position), std::vector<std::vector<char>*> &clip_selections_vec, std::vector<int> &selectionlengths_vec, copypaste &copypastetool){

	sf::RenderWindow replace_menu_window(sf::VideoMode(630, 140), "Replace instruments in selected region(s)", sf::Style::Titlebar | sf::Style::Close);
	
	sf::Texture texture0;
	texture0.loadFromFile("replace_menu.png");
	sf::Sprite replace_menu_bg(texture0);
	replace_menu_bg.setTextureRect(sf::IntRect(0, 0, 630, 140));

	sf::Texture texture1;
	texture1.loadFromFile("sprites/large/instruments_transparent.gif");
	sf::Sprite current_note_1(texture1);
	current_note_1.setOrigin(sf::Vector2f(-12, -19));
	sf::Sprite second_current_note_1(texture1);
	second_current_note_1.setOrigin(sf::Vector2f(-161, -57));
	sf::Sprite current_note_2(texture1);
	current_note_2.setOrigin(sf::Vector2f(-12, -97));
	sf::Sprite second_current_note_2(texture1);
	second_current_note_2.setOrigin(sf::Vector2f(-248, -57));
	int top_note_pos = 19;
	int bottom_note_pos = 19;

	while (replace_menu_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 630, 140));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		while (replace_menu_window.pollEvent(event_1)){

			switch (event_1.type){

			case sf::Event::Closed:
				replace_menu_window.close();
				break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(replace_menu_window);

				if (24 <= position.y && position.y <= 52)
					top_note_pos = (int)_menu_2whichnote(position.x) - 97;
				else if (102 <= position.y && position.y <= 130)
					bottom_note_pos = (int)_menu_2whichnote(position.x) - 97;
				else if (505 <= position.x && position.x <= 620 && 62 <= position.y && position.y <= 85)
				{
					if (top_note_pos != 19 && bottom_note_pos != 19)
					{
						for (int i = 0; i < clip_selections_vec.size(); i++)
						{
							copypastetool.replace(clip_selections_vec.at(i), (char)(top_note_pos + 97), (char)(bottom_note_pos + 97), selectionlengths_vec.at(i));
						}
					}
				}
				break;
			}
		}

		replace_menu_window.clear();

		replace_menu_window.draw(replace_menu_bg);
		current_note_1.setTextureRect(sf::IntRect(32 * top_note_pos, 0, 32, 36));
		replace_menu_window.draw(current_note_1);
		second_current_note_1.setTextureRect(sf::IntRect(32 * top_note_pos, 0, 32, 36));
		replace_menu_window.draw(second_current_note_1);
		current_note_2.setTextureRect(sf::IntRect(32 * bottom_note_pos, 0, 32, 36));
		replace_menu_window.draw(current_note_2);
		second_current_note_2.setTextureRect(sf::IntRect(32 * bottom_note_pos, 0, 32, 36));
		replace_menu_window.draw(second_current_note_2);

		replace_menu_window.setFramerateLimit(15);

		replace_menu_window.display();
	}
}

int _arrowright_whichindex(unsigned int mousepositiony, unsigned int current_y){

	mousepositiony = mousepositiony + current_y - 120;
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

void MPCHelperWindows::special_filter(sf::Vector2i(&position), std::string &specialmeasure_string, tab new_tab, tab &tab, int* currently_filtered_notes){

	sf::RenderWindow special_filter_menu_window(sf::VideoMode(630, 350), "Define special measure to filter", sf::Style::Titlebar | sf::Style::Close);
	special_filter_menu_window.setFramerateLimit(15);

	sf::Texture texture0;
	texture0.loadFromFile("special_filter_menu.png");
	sf::Sprite special_filter_menu_bg(texture0);
	special_filter_menu_bg.setTextureRect(sf::IntRect(0, 0, 630, 350));

	sf::Texture texture1;
	texture1.loadFromFile("sprites/large/instruments_transparent.gif");
	sf::Sprite current_note(texture1);
	current_note.setOrigin(sf::Vector2f(-12, -19));
	int note_pos = 19;

	//
	//	Selected instruments to be filtered that appear on filter bar
	//
	TileMap filtered_notes;
	filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4);
	filtered_notes.setOrigin(sf::Vector2f(-54, -61));

	std::vector<char> composition[384];
	if (!tab.getm_special_filter().empty())
	{
		for (unsigned int i = 1; i < tab.getm_special_filter().size(); ++i)
		{
			composition[0].push_back(tab.getm_special_filter().at(i));
			if (tab.getm_special_filter().at(i) != '+')
			{
				if (i + 1 >= tab.getm_special_filter().size())
					break;

				new_tab.setcurrent_note(tab.getm_special_filter().at(i));
				++i;
				composition[0].push_back(tab.getm_special_filter().at(i));
				if (tab.getm_special_filter().at(i + 1) != '+')//flat or sharp or natural
				{
					if (tab.getm_special_filter().at(i + 1) ==  '#')//sharp
						new_tab.updatecomposition(tab.getm_special_filter().at(i),
						0, false, '#', special_filter_menu_window);
					else if (tab.getm_special_filter().at(i + 1) == ';')//flat
						new_tab.updatecomposition(tab.getm_special_filter().at(i),
						0, false, ';', special_filter_menu_window);
					else
						new_tab.updatecomposition(tab.getm_special_filter().at(i),
						0, false, 'n', special_filter_menu_window);

					++i;
					composition[0].push_back(tab.getm_special_filter().at(i));
				}
				else
					new_tab.updatecomposition(tab.getm_special_filter().at(i),
					0, false, 'n', special_filter_menu_window);
			}
		}
		if (composition[0].back() == '+')
			composition[0].push_back('q');
		else
			new_tab.updatecomposition(composition[0].back(), 0, special_filter_menu_window);
	}
	TileMapComposition tile_map_comp;//this will be the one measure that appears
	tile_map_comp.setOrigin(sf::Vector2f(-375, 10));
	tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), composition);

	bool volume_specified = false;
	sf::Sprite hide_volume(texture0);
	hide_volume.setTextureRect(sf::IntRect(450, 307, 630, 350));
	hide_volume.setOrigin(sf::Vector2f(-450, -307));
	if (!tab.getm_special_filter().empty())
	{
		if (tab.getm_special_filter().back() != '+')
			volume_specified = true;
	}

	sf::RectangleShape rectangle0(sf::Vector2f(50, 30));
	rectangle0.setFillColor(sf::Color::Transparent);
	rectangle0.setOutlineThickness(2);
	rectangle0.setOutlineColor(sf::Color::Green);
	rectangle0.setPosition(30, 140);
	if (!tab.getm_special_filter().empty())
	{
		if (tab.getm_special_filter().at(0) == '|')
			rectangle0.setPosition(30, 230);
	}

	bool ctrl = false;
	bool shift = false;

	while (special_filter_menu_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 630, 140));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		while (special_filter_menu_window.pollEvent(event_1)){

			switch (event_1.type){

			case sf::Event::Closed:{

				std::string special_filter;
				for (unsigned int i = 0; i < composition[0].size(); ++i)
				{
					special_filter.push_back(composition[0].at(i));
				}
				if (rectangle0.getPosition().y == 140)
					special_filter.insert(special_filter.begin(), '&');
				else
					special_filter.insert(special_filter.begin(), '|');
				if (!volume_specified && !special_filter.empty()) 
					special_filter.pop_back();
				tab.setm_special_filter(special_filter);
				std::cout << special_filter << std::endl;
				special_filter_menu_window.close();
			}
				break;

			case sf::Event::MouseButtonPressed:{

				position = sf::Mouse::getPosition(special_filter_menu_window);

				if (position.y > 120 && 450 <= position.x && position.x <= 490)
				{
					int note_or_volume = (unsigned)(position.y - 120) % 230;

					switch (event_1.mouseButton.button){

					case sf::Mouse::Left:

						if (note_or_volume < 187 && note_pos < 19)//place a note
						{
							if (shift)//sharp
								new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97),//got lazy and didn't actually find an exact value where -133 is
								0, false, '#', special_filter_menu_window);
							else if (ctrl)//flat
								new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97),
								0, false, ';', special_filter_menu_window);
							else
								new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97),
								0, false, 'n', special_filter_menu_window);
						}
						else if (note_or_volume > 186)//adjust volume
						{
							note_or_volume = 229 - note_or_volume - 2;//now it is between 0 and 40
							if (note_or_volume < 2)
								new_tab.updatecomposition(0, 0, special_filter_menu_window);
							else
							{
								double volume_approx = (note_or_volume * 16) / 40;//find approximately what the volume level is
								if (volume_approx - (int)volume_approx < 0.5)//round the number to find exact level
									note_or_volume = (int)volume_approx;
								else
									note_or_volume = (int)volume_approx + 1;
								new_tab.updatecomposition(note_or_volume, 0, special_filter_menu_window);
							}
							volume_specified = true;
						}
						composition[0] = new_tab.getm_composition()[0];
						tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), composition);
						break;

					case sf::Mouse::Right://erase
						if (note_or_volume < 187)//place a note
						{
							new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97), 0, true, 'n', special_filter_menu_window);
							composition[0] = new_tab.getm_composition()[0];
							tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), composition);
						}
						else
							volume_specified = false;
						break;
					}
				}
			}
				 break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(special_filter_menu_window);

				if (24 <= position.y && position.y <= 52)
				{
					note_pos = (int)_menu_2whichnote(position.x) - 97;
					new_tab.setcurrent_note(note_pos + 97);
				}
				else if (30 <= position.x && position.x <= 80)
				{
					if (140 <= position.y && position.y <= 170)
					{
						rectangle0.setPosition(30, 140);
					}
					else if (230 <= position.y && position.y <= 260)
					{
						rectangle0.setPosition(30, 230);
					}
				}
				break;

			case sf::Event::KeyPressed:
				switch (event_1.key.code){

				case sf::Keyboard::F:{

					position = sf::Mouse::getPosition(special_filter_menu_window);
					int temp_note_pos = note_pos;
					if (24 <= position.y && position.y <= 52)
					{
						note_pos = (int)_menu_2whichnote(position.x) - 97;
						if (note_pos <= 18)
						{
							if (note_pos == 15)
							{
								if (currently_filtered_notes[note_pos + 1] == 19)
								{
									currently_filtered_notes[note_pos + 1] = note_pos;
									tab.addontom_filtered_notes((char)(note_pos + 97));
								}
								else
								{
									currently_filtered_notes[note_pos + 1] = 19;
									tab.removefromm_filtered_notes((char)(note_pos + 97));
								}
							}
							else if (note_pos == 16)
							{
								if (currently_filtered_notes[note_pos - 1] == 19)
								{
									currently_filtered_notes[note_pos - 1] = note_pos;
									tab.addontom_filtered_notes((char)(note_pos + 97));
								}
								else
								{
									currently_filtered_notes[note_pos - 1] = 19;
									tab.removefromm_filtered_notes((char)(note_pos + 97));
								}
							}
							else if (currently_filtered_notes[note_pos] == 19)
							{
								currently_filtered_notes[note_pos] = note_pos;
								tab.addontom_filtered_notes((char)(note_pos + 97));
							}
							else
							{
								currently_filtered_notes[note_pos] = 19;
								tab.removefromm_filtered_notes((char)(note_pos + 97));
							}
						}
					}
					filtered_notes.load("sprites/instruments_transparent.png", sf::Vector2u(24, 27), currently_filtered_notes, 19, 1, 4);

					note_pos = temp_note_pos;
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
				switch (event_1.key.code){

				case sf::Keyboard::LControl: case sf::Keyboard::RControl:
					ctrl = false;
					break;
				case sf::Keyboard::LShift: case sf::Keyboard::RShift:
					shift = false;
					break;
				}
				break;
			}
		}
		special_filter_menu_window.clear();

		special_filter_menu_window.draw(special_filter_menu_bg);
		current_note.setTextureRect(sf::IntRect(32 * note_pos, 0, 32, 36));
		special_filter_menu_window.draw(current_note);
		special_filter_menu_window.draw(filtered_notes);
		special_filter_menu_window.draw(tile_map_comp);
		special_filter_menu_window.draw(rectangle0);
		if (!volume_specified)
			special_filter_menu_window.draw(hide_volume);

		special_filter_menu_window.display();
	}
}

void MPCHelperWindows::find_and_replace(sf::Vector2i(&position), sf::Font(&font), tab new_tab, tab &tab, copypaste &copypastetool, sf::RenderWindow &window, sf::View &view_0, sf::Sprite &bg, int &current_x, int&current_y){

	sf::RenderWindow find_and_replace_menu_window(sf::VideoMode(630, 350), "Find and replace", sf::Style::Titlebar | sf::Style::Close);
	find_and_replace_menu_window.setFramerateLimit(15);

	sf::Texture texture0;
	texture0.loadFromFile("find_replace_menu.png");
	sf::Sprite find_and_replace_menu_bg(texture0);
	find_and_replace_menu_bg.setTextureRect(sf::IntRect(0, 0, 630, 350));

	sf::Texture texture1;
	texture1.loadFromFile("sprites/large/instruments_transparent.gif");
	sf::Sprite current_note(texture1);
	current_note.setOrigin(sf::Vector2f(-12, -19));
	int note_pos = 19;

	std::vector<char> composition[384];

	TileMapComposition tile_map_comp;//this will be the one measure that appears for 'find what' (using index 0 of composition) and 'replace with' (using index 3 of composition)
	tile_map_comp.setOrigin(sf::Vector2f(35, 10));
	tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), composition);

	bool volume_specified = false;//will now be used to indicate if should go through composition for all match indicies
	sf::Sprite hide_volume(texture0);
	hide_volume.setTextureRect(sf::IntRect(0, 307, 512, 350));
	hide_volume.setOrigin(sf::Vector2f(0, -307));

	std::string spec_notes_tracker = "";
	spec_notes_tracker.reserve(15);

	sf::Text num_matches;
	num_matches.setFont(font);
	num_matches.setColor(sf::Color::Black);
	num_matches.setCharacterSize(10);
	num_matches.setPosition(sf::Vector2f(320, 105));
	num_matches.setString("0");

	std::vector<int> match_indices_vec;
	match_indices_vec.reserve(384);
	int match_index = 0;//index on match_indices_vec

	sf::RectangleShape rectangle0(sf::Vector2f(40, 230));//rectangle to track and highlight matches on the composition to be displayed in regular window
	rectangle0.setFillColor(sf::Color::Transparent);
	rectangle0.setOutlineThickness(2);
	rectangle0.setOutlineColor(sf::Color::Magenta);
	rectangle0.setPosition(0, 0);

	bool changes = false;//changes occur by replacing and regular window updates in drawing
	current_y = 130;//not showing the menus while in this window

	bool ctrl = false;
	bool shift = false;

	while (find_and_replace_menu_window.isOpen()){

		sf::Event event_1;

		sf::View view_1;
		view_1.reset(sf::FloatRect(0, 0, 630, 140));
		view_1.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

		while (find_and_replace_menu_window.pollEvent(event_1)){
			switch (event_1.type){

			case sf::Event::Closed:
				find_and_replace_menu_window.close();
				break;

			case sf::Event::MouseButtonPressed:{

				position = sf::Mouse::getPosition(find_and_replace_menu_window);

				if (position.y > 120 && ((unsigned)(position.x - 40) <= (unsigned)(80 - 40)) || ((unsigned)(position.x - 160) <= (unsigned)(200 - 160)))
				{
					int note_or_volume = (unsigned)(position.y - 120) % 230;
					int find_or_replace = 0;//'find what' (i.e. index 0)
					if (position.x > 80)//'replace with' (i.e. index 3)
						find_or_replace = 3;

					switch (event_1.mouseButton.button){

					case sf::Mouse::Left:

						if (note_or_volume < 187 && note_pos < 19)//place a note
						{							
							if (shift)//sharp
								new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97),
								find_or_replace, false, '#', find_and_replace_menu_window);
							else if (ctrl)//flat
								new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97),
								find_or_replace, false, ';', find_and_replace_menu_window);
							else
								new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97),
								find_or_replace, false, 'n', find_and_replace_menu_window);					
						}
						else if (note_or_volume > 186)//adjust volume
						{
							note_or_volume = 229 - note_or_volume - 2;//now it is between 0 and 40
							if (note_or_volume < 2)
								new_tab.updatecomposition(0, find_or_replace, find_and_replace_menu_window);
							else
							{
								double volume_approx = (note_or_volume * 16) / 40;//find approximately what the volume level is
								if (volume_approx - (int)volume_approx < 0.5)//round the number to find exact level
									note_or_volume = (int)volume_approx;
								else
									note_or_volume = (int)volume_approx + 1;
								new_tab.updatecomposition(note_or_volume, find_or_replace, find_and_replace_menu_window);
							}
							//volume_specified = true;
							if (find_or_replace == 0)//'find what'
							{
								hide_volume.setTextureRect(sf::IntRect(80, 307, hide_volume.getTextureRect().left + hide_volume.getTextureRect().width, 350));//move texture to right
								hide_volume.setOrigin(sf::Vector2f(-80, -307));
								volume_specified = true;
							}
							else//'replace with'
							{
								hide_volume.setTextureRect(sf::IntRect(hide_volume.getTextureRect().left, 307, 80, 350));//reduce texture size to 80
							}
						}
						composition[find_or_replace] = new_tab.getm_composition()[find_or_replace];
						tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), composition);
						break;

					case sf::Mouse::Right://erase
						if (note_or_volume < 187)//place a note
						{
							new_tab.updatecomposition((char)((((unsigned)_arrowright_whichindex(position.y, 0) - 143) % 230) / 10 + 97), find_or_replace, true, 'n', find_and_replace_menu_window);
							composition[find_or_replace] = new_tab.getm_composition()[find_or_replace];
							tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), composition);
						}
						else//volume_specified = false;
						{
							if (find_or_replace == 0)//'find what'
							{
								hide_volume.setTextureRect(sf::IntRect(0, 307, hide_volume.getTextureRect().left + hide_volume.getTextureRect().width, 350));//move texture back
								hide_volume.setOrigin(sf::Vector2f(0, -307));
								volume_specified = false;
							}
							else//'replace with'
							{
								hide_volume.setTextureRect(sf::IntRect(hide_volume.getTextureRect().left, 307, 512, 350));//increase texture size back
							}
						}
						break;
					}

					if (find_or_replace == 0 && (composition[0].size() > 7 || volume_specified))
					{
						spec_notes_tracker.clear();
						match_indices_vec.clear();
						for (unsigned int i = 0; i < composition[0].size() - 1; ++i)//ignore volume for now
						{
							if (composition[0].at(i) != '+')
							{
								spec_notes_tracker.push_back(composition[0].at(i));
								++i;
								spec_notes_tracker.push_back(composition[0].at(i));
								if (composition[0].at(i + 1) != '+')//flat,sharp, or natural
								{
									++i;
									spec_notes_tracker.push_back(composition[0].at(i));
								}
								else
									spec_notes_tracker.push_back('+');
							}
						}
						unsigned int spec_check = spec_notes_tracker.size();

						bool special_measure_exist, volume_exist;
						//tab.correctcomposition();
						for (unsigned int i = 0; i < 384; ++i)
						{
							if (!tab.getm_composition()[i].empty())
							{
								spec_check = spec_notes_tracker.size();
								special_measure_exist = volume_exist = false;
								for (unsigned int j = 0; j < tab.getm_composition()[i].size() - 1; ++j)
								{
									if (tab.getm_composition()[i].at(j) != '+')
									{
										for (unsigned int k = spec_notes_tracker.size() - spec_check; k < spec_notes_tracker.size(); k += 3)
										{
											if (tab.getm_composition()[i].at(j) == spec_notes_tracker.at(k) && tab.getm_composition()[i].at(j + 1) == spec_notes_tracker.at(k + 1) && tab.getm_composition()[i].at(j + 2) == spec_notes_tracker.at(k + 2))//check if satisfy special measure
											{
												spec_check -= 3;//one note satisfied
												spec_notes_tracker.insert(spec_notes_tracker.begin(), spec_notes_tracker.begin() + k, spec_notes_tracker.begin() + k + 3);//move characters over
												spec_notes_tracker.erase(spec_notes_tracker.begin() + k + 3, spec_notes_tracker.begin() + k + 6);
												break;
											}
										}
										j += 2;//this will increment onto another note or '+'
									}
								}

								if (spec_check == 0)
									special_measure_exist = true;

								if (volume_specified)//there is special volume that must be considered//NOTE: there is only one case where volume_exist is false: when volume is specified but not met by composition[i]
								{
									if (tab.getm_composition()[i].back() == composition[0].back())//that volume is satisfied
										volume_exist = true;
								}
								else//there is no special volume that must be considered so any volume is satisfied
									volume_exist = true;

								if (special_measure_exist && volume_exist)
									match_indices_vec.push_back(i);
							}
						}

						if (match_indices_vec.size() > 0)
						{
							char numstr[10]; // enough to hold all numbers up to 32-bits
							sprintf_s(numstr, "%i", match_indices_vec.size());
							num_matches.setString(numstr);
						}
						else
						{
							num_matches.setString("0");
						}
						match_index = 0;
					}
					else if (composition[0].size() < 8 && !volume_specified)
					{
						spec_notes_tracker.clear();
						match_indices_vec.clear();
						num_matches.setString("0");
					}

					/*std::cout << "SPECNOTESSIZE" << spec_notes_tracker.size() << spec_notes_tracker<< "MATCHINDICIESSIZE" << match_indices_vec.size() << "COMPOSITION[0]SIZE" << composition[0].size() << std::endl;
					for (unsigned int LE = 0; LE < composition[0].size(); ++LE)
					{
						std::cout << composition[0].at(LE);
					}
					std::cout << "COMPOSITION[0]" << std::endl;
					for (unsigned int LE = 0; LE < composition[3].size(); ++LE)
					{
						std::cout << composition[3].at(LE);
					}
					std::cout << "COMPOSITION[3]" << std::endl;*/
				}
			}
				 break;

			case sf::Event::MouseButtonReleased:
				position = sf::Mouse::getPosition(find_and_replace_menu_window);

				if (24 <= position.y && position.y <= 52)
				{
					note_pos = (int)_menu_2whichnote(position.x) - 97;
					new_tab.setcurrent_note(note_pos + 97);
				}
				else if (280 <= position.x && position.x <= 396)
				{
					if (170 <= position.y && position.y <= 193)//find prev
					{
						if (match_index - 1 >= 0)
							--match_index;
						else
						{
							if (!match_indices_vec.empty())
								match_index = match_indices_vec.size() - 1;
							else
								match_index = 0;
						}
					}
					else if (210 <= position.y && position.y <= 233)//find next
					{
						if (match_index + 1 < match_indices_vec.size())
							++match_index;
						else
							match_index = 0;
					}
					else if (260 <= position.y && position.y <= 283)//replace (and then find next)
					{
						if (!match_indices_vec.empty())
						{
							std::string comp_at_0_string = "";
							for (unsigned int i = 0; i < composition[0].size() - 1; ++i)//get remove info//ignore volume for now
							{
								comp_at_0_string.push_back(composition[0].at(i));
							}
							if (volume_specified)
								comp_at_0_string.push_back(composition[0].back());

							std::vector<char>* replace_ptr = composition + 3;//get replace info

							std::vector<char>* replace_vol_ptr = composition + 4;//create volume replace info
							if (!composition[3].empty() && hide_volume.getTextureRect().width <= 80)//volume is specified for 'replace with'
								replace_vol_ptr->push_back(composition[3].back());
							else
								replace_vol_ptr->push_back('~');

							//std::cout << "VOLUMEREPLACEPOINTER" << replace_vol_ptr->at(0) << std::endl;

							copypastetool.specialremove(tab.getm_composition(), match_indices_vec.at(match_index), match_indices_vec.at(match_index), 'a', 'q', "", "&" + comp_at_0_string);
							copypastetool.paste(tab.getm_composition(), match_indices_vec.at(match_index), replace_ptr, 1);
							copypastetool.volumepaste(tab.getm_composition(), match_indices_vec.at(match_index), replace_vol_ptr, 1);

							replace_vol_ptr->clear();//clear volume

							match_indices_vec.erase(match_indices_vec.begin() + match_index);//adjust match indices and related
							if (match_index >= match_indices_vec.size())
								match_index = 0;
							char numstr[10]; // enough to hold all numbers up to 32-bits
							sprintf_s(numstr, "%i", match_indices_vec.size());
							num_matches.setString(numstr);

							tab.correctcomposition();
							tab.updateprogress();
							changes = true;
						}
					}
					else if (300 <= position.y && position.y <= 323)//replace all
					{
						if (!match_indices_vec.empty())
						{
							std::string comp_at_0_string = "";
							for (unsigned int i = 0; i < composition[0].size() - 1; ++i)//get remove info//ignore volume for now
							{
								comp_at_0_string.push_back(composition[0].at(i));
							}
							if (volume_specified)
								comp_at_0_string.push_back(composition[0].back());

							std::vector<char>* replace_ptr = composition + 3;	//get replace info

							std::vector<char>* replace_vol_ptr = composition + 4;//create volume replace info
							if (!composition[3].empty() && hide_volume.getTextureRect().width <= 80)
								replace_vol_ptr->push_back(composition[3].back());
							else
								replace_vol_ptr->push_back('~');

							for (unsigned int i = 0; i < match_indices_vec.size(); ++i)// do it for all valid indices
							{
								copypastetool.specialremove(tab.getm_composition(), match_indices_vec.at(i), match_indices_vec.at(i), 'a', 'q', "", "&" + comp_at_0_string);
								copypastetool.paste(tab.getm_composition(), match_indices_vec.at(i), replace_ptr, 1);
								copypastetool.volumepaste(tab.getm_composition(), match_indices_vec.at(i), replace_vol_ptr, 1);
							}

							replace_vol_ptr->clear();//clear volume

							match_indices_vec.clear();//adjust match indices and related
							match_index = 0;
							num_matches.setString("0");

							tab.correctcomposition();
							tab.updateprogress();
							changes = true;
						}
					}

					if (!match_indices_vec.empty())
					{
						rectangle0.setPosition(((unsigned)match_indices_vec.at(match_index) % 32) * 40 + 75, ((unsigned)match_indices_vec.at(match_index) / 32) * 230 + 130);
						if (rectangle0.getPosition().x + rectangle0.getSize().x - current_x > window.getSize().x)//move regular window accordingly to show the highlight rectangle
							current_x = current_x + rectangle0.getPosition().x + rectangle0.getSize().x - window.getSize().x;
						else if (rectangle0.getPosition().x < current_x)
							current_x = rectangle0.getPosition().x;
						if (rectangle0.getPosition().y + rectangle0.getSize().y - current_y > window.getSize().y)
							current_y = current_y + rectangle0.getPosition().y + rectangle0.getSize().y - window.getSize().y;
						else if (rectangle0.getPosition().y < current_y)
							current_y = rectangle0.getPosition().y;
					}
					//
					//	Move window view accordingly
					//
					view_0.reset(sf::FloatRect(current_x, current_y, window.getSize().x, window.getSize().y));
					window.setView(view_0);

					window.clear();

					window.draw(bg);
					tab.drawcomposition(changes, window);
					if (!match_indices_vec.empty())
						window.draw(rectangle0);

					window.display();
				}
				break;

			case sf::Event::KeyPressed:
				switch (event_1.key.code){

				/*case sf::Keyboard::A://debug helper code
					for (unsigned int LE = 0; LE < 384; ++LE)
					{
						if (!tab.getm_composition()[LE].empty())
						{
							for (unsigned int LET = 0; LET < tab.getm_composition()[LE].size(); ++LET)
							{
								std::cout << tab.getm_composition()[LE].at(LET);
							}
						}
						std::cout << ':';
					}
					std::cout << std::endl;
					break;*/

				case sf::Keyboard::LControl: case sf::Keyboard::RControl:
					ctrl = true;
					break;
				case sf::Keyboard::LShift: case sf::Keyboard::RShift:
					shift = true;
					break;
				}
				break;

			case sf::Event::KeyReleased:
				switch (event_1.key.code){

				case sf::Keyboard::LControl: case sf::Keyboard::RControl:
					ctrl = false;
					break;
				case sf::Keyboard::LShift: case sf::Keyboard::RShift:
					shift = false;
					break;
				}
				break;
			}
		}
		find_and_replace_menu_window.clear();

		find_and_replace_menu_window.draw(find_and_replace_menu_bg);
		current_note.setTextureRect(sf::IntRect(32 * note_pos, 0, 32, 36));
		find_and_replace_menu_window.draw(current_note);
		find_and_replace_menu_window.draw(tile_map_comp);
		//if (!volume_specified)
			find_and_replace_menu_window.draw(hide_volume);
		find_and_replace_menu_window.draw(num_matches);

		find_and_replace_menu_window.display();
	}
}