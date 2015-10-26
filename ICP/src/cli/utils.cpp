/**
  * @file utils.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Definition of utilities
  */

#include "utils.h"

/**
* @brief Helper for creating Position structure
* @param int x
* @param int y
* @return Position
*/
Position cpos(int x, int y){
    Position pos = { .x = x, .y = y };

    return pos;
}

/**
* @brief Function for removing whitespaces
* @param string& input
* @return string&
*/
string& trim(string& input){
    input.erase(find_if(input.rbegin(), input.rend(), not1(ptr_fun<int, int>(isspace))).base(), input.end());

    return input;
}

/**
* @brief Function for parsing input
* @param string& input
* @param char separator
* @return string 
*/
string parseData(string& input, char separator){
    istringstream sIn(input);
    ostringstream sOut;
    int pos = 0;
    char c;

    sIn >> noskipws;
    while(sIn >> c){
        pos++;
        if(c == separator || c == ';')
            break;

        sOut << c;
    }

    input.erase(0, pos);
    return sOut.str();
}

/**
* @brief Function for converting string to int
* @param string input
* @return int
*/
int stringToInt(string input){
    int i;

    istringstream s(input);
    s >> i;

    return i;
}

/**
* @brief Function for replacing string
* @param string input
* @param const string subject
* @param const string replacement
* @return string
*/
string stringReplace(string input, const string subject, const string replacement){
    if(subject.empty())
        return input;

    size_t pos = 0;

    while((pos = input.find(subject, pos)) != std::string::npos) {
        input.replace(pos, subject.length(), replacement);
        pos += replacement.length();
    }

    return input;
}

/**
* @brief Function for determining allowed moves
* @param deque<Position> positions
* @param Position pos
* @return bool
*/
bool contains(deque<Position> positions, Position pos){

    for(deque<Position>::iterator p = positions.begin(); p != positions.end(); p++)
        if((*p).x == pos.x && (*p).y == pos.y)
            return true;

    return false;
}
