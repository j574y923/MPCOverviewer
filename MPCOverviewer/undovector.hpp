
#include "undovector.h"

#include <fstream>
#include <iostream>

undovector::undovector(std::vector<char>* composition){//note: used to be std::vector<char>* composition

	//m_undovector.clear();//because only constructed once, clear will never be called beside from this one line
	std::array<std::vector<char>, 384> compositioncopy;
	for (int i = 0; i < 384; i++)
	{
		compositioncopy[i] = composition[i];
	}
	m_undovector.push_back(compositioncopy);
	m_index = 0;
	//m_maxsize = maxsize;//READ IN MAXSIZE FROM 'config.txt'
	try
	{
		std::ifstream infile("config.txt");
		std::string file_contents;

		getline(infile, file_contents);
		getline(infile, file_contents);
		file_contents.erase(file_contents.begin(), file_contents.begin() + 16);

		std::string::size_type sz;   // alias of size_t
		m_maxsize = std::stoi(file_contents, &sz);

		infile.close();
	}
	catch (std::exception & obj)
	{

	}
	m_undovector.reserve(m_maxsize);//not 100% certain how this will work
}

/*undovector::~undovector(){

	//http://stackoverflow.com/questions/860447/what-is-the-array-form-of-delete
	for (int i = 0; i < m_undovector.size(); i++)//note this might probably not be needed
	{
		delete[] m_undovector.at(i);
	}
}*/

void undovector::undo(std::vector<char>* composition){

	if (m_index - 1 < 0)
	{
		//nothing changed
	}
	else
	{
		m_index--;
		for (int i = 0; i < 384; i++)
		{
			if (m_undovector.at(m_index)[i].empty())
			{
				composition[i].clear();//CAN YOU CLEAR AN EMPTY VECTOR?
			}
			else
			{
				composition[i] = m_undovector.at(m_index)[i];//CAN YOU SET EQUAL TO AN EMPTY VECTOR?
			}
		}
	}
}

void undovector::redo(std::vector<char>* composition){

	if (m_index + 1 > m_undovector.size() - 1)
	{
		//nothing changed
	}
	else
	{
		m_index++;
		for (int i = 0; i < 384; i++)
		{
			if (m_undovector.at(m_index)[i].empty())
			{
				composition[i].clear();
			}
			else
			{
				composition[i] = m_undovector.at(m_index)[i];
			}
		}
	}
}

void undovector::progress(std::vector<char>* composition){

	if (m_index + 1 >= m_maxsize)
	{
		m_undovector.erase(m_undovector.begin());
	}
	else
	{
		m_index++;
	}

	eraseredo();
	std::array<std::vector<char>, 384> compositioncopy;
	for (int i = 0; i < 384; ++i)
	{
		compositioncopy[i] = composition[i];
	}
	m_undovector.push_back(compositioncopy);

	/*m_index++;
	if (m_undovector.size() > m_maxsize)
	{
		//delete[] m_undovector.at(0);//might be wrong, may need to just erase
		m_undovector.erase(m_undovector.begin());
		//m_undovector.reserve(m_maxsize);//not 100% certain how this will work
	}*/
}

void undovector::eraseredo(){

	//http://bytes.com/topic/c/answers/444395-pointer-vector-shall-i-delete-clear
	//http://stackoverflow.com/questions/10464992/c-delete-vector-objects-free-memory
	
	/*for (int i = m_index + 1; i < m_undovector.size(); i++)
	{
		delete[] m_undovector.at(i);//might be wrong, may need to just erase
	}*/
	
	//m_undovector.erase(m_undovector.begin() + m_index + 1, m_undovector.begin() + m_undovector.size());//note m_index + 1 should never be greater than m_undovector.size()
	m_undovector.erase(m_undovector.begin() + m_index, m_undovector.end());
	//m_undovector.reserve(m_maxsize);//not 100% certain how this will work
}

bool undovector::change(std::vector<char>* composition){

	for (unsigned int i = 0; i < 383; ++i)
	{
		for (unsigned int j = 0; j < composition[i].size(); ++j)
		{
			if (composition[i].size() != m_undovector.at(m_index)[i].size())
				return true;
			if (composition[i].at(j) != m_undovector.at(m_index)[i].at(j))
				return true;
		}
	}
	return false;
}