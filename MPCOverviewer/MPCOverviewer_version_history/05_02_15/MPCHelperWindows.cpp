
#include "MPCHelperWindows.h"
#include "tchar.h"

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
						menu_1savewindow(font, position, directory, title, save_as);
						//tab_to_save.outputcomposition();//NOT DONE
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
					//tab_to_save.outputcomposition();//NOT DONE
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
void MPCHelperWindows::menu_1savewindow(sf::Font(&font), sf::Vector2i(&position), sf::Text(&directory), sf::Text(&title), sf::Text(&save_as)){

	sf::RenderWindow save_as_window(sf::VideoMode(350, 210), "Save song as...", sf::Style::Titlebar | sf::Style::Close);
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
char menu_2whichnote(int mouseposition){

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

void MPCHelperWindows::replace(sf::Vector2i(&position), std::vector<std::vector<char>*> listofselections, std::vector<int> listofselectionlengths, copypaste(&copypastetool)){

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
					top_note_pos = (int)menu_2whichnote(position.x) - 97;
				else if (102 <= position.y && position.y <= 130)
					bottom_note_pos = (int)menu_2whichnote(position.x) - 97;
				else if (505 <= position.x && position.x <= 620 && 62 <= position.y && position.y <= 85)
				{
					if (top_note_pos != 19 && bottom_note_pos != 19)
					{
						for (int i = 0; i < listofselections.size(); i++)
						{
							copypastetool.replace(listofselections.at(i), (char)(top_note_pos + 97), (char)(bottom_note_pos + 97), listofselectionlengths.at(i));
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