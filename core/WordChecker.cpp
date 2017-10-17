// WordChecker.cpp
//
// ICS 46 Spring 2016
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
//#include <iostream>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
	return this->words.contains(word);
    // return false;
}

// bool isUniqueExist(std::vector<std::string> slist, std::string word)
// {
// 	return wordExists(word) && std::find(slist.begin(), slist.end(), word) == slist.end();
// }


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> suggestlist;

	// seperate words with white space
	for(int i=0; i<word.size(); i++)
    {
        std::string sub1 = word.substr(0,i);
        std::string sub2 = word.substr(i,word.size()-i);

        if (wordExists(sub1) && wordExists(sub2))
        	suggestlist.push_back(sub1+' '+sub2);
    }


    // delete one char
    for(int i=0; i<word.size(); i++)
    {
    	std::string oneless = word;
    	oneless.erase(i, 1);
    	if(wordExists(oneless) && (std::find(suggestlist.begin(), suggestlist.end(), oneless) == suggestlist.end() ))
    		suggestlist.push_back(oneless);
    }


    // insert one char
    for(int i=0; i<word.size()+1; i++)
    	for(char c='A'; c<'A'+26; c++)
    	{
    		std::string onemore = word;
    		onemore.insert(onemore.begin()+i, c);

    		if(wordExists(onemore) && (std::find(suggestlist.begin(), suggestlist.end(), onemore) == suggestlist.end() ))
    			suggestlist.push_back(onemore);
    	}


    // replace one char
    for(int i=0; i<word.size(); i++)
    	for(char c='A'; c<'A'+26;c++)
    	{
    		std::string replaceone = word;
    		replaceone[i] = c;

    		if(wordExists(replaceone) && (std::find(suggestlist.begin(), suggestlist.end(), replaceone) == suggestlist.end() ))
    			suggestlist.push_back(replaceone);
    	}


    // swap adjacent char
    for(int i=0; i<word.size(); i++)
    {
    	char temp;
    	std::string swapchar = word;
    	temp = swapchar[i];
    	swapchar[i] = swapchar[i+1];
    	swapchar[i+1] = temp;

    	if(wordExists(swapchar) && (std::find(suggestlist.begin(), suggestlist.end(), swapchar) == suggestlist.end() ))
    			suggestlist.push_back(swapchar);
    }

    return suggestlist;
}

