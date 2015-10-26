/**
  * @file exception.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for generic Exception class
  */
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <iostream>
#include <string>

using namespace std;

/**
* @brief Class represents generic Exception
*/
class Exception{
	public:
		Exception(string = "", int = 0, string = "");
		string getMessage();
		int getLine();
		string getFile();
    private:
        string message;
        int line;
        string file;
};
#endif // EXCEPTION_H
