// Project 2 Final Code - Search Engine
// Author:Pedro Pardo
// UIN: 656286719
// IDE: Mac using VSCODE

// This file is used to run the whole program, after the Search function is called within main 
// this program will take a file choice and search term to create a search engine.
// It will return all the URL's associated with the terms searched, if they are valid with the file.
// Within this program is a creative component that will remove unecessary terms, known as stop words,
// from the main index. Without these terms it will mmake it so only important terms are within the index.
// In order to use this said creative component please uncomment out line 121 from within buildIndex,
// that starts on line 108. By uncommenting this line out the program will run as normal but without the 
// stop wordw within the index.

#pragma once

#include <cctype>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
using namespace std;


// This crealrd the punctuation from the word and lowers all the characters
void clearPunct(string &s ,  char &c){
    if (ispunct(s.front())){ //removes front punctuation
        s.erase(s.begin());
    }
    if ( ispunct(s.back()) ){ //removes back punctuation
        s.pop_back();
    }
    c = tolower(c); //lowercases all letters
}

// This function clears a token of all punctuation and checks if the word is valid
string cleanToken(string s) {
    int i = 0;
    for(auto& c : s){ //checks if the word has a valid character
        if(isalpha(c)){
            i++;
        }
    }
    if(i > 0){ //only runs if word is valid
        for(char &c : s){
            clearPunct(s, c);
        }
        return s;
    }
    else{ //if word isnt valid returns an empty string
        return "";
    }
}

// This function retrieves the words from the file and runs it through the cleaning algorithm and inserts thw words into a set of strings
set<string> gatherTokens(string text){
    set<string> tokens;
    tokens.clear(); //clears the set after every call
    stringstream s(text);
    string token;
    while (getline(s, token, ' ')){ //gets each word sperated by a space
        tokens.insert(cleanToken(token)); //inserts the token into the set once its cleaned
        if(cleanToken(token) == ""){ //if the token is empty it removes it from the set
            tokens.erase(cleanToken(token));
        }
    }
   return tokens;
}

//This function is used for the creative component to remove all stop words from the index
void mergeIndex(map<string, set<string>>& index){
    string stopWords = "a about above after again against all am an and any are aren't as at be because been before being below between both but by can't cannot could couldn't did didn't do does doesn't doing don't down during each few for from further had hadn't has hasn't have haven't having he he'd he'll he's her here here's hers herself him himself his how how's i i'd i'll i'm i've if in into is isn't it it's its itself let's me more most mustn't my myself no nor not of off on once only or other ought our ours  ourselves out over own same shan't she she'd she'll she's should shouldn't so some such than that that's the their theirs them themselves then there there's these they they'd they'll they're they've this those through to too under until up very was wasn't we we'd we'll we're we've were weren't what what's when when's where where's which while who who's whom why why's with won't would wouldn't you you'd you'll you're you've your yours yourself yourselves";
    stringstream s(stopWords);
    string word;
    while (s >> word){ //gets each word from the stop words
        for (auto &key :index){
            if (key.first.find(word)){ //if the stop word is the word in the index it removes it
                index.erase(word);
            }
        }
    }
}

//This function inserts the tokens from the file into the index after its been cleaned along with its url
void insertIndex(string &s, string &url, map<string, set<string>>& index){
    for(auto &tokens: gatherTokens(s)){
        index[cleanToken(tokens)].insert(url);
    }
}

//This function takes into accout the line of the file to decide wether it should be a url or body text
void indexCount(int &count, string &s, string &url, map<string, set<string>>& index, int &websiteCount){
    if(count % 2 == 0){ //since body texts are only even numbered lines it would run this for the body
        insertIndex(s, url, index);
    }
    else{ //url function
        url = s;
        websiteCount++;
    }
}

//This function builds the index based of the file thats chosen
int buildIndex(string filename, map<string, set<string>>& index){
    set<string> website;
    ifstream file;
    int count = 0, websiteCount = 0;
    file.open(filename); //this checks if the file is valid and opens it 
    if(file.is_open()){
        ifstream infile (filename);
        string s, url;
        while(getline(infile, s)){ //gets each line within the file and runs it though the index count
            count++;
            indexCount(count, s, url, index, websiteCount);
        }
        //Uncomment for creative
        // mergeIndex(index);
        //end of creative
        return websiteCount;
    }
    else{ //if file cant be opened it will return 0
        return 0;
    }
}


//This function inserts a regular search word from the index and puts it into the set of reults 
void findNonSpecialWord(string &word, map<string, set<string>>& index, set<string> &result){
    for (auto &pair :index){ //gets the first word within the map
        if (word == pair.first){ //check if the word within the search is the same as the index term
            for (auto &url : pair.second){ //gets the url from the set
                result.insert(url); //inserts the url into the result set
            }
        }
    }
}

//This function inserts a "+" word from the search into the result set
void insertToPlus(string &word, map<string, set<string>>& index, set<string> &result, set<string> &search){
    search.clear(); //clears the searh set which is used as a temp set
    for (auto &pair :index){ //gets the first word within the map
        if (word == pair.first){ //check if the word within the search is the same as the index term
            set_intersection(result.begin(), result.end(), pair.second.begin(), pair.second.end(), inserter(search, search.end()));
            //this combines the two sets into one based on if both search words have the same urls in commmon
        }
    }
}

//This function inserts a "-" word from the search into the result set
void insertToMinus(string &word, map<string, set<string>>& index, set<string> &result, set<string> &search){
    search.clear(); //clears the searh set which is used as a temp set
    for (auto &pair :index){ //gets the first word within the map
        if (word == pair.first){ //check if the word within the search is the same as the index term
            set_difference(result.begin(), result.end(), pair.second.begin(), pair.second.end(), inserter(search, search.end()));
            //this combines the two sets into one based on if the results has a url that the search word doesnt
        }
    }
}

//This function is used to find the matches within the search term and the index
set<string> findQueryMatches(map<string, set<string>>& index, string &sentence) {
    set<string> result;
    set<string> search;
    string word;
    int wordCount = 0;
    stringstream input(sentence);

    while (input >> word){ //this loops though the search terms
        wordCount++; //this is used to find the first word within the stringstream
        if (wordCount == 1){ //since the first term is always going to be clean yo can store this one every time by itself
            word = cleanToken(word); //clean the word 
            findNonSpecialWord(word, index, result); //insert into the set
        }
        if (word[0] != '-' && word[0] != '+'){ //this is used to check for the words following the inital word if theyre not "+" or "-"
            word = cleanToken(word);
            findNonSpecialWord(word, index, result);
        }
        if (word[0] == '-'){ //this is used to run the function to combine the set with the difference
            word = cleanToken(word);
            insertToMinus(word, index, result, search);
            result = search; //sets the combined set to the main set
        }
        else if (word[0] == '+'){ //this is used to run the function to combine the set with the intersection
            word = cleanToken(word);
            insertToPlus(word, index, result, search);
            result = search;//sets the combined set to the main set
        }
    }
    cout << "Found " << result.size() << " matching pages" << endl; //outputs ststemt on the number of websites found
    for(auto &tokens: result){
        cout << tokens << endl; //outputs the webites
    }
   return result;
}


//this function is used to reccursivly call after each search until the exit input is inputted
void functionReccursion(map<string, set<string> > &index, string &sentence){
    cout << endl << "Enter query sentence (press enter to quit): ";
    getline (cin, sentence);
    if(sentence == "\0"){
        cout << "Thank you for searching!" << endl;
    }
    else{
        findQueryMatches(index, sentence); //calls the fucntion to search the set for the urls based of the input
        functionReccursion(index, sentence); //recalls the function for next input
    }
}

// This function is used to call every function with one. Main function that makes the search function
void searchEngine(string filename){
    string sentence;
    map<string, set<string> > index;
    buildIndex(filename, index); //builds index based off file chosen
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << buildIndex(filename, index) << " pages containing " <<  index.size() << " unique terms" << endl; //outputs info based of file chosen
    functionReccursion(index, sentence); //calls function to take input
}

