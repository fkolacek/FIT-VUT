/**
  * @file exception.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Definition of generic Exception class
  */

#include "exception.h"

/**
* @brief Constructor for class Exception
* @param string message
* @param int line
* @param string file
*/
Exception::Exception(string message, int line, string file){
    this->message = message;
    this->line = line;
    this->file = file;
}

/**
* @brief Getter for message attribute
* @return string message
*/
string Exception::getMessage(){
    return this->message;
}

/**
* @brief Getter for line attribute
* @return int line
*/
int Exception::getLine(){
    return this->line;
}

/**
* @brief Getter for file attribute
* @return string file
*/
string Exception::getFile(){
    return this->file;
}
