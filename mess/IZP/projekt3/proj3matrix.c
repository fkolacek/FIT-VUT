#include "proj3matrix.h"

MATRIX* addMatrix(MATRIX* first, MATRIX* second){
    if(!first || !second)
        return NULL;

    if(first->rowCount != second->rowCount || first->columnCount != second->columnCount)
        return NULL;

    MATRIX* result = createMatrix(first->rowCount, second->columnCount);

    if(!result)
        return NULL;

    for(int i = 0; i < first->rowCount; i++)
        for(int j = 0; j < first->columnCount; j++)
            result->data[i][j] = first->data[i][j] + second->data[i][j];

    return result;
}

MATRIX* mulMatrix(MATRIX* first, MATRIX* second){

    if(!first || !second)
        return NULL;

    if(first->columnCount != second->rowCount)
        return NULL;

    int resultRowCount = 0;
    int resultColumnCount = 0;

    resultRowCount = first->rowCount;
    resultColumnCount = second->columnCount;

    int sum = 0;
    MATRIX* result = createMatrix(resultRowCount, resultColumnCount);

    if(!result)
        return NULL;

    for(int i = 0; i < first->rowCount; i++){
        for(int j = 0; j < second->columnCount; j++){
            sum = 0;

            for(int k = 0; k < first->columnCount; k++)
                sum += first->data[i][k] * second->data[k][j];

            result->data[i][j] = sum;
        }
    }

    return result;
}

MATRIX* transMatrix(MATRIX* first){
    if(!first)
        return NULL;

    MATRIX* result = createMatrix(first->columnCount, first->rowCount);

    if(!result)
        return NULL;

    for(int i = 0; i < first->rowCount; i++)
        for(int j = 0; j < first->columnCount; j++)
            result->data[j][i] = first->data[i][j];

    return result;
}

MATRIX* exprMatrix(MATRIX* first, MATRIX* second){
    MATRIX* tmpResult = NULL;
    MATRIX* result = NULL;

    tmpResult = transMatrix(second);
    result = mulMatrix(first, tmpResult);

    clearMatrix(tmpResult);

    if(!result)
        return NULL;

    tmpResult = addMatrix(result, first);

    clearMatrix(result);

    return tmpResult;
}

int getDirection(char* direction){
    if(strcmp("V", direction) == 0)
        return DIR_EAST;
    else if(strcmp("Z", direction) == 0)
        return DIR_WEST;
    else if(strcmp("JV", direction) == 0)
        return DIR_SOUTH_EAST;
    else if(strcmp("JZ", direction) == 0)
        return DIR_SOUTH_WEST;
    else if(strcmp("SV", direction) == 0)
        return DIR_NORTH_EAST;
    else if(strcmp("SZ", direction) == 0)
        return DIR_NORTH_WEST;
    else
        return DIR_INVALID;
}

FIELD makeField(int row, int column){
    FIELD result = {
        .row = row,
        .column = column
    };

    return result;
}

FIELD getNeighbor(FIELD position, int direction){
    FIELD neighbor = makeField(position.row, position.column);

    switch(direction){
        case DIR_EAST:
            neighbor.column++;
            break;
        case DIR_NORTH_EAST:
            neighbor.column = (neighbor.row % 2 == 1)? neighbor.column + 1 : neighbor.column;
            neighbor.row--;
            break;
        case DIR_SOUTH_EAST:
            neighbor.column = (neighbor.row % 2 == 1)? neighbor.column + 1 : neighbor.column;
            neighbor.row++;
            break;

        case DIR_WEST:
            neighbor.column--;
            break;
        case DIR_NORTH_WEST:
            neighbor.column = (neighbor.row % 2 == 1)? neighbor.column : neighbor.column - 1;
            neighbor.row--;
            break;
        case DIR_SOUTH_WEST:
            neighbor.column = (neighbor.row % 2 == 1)? neighbor.column : neighbor.column - 1;
            neighbor.row++;
            break;
    }

    return neighbor;
}

bool isValidPos(MATRIX* M, FIELD position){
    return (position.row >= 0 && position.column >= 00 && position.row < M->rowCount && position.column < M->columnCount);
}

void changeWorld(MATRIX* input, FIELD source, int prevMin){
    if(!isValidPos(input, makeField(source.row, source.column)))
        return;

    FIELD neighbor;

    int minimum = prevMin;

    input->data[source.row][source.column] = 0;

    for(int i = DIR_NORTH_EAST; i <= DIR_NORTH_WEST; i++){
        neighbor = getNeighbor(makeField(source.row, source.column), i);

        if(isValidPos(input, neighbor)){
            if(minimum > input->data[neighbor.row][neighbor.column] && input->data[neighbor.row][neighbor.column] > 0)
                minimum = input->data[neighbor.row][neighbor.column];
        }
    }

    for(int i = DIR_NORTH_EAST; i <= DIR_NORTH_WEST; i++){
        neighbor = getNeighbor(makeField(source.row, source.column), i);

        if(isValidPos(input, neighbor)){

            //Pokud je okolni prvek vetsi jak minimum => -1
            if(minimum < input->data[neighbor.row][neighbor.column]){
                //input->data[neighbor.row][neighbor.column] = -1;
            }

            //Pokud jsou okolni prvky mensi nebo rovny minimu
            else if(input->data[neighbor.row][neighbor.column] > 0)
                changeWorld(input, makeField(neighbor.row, neighbor.column), minimum);
        }
    }
}

MATRIX* waterMatrix(MATRIX* input, int row, int column){
    if(!input)
        return NULL;

    if(!isValidPos(input, makeField(row, column)))
        return NULL;

    MATRIX* result = createMatrix(input->rowCount, input->columnCount);
    for(int i = 0; i < input->rowCount; i++)
        for(int j = 0; j < input->columnCount; j++)
            result->data[i][j] = input->data[i][j];

    changeWorld(result, makeField(row, column), result->data[row][column]);

    //Docisteni dat v matici
    for(int i = 0; i < result->rowCount; i++)
        for(int j = 0; j < result->columnCount; j++)
            if(result->data[i][j] != 0)
                result->data[i][j] = 1;

    return result;
}

MATRIX* caromMatrix(MATRIX* input, int row, int column, int direction, unsigned int power){
    FIELD step;
    FIELD oldStep = makeField(row, column);

    if(!isValidPos(input, oldStep))
        return NULL;

    if(power == 0)
        power = 1;

    MATRIX* result = createMatrix(1, power);

    if(!result) return NULL;

    result->data[0][0] = input->data[row][column];

    for(unsigned int i = 1; i < power; i++){

        step = getNeighbor(oldStep, direction);

        //Pokud dojde k odrazu
        if(!isValidPos(input, step)){
            //printf("DIRECTION: %d\n", direction);
            switch(direction){
                //Odraz od zdi doleva
                case DIR_EAST:
                    step.column -= 2;
                    direction = DIR_WEST;
                    break;
                //Odraz od zdi doprava
                case DIR_WEST:
                    step.column += 2;
                    direction = DIR_EAST;
                    break;
                case DIR_SOUTH_EAST:
                    if(oldStep.row == (input->rowCount -1) && oldStep.column == (input->columnCount - 1)){
                        step = getNeighbor(oldStep, DIR_NORTH_WEST);
                        direction = DIR_NORTH_WEST;
                    }
                    else if(oldStep.row == (input->rowCount - 1)){
                        step = getNeighbor(oldStep, DIR_NORTH_EAST);
                        direction = DIR_NORTH_EAST;
                    }
                    else if(oldStep.column == (input->columnCount - 1)){
                        step = getNeighbor(oldStep, DIR_SOUTH_WEST);
                        direction = DIR_SOUTH_WEST;
                    }
                    break;
                case DIR_SOUTH_WEST:
                    if(oldStep.row == (input->rowCount -1) && oldStep.column == 0){
                        step = getNeighbor(oldStep, DIR_NORTH_EAST);
                        direction = DIR_NORTH_EAST;
                    }
                    else if(oldStep.row == (input->rowCount - 1)){
                        step = getNeighbor(oldStep, DIR_NORTH_WEST);
                        direction = DIR_NORTH_WEST;
                    }
                    else if(oldStep.column == 0){
                        step = getNeighbor(oldStep, DIR_SOUTH_EAST);
                        direction = DIR_SOUTH_EAST;
                    }
                    break;
                case DIR_NORTH_EAST:
                    if(oldStep.row == 0 && oldStep.column == (input->columnCount - 1)){
                        step = getNeighbor(oldStep, DIR_SOUTH_WEST);
                        direction = DIR_SOUTH_WEST;
                    }
                    //Odraz o pravou hranu
                    else if(oldStep.column == (input->columnCount - 1)){
                        step = getNeighbor(oldStep, DIR_NORTH_WEST);
                        direction = DIR_NORTH_WEST;
                    }
                    //Odraz o horni hranu
                    else if(oldStep.row == 0){
                        step = getNeighbor(oldStep, DIR_SOUTH_EAST);
                        direction = DIR_SOUTH_EAST;
                    }
                    break;

                case DIR_NORTH_WEST:
                    if(oldStep.row == 0 && oldStep.column == 0){
                        step = getNeighbor(oldStep, DIR_SOUTH_EAST);
                        direction = DIR_SOUTH_EAST;
                    }
                    //Odraz o levou hranu
                    else if(oldStep.column == 0){
                        step = getNeighbor(oldStep, DIR_NORTH_EAST);
                        direction = DIR_NORTH_EAST;
                    }
                    //Odraz o horni hranu
                    else if(oldStep.row == 0){
                        step = getNeighbor(oldStep, DIR_SOUTH_WEST);
                        direction = DIR_SOUTH_WEST;
                    }
                    break;
            }
        }

        result->data[0][i] = input->data[step.row][step.column];

        oldStep = step;
    }

    return result;
}
