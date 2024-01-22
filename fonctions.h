
#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <stdio.h>
#include <stdbool.h>
#define ANSI_COLOR_CYN CYN
#define ANSI_COLOR_BLU BLU
#define ANSI_COLOR_RED RED
#define ANSI_COLOR_MAG MAG
#define ANSI_COLOR_GRN GRN
#define ANSI_COLOR_YEL YEL
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"

extern int i, j, main_exit;
struct date {
    int month, day, year;
};

float interest(float t, float amount, int rate);
void fordelay(int j);
void new_acc();
void view_list();
void edit(void);
void transact(void);
void erase(void);
void see(void);
void close(void);
void menu(void);
#endif
