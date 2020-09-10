#pragma once

#include <MD_Menu.h>

/* menu system */
#define MENU_TIMEOUT 5000

bool display(MD_Menu::userDisplayAction_t action, char *msg = nullptr);
MD_Menu::userNavAction_t navigation(uint16_t &incDelta);

// Function prototypes for variable get/set functions
MD_Menu::value_t *mnuLValueRqst(MD_Menu::mnuId_t id, bool bGet);
MD_Menu::value_t *mnuBValueRqst(MD_Menu::mnuId_t id, bool bGet);
MD_Menu::value_t *mnuEValueRqst(MD_Menu::mnuId_t id, bool bGet);
MD_Menu::value_t *mnuExit(MD_Menu::mnuId_t id, bool bGet);
