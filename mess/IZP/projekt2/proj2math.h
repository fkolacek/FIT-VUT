/**
 * @file	pgoj2math.h
 * @brief   Iteracni vypocty (projekt c. 2, predmet IZP)
 *
 *          Knihovna obsahujici numericke funkce simplePow, simpleAbs, getEpsilon, getXInInterval,
 *          ln, logax, arctan, arcsin a strukturu: FRACTION a funkce pro praci s ni: fractionMake, fractionValue
 *
 * @date	2011/11/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef PROJ2MATH_H_INCLUDED
#define PROJ2MATH_H_INCLUDED

/*! Definice konstanty NAN */
#ifndef NAN
#define NAN 0.0/0.0
#endif

/*! Definice konstanty INF */
#ifndef INF
#define INF 1.0/0.0
#endif

/*! Definice konstanty - prirozeny logaritmus 10 */
#ifndef LN10
#define LN10 2.3025850929940456840
#endif

/*! Definice konstanty - PI, PI/2, PI_6 */
#define CUSTOM_PI 3.14159265358979323846
#define CUSTOM_PI_2 1.57079632679489661923
#define CUSTOM_PI_6 0.52359877598298873077

/*! Definice konstanty - odmocnina ze 3 */
#define CUSTOM_SQRT3 1.732050807568877293527

/*! Struktura reprezentujici zlomek */
typedef struct fraction{
    double numerator;       /*!< numerator - citatel */
    double denominator;     /*!< denominator - jmenovatel */
} FRACTION;

/*!
 * @info Vraci hodnotu zlomku (citatel / jmenovatel)
 * @param FRACTION - zlomek
 * @return double - hodnota zlomku
 */

double fractionValue(FRACTION f){
    if(f.denominator == 0)
        return NAN;

    return (f.numerator / f.denominator);
}

/*!
 * @info Vraci vytvorenou strukturu FRACTION
 * @param double - citatel
 * @param double - jmenovatel
 * @return FRACTION - vytvorena struktura reprezentujici zlomek
 */

FRACTION fractionMake(double numerator, double denominator){
    FRACTION f = {
        .numerator = numerator,
        .denominator = denominator
    };

    return f;
}

/*!
 * @info Vraci mocninu cisla
 * @param double - zaklad mocniny
 * @param unsigned int - exponent mocniny
 * @return double - hodnota mocniny
 */

double simplePow(double number, unsigned int exponent){
    double result = number;

    for(unsigned int i = 1; i < exponent; i++)
        result *= number;

    return result;
}

/*!
 * @info Vraci absolutni hodnotu cisla
 * @param double - vstupni cislo
 * @return double - absolutni hodnota cisla
 */

double simpleAbs(double number){
    return (number > 0)? number : (-1) * number;
}

/*!
 * @info Vraci danou presnost ve tvaru 10^
 * @param unsigned int - vstupni presnost
 * @return double - presnost ve tvaru 10^
 */

double getEpsilon(unsigned int sigdig){
    double result = 0.1;

    for(unsigned int i = 0; i <= sigdig; i++)
        result *= 0.1;

    return result;
}

/*!
 * @info Vraci naplnenou strukturu PAIR (first - cislo do intervalu (0.1, 1>, second pocet, kolikrat se first vynasobil/vydelil 10
 * @param double - vstupni cislo pro prevod do intervalu
 * @return PAIR - naplnena struktura PAIR
 */

PAIR getXinInterval(double x){
    int exponent = 0;

    while(x >= 1.0){ x /= 10; exponent++; }
    while(x < 0.1){ x *= 10; exponent--; }

    return pairMake(x, exponent);
}

/*!
 * @info Vraci prirozeny logaritmus cisla x
 * @param double - logaritmovane cislo
 * @param unsigned int - zadana presnost platnych cislic
 * @return double - prirozeny logaritmus
 */

//ln(x) = ln( (1 + y) / (1 - y) ) = 2y (1/1 + y^2/3 + y^4/5 + y^6/7....)
double ln(double x, unsigned int sigdig){
    //(x - 1) / (x + 1)
    FRACTION f = fractionMake(x - 1, x + 1);
        
    double oldResult = 1;
    double newResult = 1;
    int exponent = 2;
    
    //Presnost
    double epsilon = getEpsilon(sigdig);
    double y = fractionValue(f);

    do{
        //Ulozime si predchozi vysledek kvuli kontrole
        oldResult = newResult;

        //(1y^exponent) / (exponent + 1)
        f.numerator = simplePow(y, exponent);
        f.denominator = (double)(exponent + 1);
        newResult += fractionValue(f);

        exponent += 2;

	//Dokud nebude vysledek pozadovane presny
    }while(simpleAbs(newResult * epsilon) < simpleAbs(newResult - oldResult));

    return (2 * y * newResult);
}

/*!
 * @info Vraci obecny logaritmus cisla x o zakladu a
 * @param double - logaritmovane cislo
 * @param double - zaklad logaritmu
 * @param unsigned int - zadana presnost platnych cislic
 * @return double - obecny logaritmus cisla x o zakladu a
 */

//loga X = ln(x) / ln(a)
double logax(double x, double a, unsigned int sigdig){
    if(x < 0 || x == NAN || a < 0 || (a == 1) || (a == 1 && x == 1))
        return NAN;
    else if( x == +INFINITY)
        return +INFINITY;
    else if(x == 0)
        return -INFINITY;
    else if(x == 1)
        return 0;
    else if(a == 0)
        return 0;

    //Prevedeni x a a do intervalu (0.1, 1>
    PAIR pX = getXinInterval(x);
    PAIR pA = getXinInterval(a);
    
    
	//(exponent1 * ln(10) + ln(x)) / (exponent2 * ln(10) + ln(a))
    return (pX.second * LN10 + ln(pX.first, sigdig)) / (pA.second * LN10 + ln(pA.first, sigdig));
}

/*! Vyctovy typ pro mozne stavy pri pocitani arctan */
enum arctanFlags{
    ARCTAN_NEGATIVE    = 1,     /*!< pokud vysel ARCTAN zaporny */
    ARCTAN_GTONE       = 2,     /*!< pokud vysel ARCTAN vetsi jak 1 */
    ARCTAN_IDENTITY    = 4      /*!< pokud vysel ARCTAN vetsi jak 2 - sqrt(3) */
};

/*!
 * @info Vraci arctan cisla x
 * @param double - cislo ze ktereho se ma arctan pocitat
 * @return double - arctan cisla x
 */

double arctan(double x){
    return x / sqrt(1 - simplePow(x, 2));
}

/*!
 * @info Vraci arcsin cisla x
 * @param double - cislo ze ktereho se ma arcsin pocitat
 * @param unsigned int - presnost (pocet platnych cifer)
 * @return double - arcsin cisla x
 */

double arcsin(double x, unsigned int sigdig){

    if(x == 1)
		return CUSTOM_PI_2;
	else if(x == -1)
		return CUSTOM_PI_2 * (-1);
	else if( x < -1 || x > 1 )
		return NAN;

	FRACTION f;

	int flag = 0;
	int exponent = 3;
    double epsilon = getEpsilon(sigdig);
    double result = arctan(x);
    double oldResult;
    double temp;

    //Pokud je vysledek arctan zaporny
    if(result < 0){
        flag += ARCTAN_NEGATIVE;
        result *= (-1);
    }

    else{
        //Pokud je vysledek arctan vetsi jak 1
        if(result > 1){
            flag += ARCTAN_GTONE;
            f.numerator = 1;
            f.denominator = result;
            
            result = fractionValue(f);
        }

        //Pokud je vysledek arctan vetsi jak 2 - sqrt(3)
        if(result > 2 - CUSTOM_SQRT3){
            flag += ARCTAN_IDENTITY;
            f.numerator = result * CUSTOM_SQRT3 - 1;
            f.denominator = result + CUSTOM_SQRT3;
            result = fractionValue(f);
        }
    }

    temp = result;

    do
    {
        oldResult = result;

        if(exponent % 4 == 1)
            result = result + simplePow(temp, exponent) / exponent;
        else
            result = result - simplePow(temp, exponent) / exponent;

        exponent += 2;

	}while(simpleAbs(result * epsilon) < simpleAbs(result - oldResult));

    //Pokud byl arctan vetsi jak 2 - sqrt(3)
    if(flag >= ARCTAN_IDENTITY){
        result += CUSTOM_PI_6;
        flag -= ARCTAN_IDENTITY;
    }

    //Pokud byl arctan vetsi jak 1
    if(flag >= ARCTAN_GTONE){
        result = CUSTOM_PI_2 - result;
        flag -= ARCTAN_GTONE;
    }

    //Pokud byl arctan zaporny
    if(flag >= ARCTAN_NEGATIVE)
        result *= (-1);

    return result;
}

#endif // PROJ2MATH_H_INCLUDED

/* konec proj2math.h */
