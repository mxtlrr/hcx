#pragma once
#include <stdio.h>

static char* termcmd[] = { "xfce4-terminal", NULL }; // F2 will spawn this
static char* menucmd[] = { "dmenu_run", NULL };      // F3 will spawn this


#define LMB 1
#define RMB 3

int menu_btn  = 1; // button to be pressed to spawn the menu
int other_btn = 3;