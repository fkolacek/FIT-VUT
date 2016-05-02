#include "bee.h"

Bee::Bee(Role role) : Process(){
	this->role = role;
	this->season = (SCHEDULER->isActive()? S_SUMMER : S_WINTER);
	//Ale, zimni delnice se rodi uz od konce Zari
	if(SCHEDULER->getMonth() >= 7) this->season = S_WINTER;
	
	this->age = SCHEDULER->getDay();
	this->die = 0;
	this->hungry = true;
	this->mod = SCHEDULER->getDay();
	
	//Hotfix pro vcely letavky vytvorene uprostred urciteho stadia
	if(this->role == WORKER_OUT){
		this->role = (SCHEDULER->isActive()? WORKER_OUT_SUMMER : WORKER_OUT_WINTER);
		COUNTERS[WORKER_OUT]--; COUNTERS[this->role]++;
		this->die = (this->season == S_SUMMER)? SCHEDULER->getDay() + 7 + (rand()%14 + 1) : SCHEDULER->getDay() + 6*30 +  (rand()%(2*30) + 1);
	}
	//Matka zije 2-3 roky, coz je 2*365 - 3*365
	if(this->role == MOTHER){
		this->die = this->age + 2*365 +  (rand()%(365) + 1);;
	}
}

void Bee::Behavior(){
	
	while(true){
		this->selectRole();
		this->eat();
		
		if(SCHEDULER->isActive()){
			if(SCHEDULER->isDay()){
				
				switch(this->role){
					case WORKER_EGG:		//Vajicko nic nedela
					case WORKER_MAGGOT:		//Larva pouze ji
					case WORKER_CAWL:		//Kukla je zakuklana
					case WORKER_CLEANER: 	//Delnice cisti ul					
					case WORKER_FEEDER: 	//Delnice krmi larvicky
					case WORKER_BUILDER: 	//Delnice stavi komurky
					case WORKER_GUARD:		//Delnice brani ul
						break;
					//Delnice vyleta ven z ulu a sbira nektar
					case WORKER_OUT_SUMMER:
					case WORKER_OUT_WINTER:
						int speed, chance;
						double time;
					
						//Sance, ze se vcela nevrati 0.02%
						chance = rand()%(100 * 1000);
						if(chance <= 20){
							cout << "[?] Accidentally killed, chance: " << chance << endl;
							COUNTERS[this->role]--; COUNTERS[ACCIDENTALLY_KILLED]++;
							this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
						}
					
						//Maximalni rychlost vcely 29 km/h
						if(SCHEDULER->getSeason() == S_SUMMER)
							speed = 24 + (rand()%5 + 1);
						else
							speed = 21 + (rand()%8 + 1);
						
						//Cesta tam a zase zpatky trva nejaky cas
						time = (double)(D_WORKER_OUT * 2) / speed;
						//Vcela donese nekolikrat danou davku						
						COUNTER_HONEY += 3.7*((int)(T_STEP / time) *  0.02684) * (1 / 1.4); //1.4ml = 1mg
						//cout << "[?] Honey++ " << (int)(T_STEP / time) *  0.02684 * (1 / 1.4) << " mg " << ((this->role == WORKER_OUT_SUMMER)? "summer" : "winter")<<endl;
						//COUNTER_DUST += (int)(T_STEP / time) * 16.0; //16mg pylu

						break;
					//Matka
					case MOTHER_EGG:		//Vajicko nic nedela
					case MOTHER_MAGGOT:		//Larva pouze ji
					case MOTHER_CAWL:		//Kukla je zakuklana
					case MOTHER_WEDDING:
						break;
					case MOTHER_VIRGIN:
						//break; //TO-BE-REMOVED
					case MOTHER:
						int month, egg, megg, degg;
						
						month = SCHEDULER->getMonth();
					
						//Od brezna do kvetna az 1500 vajicek / den
						if(month >= 2 && month <= 4)
							egg = (int)((500 + (rand()%1000 + 1)) / (T_DAY_END - T_DAY_START));
						//Od cervna do srpna az 2000 vajicek / den
						else if(month >= 5 && month <= 6)
							egg = (int)((500 + (rand()%1500 + 1)) / (T_DAY_END - T_DAY_START));
						//Od zari do rijna az 1000 vajicek / den
						else
							egg = (int)((300 + (rand()%700 + 1)) / (T_DAY_END - T_DAY_START));
						
						//Neoplodnena matka rodi pouze trubce
						if(this->role == MOTHER_VIRGIN){
							degg = egg;
							egg = 0;
							megg = 0;							
						}
						//Oplodnena matka 99.5% larev delnice, 0.5% trubcu
						else{
							degg = (int)((double)egg * 0.05);
							egg = (int)((double)egg * 0.95);
							megg = 0;
							
							//Pokud je rojeni zapnute a je cervenec
							if(ENABLE_SWARM == 1 && month == 6){
								megg = (int)((double)egg * 0.0100);
								egg = (int)((double)egg * 0.9900);
							}
						}
						
						COUNTER_EGGS += egg + degg + megg;
						
						for(int i = 0; i < egg; i++) SCHEDULER->Schedule(WORKER_EGG);					
						for(int i = 0; i < megg; i++) SCHEDULER->Schedule(MOTHER_EGG);					
						for(int i = 0; i < degg; i++) SCHEDULER->Schedule(DRONE_EGG);
						
						break;
					//Trubec
					case DRONE_EGG:			//Vajicko nic nedela
					case DRONE_MAGGOT:		//Larva pouze ji
					case DRONE_CAWL:		//Kukla je zakuklana
					case DRONE:	
					case DRONE_READY:
						break;
					default:
						cerr << "[!] Bee with invalid role assigned: " << this->getRole() << endl;
						this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
						break;
				}
				
				this->Wait(T_STEP);
			}
			//It's a deep night
			else{
				this->Wait(T_STEP * 4);
			}
		}
		//It's a deep winter
		else{
			this->Wait(T_STEP * 24);
		}
	}

}

void Bee::selectRole(){

	switch(this->role){
		/* ---------------------------------------------------------------------------------------------------
		 * Delnice
		 * --------------------------------------------------------------------------------------------------- */
		case WORKER_EGG:
		case WORKER_MAGGOT:
		case WORKER_CAWL:
		case WORKER_CLEANER:
		case WORKER_FEEDER:
		case WORKER_BUILDER:
		case WORKER_GUARD:
		case WORKER_OUT_SUMMER:
		case WORKER_OUT_WINTER:
			//Delnice se stava larvou po 3d
			if(this->role == WORKER_EGG && (SCHEDULER->getDay() - this->age) > 3){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_EGG=>WORKER_MAGGOT" << endl;
				this->role = WORKER_MAGGOT; COUNTERS[WORKER_EGG]--; COUNTERS[WORKER_MAGGOT]++;
			}
			//Delnice se zakukli po 7d
			if(this->role == WORKER_MAGGOT && (SCHEDULER->getDay() - this->age) > 7){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_MAGGOT=>WORKER_CAWL" << endl;
				this->role = WORKER_CAWL; COUNTERS[WORKER_MAGGOT]--; COUNTERS[WORKER_CAWL]++;
			}
			//Delnice se stava delnici po 11d
			if(this->role == WORKER_CAWL && (SCHEDULER->getDay() - this->age) > 11){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_CAWL=>WORKER_CLEANER" << endl;
				this->role = WORKER_CLEANER; COUNTERS[WORKER_CAWL]--; COUNTERS[WORKER_CLEANER]++;
			}
			//Delnice se stava krmickou po 1d
			if(this->role == WORKER_CLEANER && (SCHEDULER->getDay() - this->age) > 1){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_CLEANER=>WORKER_FEEDER" << endl;
				this->role = WORKER_FEEDER; COUNTERS[WORKER_CLEANER]--; COUNTERS[WORKER_FEEDER]++;
			}
			//Delnice se stava stavitelkou po 7d
			if(this->role == WORKER_FEEDER && (SCHEDULER->getDay() - this->age) > 7){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_FEEDER=>WORKER_BUILDER" << endl;
				this->role = WORKER_BUILDER; COUNTERS[WORKER_FEEDER]--; COUNTERS[WORKER_BUILDER]++;
			}
			//Delnice se stava obranarkou po 11d
			if(this->role == WORKER_BUILDER && (SCHEDULER->getDay() - this->age) > 11){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_BUILDER=>WORKER_GUARD" << endl;
				this->role = WORKER_GUARD; COUNTERS[WORKER_BUILDER]--; COUNTERS[WORKER_GUARD]++;
			}
			//Delnice se stava letavkou po 18d
			if(this->role == WORKER_GUARD && (SCHEDULER->getDay() - this->age) > 18){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: WORKER_GUARD=>WORKER_OUT_" << ((this->season == S_SUMMER)? "SUMMER" : "WINTER") << endl;
				
				//Letni letavka zije jeste 7-21d
				if(this->season == S_SUMMER){
					this->role = WORKER_OUT_SUMMER;					
					this->die = SCHEDULER->getDay() + 7 + (rand()%14 + 1);
				}
				//Zimni letavka zije jeste 6-8m
				else{
					this->role = WORKER_OUT_WINTER;					
					this->die = SCHEDULER->getDay() + 4*30 +  (rand()%(2*30) + 1);
				}
				COUNTERS[WORKER_GUARD]--; COUNTERS[this->role]++;
			}
			//Delnice umira
			if( (this->role == WORKER_OUT_SUMMER || this->role == WORKER_OUT_WINTER) && (SCHEDULER->getDay() >= this->die)){
				cout << "[?] Worker has been terminated" << endl;
				COUNTERS[this->role]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG				
			}
			
			break;
		/* ---------------------------------------------------------------------------------------------------
		 * Matka
		 * --------------------------------------------------------------------------------------------------- */
		case MOTHER_EGG:
		case MOTHER_MAGGOT:
		case MOTHER_CAWL:
		case MOTHER_WEDDING:
		case MOTHER_VIRGIN:
		case MOTHER:
			//Matka se stava larvou po 3d
			if(this->role == MOTHER_EGG && (SCHEDULER->getDay() - this->age) > 3){
				this->age = SCHEDULER->getDay();
				cout << "[?] Mother promote: MOTHER_EGG=>MOTHER_MAGGOT" << endl;
				this->role = MOTHER_MAGGOT; COUNTERS[MOTHER_EGG]--; COUNTERS[MOTHER_MAGGOT]++;
			}
			//Matka se zakukli po 5d
			if(this->role == MOTHER_MAGGOT && (SCHEDULER->getDay() - this->age) > 5){
				this->age = SCHEDULER->getDay();
				cout << "[?] Mother promote: MOTHER_MAGGOT=>MOTHER_CAWL" << endl;
				this->role = MOTHER_CAWL; COUNTERS[MOTHER_MAGGOT]--; COUNTERS[MOTHER_CAWL]++;
			}
			//Matka je dospela po 7d
			if(this->role == MOTHER_CAWL && (SCHEDULER->getDay() - this->age) > 7){
				this->age = SCHEDULER->getDay();
				cout << "[?] Mother promote: MOTHER_CAWL=>MOTHER_WEDDING" << endl;
				//Matka zije 2-3 roky, coz je 2*365 - 3*365
				this->die = this->age + 2*365 +  (rand()%(365) + 1);
				
				//Spustime genocidu..
				if(COUNTERS[MOTHER_WEDDING] == 0){
				//Nastaveni parametru rojeni - pulka populace odchazi
				COUNTERS_TMP[WORKER_CLEANER] = COUNTERS[WORKER_CLEANER] / 2;
				COUNTERS_TMP[WORKER_FEEDER] = COUNTERS[WORKER_FEEDER] / 2;
				COUNTERS_TMP[WORKER_BUILDER] = COUNTERS[WORKER_BUILDER] / 2;
				COUNTERS_TMP[WORKER_GUARD] = COUNTERS[WORKER_GUARD] / 2;
				COUNTERS_TMP[WORKER_OUT] = COUNTERS[WORKER_OUT] / 2;
				COUNTERS_TMP[WORKER_OUT_SUMMER] = COUNTERS[WORKER_OUT_SUMMER] / 2;
				COUNTERS_TMP[WORKER_OUT_WINTER] = COUNTERS[WORKER_OUT_WINTER] / 2;
				
				COUNTERS_TMP[MOTHER] = 0;
				}
				
				this->role = MOTHER_WEDDING; COUNTERS[MOTHER_CAWL]--; COUNTERS[MOTHER_WEDDING]++;
			}
			//Matka ma 6d na spareni
			if(this->role == MOTHER_WEDDING && (SCHEDULER->getDay() - this->age) > 6){
				this->age = SCHEDULER->getDay();
				
				if(COUNTERS[FERTIZILED] > 0){
					cout << "[?] Mother promote: MOTHER_WEDDING=>MOTHER" << endl;
					this->role = MOTHER; 
					COUNTERS[MOTHER_WEDDING]--; COUNTERS[MOTHER]++;
				}
				else{
					cout << "[?] Mother promote: MOTHER_WEDDING=>MOTHER_VIRGIN" << endl;
					this->role = MOTHER_VIRGIN; 
					COUNTERS[MOTHER_WEDDING]--; COUNTERS[MOTHER_VIRGIN]++;					
				}				
			}
			//Vcely nechaji narodit pouze jednu kralovnu, zbytek nechaji zemrit
			if(this->role == MOTHER_WEDDING && (COUNTERS[MOTHER_VIRGIN] > 1 || COUNTERS[MOTHER_WEDDING] > 1 || COUNTERS[MOTHER] >1)){
				COUNTERS[MOTHER_WEDDING]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
			}
			//Matka umira
			if(this->role == MOTHER && (SCHEDULER->getDay() >= this->die)){
				cout << "[?] Mother has been terminated" << endl;
				COUNTERS[this->role]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG				
			}
			break;
		/* ---------------------------------------------------------------------------------------------------
		 * Trubec
		 * --------------------------------------------------------------------------------------------------- */
		case DRONE_EGG:
		case DRONE_MAGGOT:
		case DRONE_CAWL:
		case DRONE:
		case DRONE_READY:		
			//Trubec se stava larvou po 3d
			if(this->role == DRONE_EGG && (SCHEDULER->getDay() - this->age) > 3){
				this->age = SCHEDULER->getDay();
				cout << "[?] Drone promote: DRONE_EGG=>DRONE_MAGGOT" << endl;
				this->role = DRONE_MAGGOT; COUNTERS[DRONE_EGG]--; COUNTERS[DRONE_MAGGOT]++;
			}
			//Trubec se zakukli po 7d
			if(this->role == DRONE_MAGGOT && (SCHEDULER->getDay() - this->age) > 7){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: MAGGOT=>CAWL" << endl;
				this->role = DRONE_CAWL; COUNTERS[DRONE_MAGGOT]--; COUNTERS[DRONE_CAWL]++;
			}
			//Trubec je dospely po 14d
			if(this->role == DRONE_CAWL && (SCHEDULER->getDay() - this->age) > 14){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: CAWL=>DRONE" << endl;
				this->role = DRONE; COUNTERS[DRONE_CAWL]--; COUNTERS[DRONE]++;
			}
			//Trubec se muze parit po 9d
			if(this->role == DRONE && (SCHEDULER->getDay() - this->age) > 9){
				this->age = SCHEDULER->getDay();
				cout << "[?] Worker promote: DRONE=>DRONE_READY" << endl;
				this->role = DRONE_READY;
				//Trubec sam umira za 23d
				this->die = SCHEDULER->getDay() + 23;
				COUNTERS[DRONE]--; COUNTERS[DRONE_READY]++;
			}
			//Pareni
			if(this->role == DRONE_READY && COUNTERS[MOTHER_WEDDING] > 0){
				COUNTERS[FERTIZILED]++;
				COUNTERS[DRONE_READY]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
			}
			//Trubec umira
			if(this->role == DRONE_READY && (SCHEDULER->getDay() >= this->die)){
				cout << "[?] Drone has been terminated" << endl;
				COUNTERS[DRONE_READY]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
			}
			if((this->role == DRONE_READY || this->role == DRONE || this->role == DRONE_CAWL || this->role == DRONE_EGG) && SCHEDULER->getMonth() >= 7){
				cout << "[?] Drone has been kicked out from beehive." << endl;
				COUNTERS[this->role]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
			}
		default:
			//Tady se stane neco velice, velice zleho..
			break;
	}
}

void Bee::eat(){
	if(this->mod != SCHEDULER->getDay()){
		this->hungry = true;
		this->mod = SCHEDULER->getDay();
	}
	
	//Umirani resime pouze v zime
	if(!SCHEDULER->isActive()){
		int total = 0;
		for(int i = 0; i < END; i++) total += COUNTERS[i];
			
		//Matku nenechame umrit
		if(this->role != MOTHER)
			if((total-1) > TMP)
				if(SCHEDULER->getMonth() == 1 || Random() <= 0.0008){
					COUNTERS[this->role]--;
					cout << "[?] Winter is too cruel.." << endl;
					this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
				}
	}
	
	//Rojeni resime pouze v lete
	if(SCHEDULER->isActive() && COUNTERS[MOTHER_WEDDING] >= 1){
		if(this->role == WORKER_CLEANER || this->role == WORKER_FEEDER || this->role == WORKER_BUILDER 
		 || this->role == WORKER_GUARD || this->role == WORKER_OUT || this->role == WORKER_OUT_SUMMER
		 || this->role == WORKER_OUT_WINTER || this->role == MOTHER)
			if(COUNTERS[this->role] > COUNTERS_TMP[this->role]){
				cout << "[?] Swarm mode - actually: " << COUNTERS[this->role] << " excpected: " << COUNTERS_TMP[this->role] << " role: " << this->role << endl;
				COUNTERS[this->role]--;
				this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
			}
	}

	double amount;
	//Vcela je hladova pouze jednou za den
	
	if(this->hungry){
		//V lete
		if(SCHEDULER->isActive()){
			//Krmicka v lete sni 14.9g medu za den - med ale drzime v mg, spotreba pocitana na 1k
			if(this->role == WORKER_FEEDER){
				amount = Uniform(13,15);
				if(amount > COUNTER_HONEY){
					COUNTERS[this->role]--;
					this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
				}
				else{
					COUNTER_HONEY -= amount;
					//cout << "[?] Honey-- " << amount << " mg " << ((this->role == WORKER_OUT_SUMMER)? "summer" : "winter")<<endl;
					COUNTER_CONSUMED += amount;
				}
			}
			//Vcely v ulu v lete sni 5.86g medu za den - med ale drzime v mg, zbytek je krmen feedery
			else if(this->role == WORKER_BUILDER || this->role == WORKER_GUARD){
				amount = Uniform(4,6);
				if(amount > COUNTER_HONEY){
					COUNTERS[this->role]--;
					this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
				}
				else{
					COUNTER_HONEY -= amount;
					//cout << "[?] Honey-- " << amount << " mg " << ((this->role == WORKER_OUT_SUMMER)? "summer" : "winter")<<endl;
					COUNTER_CONSUMED += amount;
				}
			}
		}
		//V zime
		else{
			if(this->role == WORKER_FEEDER || this->role == WORKER_BUILDER || this->role == WORKER_GUARD || this->role == WORKER_OUT || this->role == WORKER_OUT_SUMMER || this->role == WORKER_OUT_WINTER){
				amount = Uniform(7,9);
				if(amount > COUNTER_HONEY){
					COUNTERS[this->role]--;
					this->Passivate(); // Tady mel fungovat Cancel.. ale Merlin SISGSEG
				}
				else{
					COUNTER_HONEY -= amount;
					//cout << "[?] Honey-- " << amount << " mg " << ((this->role == WORKER_OUT_SUMMER)? "summer" : "winter")<<endl;
					COUNTER_CONSUMED += amount;
				}
			}
		}
		
		this->hungry = false;
	}
}
