
#include "undovector.h"

#include <fstream>

undovector::undovector(std::vector<char>(&composition)[384]){//note: used to be std::vector<char>* composition

	//m_undovector.clear();//because only constructed once, clear will never be called beside from this one line
	std::vector<char> compositioncopy[384];
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

		std::string::size_type sz;   // alias of size_t
		m_maxsize = std::stoi(file_contents, &sz);
	}
	catch (std::exception & obj)
	{

	}
	//m_undovector.reserve(m_maxsize);//not 100% certain how this will work
}

/*undovector::~undovector(){

	//http://stackoverflow.com/questions/860447/what-is-the-array-form-of-delete
	for (int i = 0; i < m_undovector.size(); i++)//note this might probably not be needed
	{
		delete[] m_undovector.at(i);
	}
}*/

void undovector::undo(std::vector<char>(&composition)[384]){

	if (m_index - 1 < 0)
	{
		//nothing changed
	}
	else
	{
		m_index--;
		for (int i = 0; i < 384; i++)
		{
			if (m_undovector[m_index][i].empty())
			{
				composition[i].clear();//CAN YOU CLEAR AN EMPTY VECTOR?
			}
			else
			{
				composition[i] = m_undovector[m_index][i];//CAN YOU SET EQUAL TO AN EMPTY VECTOR?
			}
		}
	}
}

void undovector::redo(std::vector<char>(&composition)[384]){

	if (m_index + 1 > m_undovector.size() - 1)
	{
		//nothing changed
	}
	else
	{
		m_index++;
		for (int i = 0; i < 384; i++)
		{
			if (m_undovector[m_index][i].empty())
			{
				composition[i].clear();
			}
			else
			{
				composition[i] = m_undovector[m_index][i];
			}
		}
	}
}

void undovector::progress(std::vector<char> (&composition)[384]){

	eraseredo();
	std::vector<char> compositioncopy[384];
	for (int i = 0; i < 384; ++i)
	{
		compositioncopy[i] = composition[i];
	}
	m_undovector.push_back(compositioncopy);
	m_index++;
	if (m_undovector.size() > m_maxsize)
	{
		//delete[] m_undovector.at(0);//might be wrong, may need to just erase
		m_undovector.erase(m_undovector.begin());
		//m_undovector.reserve(m_maxsize);//not 100% certain how this will work
	}
}

void undovector::eraseredo(){

	//http://bytes.com/topic/c/answers/444395-pointer-vector-shall-i-delete-clear
	//http://stackoverflow.com/questions/10464992/c-delete-vector-objects-free-memory
	
	/*for (int i = m_index + 1; i < m_undovector.size(); i++)
	{
		delete[] m_undovector.at(i);//might be wrong, may need to just erase
	}*/
	
	m_undovector.erase(m_undovector.begin() + m_index + 1, m_undovector.begin() + m_undovector.size());//note m_index + 1 should never be greater than m_undovector.size()
	//m_undovector.reserve(m_maxsize);//not 100% certain how this will work
}