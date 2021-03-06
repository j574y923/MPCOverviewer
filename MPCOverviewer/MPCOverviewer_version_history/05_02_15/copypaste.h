
#include <string>
#include <vector>

#ifndef COPY_PASTE_H
#define COPY_PASTE_H

class copypaste{

public:

	/**
	*	-copy function used for copy and cut; note: this is the one copypaste function that takes in reference to composition but does NOT modify it
	*	-selection affected defined by range of beginningindex and endingindex
	*	-notes affected defined by the string 'notes'
	*	-also need to change a reference to the selectionlength as the array selection is variable
	*	-return pointer to array selection to be stored in clipboard
	*/
	//http://stackoverflow.com/questions/21799370/what-is-more-efficient-pass-parameter-by-pointer-or-by-value
	std::vector<char>* copy(const std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, std::string notes, int selectionlength);

	/**		
	*	-remove function used for cut and delete; note: takes in reference to composition
	*	-selection affected defined by range of beginningindex and endingindex
	*	-notes affected defined by the string 'notes'
	*/
	//note: is it (&composition)[384] and take in as reference or composition[] and call with the parameter set as reference?
	//or just composition[] and defaults as reference?
	void remove(std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, std::string notes);

	/**
	*	-paste function; note: takes in reference to composition
	*	-place to paste selection defined by pastingindex
	*	-selection to paste defined by vector array 'selection' which will specify the selection array and the notes attached
	*	-selection array should be passed in as a reference; because its length is variable, need to specify 'selectionlength'
	*	-another note: this will only superimpose notes onto the place to paste; 
	*	-so if notes already exist in the place to paste, it will put the selection on top of those notes
	*	-bool checkredundant will call checkredundant helper method to check for notes of same instrument and same position already placed
	*/
	void paste(std::vector<char>* composition, int pastingindex, std::vector<char>* selection, int selectionlength);

	/**
	*	-check the measure passed in which should be a measure in the composition
	*	-check if the instrument of staffindex is already in the measure
	*	-if it is in the measure return true else return false
	*/
	//this should be deprecated
	bool checkredundant(std::vector<char> measure, char instrument, char staffindex);

	/**
	*	-replace function to be used after copy, remove are all called in that order
	*	-modify the selection from copy by replacing the desired notes
	*	-selection array should be passed in as a reference; because its length is variable, need to specify 'selectionlength'
	*	-return pointer to array selection to be stored in TEMPORARY clipboard
	*	-paste function should then be called with this TEMPORARY clipboard as its selection
	*	-reason it uses all the functions: this will save code space and it will do the staffrange and selectionlength processes
	*/
	std::vector<char>* replace(std::vector<char>* selection, char whattoreplace, char replacewith, int selectionlength);

	//--------------------------------------------------
	//	volume modulation copy and paste functions
	//--------------------------------------------------

	/**
	*	-copy function used for copy and cut; note: this is the one copypaste function that takes in reference to composition but does NOT modify it
	*	-AS IT AFFECTS VOLUME: copies the last char in every vector in the array selection
	*	-selection affected defined by range of beginningindex and endingindex
	*	-return pointer to vector of size one that contains string of volume modifier chars
	*/
	std::vector<char>* volumecopy(const std::vector<char>* composition, int beginningindex, int endingindex);

	/**
	*	-remove function used for cut and delete; note: takes in reference to composition
	*	-AS IT AFFECTS VOLUME: takes out the last char in every vector in the array selection and replaces with default volume
	*	-selection affected defined by range of beginningindex and endingindex
	*/
	void volumeremove(std::vector<char>* composition, int beginningindex, int endingindex);

	/**
	*	-paste function; note: takes in reference to composition
	*	-AS IT AFFECTS VOLUME: will write over volume in the place to paste with selection
	*	-place to paste selection defined by pastingindex
	*	-selection to paste defined by vector array 'selection' which will specify the selection array and the volume attached
	*	-selection array should be passed in as a reference; 'selectionlength' refers to the array's one vector length
	*/
	void volumepaste(std::vector<char>* composition, int pastingindex, std::vector<char>* selection, int selectionlength);
private:
	/**
	*	-reduces selection to what is defined by staff range; this will be used by copy and remove functions
	*	-staff range defined by staffbeg to staffend; all values outside of these will be eliminated from the selection
	*	-selection array should be passed in as a reference; because its length is variable, need to specify 'selectionlength'
	*	-if removing, will eliminate everything in staff range instead
	*/
	void staffrange(char staffbeg, char staffend, std::vector<char>* selection, int selectionlength, bool remove);
};


#endif