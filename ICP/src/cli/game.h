/**
  * @file game.h
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Header file for classes Stone, Desk and Game
  */
#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <deque>
#include "exception.h"
#include "client.h"
#include "utils.h"

using namespace std;

/**
* @brief Enum represents stone type
*/
enum stoneType{
    STONE_L=0,
    STONE_T,
    STONE_I,
    STONE_NONE,
    STONE_TYPE_END,
};

/**
* @brief Enum represents stone angle
*/
enum stoneAngle{
    STONE_UP = 0,
    STONE_LEFT,
    STONE_RIGHT,
    STONE_DOWN,

    STONE_ANGLE_END,
};

/**
* @brief Enum represents stone item
*/
enum stoneItem{
    STONE_ITEM_A = 0, STONE_ITEM_B, STONE_ITEM_C, STONE_ITEM_D, STONE_ITEM_E, STONE_ITEM_F, STONE_ITEM_G, STONE_ITEM_H, STONE_ITEM_I, STONE_ITEM_J,
    STONE_ITEM_K, STONE_ITEM_L, STONE_ITEM_M, STONE_ITEM_N, STONE_ITEM_O, STONE_ITEM_P, STONE_ITEM_Q, STONE_ITEM_R, STONE_ITEM_S, STONE_ITEM_T,
    STONE_ITEM_U, STONE_ITEM_V, STONE_ITEM_W, STONE_ITEM_X, STONE_ITEM_Y, STONE_ITEM_Z,
    STONE_ITEM_NONE,
};

/**
* @brief Stone maps for CLI representation
*/
extern const char maps[STONE_TYPE_END][STONE_ANGLE_END][9];

/**
* @brief Class represents Stone
*/
class Stone{
    public:
		/**
		* @brief Constructor of class Stone
		* @param stoneType type
		* @param stoneAngle angle
		* @param stoneItem item
		*/
        Stone(stoneType type = STONE_NONE, stoneAngle angle = STONE_UP, stoneItem item = STONE_ITEM_NONE){
            this->type = type;
            this->angle = angle;
            this->item = item;
        }

		/**
		* @brief Setter for attribute type
		* @param stoneType type
		*/
        inline void setType(stoneType type){ this->type = type; }
		/**
		* @brief Setter for attribute angle
		* @param stoneAngle angle
		*/
        inline void setAngle(stoneAngle angle){ this->angle = angle; }
		/**
		* @brief Setter for attribute item
		* @param stoneItem item
		*/
        inline void setItem(stoneItem item){ this->item = item; }

		/**
		* @brief Rotate stone left
		*/
        inline void rotateLeft(){
            switch(this->angle){
                case STONE_UP: this->angle = STONE_LEFT; break;
                case STONE_LEFT: this->angle = STONE_DOWN; break;
                case STONE_DOWN: this->angle = STONE_RIGHT; break;
                default: this->angle = STONE_UP; break;
        }}
		/**
		* @brief Rotate stone right
		*/
        inline void rotateRight(){
            switch(this->angle){
                case STONE_UP: this->angle = STONE_RIGHT; break;
                case STONE_RIGHT: this->angle = STONE_DOWN; break;
                case STONE_DOWN: this->angle = STONE_LEFT; break;
                default: this->angle = STONE_UP; break;
        }}

		/**
		* @brief Getter for attribute type
		* @return stoneType
		*/
        inline stoneType getType(){ return this->type; }
		/**
		* @brief Getter for attribute angle
		* @param stoneAngle
		*/
        inline stoneAngle getAngle(){ return this->angle; }
		/**
		* @brief Getter for attribute item
		* @param stoneItem
		*/
        inline stoneItem getItem(){ return this->item; }

    private:
        stoneType type;
        stoneAngle angle;
        stoneItem item;
};

/**
* @brief Class represents Desk
*/
class Desk{
    public:
        Desk(int = 7, int = 24);
        virtual ~Desk();
        void init();

		/**
		* @brief Getter for attribute size
		* @return int
		*/
        inline int getSize(){ return this->size; }
		/**
		* @brief Getter for attribute items
		* @return int
		*/  
        inline int getItems(){ return this->items; }
		/**
		* @brief Getter for attribute cards.size
		* @return int
		*/
        inline int getCards(){ return this->cards.size(); }
		/**
		* @brief Getter for attribute card.at
		* @return stoneItem
		*/
        inline stoneItem getCard(int i){ return this->cards.at(i); }
		/**
		* @brief Getter for attribute figures.position
		* @param int i
		* @return Position
		*/
        inline Position getFigurePosition(int i){ return this->figures[i]; }

		/**
		* @brief Getter for attribute currentStone
		* @param int i
		* @return Stone*
		*/
        inline Stone* getCurrentStone(){ return this->currentStone; }
        /**
		* @brief Getter for attribute generic.location
		* @param int x
		* @param int y
		* @param int size
		* @return int
		*/
        inline int getStoneLocationGeneric(int x, int y, int size){ return ((y - 1)*size) + (x - 1); }
		/**
		* @brief Getter for attribute location
		* @param int x
		* @param int y
		* @return int
		*/
        inline int getStoneLocation(int x, int y){ return this->getStoneLocationGeneric(x, y, this->size); }
        
        deque<Position> getPossibleMoves(Position pos);
        bool canMove(Stone*, Stone*, stoneAngle);
        
        
		/**
		* @brief Add stone to stones
		* @return Stone* stone
		*/
        inline void addStone(Stone* stone){ this->stones.push_back(stone); }
		/**
		* @brief Add card to cards
		* @return stoneItem
		*/
        inline void addCard(stoneItem card){ this->cards.push_back(card); }
		/**
		* @brief Return card from cards
		* @return stoneItem
		*/
        inline stoneItem drawCard(){
            stoneItem item;
            if(this->cards.size() == 0)
                item = STONE_ITEM_NONE;
            else{
                item = this->cards.front();
                this->cards.pop_front();
            }

            return item;
        }

		/**
		* @brief Get stone on specified index
		* @param int i
		* @return Stone*
		*/
        inline Stone* getStone(int i){
            if(this->stones.size() == 0)
                return NULL;

            if(i < 0 || (unsigned int)i >= this->stones.size())
                return NULL;

            return this->stones[i];
        }

		/**
		* @brief Getter for attribute stone
		* @param int x
		* @param int y
		* @return Stone*
		*/
        inline Stone* getStone(int x, int y){ return this->getStone(this->getStoneLocation(x, y)); }
        /**
		* @brief Setter for attribute size
		* @param int size
		*/
        inline void setSize(int size = 7){ this->size = size; }
        /**
		* @brief Setter for attribute items
		* @param int items
		*/
        inline void setItems(int items = 24){ this->items = items; }
        /**
		* @brief Setter for attribute stone
		* @param Stone* stone
		* @param int x
		* @param int y
		*/
        inline void setStone(Stone* stone, int x, int y){ this->stones[(this->getStoneLocation(x, y))] = stone; }
        /**
		* @brief Setter for attribute currentStone
		* @param Stone* stone
		*/
        inline void setCurrentStone(Stone* stone){ this->currentStone = stone; }
        /**
		* @brief Setter for attribute currentStone.angle
		* @param stoneAngle angle
		*/
        inline void setCurrentStoneAngle(stoneAngle angle){ this->currentStone->setAngle(angle); }
        /**
		* @brief Setter for attribute figure.position
		* @param int x
		* @param int y
		* @param int i
		*/
        inline void setFigurePosition(int i, int x, int y){ this->figures[i].x = x; this->figures[i].y = y; }

    private:
        int size;
        int items;
        Stone* currentStone;
        vector<Stone*> stones;
        deque<stoneItem> cards;
        Position figures[4];
};

/**
* @brief Class represents Game
*/
class Game{
    public:
        Game(string = "new_game");
        virtual ~Game();

        /**
		* @brief Setter for attribute name
		* @param string name
		*/
        inline void setName(string name){ this->name = name; };
        /**
		* @brief Setter for attribute playersCount
		* @param int playersCount
		*/
        inline void setPlayersCount(int playersCount){ this->playersCount = playersCount; }
        /**
		* @brief Setter for attribute Player
		* @param int i
		* @param Client client
		*/
        inline void setPlayer(int i, Client client){ this->players[i] = client; }
        /**
		* @brief Setter for attribute Player.score
		* @param int i
		* @param int score
		*/
        inline void setPlayerScore(int i, int score){ this->playersScore[i] = score; }
        /**
		* @brief Setter for attribute Player.card
		* @param int i
		* @param stoneItem card
		*/
        inline void setPlayerCard(int i, stoneItem card){ this->playersCards[i] = card; }
        /**
		* @brief Setter for attribute UID
		* @param int uid
		*/
        inline void setUID(int uid){ this->uid = uid; }
        /**
		* @brief Setter for attribute status
		* @param int status
		*/
        inline void setStatus(int status){ this->status = status; }
        /**
		* @brief Setter for attribute turn
		* @param int turn
		*/
        inline void setTurn(int turn){ this->turn = turn; }
        /**
		* @brief Rotate current stone left
		*/
        inline void rotateLeft(){ this->desk->getCurrentStone()->rotateLeft(); }
        /**
		* @brief Rotate current stone right
		*/
        inline void rotateRight(){ this->desk->getCurrentStone()->rotateRight(); }

        void put(int, int, stoneAngle);
        void move(int, int, int);

        /**
		* @brief Draw card
		* @return stoneItem
		*/
        inline stoneItem drawCard(){ return this->desk->drawCard(); }
        /**
		* @brief Getter for attribute playersCount
		* @return int
		*/
        inline int getPlayersCount(){ return this->playersCount; }
        /**
		* @brief Getter for attribute playerScore
		* @param int i
		* @return int
		*/
        inline int getPlayerScore(int i){ return this->playersScore[i]; }
        /**
		* @brief Getter for attribute stoneItem
		* @param int x
		* @param int y
		* @return stoneItem
		*/
        inline stoneItem getStoneItem(int x, int y){ return this->desk->getStone(x, y)->getItem(); }
        /**
		* @brief Getter for attribute playerCard
		* @param int i
		* @return stoneItem
		*/
        inline stoneItem getPlayerCard(int i){ return this->playersCards[i]; }
        /**
		* @brief Getter for attribute player.at
		* @param int i
		* @return Client
		*/
        inline Client getPlayer(int i){ return this->players[i]; }
        /**
		* @brief Getter for attribute turn
		* @return int
		*/
        inline int getTurn(){ return this->turn; }
        /**
		* @brief Getter for attribute turn.UID
		* @return int
		*/
        inline int getTurnUID(){ return this->players[(this->turn)].getUID(); }
        /**
		* @brief Getter for attribute uid
		* @param int i
		* @return int
		*/        
        inline int getUID(int i){ return this->players[i].getUID();}
        /**
		* @brief Getter for attribute Desk.size
		* @return int
		*/
        inline int getSize(){ return this->desk->getSize(); }
        /**
		* @brief Getter for attribute Desk
		* @return Desk*
		*/
        inline Desk* getDesk(){ return this->desk; }
        /**
		* @brief Return if this player is in this game?
		* @param int i
		* @return bool
		*/
        inline bool isPlayer(int uid){ for(int i = 0; i < this->playersCount; i++) if(this->players[i].getUID() == uid) return true; return false; }
        /**
		* @brief Find player by uid
		* @param int uid
		* @return int
		*/
        inline int findPlayerByUID(int uid){ for(int i = 0; i < this->playersCount; i++) if(this->players[i].getUID() == uid) return i; return -1; }

        /**
		* @brief Getter for attribute P
		* @return int
		*/
        inline int getP(){ return this->P; }
        /**
		* @brief Getter for attribute K
		* @return int
		*/
        inline int getK(){ return this->K; }
        /**
		* @brief Getter for attribute status
		* @return int
		*/
        inline int getStatus(){ return this->status; }

        void init(int, int = 7, int = 24);
        void initFromString(string);
        string serialize();
        void debug();
    private:
        string name;
        Desk* desk;

        position lastPut;
        int playersCount;

        Client players[4];
        int playersScore[4];
        stoneItem playersCards[4];

        int uid;
        int turn;

        int P;
        int K;
        int status;

};

#endif // GAME_H
