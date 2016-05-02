/**
 * @file	proj2putils.h
 * @brief   Iteracni vypocty (projekt c. 2, predmet IZP)
 *
 *          Knihovna obsahujici struktury: PAIR, RESULTSET a funkce
 *          pro praci s nimi: pairMake, pairSwap, pairCmp, pairInInterval
 *
 * @date	2011/11/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef PROJ2PUTILS_H_INCLUDED
#define PROJ2PUTILS_H_INCLUDED

/*! Struktura pro ulozeni dvou hodnot typu double */
typedef struct pair{
    double first;       /*!< first - prvni clen */
    double second;      /*!< second - druhy clen */
} PAIR;

/*! Struktura pro ukladani mezivysledku */
typedef struct resultset{
    PAIR point;         /*!< point - struktura pair pro prubezne vysledky */

    double first;       /*!< first - prvni clen */
    double second;      /*!< second - druhy clen */

} RESULTSET;

/*!
 * @info Vraci naplnenou strukturu PAIR
 * @param double - prvni clen struktury PAIR
 * @param double - druhy clen struktury PAIR
 * @return PAIR - naplnena struktura PAIR
 */

PAIR pairMake(double first, double second){
    PAIR result = {
        .first = first,
        .second = second
    };

    return result;
}

/*!
 * @info Vraci strukturu s prohozenymi prvky
 * @param PAIR - struktura, ve ktere se maji prvky prohodit
 * @return PAIR - prohozena struktura PAIR
 */

PAIR pairSwap(PAIR point){
        double tmp = point.first;
        point.first = point.second;
        point.second = tmp;

        return point;
}

/*!
 * @info Vraci serazenou strukturu PAIR (first < second)
 * @param PAIR - struktura pro serazeni
 * @return PAIR - serazena struktura
 */

PAIR pairCmp(PAIR point){
    PAIR result = point;

    if(point.first > point.second)
        result = pairSwap(point);

    return result;
}

/*!
 * @info Kontroluje zda je cislo v danem intervalu
 * @param PAIR - struktura PAIR, interval od <first, second>
 * @param double - cislo, ktere se bude kontrolovat v intervalu
 * @return bool - vraci TRUE, pokud se cislo nachazi v intervalu, jinak FALSE
 */

bool pairInInterval(PAIR interval, double number){
    return (number >= interval.first && number <= interval.second);
}

#endif // PROJ2PUTILS_H_INCLUDED

/* konec proj2putils.h */
