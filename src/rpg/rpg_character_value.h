#ifndef CIRCLEMUD_RPG_CHARACTER_VALUE_H
#define CIRCLEMUD_RPG_CHARACTER_VALUE_H

#include "rpg_character.h"
#include "rpg_value.h"

double rpg_character_rating(const rpg_character *character,
                            rpg_value_context context);

#endif
