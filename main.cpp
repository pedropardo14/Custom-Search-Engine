// Project 2 Final Code - Search Engine
// Author:Pedro Pardo
// UIN: 656286719
// IDE: Mac using VSCODE

// This file is used to call the "Search.h" file and outputs the main starting message
// It also initallized the main map used and search term.

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
#include "search.h"
using namespace std;

int main() {
    // string text;
    // cin >> text;
    // cleanToken(text);
    // cout << cleanToken(text);
    map<string, set<string> > index;
    string filename, sentence;
    ifstream file;


    // test cases for cleanToken()
    // string ans = "hello";
    // cout << "1. " << (ans == cleanToken(".hello")) << endl;
    // cout << "2. " << (ans == cleanToken("...hello")) << endl;
    // cout << "3. " << (ans == cleanToken("...hello!!!!")) << endl;
    // cout << "4. " << (ans == cleanToken("HELLO")) << endl;
    // cout << "5. " << (ans == cleanToken("HELLO{}")) << endl;
    // cout << "6. " << (ans == cleanToken("1268")) << endl;
    // // cout << endl;
    // cout << "---------------" << endl;


    // //test cases for gatherToken()
    // set<string> tokens = gatherTokens("EGGS! milk, fish,      @  bread cheese");
    // set<string> tokensANS;
    // tokensANS.insert("never");
    // tokensANS.insert("would");
    // tokensANS.insert("could");
    // tokensANS.insert("should");
    // cout << tokens.size() <<endl;
    // for (auto it = tokens.begin(); it != tokens.end(); ++it){
    //     cout << *it << endl;
    // }
    // cout << endl << tokensANS.size() <<endl;
    // cout << "1. " << (tokens.size() == 4) << endl;
    // cout << "2. " << (tokens == tokensANS) << endl;
    // buildIndex(filename,index);
    //fish -two red +blue
    // findQueryMatches(index, sentence);


    cout << "Enter a database (press enter to quit): ";
    getline (cin, filename); //gets input of file 
    // filename = "cplusplus.txt";
    if(filename == "\0"){ //check if input isnt the ending search
        cout << "Thank you for searching!" << endl;
    }
    else{ 
        file.open(filename); //checks if file is valid 
        if(file.is_open()){ //opens file 
            searchEngine(filename);  //calls search.h search function file
        }
    }

    return 0;
}
