/**
 * @file	proj2lbl.h
 * @brief   Iteracni vypocty (projekt c. 2, predmet IZP)
 *
 *          Knihovna obsahujici funkce pro vypocet delky lomene
 *          cary (lbl, lblPair) a lomene cary s chybou (lble, lblePair)
 *
 * @date	2011/11/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef PROJ2LBL_H_INCLUDED
#define PROJ2LBL_H_INCLUDED

/*!
 * @info Vraci delku lomene cary mezi dvema body
 * @param double - souradnice x prvniho bodu
 * @param double - souradnice y prvniho bodu
 * @param double - souradnice x druheho bodu
 * @param double - souradnice y druheho bodu
 * @return double - delka lomene cary
 */

double lbl(double x1, double y1, double x2, double y2){
    double u1 = x2 - x1;
    double u2 = y2 - y1;

    return sqrt((u1 * u1) + (u2 * u2));
}

/*!
 * @info Interface lbl pro praci s strukturou PAIR
 * @param PAIR - struktura obsahujici souradnice prvniho bodu
 * @param PAIR - struktura obsahujici souradnice druheho bodu
 * @return double - delka lomene cary
 */

double lblPair(PAIR X, PAIR Y){
    return lbl(X.first, X.second, Y.first, Y.second);
}

/*!
 * @info Vraci delku lomene cary s chybou mezi dvema body
 * @param double - souradnice x prvniho bodu
 * @param double - souradnice y prvniho bodu
 * @param double - souradnice x druheho bodu
 * @param double - souradnice y druheho bodu
 * @param double - chyba lomene cary
 * @return PAIR - struktura pair obsahujici minimalni a maximalni delku cary
 */

PAIR lble(double x1, double y1, double x2, double y2, double err){
    PAIR results = {
        .first = -1, //minimum
        .second = -1 //maximum
    };

    //Pokud se oba shoduji, budou vzdalenosti 0
    if(x1 == x2 && y1 == y2)
        return pairMake(0,0);

    //Hranicni intervaly pro x vcetne chyby
    double minX = pairCmp(pairMake(x1, x2)).first - err;

    //Hranicni intervaly pro y vcetne chyby
    double minY = pairCmp(pairMake(y1, y2)).first;

    //Presuneme oba body do prvniho kvadrantu
    if(minX < 0){ x1 += (minX * (-1)); x2 += (minX * (-1)); }
    if(minY < 0){ y1 += (minY * (-1)); y2 += (minY * (-1)); }

    //Specialni pripady

    // Pokud se navzajem dotykaji (maji spolecnou osu x i osu y) -> pak jejich nejkratsi delka je 0
    if( (pairInInterval(pairMake(x1 - err, x1 + err), x2 - err) || pairInInterval(pairMake(x1 - err, x1 + err), x2 + err)) && (pairInInterval(pairMake(y1 - err, y1 + err), y2 - err) || pairInInterval(pairMake(y1 - err, y1 + err), y2 + err)) ){
        results.first = 0;
    }
    //Pokud jsou body vedle sebe (maji spolecnou osu y), nejkratsi delka je hrana - hrana
    else if( pairInInterval(pairMake(y1 - err, y1 + err), y2 - err) || pairInInterval(pairMake(y1 - err, y1 + err), y2 + err) ){
        //Pokud je Y nad X
        results.first = (x2 > x1)? ((x2 - err) - (x1 + err)) : ((x1 - err) - (x2 + err));
    }

    //Pokud jsou body nad sebou (maji spolecnou osu x), pak je nejkratsi delka hrana - hrana
    else if( pairInInterval(pairMake(x1 - err, x1 + err), x2 - err) || pairInInterval(pairMake(x1 - err, x1 + err), x2 + err) ){
        //Pokud je Y vpravo od X
        results.first = (y2 > y1)? ((y2 - err) - (y1 + err)) : ((y1 - err) - (y2 + err));
    }

    double min = 0;
    //Obecne pripady
        //printf("LADENI: X[%lf,%lf], Y[%lf,%lf]\n", x1, y1, x2, y2);
        //Pokud je X nalevo a pod Y
        if(x2 >= x1 && y2 >= y1){
            min = lblPair(pairMake(x1 + err, y1 + err), pairMake(x2 - err, y2 - err));
            results.second = lblPair(pairMake(x1 - err, y1 - err), pairMake(x2 + err, y2 + err));
        }
        //Pokud je X napravo a pod Y
        else if(x1 >= x2 && y2 >= y1){
            min = lblPair(pairMake(x2 + err, y2 - err), pairMake(x1 - err, y1 + err));
            results.second = lblPair(pairMake(x2 - err, y2 + err), pairMake(x1 + err, y1 - err));
        }
        //Pokud je X nalevo a nad Y
        else if(x2 >= x1 && y1 >= y2){
            min = lblPair(pairMake(x1 + err, y1 - err), pairMake(x2 - err, y2 + err));
            results.second = lblPair(pairMake(x1 - err, y1 + err), pairMake(x2 + err, y2 - err));
        }
        //Pokud je X napravo a nad Y
        else if(x1 >= x2 && y1 >= y2){
            min = lblPair(pairMake(x2 + err, y2 + err), pairMake(x1 - err, y1 - err));
            results.second = lblPair(pairMake(x2 - err, y2 - err), pairMake(x1 + err, y1 + err));
        }

    if(results.first == -1)
        results.first = min;


    return results;
}

/*!
 * @info Interface lble pro praci s strukturou PAIR
 * @param PAIR - struktura obsahujici souradnice prvniho bodu
 * @param PAIR - struktura obsahujici souradnice druheho bodu
 * @param double - chyba lomene cary
 * @return double - delka lomene cary
 */

PAIR lblePair(PAIR X, PAIR Y, double err){
    return lble(X.first, X.second, Y.first, Y.second, err);
}

#endif // PROJ2LBL_H_INCLUDED

/* konec proj2lbl.h */
