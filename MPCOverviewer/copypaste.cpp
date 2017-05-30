
#include "copypaste.h"
#include <iostream>

//note: before there were no unsigned ints and no prefix increments (e.g. ++i) so if anything wrong happens in here, that must've been the change that caused it

std::vector<char>* copypaste::copy(const std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, const std::string &notes, int selectionlength){

	std::vector<char>* selection = new std::vector<char>[endingindex - beginningindex + 1];
	unsigned int i = beginningindex;
	unsigned int j = 0;
	if (notes.empty())//no filter is applied
	{
		//get everything in the range defined by beginningindex and endingindex
		while (i <= endingindex)
		{
			if (j != composition[i].size())
			{
				selection[i - beginningindex].push_back(composition[i].at(j));
				++j;
			}
			else
			{
				++i;
				j = 0;
			}
		}
	}
	else
	{
		//look for charAt_char_+; check for charAt_char_; or charAt_char_# for each character in string 'notes' (loop multiple times)
		unsigned int k = 0;
		while (i <= endingindex)
		{
			if (j < composition[i].size() - 1 && !composition[i].empty())//if empty at i, will just ignore
			{
				while (k < notes.length())
				{
					if (composition[i].at(j) == notes.at(k))
					{
						selection[i - beginningindex].push_back(composition[i].at(j));
						++j;
						selection[i - beginningindex].push_back(composition[i].at(j));
						++j;
						if (composition[i].at(j) != '+')//natural, flat, or sharp
						{
							selection[i - beginningindex].push_back(composition[i].at(j));
							++j;
						}
						//selection[i - beginningindex].push_back(composition[i].at(j));
						//++j;
						k = 0;
					}
					else
					{
						++k;
					}
				}
				k = 0;
				if (composition[i].at(j) == '+')
				{
					selection[i - beginningindex].push_back(composition[i].at(j));
					++j;
				}
				else
				{
					if (j == composition[i].size() - 1)//volume data; include this
					{
						selection[i - beginningindex].push_back(composition[i].at(j));
						++j;
					}
					else
					{
						if (composition[i].at(j + 2) != '+')//note: this should not go off the array as there are '+' buffers
						{
							selection[i - beginningindex].push_back(composition[i].at(j + 3));//ignoring data not in the notes filter
							j += 4;
						}
						else
						{
							selection[i - beginningindex].push_back(composition[i].at(j + 2));
							j += 3;
						}
					}
				}
			}
			else
			{
				++i;
				j = 0;
			}
		}
	}
	//modify selection based on staffbeginningindex (staffbeg) and staffendingindex (staffend) here
	//selectionlength = endingindex - beginningindex + 1;<- this is what selectionlength is supposed to be passed in as
	staffrange(staffbeg, staffend, selection, selectionlength, false);
	return selection;
}

void copypaste::remove(std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, const std::string &notes){

	unsigned int i = beginningindex;
	unsigned int j = 0;
	if (notes.empty())//no filter is applied
	{
		//remove it all;
		//dont actually need to do anything here, just pass into staffrange and remove all within the staff range
		/*while (i <= endingindex)
		{
			if (j != composition[i].size())
			{
				composition[i].erase(composition[i].begin(), composition[i].begin() + composition[i].size());//erases all elements
			}
			++i;
		}*/
		//for staffrange: pointer math for composition and pass into staffrange with remove true
		staffrange(staffbeg, staffend, composition + beginningindex, endingindex - beginningindex + 1, true);
	}
	else
	{
		//look for charAt_char_+; check for charAt_char_; or charAt_char_# for each character in string 'notes' (loop multiple times)
		unsigned int k = 0;
		while (i <= endingindex)
		{
			if (j < composition[i].size() - 1 && !composition[i].empty())
			{
				while (k < notes.length())
				{					
					if (composition[i].at(j) == notes.at(k))//crashed here once..., look at the j comparison with size and the check of whether composition is empty above for the fix (maybe)
					{
						if ((int)composition[i].at(j + 1) < (int)staffbeg || (int)composition[i].at(j + 1) > (int)staffend)//FIXED??//THIS CAN GO OFF THE VECTOR, FIX NEEDED
						{
							j += 2;
							if (composition[i].at(j) != '+')//natural, flat, or sharp
							{
								++j;
							}
							//++j;
						}
						else
						{
							if (composition[i].at(j + 2) != '+')//natural, flat, or sharp
							{
								composition[i].erase(composition[i].begin() + j + 2);//note: erases (j + 2)th element, the flat, sharp or natural modifier
							}
							composition[i].erase(composition[i].begin() + j + 1);//note: erases (j + 1)th element, the staff position
							composition[i].erase(composition[i].begin() + j);//note: erases jth element, the instrument type
							composition[i].erase(composition[i].begin() + j);//erase the '+'
							composition[i].insert(composition[i].begin() + composition[i].size() - 1, '+');//put the '+' in the back before the volume data
							k = 0;
						}
					}
					else
					{
						++k;
					}
				}
				k = 0;
				if (composition[i].at(j) == '+')
				{
					++j;
				}
				else
				{
					if (j == composition[i].size() - 1)//volume data; include this
					{
						++j;
					}
					else
					{
						if (composition[i].at(j + 2) != '+')//note: this should not go off the array as there are '+' buffers
						{
							//ignoring data not in the notes filter
							j += 4;
						}
						else
						{
							j += 3;
						}
					}
				}
			}
			else
			{
				++i;
				j = 0;
			}
		}
	}
}

void copypaste::paste(std::vector<char>* composition, int pastingindex, std::vector<char>* selection, int selectionlength){

	unsigned int note_count = 0;//up to 5 per measure
	unsigned int i = 0;
	unsigned int j = 0;
	while (i < selectionlength)
	{
		if (!selection[i].empty())//there is something to paste
		{
			if (composition[pastingindex + i].empty())
			{
				for (unsigned int p = 0; p < 6; ++p)
				{
					composition[pastingindex + i].push_back('+');
				}
				composition[pastingindex + i].push_back('q');
			}
			note_count = 0;
			while (j < composition[pastingindex + i].size() - 1)//check composition for how much room; ignore volume
			{
				if (composition[pastingindex + i].at(j) != '+')
				{
					if (composition[pastingindex + i].at(j + 2) != '+')
					{
						++j;
					}
					j += 2;
					++note_count;
				}
				else
				{
					++j;
				}
			}

			j = 0;
			unsigned int k = 0;
			while (note_count < 5 && j < composition[pastingindex + i].size() - 1 && k < selection[i].size() - 1)
			{
				while (selection[i].at(k) == '+' && k < selection[i].size() - 1)
				{
					++k;
				}
				if (composition[pastingindex + i].at(j) == '+')
				{
					if (composition[pastingindex + i].at(j + 1) == '+')
					{
						//insert here
						if (k == selection[i].size() - 1)
						{
							//nothing to paste
						}
						else
						{
							++j;
							composition[pastingindex + i].insert(composition[pastingindex + i].begin() + j, selection[i].at(k));
							++j;
							++k;
							composition[pastingindex + i].insert(composition[pastingindex + i].begin() + j, selection[i].at(k));
							++j;
							++k;
							if (selection[i].at(k) != '+')
							{
								composition[pastingindex + i].insert(composition[pastingindex + i].begin() + j, selection[i].at(k));
								++j;
								++k;
							}
							++note_count;
						}
					}
					else
					{
						if (j + 3 < composition[pastingindex + i].size())//extra preventive measure to prevent going off the vector
						{
							if (composition[pastingindex + i].at(j + 3) != '+')
							{
								++j;
							}
						}
						j += 3;//note: make sure this ends up at a '+'
					}
				}
				else
				{
					if (composition[pastingindex + i].at(j + 2) != '+')
					{
						++j;
					}
					j += 2;//note: make sure this ends up at a '+'
				}
			}

			if (composition[pastingindex + i].at(0) == '+')
			{
				composition[pastingindex + i].erase(composition[pastingindex + i].begin());//erase the '+'
				composition[pastingindex + i].insert(composition[pastingindex + i].begin() + composition[pastingindex + i].size() - 1, '+');//put the '+' in the back before the volume data
			}
			++i;
			j = 0;
		}
		else//nothing to paste
		{
			++i;
		}
	}
}

bool copypaste::checkredundant(std::vector<char> measure, char instrument, char staffindex){

	for (unsigned unsigned int i = 0; i < measure.size() - 1; ++i)
	{
		if (measure.at(i) == instrument)
		{
			++i;
			if (measure.at(i) == staffindex)
				return true;
		}
	}
	return false;
}

void copypaste::replace(std::vector<char>* selection, char whattoreplace, char replacewith, int selectionlength){

	for (unsigned int i = 0; i < selectionlength; ++i)
	{
		if (!selection[i].empty())
		{
			for (unsigned int j = 0; j < selection[i].size() - 1; ++j)
			{
				if (selection[i].at(j) != '+')
				{
					if (selection[i].at(j) == whattoreplace)
					{
						selection[i].erase(selection[i].begin() + j);
						selection[i].insert(selection[i].begin() + j, replacewith);
					}
					if (selection[i].at(j + 2) != '+')
					{
						++j;
					}
					++j;
				}
			}//++j
		}
	}
	//return selection;
}

void copypaste::staffrange(char staffbeg, char staffend, std::vector<char>* selection, int selectionlength, bool remove){

	//http://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
	unsigned int i = 0;
	unsigned int j = 0;
	while (i < selectionlength)
	{
		if (j != selection[i].size() && j != selection[i].size() - 1)//second parameter to ensure don't modify volume data
		{
			if (selection[i].at(j) != '+')
			{
				if ((int)selection[i].at(j + 1) < (int)staffbeg || (int)selection[i].at(j + 1) > (int)staffend)
				{		
					if (!remove)
					{
						if (selection[i].at(j + 2) != '+')//natural, flat, or sharp
						{
							selection[i].erase(selection[i].begin() + j + 2);//note: erases (j + 2)th element, the flat, sharp or natural modifier
						}
						selection[i].erase(selection[i].begin() + j + 1);//note: erases (j + 1)th element, the staff position
						selection[i].erase(selection[i].begin() + j);//note: erases jth element, the instrument type
						selection[i].erase(selection[i].begin() + j);//erase the '+'
						selection[i].insert(selection[i].begin() + selection[i].size() - 1, '+');//put the '+' in the back before the volume data
					}
					else
					{
						//check for naturals; 
						//selection will be changed so natural goes from 17 into q7 which is one value more on the ascii table than 'p'(last note) before being passed in					
						if (selection[i].at(j + 2) != '+')//natural, flat, or sharp
						{
							++j;
						}
						j += 2;
					}
				}
				else
				{
					if (!remove)
					{
						//check for naturals; 
						//selection will be changed so natural goes from 17 into q7 which is one value more on the ascii table than 'p'(last note) before being passed in
						if (selection[i].at(j + 2) != '+')//natural, flat, or sharp
						{
							++j;
						}
						j += 2;
					}
					else
					{
						if (selection[i].at(j + 2) != '+')//natural, flat, or sharp
						{
							selection[i].erase(selection[i].begin() + j + 2);//note: erases (j + 2)th element, the flat, sharp or natural modifier
						}
						selection[i].erase(selection[i].begin() + j + 1);//note: erases (j + 1)th element, the staff position
						selection[i].erase(selection[i].begin() + j);//note: erases jth element, the instrument type
						selection[i].erase(selection[i].begin() + j);//erase the '+'
						selection[i].insert(selection[i].begin() + selection[i].size() - 1, '+');//put the '+' in the back before the volume data
					}
				}
			}
			else
			{
				++j;
			}
		}
		else//ignore following ->//note: can probably change copy to just have this one if-statement at the end as well//note: actually do not do this
		{
			++i;
			j = 0;
		}
	}
}

//--------------------------------------------------
//	volume modulation copy and paste functions
//--------------------------------------------------

std::vector<char>* copypaste::volumecopy(const std::vector<char>* composition, int beginningindex, int endingindex){

	std::vector<char>* selection = new std::vector<char>[1];
	for (unsigned int i = beginningindex; i <= endingindex; ++i)
	{
		if (composition[i].empty())
			selection[0].push_back('~');//let '~' signify there is nothing
		else
			selection[0].push_back(composition[i].back());
	}
	return selection;
}

void copypaste::volumeremove(std::vector<char>* composition, int beginningindex, int endingindex){

	for (unsigned int i = beginningindex; i <= endingindex; ++i)
	{
		if (composition[i].empty())
		{
			//nothing
		}
		else
		{
			composition[i].pop_back();
			composition[i].push_back('q');
		}
	}
}

void copypaste::volumepaste(std::vector<char>* composition, int pastingindex, std::vector<char>* selection, int selectionlength){

	for (unsigned int i = 0; i < selectionlength; ++i)
	{
		if (selection[0].at(i) != '~')
		{
			if (composition[pastingindex + i].empty())
			{
				for (unsigned int j = 0; j < 6; ++j)
				{
					composition[pastingindex + i].push_back('+');
				}
				composition[pastingindex + i].push_back(selection[0].at(i));
			}
			else
			{
				composition[pastingindex + i].pop_back();
				composition[pastingindex + i].push_back(selection[0].at(i));
			}
		}
	}
}

void copypaste::v_slide(std::vector<char>* composition, int beginningindex, int endingindex, bool decrease){

	if (decrease)
	{
		for (unsigned int i = beginningindex; i <= endingindex; ++i)
		{
			if (!composition[i].empty())
			{
				if (composition[i].back() - 1 > 96)
				{
					composition[i].push_back(composition[i].back() - 1);
					composition[i].erase(composition[i].end() - 2);//second to last meaning referring to previous volume
				}
			}
		}
	}
	else
	{
		for (unsigned int i = beginningindex; i <= endingindex; ++i)
		{
			if (!composition[i].empty())
			{
				if (composition[i].back() + 1 < 114)
				{
					composition[i].push_back(composition[i].back() + 1);
					composition[i].erase(composition[i].end() - 2);
				}
			}
		}
	}
}

//--------------------------------------------------
//	special filter copy and remove functions (note: no paste, etc.)
//--------------------------------------------------

void copypaste::specialcheck(const std::vector<char> &composition_at_i, std::string &spec_notes_tracker, unsigned int spec_check, const std::string &notes, std::string &potential_selection_filtered_notes, const std::string &special_filter){

	for (unsigned int j = 0; j < composition_at_i.size() - 1; ++j)
	{
		if (composition_at_i.at(j) != '+')
		{
			for (unsigned int k = spec_notes_tracker.size() - spec_check; k < spec_notes_tracker.size(); k += 3)
			{
				if (composition_at_i.at(j) == spec_notes_tracker.at(k) && composition_at_i.at(j + 1) == spec_notes_tracker.at(k + 1) && composition_at_i.at(j + 2) == spec_notes_tracker.at(k + 2))//check if satisfy special measure
				{
					spec_check -= 3;//one note satisfied
					spec_notes_tracker.insert(spec_notes_tracker.begin(), spec_notes_tracker.begin() + k, spec_notes_tracker.begin() + k + 3);//move characters over
					spec_notes_tracker.erase(spec_notes_tracker.begin() + k + 3, spec_notes_tracker.begin() + k + 6);
					break;
				}
			}
			for (unsigned int l = 0; l < notes.size(); ++l)//check if satisfy filtered notes
			{
				if (composition_at_i.at(j) == notes.at(l))
				{
					filtered_notes_exist = true;
					potential_selection_filtered_notes.insert(potential_selection_filtered_notes.end(), composition_at_i.begin() + j, composition_at_i.begin() + j + 3);
					break;
				}
			}
			j += 2;//this will increment onto another note or '+'
		}
	}

	if (notes.empty())
		filtered_notes_exist = true;

	if (spec_check == 0)
		special_measure_exist = true;

	if (special_filter.back() != '+')//there is special volume that must be considered//NOTE: there is only one case where volume_exist is false: when volume is specified but not met by composition[i]
	{
		if (composition_at_i.back() == special_filter.back())//that volume is satisfied
			volume_exist = true;
	}
	else//there is no special volume that must be considered so any volume is satisfied
		volume_exist = true;
}

std::vector<char>* copypaste::specialcopy(const std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, const std::string &notes, int selectionlength, const std::string &special_filter){

	std::vector<char>* selection = new std::vector<char>[selectionlength];

	std::string spec_notes_tracker = "";//this will compare special_filter with the measure to see if they are the same
	spec_notes_tracker.reserve(15);
	for (unsigned int i = 1; i < special_filter.size() - 1; ++i)//ignore volume for now
	{
		if (special_filter.at(i) != '+')
		{
			spec_notes_tracker.push_back(special_filter.at(i));
			++i;
			spec_notes_tracker.push_back(special_filter.at(i));
			if (special_filter.at(i + 1) != '+')//flat,sharp, or natural
			{
				++i;
				spec_notes_tracker.push_back(special_filter.at(i));
			}
			else
				spec_notes_tracker.push_back('+');
		}
	}
	unsigned int spec_check = spec_notes_tracker.size();
	//std::vector<int> wrong_notes_tracker;
	//wrong_notes_tracker.reserve(10);

	filtered_notes_exist = false;
	special_measure_exist = false;
	volume_exist = false;

	std::string potential_selection_filtered_notes = "";
	potential_selection_filtered_notes.reserve(18);//5 triplets, extra 3 for rearranging elements via insert then erase

	for (unsigned int i = beginningindex; i <= endingindex; ++i)
	{
		if (!composition[i].empty())
		{

			specialcheck(composition[i], spec_notes_tracker, spec_check, notes, potential_selection_filtered_notes, special_filter);

			if (special_filter.at(0) == '&')//AND
			{
				if (filtered_notes_exist && special_measure_exist && volume_exist)
				{
					unsigned int p = 0;
					for (unsigned int k = 0; k < spec_notes_tracker.size(); k += 3)
					{
						selection[i - beginningindex].insert(selection[i - beginningindex].end(), spec_notes_tracker.begin() + k, spec_notes_tracker.begin() + k + 3);
						if (spec_notes_tracker.at(k + 2) != '+')
							selection[i - beginningindex].push_back('+');
						++p;
					}
					unsigned int q = 0;
					for (unsigned int l = 0; l < potential_selection_filtered_notes.size(); l += 3)//check if filtered note already exists on the special measure//it doesn't matter if it already exists
					{
						for (unsigned int k = 0; k < spec_notes_tracker.size(); k += 3)
						{
							if (potential_selection_filtered_notes.at(l) == spec_notes_tracker.at(k)
								&& potential_selection_filtered_notes.at(l + 1) == spec_notes_tracker.at(k + 1)
								&& potential_selection_filtered_notes.at(l + 2) == spec_notes_tracker.at(k + 2))
							{
								potential_selection_filtered_notes.insert(potential_selection_filtered_notes.begin(), potential_selection_filtered_notes.begin() + l, potential_selection_filtered_notes.begin() + l + 3);
								potential_selection_filtered_notes.erase(potential_selection_filtered_notes.begin() + l, potential_selection_filtered_notes.begin() + l + 3);
								q += 3;
							}
						}
					}
					for (unsigned int l = q; l < potential_selection_filtered_notes.size(); l += 3)//check if filtered note already exists on the special measure
					{
						selection[i - beginningindex].insert(selection[i - beginningindex].end(), potential_selection_filtered_notes.begin() + l, potential_selection_filtered_notes.begin() + l + 3);
						if (potential_selection_filtered_notes.at(l + 2) != '+')
							selection[i - beginningindex].push_back('+');
						++p;
					}
					for (p; p < 6; ++p)
					{
						selection[i - beginningindex].push_back('+');
					}
					selection[i - beginningindex].push_back(composition[i].back());
				}
			}
			else//OR
			{
				unsigned int p = 0;
				if (special_measure_exist && volume_exist)
				{
					for (unsigned int k = 0; k < spec_notes_tracker.size(); k += 3)
					{
						selection[i - beginningindex].insert(selection[i - beginningindex].end(), spec_notes_tracker.begin() + k, spec_notes_tracker.begin() + k + 3);
						if (spec_notes_tracker.at(k + 2) != '+')
							selection[i - beginningindex].push_back('+');
						++p;
					}				
				}
				if (filtered_notes_exist)
				{
					unsigned int q = 0;
					if (special_measure_exist)
					{
						for (unsigned int l = 0; l < potential_selection_filtered_notes.size(); l += 3)//check if filtered note already exists on the special measure
						{
							for (unsigned int k = 0; k < spec_notes_tracker.size(); k += 3)
							{
								if (potential_selection_filtered_notes.at(l) == spec_notes_tracker.at(k)
									&& potential_selection_filtered_notes.at(l + 1) == spec_notes_tracker.at(k + 1)
									&& potential_selection_filtered_notes.at(l + 2) == spec_notes_tracker.at(k + 2))
								{
									potential_selection_filtered_notes.insert(potential_selection_filtered_notes.begin(), potential_selection_filtered_notes.begin() + l, potential_selection_filtered_notes.begin() + l + 3);
									potential_selection_filtered_notes.erase(potential_selection_filtered_notes.begin() + l, potential_selection_filtered_notes.begin() + l + 3);
									q += 3;
								}
							}
						}
					}
					for (unsigned int l = q; l < potential_selection_filtered_notes.size(); l += 3)//check if filtered note already exists on the special measure
					{
						selection[i - beginningindex].insert(selection[i - beginningindex].end(), potential_selection_filtered_notes.begin() + l, potential_selection_filtered_notes.begin() + l + 3);
						if (potential_selection_filtered_notes.at(l + 2) != '+')
							selection[i - beginningindex].push_back('+');
						++p;
					}
				}

				if ((special_measure_exist && volume_exist) || filtered_notes_exist)
				{
					for (p; p < 6; ++p)
					{
						selection[i - beginningindex].push_back('+');
					}
					if (volume_exist)
						selection[i - beginningindex].push_back(composition[i].back());
					else
						selection[i - beginningindex].push_back('q');
				}
			}

			//spec_check = spec_notes_tracker.size();
			filtered_notes_exist = special_measure_exist = volume_exist = false;
			potential_selection_filtered_notes.clear();

		}
	}
	/*for (unsigned int w = 0; w < selectionlength; ++w)
	{
		for (unsigned int v = 0; v < selection[w].size(); ++v)
		{
			std::cout << selection[w].at(v);
		}
		std::cout << "~END" << std::endl;
	}*/
	return selection;
}

void copypaste::specialremove(std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, const std::string &notes, const std::string &special_filter){

	std::string spec_notes_tracker = "";//this will compare special_filter with the measure to see if they are the same
	spec_notes_tracker.reserve(15);
	for (unsigned int i = 1; i < special_filter.size() - 1; ++i)//ignore volume for now
	{
		if (special_filter.at(i) != '+')
		{
			spec_notes_tracker.push_back(special_filter.at(i));
			++i;
			spec_notes_tracker.push_back(special_filter.at(i));
			if (special_filter.at(i + 1) != '+')//flat,sharp, or natural
			{
				++i;
				spec_notes_tracker.push_back(special_filter.at(i));
			}
			else
				spec_notes_tracker.push_back('+');
		}
	}
	unsigned int spec_check = spec_notes_tracker.size();
	//std::vector<int> wrong_notes_tracker;
	//wrong_notes_tracker.reserve(10);

	filtered_notes_exist = false;
	special_measure_exist = false;
	volume_exist = false;

	std::string potential_selection_filtered_notes = "";
	potential_selection_filtered_notes.reserve(18);//5 triplets, extra 3 for rearranging elements via insert then erase

	for (unsigned int i = beginningindex; i <= endingindex; ++i)
	{
		if (!composition[i].empty())
		{

			specialcheck(composition[i], spec_notes_tracker, spec_check, notes, potential_selection_filtered_notes, special_filter);

			if (special_filter.at(0) == '&')//AND
			{
				if (filtered_notes_exist && special_measure_exist && volume_exist)
				{
					for (unsigned int k = 0; k < spec_notes_tracker.size(); k += 3)
					{
						for (unsigned int d = 0; d < composition[i].size() - 1; ++d)
						{
							if (composition[i].at(d) != '+')
							{
								if (composition[i].at(d) == spec_notes_tracker.at(k) && composition[i].at(d + 1) == spec_notes_tracker.at(k + 1) && composition[i].at(d + 2) == spec_notes_tracker.at(k + 2))
								{
									composition[i].erase(composition[i].begin() + d, composition[i].begin() + d + 2);//d + 2 or d + 1??
									if (composition[i].at(d) != '+')//flat,sharp,natural
										composition[i].erase(composition[i].begin() + d);
								}
								else
									d += 2;
							}
						}
					}
					/*if (special_filter.back() != '+')
					{
						composition[i].pop_back();
						composition[i].push_back('q');
					}//else ignore the volume*/ //ignore it in general
					if (!notes.empty())//when empty, filtered_notes_exist will be true mainly to satisfy this AND branch but do not want to remove without filtered notes
						remove(composition, i, i, staffbeg, staffend, notes);
				}
			}
			else//OR
			{
				if (special_measure_exist && volume_exist)
				{
					for (unsigned int k = 0; k < spec_notes_tracker.size(); k += 3)
					{
						for (unsigned int d = 0; d < composition[i].size() - 1; ++d)
						{
							if (composition[i].at(d) != '+')
							{
								if (composition[i].at(d) == spec_notes_tracker.at(k) && composition[i].at(d + 1) == spec_notes_tracker.at(k + 1) && composition[i].at(d + 2) == spec_notes_tracker.at(k + 2))
								{
									composition[i].erase(composition[i].begin() + d, composition[i].begin() + d + 2);//d + 2 or d + 1??
									if (composition[i].at(d) != '+')//flat,sharp,natural
										composition[i].erase(composition[i].begin() + d);
								}
								else
									d += 2;
							}
						}
					}
					/*if (special_filter.back() != '+')
					{
						composition[i].pop_back();
						composition[i].push_back('q');
					}//else ignore the volume*/ //ignore it in general
				}
				if (filtered_notes_exist && !notes.empty())//when empty, filtered_notes_exist will be true mainly to satisfy this AND branch but do not want to remove without filtered notes
				{
					remove(composition, i, i, staffbeg, staffend, notes);
				}
			}

			//spec_check = spec_notes_tracker.size();
			filtered_notes_exist = special_measure_exist = volume_exist = false;
			potential_selection_filtered_notes.clear();

		}
	}
}

std::vector<char>* copypaste::volumespecialcopy(const std::vector<char>* composition, int beginningindex, int endingindex, const std::string &notes, const std::string &special_filter){

	std::vector<char>* selection = new std::vector<char>[1];
	std::string spec_notes_tracker = "";//this will compare special_filter with the measure to see if they are the same
	spec_notes_tracker.reserve(15);
	for (unsigned int i = 1; i < special_filter.size() - 1; ++i)//ignore volume for now
	{
		if (special_filter.at(i) != '+')
		{
			spec_notes_tracker.push_back(special_filter.at(i));
			++i;
			spec_notes_tracker.push_back(special_filter.at(i));
			if (special_filter.at(i + 1) != '+')//flat,sharp, or natural
			{
				++i;
				spec_notes_tracker.push_back(special_filter.at(i));
			}
			else
				spec_notes_tracker.push_back('+');
		}
	}
	unsigned int spec_check = spec_notes_tracker.size();
	//std::vector<int> wrong_notes_tracker;
	//wrong_notes_tracker.reserve(10);

	filtered_notes_exist = false;
	special_measure_exist = false;
	volume_exist = false;

	std::string potential_selection_filtered_notes = "";
	potential_selection_filtered_notes.reserve(18);//5 triplets, extra 3 for rearranging elements via insert then erase

	for (unsigned int i = beginningindex; i <= endingindex; ++i)
	{
		if (!composition[i].empty())
		{

			specialcheck(composition[i], spec_notes_tracker, spec_check, notes, potential_selection_filtered_notes, special_filter);

			if (special_filter.at(0) == '&')//AND
			{
				if (filtered_notes_exist && special_measure_exist && volume_exist)
					selection[0].push_back(composition[i].back());
				else
					selection[0].push_back('~');
			}
			else//OR
			{
				if (special_measure_exist && volume_exist) 
					selection[0].push_back(composition[i].back());
				else if (filtered_notes_exist)
					selection[0].push_back(composition[i].back());
				else if (!special_measure_exist && !volume_exist && !filtered_notes_exist)
					selection[0].push_back('~');
			}

			filtered_notes_exist = special_measure_exist = volume_exist = false;
			potential_selection_filtered_notes.clear();

		}
		else
			selection[0].push_back('~');
	}

	return selection;
}

void copypaste::volumespecialremove(std::vector<char>* composition, int beginningindex, int endingindex, const std::string &notes, const std::string &special_filter){

	std::string spec_notes_tracker = "";//this will compare special_filter with the measure to see if they are the same
	spec_notes_tracker.reserve(15);
	for (unsigned int i = 1; i < special_filter.size() - 1; ++i)//ignore volume for now
	{
		if (special_filter.at(i) != '+')
		{
			spec_notes_tracker.push_back(special_filter.at(i));
			++i;
			spec_notes_tracker.push_back(special_filter.at(i));
			if (special_filter.at(i + 1) != '+')//flat,sharp, or natural
			{
				++i;
				spec_notes_tracker.push_back(special_filter.at(i));
			}
			else
				spec_notes_tracker.push_back('+');
		}
	}
	unsigned int spec_check = spec_notes_tracker.size();
	//std::vector<int> wrong_notes_tracker;
	//wrong_notes_tracker.reserve(10);

	filtered_notes_exist = false;
	special_measure_exist = false;
	volume_exist = false;

	std::string potential_selection_filtered_notes = "";
	potential_selection_filtered_notes.reserve(18);//5 triplets, extra 3 for rearranging elements via insert then erase

	for (unsigned int i = beginningindex; i <= endingindex; ++i)
	{
		if (!composition[i].empty())
		{

			specialcheck(composition[i], spec_notes_tracker, spec_check, notes, potential_selection_filtered_notes, special_filter);

			if (special_filter.at(0) == '&')//AND
			{
				if (filtered_notes_exist && special_measure_exist && volume_exist)
				{
					composition[i].pop_back();
					composition[i].push_back('q');
				}//else don't remove anything
			}
			else//OR
			{
				if (special_measure_exist && volume_exist)
				{
					composition[i].pop_back();
					composition[i].push_back('q');
				}
				else if (filtered_notes_exist)
				{
					composition[i].pop_back();
					composition[i].push_back('q');
				}//else don't remove anything
			}

			filtered_notes_exist = special_measure_exist = volume_exist = false;
			potential_selection_filtered_notes.clear();

		}
	}//else don't remove anything
}