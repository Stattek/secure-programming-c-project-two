/**
 * Created by Waleed Chatta
 * Summary: Header for Waleed's rules.
 */

#ifndef WALEEDRULES_H
#define WALEEDRULES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processInput_CWE1287(const char *input);
void checkValue_CWE480(int value);
void accessArray_CWE431(void);
void handleError_CWE430(int value);
void typeConfusion_CWE351(void);
void processInput_CWE115(const char *input);

#endif
