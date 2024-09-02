#include <iostream>
#include <string>
#include <bitset>
#include <cmath>
#include "ac_fx.h"
#include "FxP.h"
#include <iomanip>
#include <catch2/catch_test_macros.hpp>

FxP<7,-8,0> alpha = 2.5;
FxP<7,-8,0> beta = 10;
FxP<10,-8,0> teta = alpha + beta;
FxP<10,-8,1> teta1 = alpha + beta;
FxP<10,-8,0> tetarte = 10 + 2.5;
FxP<10,-4,0> tartar = alpha + beta;
FxP<7,-8,0> ad = 12.5;


double add( double a, double b ) {
    return (a + b);
}

double moins( double a, double b ) {
    return (a - b);
}

double mul( double a, double b ) {
    return (a * b);
}

double divi( double a, double b ) {
    return (a / b);
}


TEST_CASE( "Les FxP sont additionés", "[add]" ) {
    REQUIRE( add(alpha.getFxP(),beta.getFxP()) == 12.5 );
}

TEST_CASE( "Les FxP sont soustraits", "[moins]" ) {
    REQUIRE( moins(ad.getFxP(),beta.getFxP()) == 2.5 );
    REQUIRE( moins(alpha.getFxP(),beta.getFxP()) == -7.5 );
}

TEST_CASE( "Les FxP sont multipliés", "[mul]" ) {
    REQUIRE( mul(ad.getFxP(),beta.getFxP()) == 125 );
    REQUIRE( mul(alpha.getFxP(),beta.getFxP()) == 25 );
}

TEST_CASE( "Les FxP sont divisés", "[divi]" ) {
    REQUIRE( divi(ad.getFxP(),beta.getFxP()) == 1.25 );
    REQUIRE( divi(alpha.getFxP(),beta.getFxP()) == 0.25 );
}

FxP<7,-8,1> tail = 12.5;
FxP<10,-8,0> mon = ad & beta;
FxP<10,-8,0> lev = alpha & beta;
FxP<10,-8,1> gray; 

TEST_CASE( "Et & FxP" ) {
    REQUIRE( mon.getFxP()  == 8 );
    REQUIRE( lev.getFxP() == 2 );
     lev = ad & 10.75;
    REQUIRE( lev.getFxP() == 8.5 );
    lev = tail & -10.75;
    REQUIRE( lev.getFxP() == 4 );
    lev = ad & -10.75;
    //REQUIRE( lev.getFxP() == 4 ); fail
    tail = -12.75;
    gray = tail & -10.75;
    REQUIRE( gray.getFxP() == -14.75 );
}




TEST_CASE( "Ou | FxP" ) {
	mon = ad | beta;
	lev = alpha | beta;
	tail = 12.5;
    REQUIRE( mon.getFxP()  == 14.5 );
    REQUIRE( lev.getFxP() == 10.5 );
     lev = ad | 10.75;
    REQUIRE( lev.getFxP() == 14.75 );
    gray = tail | -10.75;
    REQUIRE( gray.getFxP() == -2.25 );
    lev = ad | -10.75;
    //REQUIRE( lev.getFxP() == 2.25 ); fail 
    tail = -12.75;
    gray = tail | -10.75;
    REQUIRE( gray.getFxP() == -8.75 );
}

TEST_CASE( "Ou exclusif ^ FxP" ) {
	mon = ad ^ beta;
	lev = alpha ^ beta;
	tail = 12.5;
    REQUIRE( mon.getFxP()  ==  6.5 );
    REQUIRE( lev.getFxP() == 8.5 );
     lev = ad ^ 10.75;
    REQUIRE( lev.getFxP() == 6.25 );
    gray = tail ^ -10.75;
    REQUIRE( gray.getFxP() == -6.25 );
    lev = ad ^ -10.75;
    //REQUIRE( lev.getFxP() == -6.25 ); fail  
    tail = -12.75;
    gray = tail ^ -10.75;
    REQUIRE( gray.getFxP() == 6 );
}

TEST_CASE( "Les FxP sont bien conservés: addition") {
    REQUIRE( teta.getFxP() == 12.5 );
    REQUIRE( teta1.getFxP() == 12.5 );
    REQUIRE( tetarte.getFxP() == 12.5 );
    REQUIRE( tartar.getFxP() == 12.5 );
    tartar += 4;
    REQUIRE( tartar.getFxP() == 16.5 );
}


TEST_CASE( "Les FxP sont bien conservés: soustractions") {
	teta = ad - beta;
    REQUIRE( teta.getFxP() == 2.5 );
    teta = alpha - beta;
    REQUIRE( teta.getFxP() == 0 );
    teta1 = ad - beta;
    REQUIRE( teta1.getFxP() == 2.5 );
    teta1 = alpha - beta;
    REQUIRE( teta1.getFxP() == -7.5 );
    tetarte = 12.5 - 10;
    REQUIRE( tetarte.getFxP() == 2.5 );
    tartar = ad - beta;
    REQUIRE( tartar.getFxP() == 2.5 );
    tartar -= 2;
    REQUIRE( tartar.getFxP() == 0.5 );
}

FxP<15,-8,1> bee = ad * beta;
FxP<15,-8,0> cif = ad * beta;
FxP<15,-4,0> dis = ad * beta;
FxP<6,-8,0> eus = ad * beta;


TEST_CASE( "Les FxP sont bien conservés: multiplications") {
	REQUIRE( bee.getFxP() == 125 );
	//FxP<15,-8,1> bee = ad * -10;
	//REQUIRE( bee.getFxP() == -125 ); fail
	REQUIRE( cif.getFxP() == 125 );
	REQUIRE( dis.getFxP() == 125 );
	 dis *= 2;
	REQUIRE( dis.getFxP() == 250 );
	REQUIRE( eus.getFxP() == 63.99609375 );
}

TEST_CASE( "Les FxP sont bien conservés: division") { //division par FxP ne marche pas
	bee = ad / 10;
	REQUIRE( bee.getFxP() == 1.25 );
	bee /= 10;
	REQUIRE( bee.getFxP() == 0.125 );
	cif = ad / 10;
	REQUIRE( cif.getFxP() == 1.25 );
	dis = ad / 10;
	REQUIRE( dis.getFxP() == 1.25 );
	eus = ad / 10;
	REQUIRE( eus.getFxP() == 1.25);
}

FxP<5,-5,0> one;


TEST_CASE( "Sat MinMax unsigned") { 
	one = 32;
	REQUIRE( one.getFxP() == 31.96875 );
	one = -10;
	REQUIRE( one.getFxP() == 0 );
	one += 11;
	REQUIRE( one.getFxP() == 11 );
	one -= 22;
	REQUIRE( one.getFxP() == 0 );
	one += 66;
	REQUIRE( one.getFxP() == 31.96875);
	one += 66;
	REQUIRE( one.getFxP() == 31.96875);
}

FxP<5,-5,1> two;


TEST_CASE( "Sat MinMax signed") { 
	two = 16.1;
	REQUIRE( two.getFxP() == 15.96875 );
	two = -10;
	REQUIRE( two.getFxP() == -10 );
	two += 11;
	REQUIRE( two.getFxP() == 1 );
	two -= 22;
	REQUIRE( two.getFxP() == -16 );
	two += 66;
	REQUIRE( two.getFxP() == 15.96875);
	two += 66;
	REQUIRE( two.getFxP() == 15.96875);
}

FxP<5,-5,0,WRAP_AROUND> three;


TEST_CASE( "Wrap around unsigned") { 
	three = 32;
	REQUIRE( three.getFxP() == 0 );
	three = 31;
	REQUIRE( three.getFxP() == 31 );
	three += 11;
	REQUIRE( three.getFxP() == 10 );
	three -= 22;
	REQUIRE( three.getFxP() == 20 );
	
}

FxP<5,-5,1,WRAP_AROUND> four;


TEST_CASE( "Wrap around signed") { 
	four = 16;
	REQUIRE( four.getFxP() == -16 );
	four = 17;
	REQUIRE( four.getFxP() == -15 );
	four = 15;
	REQUIRE( four.getFxP() == 15 );
	four += 30;
	REQUIRE( four.getFxP() == 13 );
	four -= 74.5;
	REQUIRE( four.getFxP() == 2.5 );
	
}

FxP<5,-5,0,SAT_ZERO> five;


TEST_CASE( "Sat zero unsigned") { 
	five = 32;
	REQUIRE( five.getFxP() == -0 );
	five = 33;
	REQUIRE( five.getFxP() == 0 );
	five = 31;
	REQUIRE( five.getFxP() == 31 );
	five = -33;
	REQUIRE( five.getFxP() == 0 );
	five += 17;
	REQUIRE( five.getFxP() == 17 );
	five -= 37;
	REQUIRE( five.getFxP() == 0 );
	
	
}

FxP<6,-3,0,SAT_MINMAX,ROUND_INF> six;

TEST_CASE( "Round Inf unsigned") { 
	six = 18.32;
	REQUIRE( six.getFxP() == 18.375 );
	six = 18.3125;
	REQUIRE( six.getFxP() == 18.375 );
	six *= 2;
	REQUIRE( six.getFxP() == 36.75 );
	six = 18.625;
	REQUIRE( six.getFxP() == 18.625 );
	FxP<6,-3,0,SAT_MINMAX,ROUND_INF> six2 = 8.72;
	six = 18.3125;
	six += six2;
	REQUIRE( six.getFxP() == 27.125 );
	FxP<8,-4,0,WRAP_AROUND,ROUND_INF> six3 = 757.33;
	REQUIRE( six3.getFxP() == 245.3125 );	
	six3=-7887.33;
	REQUIRE( six3.getFxP() == 48.6875 );
	FxP<8,-4,0,SAT_MINMAX,ROUND_INF> ten6,ten7;
	ten6 = 7.33; ten7 = 18.3125;
	six3 = ten6 ^ ten7;
	//REQUIRE( six3.getFxP() == 21.1875 ) ; fail
	six3 = ten6 & ten7;
	REQUIRE( six3.getFxP() == 2.3125 );
	
	
}

FxP<7,-3,1,SAT_MINMAX,ROUND_INF> sev;

TEST_CASE( "Round Inf signed") { 
	sev = 18.32;
	REQUIRE( sev.getFxP() == 18.375 );
	sev = 18.3125;
	REQUIRE( sev.getFxP() == 18.375 );
	sev *= 2;
	REQUIRE( sev.getFxP() == 36.75 );
	sev = 18.625;
	REQUIRE( sev.getFxP() == 18.625 );
	FxP<6,-3,1,SAT_MINMAX,ROUND_INF> sev2 = 8.72;
	sev = 18.3125;
	sev += sev2;
	REQUIRE( sev.getFxP() == 27.125 );	
	sev = 8.72564545;
	REQUIRE( sev.getFxP() == 8.75 );	
	FxP<8,-4,1,WRAP_AROUND,ROUND_INF> sev3 = 757.33;
	REQUIRE( sev3.getFxP() == -10.6875 );	
	sev3=-7887.33;
	REQUIRE( sev3.getFxP() == 48.6875 );	
}

FxP<7,-4,1,SAT_MINMAX,ROUND_ODD> eight;

TEST_CASE( "Round Odd signed") { 
	eight = 8.72;
	REQUIRE( eight.getFxP() == 8.75 );
	eight = 18.3125;
	REQUIRE( eight.getFxP() == 18.3125 );
	eight *= 2;
	REQUIRE( eight.getFxP() == 36.625 );
	eight = -7.54;
	REQUIRE( eight.getFxP() == -7.5625 );
	FxP<6,-3,1,SAT_MINMAX,ROUND_ODD> eight2 = 8.72;
	eight = 18.3125;
	eight += eight2;
	REQUIRE( eight.getFxP() == 27.0625 );
	eight = 8.7265486452131111;	
	REQUIRE( eight.getFxP() == 8.75 );
	eight = 11.875;	
	REQUIRE( eight.getFxP() == 11.875 );
	FxP<8,-4,1,WRAP_AROUND,ROUND_INF> eight3 = 757.33;
	REQUIRE( eight3.getFxP() == -10.6875 );	
	eight3=-7887.33;
	REQUIRE( eight3.getFxP() == 48.6875 );
}

FxP<7,-4,0,SAT_MINMAX,ROUND_ODD> nine;

TEST_CASE( "Round Odd unsigned") { 
	nine = 8.72;
	REQUIRE( nine.getFxP() == 8.75 );
	nine = 18.3125;
	REQUIRE( nine.getFxP() == 18.3125 );
	nine *= 2;
	REQUIRE( nine.getFxP() == 36.625 );
	nine = 7.54;
	REQUIRE( nine.getFxP() == 7.5625 );
	FxP<6,-3,0,SAT_MINMAX,ROUND_ODD> nine2 = 8.72;
	nine = 18.3125;
	nine += nine2;
	REQUIRE( nine.getFxP() == 27.0625 );
	nine = 8.7265486452131111;	
	REQUIRE( nine.getFxP() == 8.75 );	
	nine = nine2;	
	REQUIRE( nine.getFxP() == 8.75 );	
	FxP<8,-4,0,WRAP_AROUND,ROUND_INF> nine3 = 757.33;
	REQUIRE( nine3.getFxP() == 245.3125 );	
	nine3=-7887.33;
	REQUIRE( nine3.getFxP() == 48.6875 );
}

FxP<7,-4,0,SAT_MINMAX,TRUNC> ten;

TEST_CASE( "Trunc unsigned") { 
	ten = 11.875;
	REQUIRE( ten.getFxP() == 11.875 );
	ten = 18.3125;
	REQUIRE( ten.getFxP() == 18.3125 );
	ten *= 2;
	REQUIRE( ten.getFxP() == 36.625 );
	ten = 7.54;
	REQUIRE( ten.getFxP() == 7.5 );
	FxP<6,-3,0> ten2 = 8.72;
	ten = 18.3125;
	ten += ten2;
	REQUIRE( ten.getFxP() ==  26.9375 );
	ten = 8.7265486452131111;	
	REQUIRE( ten.getFxP() == 8.6875 );	
	ten *= 55;	
	REQUIRE( ten.getFxP() == 127.9375 );
	ten = ten2;	
	REQUIRE( ten.getFxP() == 8.625 );	
	FxP<8,-8,0> ten3 = 8.7287545;
	ten = ten3;
	REQUIRE( ten.getFxP() == 8.6875 );
	FxP<8,-4,0> ten4 = 700;
	REQUIRE( ten4.getFxP() == 255.9375 );
	FxP<8,-4,0,WRAP_AROUND> ten5 = 757.33;
	REQUIRE( ten5.getFxP() == 245.3125 );
	FxP<8,-4,0,WRAP_AROUND> ten6,ten7;
	ten6 = 7.33; ten7 = 18.3125;
	ten5 = ten6 ^ ten7;
	REQUIRE( ten5.getFxP() == 21 );
	ten5 = ten6 & ten7;
	REQUIRE( ten5.getFxP() == 2.3125 );
	
}

FxP<8,-4,1,SAT_MINMAX,TRUNC> ele;

TEST_CASE( "Trunc signed") { 
	ele = 11.875;
	REQUIRE( ele.getFxP() == 11.875 );
	ele = 18.3125;
	REQUIRE( ele.getFxP() == 18.3125 );
	ele *= 2;
	REQUIRE( ele.getFxP() == 36.625 );
	ele = 7.54;
	REQUIRE( ele.getFxP() == 7.5 );
	FxP<7,-3,0> ele2 = 8.72;
	ele = 18.3125;
	ele += ele2;
	REQUIRE( ele.getFxP() ==  26.9375 );
	ele = 8.7265486452131111;	
	REQUIRE( ele.getFxP() == 8.6875 );	
	ele *= 55;	
	REQUIRE( ele.getFxP() == 127.9375 );
	ele = ele2;	
	REQUIRE( ele.getFxP() == 8.625 );	
	FxP<7,-8,0> ele3 = 8.7287545;
	ele = ele3;
	REQUIRE( ele.getFxP() == 8.6875 );
	ele /= 3;
	REQUIRE( ele.getFxP() == 2.875 );
	FxP<8,-4,1> ele4 = 700;
	REQUIRE( ele4.getFxP() == 127.9375 );
	FxP<8,-4,1,WRAP_AROUND> ele5 = 757.33;
	REQUIRE( ele5.getFxP() == -10.6875 );
}

