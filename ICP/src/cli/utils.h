/**
  * @file utils.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for utilities
  */

#ifndef UTILS_H
#define UTILS_H

using namespace std;

#include <iostream>
#include <sstream>
#include <algorithm>
#include <deque>

/**
* @brief Struct represents position
*/
typedef struct position{
    int x;
    int y;
} Position;

/**
* @brief Struct represents message (text protocol)
*/
typedef struct message{
    string cmd;
    string arg;
} Message;

Position cpos(int, int);

string &trim(string&);

string parseData(string&, char = ';');

int stringToInt(string);

string stringReplace(string, const string, const string);

bool contains(deque<Position>, Position);

#endif // UTILS_H
