
#include <string>
#include <vector>

#ifndef UNDO_VECTOR_H
#define UNDO_VECTOR_H

class undovector{

public:

	/**
	*	-constructor; initializes m_regionschanged to null
	*	-sets m_undovector first index to the very first state of the composition through a COPY of the composition
	*	-sets m_index to 0
	*	-sets m_maxsize to maxsize defined by user in 'config.txt'; default should be 10
	*/
	undovector(std::vector<char>(&m_composition)[384]);

	/**
	*	-deconstructor to deallocate stored composition states
	*/
	//~undovector();

	/**
	*	-go back one index in undo vector
	*	-put a former state of the composition into reference composition passed in
	*/
	void undo(std::vector<char>(&composition)[384]);

	/**
	*	-go forward one index in undo vector
	*	-put a latter state of the composition into reference composition passed in
	*/
	void redo(std::vector<char>(&composition)[384]);

	/**
	*	-get rid of all latter states of the composition by calling eraseredo()
	*	-go forward one index in undo vector
	*	-store what was changed into m_undovector through a COPY of the composition
	*	-check if exceeded the maximum number of undos; if so erase the first state of the composition put into the undovector
	*/
	void progress(std::vector<char> (&composition)[384]);

	/**
	*	-get rid of all latter states of the composition when progress has been made
	*/
	void eraseredo();

private:
	//std::vector<char>** m_regionschanged;//this value will be read in from clipboard
	std::vector<std::vector<char>*> m_undovector;//vector of the states of the composition after each editing instance
	int m_index;//index of which state currently at
	int m_maxsize;//maximum number of undos
};

#include "undovector.hpp"

#endif