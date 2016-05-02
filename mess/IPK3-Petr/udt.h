#ifndef UDT_H
#define UDT_H

#include <sstream>
#include <sys/time.h>

using namespace std;

class UDT
{
    public:
        UDT();
        UDT(unsigned int, string);
		
		string toS();
        string toA();

        void setW(int);
        void setT(unsigned int);

        unsigned int getSN();
        unsigned int getT();
        string getD();
	private:
		string toString(int);
    protected:
        unsigned int SN;
		int W;
		unsigned int T;
        string D;
};

#endif // UDT_H
