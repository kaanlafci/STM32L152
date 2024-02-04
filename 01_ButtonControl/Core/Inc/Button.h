/*
 * Button.h
 *
 *  Created on: Feb 4, 2024
 *      Author: win
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

void ButtonMainTask(void);
void ButtonInitTask(void);

extern uint32_t BC_Debounce_Set_Val;
#endif /* INC_BUTTON_H_ */
