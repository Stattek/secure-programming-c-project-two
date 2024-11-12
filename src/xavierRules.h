/**
 * Created by Xavier Zamora
 * Summary: Header for Xavier's rules.
 */
#ifndef XAVIERRULES_H
#define XAVIERRULES_H

void addItem(int index, char *itemName);

int getItemQuantity();

void addDuplicateItem(char *itemName, int quantity);

void displayMenu(int choice);

int isInventoryFull(int count);

void getItemName(char *name);

#endif