#pragma once
#include "button.hpp"

#define IN_SHOP_TIME 10

extern double shop_entering_anim_time; // Animacao para saida eh a mesma que a entrada, mas de tras para frente
extern bool in_shop;
extern double time_on_shop;
extern bool is_leaving_shop;
void update_shop();
void draw_shop();
void init_shop();