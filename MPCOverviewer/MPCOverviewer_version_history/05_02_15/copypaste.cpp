
#include "copypaste.h"
#include <iostream>

std::vector<char>* copypaste::copy(const std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, std::string notes, int selectionlength){

	std::vector<char>* selection = new std::vector<char>[endingindex - beginningindex + 1];
	int i = beginningindex;
	int j = 0;
	if (notes.empty())//no filter is applied
	{
		//get everything in the range defined by beginningindex and endingindex
		while (i <= endingindex)
		{
			if (j != composition[i].size())
			{
				selection[i - beginningindex].push_back(composition[i].at(j));
				j++;
			}
			else
			{
				i++;
				j = 0;
			}
		}
	}
	else
	{
		//look for charAt_char_+; check for charAt_char_; or charAt_char_# for each character in string 'notes' (loop multiple times)
		int k = 0;
		while (i <= endingindex)
		{
			if (j != composition[i].size())//if empty at i, will just ignore
			{
				while (k < notes.length())
				{
					if (composition[i].at(j) == notes.at(k))
					{
						selection[i - beginningindex].push_back(composition[i].at(j));
						j++;
						selection[i - beginningindex].push_back(composition[i].at(j));
						j++;
						if (composition[i].at(j) != '+')//natural, flat, or sharp
						{
							selection[i - beginningindex].push_back(composition[i].at(j));
							j++;
						}
						//selection[i - beginningindex].push_back(composition[i].at(j));
						//j++;
						k = 0;
					}
					else
					{
						k++;
					}
				}
				k = 0;
				if (composition[i].at(j) == '+')
				{
					selection[i - beginningindex].push_back(composition[i].at(j));
					j++;
				}
				else
				{
					if (j == composition[i].size() - 1)//volume data; include this
					{
						selection[i - beginningindex].push_back(composition[i].at(j));
						j++;
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
				i++;
				j = 0;
			}
		}
	}
	//modify selection based on staffbeginningindex (staffbeg) and staffendingindex (staffend) here
	//selectionlength = endingindex - beginningindex + 1;<- this is what selectionlength is supposed to be passed in as
	staffrange(staffbeg, staffend, selection, selectionlength, false);
	return selection;
}

void copypaste::remove(std::vector<char>* composition, int beginningindex, int endingindex, char staffbeg, char staffend, std::string notes){

	int i = beginningindex;
	int j = 0;
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
			i++;
		}*/
		//for staffrange: pointer math for composition and pass into staffrange with remove true
		staffrange(staffbeg, staffend, composition + beginningindex, endingindex - beginningindex + 1, true);
	}
	else
	{
		//look for charAt_char_+; check for charAt_char_; or charAt_char_# for each character in string 'notes' (loop multiple times)
		int k = 0;
		while (i <= endingindex)
		{
			if (j < composition[i].size())
			{
				while (k < notes.length())
				{
					if (composition[i].at(j) == notes.at(k))
					{
						if ((int)composition[i].at(j + 1) < (int)staffbeg || (int)composition[i].at(j + 1) > (int)staffend)//FIXED??//THIS CAN GO OFF THE VECTOR, FIX NEEDED//CHECKPOINT
						{
							j += 2;
							if (composition[i].at(j) != '+')//natural, flat, or sharp
							{
								j++;
							}
							//j++;
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
						k++;
					}
				}
				k = 0;
				if (composition[i].at(j) == '+')
				{
					j++;
				}
				else
				{
					if (j == composition[i].size() - 1)//volume data; include this
					{
						j++;
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
				i++;
				j = 0;
			}
		}
	}
}

void copypaste::paste(std::vector<char>* composition, int pastingindex, std::vector<char>* selection, int selectionlength){

	int note_count = 0;//up to 5 per measure
	int i = 0;
	int j = 0;
	while (i < selectionlength)
	{
		if (!selection[i].empty())//there is something to paste
		{
			if (composition[pastingindex + i].empty())
			{
				for (int p = 0; p < 6; p++)
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
						j++;
					}
					j += 2;
					note_count++;
				}
				else
				{
					j++;
				}
			}

			j = 0;
			int k = 0;
			while (note_count < 5 && j < composition[pastingindex + i].size() - 1 && k < selection[i].size() - 1)
			{
				while (selection[i].at(k) == '+')
				{
					k++;
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
							j++;
							composition[pastingindex + i].insert(composition[pastingindex + i].begin() + j, selection[i].at(k));
							j++;
							k++;
							composition[pastingindex + i].insert(composition[pastingindex + i].begin() + j, selection[i].at(k));
							j++;
							k++;
							if (selection[i].at(k) != '+')
							{
								composition[pastingindex + i].insert(composition[pastingindex + i].begin() + j, selection[i].at(k));
								j++;
								k++;
							}
							note_count++;
						}
					}
					else
					{
						if (composition[pastingindex + i].at(j + 3) != '+')
						{
							j++;
						}
						j += 3;//note: make sure this ends up at a '+'
					}
				}
				else
				{
					if (composition[pastingindex + i].at(j + 2) != '+')
					{
						j++;
					}
					j += 2;//note: make sure this ends up at a '+'
				}
			}

			if (composition[pastingindex + i].at(0) == '+')
			{
				composition[pastingindex + i].erase(composition[pastingindex + i].begin());//erase the '+'
				composition[pastingindex + i].insert(composition[pastingindex + i].begin() + composition[pastingindex + i].size() - 1, '+');//put the '+' in the back before the volume data
			}
			i++;
			j = 0;
		}
		else//nothing to paste
		{
			i++;
		}
	}
}

bool copypaste::checkredundant(std::vector<char> measure, char instrument, char staffindex){

	for (unsigned int i = 0; i < measure.size() - 1; ++i)
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

std::vector<char>* copypaste::replace(std::vector<char>* selection, char whattoreplace, char replacewith, int selectionlength){

	for (int i = 0; i < selectionlength; i++)
	{
		for (int j = 0; j < selection[i].size() - 1; j++)
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
					j++;
				}
				j++;
			}
		}//j++
	}
	return selection;
}

void copypaste::staffrange(char staffbeg, char staffend, std::vector<char>* selection, int selectionlength, bool remove){

	//http://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
	int i = 0;
	int j = 0;
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
							j++;
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
							j++;
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
				j++;
			}
		}
		else//ignore following ->//note: can probably change copy to just have this one if-statement at the end as well//note: actually do not do this
		{
			i++;
			j = 0;
		}
	}
}

//--------------------------------------------------
//	volume modulation copy and paste functions
//--------------------------------------------------

std::vector<char>* copypaste::volumecopy(const std::vector<char>* composition, int beginningindex, int endingindex){

	std::vector<char>* selection = new std::vector<char>[1];
	for (int i = beginningindex; i <= endingindex; i++)
	{
		if (composition[i].empty())
			selection[0].push_back('q');
		else
			selection[0].push_back(composition[i].back());
	}
	return selection;
}

void copypaste::volumeremove(std::vector<char>* composition, int beginningindex, int endingindex){

	for (int i = beginningindex; i <= endingindex; i++)
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

	for (int i = 0; i < selectionlength; i++)
	{
		if (composition[pastingindex + i].empty())
		{
			for (int j = 0; j < 6; j++)
			{
				composition[pastingindex + i].push_back('+');
			}
			composition[pastingindex + i].push_back(selection[0].at(i));
		}
		else
		{
			composition[i].pop_back();
			composition[i].push_back(selection[0].at(i));
		}
	}
}