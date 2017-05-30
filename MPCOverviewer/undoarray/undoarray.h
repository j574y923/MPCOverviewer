
#include <string>
#include <vector>

#ifndef UNDO_ARRAY_H
#define UNDO_ARRAY_H

class undoarray
{
public:

	/**
	*	-constructor; initializes m_regionschanged to null
	*	-initializes m_undoarray to a size defined by the user in a txt file in the same directory as the program
	*	-sets m_index to 0
	*/
	undoarray();

	/**
	*	-go back one index in undo array
	*	-load the changes into referenced composition
	*/
	void undo(std::vector<char> (&composition)[384]);

	/**
	*	-go forward one index in undo array
	*	-load the changes into referenced composition
	*/
	void redo(std::vector<char> (&composition)[384]);

	/**
	*	-go forward one index in undo array
	*	-store what was changed into m_undoarray through a copy of what was put on clipboard
	*/
	void progress(std::vector<char>** clipboard);

private:
	std::vector<char>** m_regionschanged;//this value will be read in from clipboard
	std::vector<char>*** m_undoarray;
	int m_index;
};

#include "undoarray.hpp"

#endif