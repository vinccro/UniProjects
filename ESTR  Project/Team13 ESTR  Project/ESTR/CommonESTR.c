/*
 * CommonESTR.c
 *
 *  Created on: Oct 3, 2015
 *      Author: ControlHat
 */
#include "commonESTR.h"
//*****************************************************************************
//
//! Checks if char is number or space or dot.
//!
//! \param character to be checked.
//!
//!
//! \return one if character is number or space or dot.
//
//*****************************************************************************
int isNum(char c){
	if(c == '0') return 1;
	if(c == '1') return 1;
	if(c == '2') return 1;
	if(c == '3') return 1;
	if(c == '4') return 1;
	if(c == '5') return 1;
	if(c == '6') return 1;
	if(c == '7') return 1;
	if(c == '8') return 1;
	if(c == '9') return 1;
	if(c == '.') return 1;
	if(c == ' ') return 1;
	return 0;
}
//*****************************************************************************
//
//! Checks if char is number.
//!
//! \param character to be checked.
//!
//!
//! \return one if character number else zero.
//
//*****************************************************************************
int issNum(char c){
	if(c == '0') return 1;
	if(c == '1') return 1;
	if(c == '2') return 1;
	if(c == '3') return 1;
	if(c == '4') return 1;
	if(c == '5') return 1;
	if(c == '6') return 1;
	if(c == '7') return 1;
	if(c == '8') return 1;
	if(c == '9') return 1;
	return 0;
}
//*****************************************************************************
//
//! Checks to see string is in a correct format
//!
//! \param *string and *stringformat
//!
//!
//! \return one if string is in correct format.
//
//*****************************************************************************
int FSM(char* string1, char* string2){
    int i;
 for( i = 0; i < strlen(string1); i++){
	 if(string1[i] == '&'){
		 if(isNum(string2[i]) == 0) return 0;
	 }else{
		 if(string1[i] == string2[i]){
		 }else{
			 return 0;
		 }
	 }
 }
// printf(":::%s:::", string2);
return 1;
}
