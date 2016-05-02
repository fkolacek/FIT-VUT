/**
  * @file game.cpp
  * @author Frantisek Kolacek <xkolac12@stud.fit.vutbr.cz>
  * @version 0.91a
  * @brief Definition of Game's behavior
  */
  
#include "game.h"

const char Maps[STONE_TYPE_END][STONE_ANGLE_END][9] = {
    [STONE_L] = {
        [STONE_UP]      = { '.', '.', '.', '.', '#', '#', '.', '#', '.'},
        [STONE_LEFT]    = { '.', '#', '.', '.', '#', '#', '.', '.', '.'},
        [STONE_RIGHT]   = { '.', '.', '.', '#', '#', '.', '.', '#', '.'},
        [STONE_DOWN]    = { '.', '#', '.', '#', '#', '.', '.', '.', '.'}
    },
    [STONE_T] = {
        [STONE_UP]      = { '.', '#', '.', '#', '#', '#', '.', '.', '.'},
        [STONE_LEFT]    = { '.', '#', '.', '.', '#', '#', '.', '#', '.'},
        [STONE_RIGHT]   = { '.', '#', '.', '#', '#', '.', '.', '#', '.'},
        [STONE_DOWN]    = { '.', '.', '.', '#', '#', '#', '.', '#', '.'}
    },
    [STONE_I] = {
        [STONE_UP]      = { '.', '#', '.', '.', '#', '.', '.', '#', '.'},
        [STONE_LEFT]    = { '.', '.', '.', '#', '#', '#', '.', '.', '.'},
        [STONE_RIGHT]   = { '.', '.', '.', '#', '#', '#', '.', '.', '.'},
        [STONE_DOWN]    = { '.', '#', '.', '.', '#', '.', '.', '#', '.'}
    },
    [STONE_NONE] = {
        [STONE_UP]      = { '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        [STONE_LEFT]    = { '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        [STONE_RIGHT]   = { '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        [STONE_DOWN]    = { '.', '.', '.', '.', '.', '.', '.', '.', '.'}
    }
};

/**
 * @brief Constructor of class Desk
 * @param int size
 * @param int items
 */
Desk::Desk(int size, int items){
    this->size = size;
    this->items = items;

    this->stones.clear();
    this->cards.clear();

    this->currentStone = NULL;

    Position pos = { .x = 0, .y = 0};
    for(int i = 0; i < 4; i++)
        this->figures[i] = pos;

}
/**
 * @brief Destructor of class Desk
 */
Desk::~Desk(){
    if(this->currentStone)
        delete this->currentStone;

    this->currentStone = NULL;

    for(vector<Stone*>::iterator p = this->stones.begin(); p != this->stones.end(); p++){
        Stone* ptr = *p;
        delete ptr;
        ptr = NULL;
    }

    this->stones.clear();
    this->cards.clear();
}
/**
 * @brief Method for initialization of game desk
 */
void Desk::init(){
    this->stones.clear();
    this->cards.clear();

    for(int i = 0; i < items; i++){
        this->cards.push_back((stoneItem)i);
    }

    random_shuffle(this->cards.begin(), this->cards.end());

    //Generate items collection
    deque<stoneItem> itemsCollection;
    for(int i = 0; i < this->items; i++)
        itemsCollection.push_back((stoneItem)i);

    //Fill missing
    while(itemsCollection.size() < (unsigned int)(this->size * this->size))
        itemsCollection.push_back(STONE_ITEM_NONE);

    //Shake it baby
    random_shuffle(itemsCollection.begin(), itemsCollection.end());

    int counters[STONE_TYPE_END] = {
        [STONE_L] = 0,
        [STONE_T] = 0,
        [STONE_I] = 0,
        [STONE_NONE] = 0
    };

    for(int i = 0; i < (this->size * this->size); i++){
        this->stones.push_back(new Stone(STONE_NONE,STONE_UP,itemsCollection.front()));
        itemsCollection.pop_front();
        counters[STONE_NONE]++;
    }

    //StoneT
    for(int y = 1; y <= this->size; y++){
        for(int x = 1; x <= this->size; x++){
            if(y%2 == 1 && x%2 == 1){
                this->getStone(x, y)->setType(STONE_T);

                if(y == 1) this->getStone(x, y)->setAngle(STONE_DOWN);
                else if(y == this->size) this->getStone(x, y)->setAngle(STONE_UP);
                else if(x == 1) this->getStone(x, y)->setAngle(STONE_LEFT);
                else if(x == this->size) this->getStone(x, y)->setAngle(STONE_RIGHT);
                else{ this->getStone(x, y)->setAngle((stoneAngle)(rand()%(int)STONE_DOWN)); }

                counters[STONE_T]++;
                counters[STONE_NONE]--;
            }
        }
    }

    //StoneL - Left bottom
    this->getStone(1, 1)->setType(STONE_L);
    this->getStone(1, 1)->setAngle(STONE_UP);
    this->setFigurePosition(0, 1, 1);

    //StoneL - Right bottom
    this->getStone(1, this->size)->setType(STONE_L);
    this->getStone(1, this->size)->setAngle(STONE_LEFT);
    this->setFigurePosition(1, this->size, 1);

    //StoneL - Left top
    this->getStone(this->size, 1)->setType(STONE_L);
    this->getStone(this->size, 1)->setAngle(STONE_RIGHT);
    this->setFigurePosition(2, 1, this->size);

    //StoneL - Right top
    this->getStone(this->size, this->size)->setType(STONE_L);
    this->getStone(this->size, this->size)->setAngle(STONE_DOWN);
    this->setFigurePosition(3, this->size, this->size);

    counters[STONE_L] = 4;
    counters[STONE_T] =- counters[STONE_L];

    //Gen random stone
    stoneType type = (stoneType)(rand()%(int)STONE_DOWN);
    stoneAngle angle = (stoneAngle)(rand()%(int)STONE_DOWN);

    this->currentStone = new Stone(type, angle);

    //Fill gaps
    for(int i = 0; i < (this->size * this->size); i++){
        if(this->stones.at(i)->getType() == STONE_NONE){
            int sum;
            do{
                type = (stoneType)(rand()%(int)STONE_DOWN);

                int s1 = counters[type] - counters[STONE_L];
                int s2 = counters[type] - counters[STONE_T];
                int s3 = counters[type] - counters[STONE_I];

                sum = s1 + s2 + s3;
            }while(sum > 1);

            angle = (stoneAngle)(rand()%(int)STONE_DOWN);

            counters[type]++;
            counters[STONE_NONE]--;

            this->stones[i]->setType(type);
            this->stones[i]->setAngle(angle);
        }
    }
}

/**
 * @brief Method for getting all possible moves (based on BFS)
 * @param Position pos
 * @return deque<Position>
 */
deque<Position> Desk::getPossibleMoves(Position pos){
    deque<Position> open;
    deque<Position> closed;

    Position p;
    Stone* stoneCurrent = NULL;
    Stone* stoneNeighbor = NULL;

    open.push_back(pos);

    // 6 7 8
    // 3 4 5
    // 0 1 2
    while(open.size() != 0){
        p = open.front();
        open.pop_front();

        if(p.x < 1 || p.x > this->size) continue;
        if(p.y < 1 || p.y > this->size) continue;

        stoneCurrent = this->getStone(p.x, p.y);

        //1
        if(p.y - 1 >= 1){
            stoneNeighbor = this->getStone(p.x, p.y - 1);
            if(this->canMove(stoneCurrent, stoneNeighbor, STONE_DOWN))
                if(!contains(open, cpos(p.x, p.y - 1)) && !contains(closed, cpos(p.x, p.y - 1)))
                    open.push_back(cpos(p.x, p.y - 1));
        }
        //3
        if(p.x - 1 >= 1){
            stoneNeighbor = this->getStone(p.x - 1, p.y);
            if(this->canMove(stoneCurrent, stoneNeighbor, STONE_LEFT))
                if(!contains(open, cpos(p.x - 1, p.y)) && !contains(closed, cpos(p.x - 1, p.y)))
                    open.push_back(cpos(p.x - 1, p.y));
        }
        //5
        if(p.x + 1 <= this->size){
            stoneNeighbor = this->getStone(p.x + 1, p.y);
            if(this->canMove(stoneCurrent, stoneNeighbor, STONE_RIGHT))
                if(!contains(open, cpos(p.x + 1, p.y)) && !contains(closed, cpos(p.x + 1, p.y)))
                    open.push_back(cpos(p.x + 1, p.y));
        }
        //7
        if(p.y + 1 <= this->size){
            stoneNeighbor = this->getStone(p.x, p.y + 1);
            if(this->canMove(stoneCurrent, stoneNeighbor, STONE_UP))
                if(!contains(open, cpos(p.x, p.y + 1)) && !contains(closed, cpos(p.x, p.y + 1)))
                    open.push_back(cpos(p.x, p.y + 1));
        }

        if(!contains(closed, p))
            closed.push_back(p);
    }

    return closed;
}

/**
 * @brief Method for evaluating possibility of actual move
 * @param Stone* s1
 * @param Stone* s2
 * @param stoneAngle angle
 * @return bool
 */
bool Desk::canMove(Stone* s1, Stone* s2, stoneAngle angle){
    if(!s1 || !s2)
        return false;

    stoneType s1Type = s1->getType();
    stoneType s2Type = s2->getType();
    stoneAngle s1Angle = s1->getAngle();
    stoneAngle s2Angle = s2->getAngle();

    if(angle == STONE_UP || angle == STONE_DOWN){
        char field[6][3] = { '.' };

        if(angle == STONE_UP){
            /*field[5][2] = Maps[s2Type][s2Angle][8];
            field[5][1] = Maps[s2Type][s2Angle][7];
            field[5][0] = Maps[s2Type][s2Angle][6];
            field[4][2] = Maps[s2Type][s2Angle][5];
            field[4][1] = Maps[s2Type][s2Angle][4];
            field[4][0] = Maps[s2Type][s2Angle][3];
            field[3][2] = Maps[s2Type][s2Angle][2];*/
            field[3][1] = Maps[s2Type][s2Angle][1];
            /*field[3][0] = Maps[s2Type][s2Angle][0];
            field[2][2] = Maps[s1Type][s1Angle][8];*/
            field[2][1] = Maps[s1Type][s1Angle][7];
            /*field[2][0] = Maps[s1Type][s1Angle][6];
            field[1][2] = Maps[s1Type][s1Angle][5];
            field[1][1] = Maps[s1Type][s1Angle][4];
            field[1][0] = Maps[s1Type][s1Angle][3];
            field[0][2] = Maps[s1Type][s1Angle][2];
            field[0][1] = Maps[s1Type][s1Angle][1];
            field[0][0] = Maps[s1Type][s1Angle][0];*/
        }
        else{
            /*field[5][2] = Maps[s1Type][s1Angle][8];
            field[5][1] = Maps[s1Type][s1Angle][7];
            field[5][0] = Maps[s1Type][s1Angle][6];
            field[4][2] = Maps[s1Type][s1Angle][5];
            field[4][1] = Maps[s1Type][s1Angle][4];
            field[4][0] = Maps[s1Type][s1Angle][3];
            field[3][2] = Maps[s1Type][s1Angle][2];*/
            field[3][1] = Maps[s1Type][s1Angle][1];
            /*field[3][0] = Maps[s1Type][s1Angle][0];
            field[2][2] = Maps[s2Type][s2Angle][8];*/
            field[2][1] = Maps[s2Type][s2Angle][7];
            /*field[2][0] = Maps[s2Type][s2Angle][6];
            field[1][2] = Maps[s2Type][s2Angle][5];
            field[1][1] = Maps[s2Type][s2Angle][4];
            field[1][0] = Maps[s2Type][s2Angle][3];
            field[0][2] = Maps[s2Type][s2Angle][2];
            field[0][1] = Maps[s2Type][s2Angle][1];
            field[0][0] = Maps[s2Type][s2Angle][0];*/
        }

        if(field[2][1] == '#' && field[3][1] == '#')
            return true;
    }
    else{
        char field[3][6] = { '.' };

        if(angle == STONE_LEFT){
            /*field[2][2] = Maps[s2Type][s2Angle][8];
            field[2][1] = Maps[s2Type][s2Angle][7];
            field[2][0] = Maps[s2Type][s2Angle][6];*/
            field[1][2] = Maps[s2Type][s2Angle][5];
            /*field[1][1] = Maps[s2Type][s2Angle][4];
            field[1][0] = Maps[s2Type][s2Angle][3];
            field[0][2] = Maps[s2Type][s2Angle][2];
            field[0][1] = Maps[s2Type][s2Angle][1];
            field[0][0] = Maps[s2Type][s2Angle][0];
            field[2][5] = Maps[s1Type][s1Angle][8];
            field[2][4] = Maps[s1Type][s1Angle][7];
            field[2][3] = Maps[s1Type][s1Angle][6];
            field[1][5] = Maps[s1Type][s1Angle][5];
            field[1][4] = Maps[s1Type][s1Angle][4];*/
            field[1][3] = Maps[s1Type][s1Angle][3];
            /*field[0][5] = Maps[s1Type][s1Angle][2];
            field[0][4] = Maps[s1Type][s1Angle][1];
            field[0][3] = Maps[s1Type][s1Angle][0];*/
        }
        else{
            /*field[2][2] = Maps[s1Type][s1Angle][8];
            field[2][1] = Maps[s1Type][s1Angle][7];
            field[2][0] = Maps[s1Type][s1Angle][6];*/
            field[1][2] = Maps[s1Type][s1Angle][5];
            /*field[1][1] = Maps[s1Type][s1Angle][4];
            field[1][0] = Maps[s1Type][s1Angle][3];
            field[0][2] = Maps[s1Type][s1Angle][2];
            field[0][1] = Maps[s1Type][s1Angle][1];
            field[0][0] = Maps[s1Type][s1Angle][0];
            field[2][5] = Maps[s2Type][s2Angle][8];
            field[2][4] = Maps[s2Type][s2Angle][7];
            field[2][3] = Maps[s2Type][s2Angle][6];
            field[1][5] = Maps[s2Type][s2Angle][5];
            field[1][4] = Maps[s2Type][s2Angle][4];*/
            field[1][3] = Maps[s2Type][s2Angle][3];
            /*field[0][5] = Maps[s2Type][s2Angle][2];
            field[0][4] = Maps[s2Type][s2Angle][1];
            field[0][3] = Maps[s2Type][s2Angle][0];*/
        }

        if(field[1][2] == '#' && field[1][3] == '#')
            return true;
    }

    return false;
}

/**
 * @brief Constructor of class Game
 * @param string name
 */
Game::Game(string name){
    this->name = name;
    this->desk = NULL;
    this->turn = 0;
    this->playersCount = 0;
    this->lastPut.x = 0;
    this->lastPut.y = 0;
    this->uid = 0;
    this->status = 0;

    srand(time(NULL));

    for(int i = 0; i < 4; i++){
        this->playersScore[i] = 0;
        this->playersCards[i] = STONE_ITEM_NONE;
    }
}
/**
 * @brief Destructor of class Game
 */
Game::~Game(){
    if(this->desk)
        delete this->desk;

    this->desk = NULL;
}

/**
 * @brief Method for operation put
 * @param int x
 * @param int y
 * @param stoneAngle angle
 */
void Game::put(int x, int y, stoneAngle angle){

    if(!this->desk)
        throw new Exception("Game is not running");

    if(!this->status)
        throw new Exception("Game is not running");

    if(x < 1 || x > this->desk->getSize())
        throw new Exception("X position is out of range");

    if(y < 1 || y > this->desk->getSize())
        throw new Exception("Y position is out of range");

    if(angle < STONE_UP || angle > STONE_DOWN)
        throw new Exception("Invalid angle");


    if(!((x%2 == 1 && y%2 == 0 ) || (x%2 == 0 && y%2 == 1)))
        throw new Exception("You can play only on even rows and columns");

    if(x == 1)
        if(y == this->lastPut.y && this->lastPut.x == this->desk->getSize())
            throw new Exception("This move is not allowed in this turn");

    if(x == this->desk->getSize())
        if(y == this->lastPut.y && this->lastPut.x == 1)
            throw new Exception("This move is not allowed in this turn");

    if(y == 1)
        if(x == this->lastPut.x && this->lastPut.y == this->desk->getSize())
            throw new Exception("This move is not allowed in this turn");

    if(y == this->desk->getSize())
        if(x == this->lastPut.x && this->lastPut.y == 1)
            throw new Exception("This move is not allowed in this turn");


    this->desk->getCurrentStone()->setAngle(angle);

    //shifting..
    Stone* stone = NULL;
    int i;

    //Figures hacking
    Position posOld, posNew = cpos(x, y);

    //From left
    if(x == 1){
        stone = this->desk->getStone(this->desk->getSize(), y);
        posOld = cpos(this->desk->getSize(), y);

        for(i = this->desk->getSize(); i > 1; i--)
            this->desk->setStone(this->desk->getStone(i-1, y), i, y);
    }
    //From right
    else if(x == this->desk->getSize()){
        stone = this->desk->getStone(1, y);
        posOld = cpos(1, y);

        for(i = 1; i < this->desk->getSize(); i++)
            this->desk->setStone(this->desk->getStone(i+1, y), i, y);
    }
    //From bottom
    else if(y == 1){
        stone = this->desk->getStone(x, this->desk->getSize());
        posOld = cpos(x, this->desk->getSize());

        for(i = this->desk->getSize(); i > 1; i--)
            this->desk->setStone(this->desk->getStone(x, i-1), x, i);
    }
    //From top
    else if(y == this->desk->getSize()){
        stone = this->desk->getStone(x, 1);
                posOld = cpos(x, 1);

        for(i = 1; i < this->desk->getSize(); i++)
            this->desk->setStone(this->desk->getStone(x, i+1), x, i);
    }
    else
        throw new Exception("Wow.. maybe parallel universe..");

    this->desk->setStone(this->desk->getCurrentStone(), x, y);
    this->desk->setCurrentStone(stone);

    this->lastPut.x = x;
    this->lastPut.y = y;

    for(int i = 0; i < this->playersCount; i++){
        Position figure = this->desk->getFigurePosition(i);

        if(figure.x == posOld.x && figure.y == posOld.y)
            this->desk->setFigurePosition(i, posNew.x, posNew.y);
    }
}

/**
 * @brief Method for operation move
 * @param int x
 * @param int y
 * @param int i
 */
void Game::move(int x, int y, int i){
    if(!this->desk)
        throw new Exception("Game is not running");

    if(!this->status)
        throw new Exception("Game is not running");

    if(x < 1 || x > this->desk->getSize())
        throw new Exception("X position is out of range");

    if(y < 1 || y > this->desk->getSize())
        throw new Exception("Y position is out of range");

    if(i < 0 || i >= this->playersCount)
        throw new Exception("Invalid client");

    deque<Position> moves = this->desk->getPossibleMoves(this->desk->getFigurePosition(i));

    Position pos = { .x = x, .y = y};

    if(contains(moves, pos)){
        this->desk->setFigurePosition(i, x, y);
        this->turn++;

        if(this->turn == this->playersCount)
            this->turn = 0;
    }
    else
        throw new Exception("Invalid move");
}

/**
 * @brief Method for initialization game
 * @param int playersCount
 * @param int size
 * @param int items
 */
void Game::init(int playersCount, int size, int items){
    if(!(playersCount == 2 || playersCount == 3 || playersCount == 4))
        throw new Exception("Invalid number of players, allowed values are: 2, 3, 4");

    if(!(size == 5 || size == 7 || size == 9 || size == 11))
        throw new Exception("Invalid desk size, allowed values are: 5, 7, 9, 11");

    if(!(items == 12 || items == 24))
        throw new Exception("Invalid number of items, allowed values are: 12, 24");

    this->playersCount = playersCount;
    this->P = playersCount;
    this->K = items;
    this->status = 1;

    if(this->desk)
        delete this->desk;

    this->desk = NULL;
    this->lastPut.x = 0;
    this->lastPut.y = 0;

    this->desk = new Desk(size, items);
    this->desk->init();

    this->turn = 0;
}

//'GAMEINIT;5|I|24|CS|0,0,24|D|0,0,24|1,0,4|1,3,8|2,0,10|0,2,24|1,1,24|2,0,9|1,0,0|1,2,1|2,2,5|1,1,24|1,0,11|1,1,24|1,1,7|1,2,6|1,2,24|1,1,24|0,0,3|1,2,24|2,1,24|0,1,2|2,2,24|1,0,24|1,0,24|0,3,24|P|2|PS|0|0|C|12|1|5|7|9|11|2|3|0|8|10|4|6|EOF;
/**
 * @brief Method for initialization game from string
 * @param string data
 */
void Game::initFromString(string data){
    int size;
    int items;
    Stone currentStone;
    stoneType type;
    stoneAngle angle;
    stoneItem item;
    int players;
    int id, uid;
    string name;
    int card;
    int playerScore;
    int cardsCount;
    int x;
    int y;

    string tmp;

    /* Size     */ tmp = parseData(data, '|'); size = stringToInt(tmp);
    /* I        */ tmp = parseData(data, '|'); if(tmp != "I") throw new Exception("Invalid syntax of serialized game");
    /* Items    */ tmp = parseData(data, '|'); items = stringToInt(tmp);
    /* CS       */ tmp = parseData(data, '|'); if(tmp != "CS") throw new Exception("Invalid syntax of serialized game");
    /* CS type  */ tmp = parseData(data, ','); type = (stoneType)stringToInt(tmp);
    /* CS angle */ tmp = parseData(data, ','); angle = (stoneAngle)stringToInt(tmp);
    /* CS item  */ tmp = parseData(data, '|'); item = (stoneItem)stringToInt(tmp);
    /* D        */ tmp = parseData(data, '|'); if(tmp != "D") throw new Exception("Invalid syntax of serialized game");

    if(this->desk)
        delete this->desk;

    this->desk = NULL;

    this->lastPut.x = 0;
    this->lastPut.y = 0;

    this->desk = new Desk(size, items);
    this->desk->setCurrentStone(new Stone(type, angle, item));

    this->K = items;

    for(int i = 0; i < size * size; i++){
        /* CS type  */ tmp = parseData(data, ','); type = (stoneType)stringToInt(tmp);
        /* CS angle */ tmp = parseData(data, ','); angle = (stoneAngle)stringToInt(tmp);
        /* CS item  */ tmp = parseData(data, '|'); item = (stoneItem)stringToInt(tmp);

        this->desk->addStone(new Stone(type, angle, item));
    }

    /* P        */ tmp = parseData(data, '|'); if(tmp != "P") throw new Exception("Invalid syntax of serialized game");
    /* players  */ tmp = parseData(data, '|'); players = stringToInt(tmp);
    /* PS       */ tmp = parseData(data, '|'); if(tmp != "PS") throw new Exception("Invalid syntax of serialized game");

    this->playersCount = players;
    this->P = players;

    for(int i = 0; i < players; i++){
        /* playerScore  */ tmp = parseData(data, '|'); playerScore = stringToInt(tmp);
        this->playersScore[i] = playerScore;
    }

    /* PU       */ tmp = parseData(data, '|'); if(tmp != "PU") throw new Exception("Invalid syntax of serialized game");

    for(int i = 0; i < players; i++){
        /* id      */ tmp = parseData(data, ','); id = stringToInt(tmp);
        /* uid     */ tmp = parseData(data, ','); uid = stringToInt(tmp);
        /* name    */ tmp = parseData(data, '|'); name = stringToInt(tmp);

        if(id != i) throw new Exception("Invalid syntax of serialized game");

        this->players[i].setUID(uid);
        this->players[i].setName(name);
    }

    /* PC           */ tmp = parseData(data, '|'); if(tmp != "PC") throw new Exception("Invalid syntax of serialized game");

    for(int i = 0; i < players; i++){
        /* id      */ tmp = parseData(data, ','); id = stringToInt(tmp);
        /* card    */ tmp = parseData(data, '|'); card = stringToInt(tmp);

        if(id != i) throw new Exception("Invalid syntax of serialized game");

        this->playersCards[i] = (stoneItem)card;
    }

    /* C            */ tmp = parseData(data, '|'); if(tmp != "C") throw new Exception("Invalid syntax of serialized game");
    /* cardsCount   */ tmp = parseData(data, '|'); cardsCount = stringToInt(tmp);

    for(int i = 0; i < cardsCount; i++){
        /* card  */ tmp = parseData(data, '|'); item = (stoneItem)stringToInt(tmp);

        this->desk->addCard(item);
    }

    /* F            */ tmp = parseData(data, '|'); if(tmp != "F") throw new Exception("Invalid syntax of serialized game");

    for(int i = 0; i < 4; i++){
        /* x  */ tmp = parseData(data, ','); x = (stoneItem)stringToInt(tmp);
        /* y  */ tmp = parseData(data, '|'); y = (stoneItem)stringToInt(tmp);

        this->desk->setFigurePosition(i, x, y);
    }

    /* EOF          */ tmp = parseData(data, ';'); if(tmp != "EOF") throw new Exception("Invalid syntax of serialized game");

    this->turn = 0;
    this->status = 1;
}

/**
 * @brief Method for serializing games
 * @return string
 */
string Game::serialize(){
    ostringstream serialized;

    if(this->desk){
        int size = this->desk->getSize();

        serialized << size << "|" << "I|" << this->desk->getItems() << "|CS|";

        Stone* stone = this->desk->getCurrentStone();

        serialized << stone->getType() << "," << stone->getAngle() << "," << stone->getItem() << "|";
        serialized << "D|";

        for(int y = 1; y <= size; y++){
            for(int x = 1; x <= size; x++){
                stone = this->desk->getStone(x, y);

                serialized << stone->getType() << "," << stone->getAngle() << "," << stone->getItem() << "|";
            }
        }

        serialized << "P|" << this->playersCount << "|PS|";
        for(int i = 0; i < this->playersCount; i++)
            serialized << this->playersScore[i] << "|";

        serialized << "PU|";
        for(int i = 0; i < this->playersCount; i++)
            serialized << i << "," << this->players[i].getUID() << "," << this->players[i].getName() << "|";

        serialized << "PC|";
        for(int i = 0; i < this->playersCount; i++)
            serialized << i << "," << this->playersCards[i] << "|";

        serialized << "C|" << this->desk->getCards() << "|";

        for(int i = 0; i < this->desk->getCards(); i++)
            serialized << this->desk->getCard(i) << "|";

        serialized << "F|" ;
        Position pos;
        for(int i = 0; i < 4; i++){
            pos = this->desk->getFigurePosition(i);

            serialized << pos.x << "," << pos.y << "|";
        }


        serialized << "EOF";
    }
    else
        serialized << "0|EOF";

    return serialized.str();
}

/**
 * @brief Method for drawing map to console, used also for debugging
 */
void Game::debug(){
    if(!this->desk){
        cerr << "[*] Game is not initialized yet!" << endl;
        return;
    }

    int size = this->desk->getSize();
    int count = size * 3 * size * 3;

    //Allocate map
    char Map[count];

    //Predefine
    for(int i = 0; i < count; i++) Map[i] = 'Q';

    stoneType type = STONE_NONE;
    stoneAngle angle = STONE_UP;
    stoneItem item = STONE_ITEM_NONE;

    for(int y = 1, yy = 0; y <= size; y++, yy+=3){
        for(int x = 1, xx = 0; x <= size; x++, xx+=3){
            if(this->desk->getStone(x, y)){
                type = this->desk->getStone(x, y)->getType();
                angle = this->desk->getStone(x, y)->getAngle();
                item = this->desk->getStone(x, y)->getItem();
            }
            //Not supposed to happen, fallback
            else{
                type = STONE_NONE;
                angle = STONE_UP;
                item = STONE_ITEM_NONE;
                cerr << "[!] Something went terribly wrong.. x: " << x << " y:" << y << " " << this->desk->getStoneLocation(x, y) << endl;
            }
            //Copy from Maps to Map
            for(int i = 0; i < 3; i++){
                Map[(yy+i) * size*3 + xx] = Maps[type][angle][3*i + 0];
                Map[(yy+i) * size*3 + xx + 1] = Maps[type][angle][3*i + 1];
                Map[(yy+i) * size*3 + xx + 2] = Maps[type][angle][3*i + 2];
            }
            //Show items on map
            if(item == STONE_ITEM_NONE)
                Map[(yy+1) * size*3 + xx + 1] = '#';
            else
                Map[(yy+1) * size*3 + xx + 1] = (char)((int)(item) +(int)('A'));
        }
    }

    Stone* stone = this->desk->getCurrentStone();

    //Print Map
    for(int y = size * 3; y > 0; y--){

        if(y%3 == 2) cout << (y/3)+1;
        else cout << " ";

        for(int x = 1; x <= size * 3; x++){
            cout << " " << Map[((x - 1) + (y - 1) * this->desk->getSize()*3)];
        }

        if(y == 8){ cout << "\t\tScore:    "; for(int i = 0; i < this->playersCount; i++){ cout << "#" << this->players[i].getUID() << " (" << this->playersScore[i] << ") "; } }
        if(y == 7){ cout << "\t\tPosition: "; for(int i = 0; i < this->playersCount; i++){ Position pos = this->desk->getFigurePosition(i); cout << "#" << this->players[i].getUID() << " (X:" << pos.x << ", Y:" << pos.y << ") "; } }
        if(y == 6){ cout << "\t\tCards: " << this->desk->getCards() << " Current card: ";
            stoneItem card = this->playersCards[(this->findPlayerByUID(this->uid))];

            if(card == STONE_ITEM_NONE)
                cout << "None";
            else
                cout << (char)((int)(card) +(int)('A'));
        }

        if(y == 4){ cout << "\t\tCurrent stone:"; }
        if(y == 3){ cout << "\t\t" << Maps[stone->getType()][stone->getAngle()][6] << " " << Maps[stone->getType()][stone->getAngle()][7] << " " << Maps[stone->getType()][stone->getAngle()][8] << " "; }
        if(y == 2){ cout << "\t\t" << Maps[stone->getType()][stone->getAngle()][3] << " " << ((stone->getItem() == STONE_ITEM_NONE)? '#' : (char)((int)(stone->getItem()) +(int)('A'))) << " " << Maps[stone->getType()][stone->getAngle()][5] << " "; }
        if(y == 1){ cout << "\t\t" << Maps[stone->getType()][stone->getAngle()][0] << " " << Maps[stone->getType()][stone->getAngle()][1] << " " << Maps[stone->getType()][stone->getAngle()][2] << " "; }

        cout << endl;
    }

    cout << "  ";
    for(int x = 1; x <= size; x++)
            cout << "  " << x << "   ";

    cout << endl;

    //debug - print from Maps
    //for(int y = 3; y > 0; y--){ for(int x = 1; x <= 3; x++) cout << " " << Maps[STONE_I][STONE_LEFT][(x-1) + (y - 1) * 3]; cout << endl; } cout << endl;
    /*
    cout << "#----------------------------------------------------#" << endl;
    cout << "| Game: " << this->name << " | Size: " << this->desk->getSize() << endl;
    cout << "#- Type ---------------------------------------------#" << endl;
    for(int y = this->desk->getSize(); y > 0; y--){
        cout << y;
        for(int x = 1; x <= this->desk->getSize(); x++){
            if(this->desk->getStone(x, y)){
                cout << " " << this->desk->getStone(x, y)->getType();
            }
            else
                cout << " X";
        }
        cout << endl;
    }
    cout << " ";
    for(int x = 1; x <= this->desk->getSize(); x++)
        cout << " "<< x;

    cout << endl;
    */
}
