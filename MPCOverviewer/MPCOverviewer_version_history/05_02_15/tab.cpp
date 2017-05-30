
#include "tab.h"
#include <iostream>

tab::tab(){

	if (!m_tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), m_composition))
	{
		//cant return anything
	}
	m_filename = "NewSong]MarioPaint";
	m_tempo = 358;
	m_current_x, m_current_y = 0;
	m_note_pos = 19;
	//m_instruments_transparent.loadFromFile("sprites/large/instruments_transparent.gif");
	//m_current_note.setTexture(m_instruments_transparent);
	//m_current_note.setOrigin(sf::Vector2f(-12, -50));
	m_filtered_notes = "";
	m_undovector = undovector(m_composition);
}

/*tab::~tab(){

	//delete[] m_composition;//this is not dynamically allocated
	//delete m_undovector;
}*/

void tab::drawcomposition(bool changes, sf::RenderWindow(&window)){

	if (changes)
	{
		if (!m_tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), m_composition))
		{
			//cant return anything
		}
	}
	window.draw(m_tile_map_comp);
}

void tab::updatecomposition(bool undo, sf::RenderWindow(&window)){

	if (undo)
	{
		m_undovector.undo(m_composition);
	}
	else
	{
		m_undovector.redo(m_composition);
	}
	drawcomposition(true, window);//may not need to call drawcomposition, can just let main do it
}

void tab::updatecomposition(char staffindex, int measureindex, bool remove, char flatsharpnat, sf::RenderWindow(&window)){

	//m_note_pos = 0;//remove this
	if (remove && !m_composition[measureindex].empty())
	{
		for (int i = 0; i < m_composition[measureindex].size() - 1; i++)
		{
			if (m_composition[measureindex].at(i) != '+')
			{
				if (m_composition[measureindex].at(i + 1) == staffindex)
				{
					if (m_composition[measureindex].at(i + 2) != '+')
					{
						m_composition[measureindex].erase(m_composition[measureindex].begin() + i, m_composition[measureindex].begin() + i + 3);//erase 3 elements
					}
					else
					{
						m_composition[measureindex].erase(m_composition[measureindex].begin() + i, m_composition[measureindex].begin() + i + 2);//erase 2 elements
					}
				}
				else
				{
					if (m_composition[measureindex].at(i + 2) != '+')
					{
						i++;
					}
					i++;
				}
			}			
		}
	}
	else
	{
		//also use m_current_note
		int note_count = 0;//up to 5 per measure
		int where_to_place = 0;
		if (m_composition[measureindex].empty())
		{
			for (int p = 0; p < 6; p++)
			{
				m_composition[measureindex].push_back('+');
			}
			m_composition[measureindex].push_back('q');
		}
		for (int i = 0; i < m_composition[measureindex].size() - 1; i++)
		{
			if (m_composition[measureindex].at(i) != '+')
			{
				note_count++;
				if (m_composition[measureindex].at(i + 2) != '+')
				{
					i++;
				}
				i++;
				where_to_place = i + 2;
			}
		}
		if (note_count < 5)
		{
			if (m_note_pos != 19)
			{
				m_composition[measureindex].insert(m_composition[measureindex].begin() + where_to_place, (char)(m_note_pos + 97));//the instrument type
				m_composition[measureindex].insert(m_composition[measureindex].begin() + where_to_place + 1, staffindex);//staff index/key
				if (flatsharpnat != 'n')
				{
					m_composition[measureindex].insert(m_composition[measureindex].begin() + where_to_place + 2, flatsharpnat);//flat, or sharp
				}
				if (staffindex == 'q')
				{
					m_composition[measureindex].insert(m_composition[measureindex].begin() + where_to_place + 2, '7');//natural
				}
			}
		}
	}
	m_undovector.progress(m_composition);
	drawcomposition(true, window);
}

void tab::updatecomposition(int volumelevel, int measureindex, sf::RenderWindow(&window)){

	if (m_composition[measureindex].empty())
	{
		for (int p = 0; p < 6; p++)
		{
			m_composition[measureindex].push_back('+');
		}
		m_composition[measureindex].push_back((char)(volumelevel + 97));
	}
	else
	{
		m_composition[measureindex].pop_back();
		m_composition[measureindex].push_back((char)(volumelevel + 97));
	}
	m_undovector.progress(m_composition);
	drawcomposition(true, window);
}

void tab::populatecomposition(std::string directory){

	for (int i = 0; i < 384; i++)
	{
		m_composition[i].clear();
	}
	try
	{
		std::ifstream infile(directory + "\\" + m_filename + ".txt");//this will be specified by user in gui
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
				if (c == '1')//CHANGES '1' TO 'q'
					m_composition[i].push_back('q');
				else
					m_composition[i].push_back(c);
			}
		}
		std::string temp_tempo_string;
		j += 2;//should be past the '"%' now
		for (j; j < file_contents.size(); j++)
		{
			temp_tempo_string.push_back(file_contents.at(j));
		}
		m_tempo = std::stoi(temp_tempo_string);

		infile.close();
	}
	catch (std::exception & obj)
	{

	}

	if (!m_tile_map_comp.load("sprites/instruments_transparent_-_Copy-3.png", sf::Vector2u(24, 42), m_composition))
	{
		//cant return anything
	}

	m_undovector = undovector(m_composition);
}

std::vector<char>* tab::getm_composition(){//might not be used
	return m_composition;
}

void tab::outputcomposition(){//change 'q' back to '1' if not at end of vector

	std::ofstream outfile;
	outfile.open(m_filename + ".txt");
	outfile << "4/4*";
	for (int i = 0; i < 384; i++)
	{
		for (int j = 0; j < m_composition[i].size(); j++)
		{
			outfile << m_composition[i].at(j);
		}
	}
	outfile << "\"%";
	outfile << m_tempo;

	outfile.close();
}

std::string tab::getm_filename(){
	return m_filename;
}

void tab::setm_filename(std::string filename){
	m_filename = filename;
}

int tab::getcurrent_x(){
	return m_current_x;
}

void tab::setcurrent_x(int current_x){
	m_current_x = current_x;
}

int tab::getcurrent_y(){
	return m_current_y;
}

void tab::setcurrent_y(int current_y){

	m_current_y = current_y;
	//move m_current_note accordingly
	/*if (m_current_y > 50)
	{
		m_current_note.setPosition(sf::Vector2f(0, m_current_y - 50));
	}
	else
	{
		m_current_note.setPosition(sf::Vector2f(0, 0));
	}*/
}

void tab::setcurrent_note(char current_note){

	m_note_pos = (int)current_note - 97;
	//m_current_note.setTextureRect(sf::IntRect(32 * m_note_pos, 0, 32, 36));
}

int tab::getm_note_pos(){
	return m_note_pos;
}

void tab::addontom_filtered_notes(char note_to_filter){
	m_filtered_notes = m_filtered_notes + note_to_filter;
}

void tab::removefromm_filtered_notes(char note_to_remove){

	for (int i = 0; i < m_filtered_notes.length(); i++)
	{
		if (m_filtered_notes.at(i) == note_to_remove)
		{
			m_filtered_notes.erase(m_filtered_notes.begin() + i);
			break;
		}
	}
}

std::string tab::getm_filtered_notes(){
	return m_filtered_notes;
}