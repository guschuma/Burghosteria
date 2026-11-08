#pragma once
#include "structs/history.hpp"

void update_defeat(History &name_history);

void draw_defeat();

void init_defeat();
bool get_name(); // 1 = Usuario parou de digitar (pressionou enter), 0 = Usuario continua a digitar