#ifndef BEE_H
#define BEE_H

#include "scheduler.h"

class Bee : public Process{
	public:
		Bee(Role);
		
		void Behavior();
		void selectRole();
		void eat();
		
		inline Role getRole(){ return this->role; }
		inline Season getSeason(){ return this->season; }
		inline int getAge(){ return this->age; }
		inline int getDie(){ return this->die; }
	private:
		Role role;
		Season season;
		unsigned int age;
		unsigned int die;
		unsigned int mod;
		bool hungry;

};

#endif
