/*
 * commonESTR.h
 *
 *  Created on: Oct 3, 2015
 *      Author: ControlHat
 */

#ifndef COMMONESTR_H_
#define COMMONESTR_H_
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
int isNum(char c);
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
int issNum(char c);

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
int FSM(char* string1, char* string2);

#endif /* COMMONESTR_H_ */
