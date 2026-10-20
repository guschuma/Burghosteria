#include "button.hpp"

#include <iostream>

#include "global.hpp"

Button::Button(Rectangle bound, std::string text, Color normal_color, Color hover_color, Color clicked_color, int text_offset_px)
    : bound(bound), text(text), color(normal_color), hover_color(hover_color), clicked_color(clicked_color), text_offset_px(text_offset_px) {
	if(!text.empty()) has_text = true; 
}
void Button::update(){
	is_being_hovered = CheckCollisionPointRec(cursor_pos(), bound);
	current_color = is_being_hovered? hover_color : color;
	current_color = is_held()? clicked_color : current_color;
	
	if(!is_held()) percentage_held = 0.0f;
	if(is_blocked) {percentage_held = 1.0f;}
}

void Button::draw(){ // Assumindo que ja esta em texture ou render environment
	DrawRectangleRec(bound, current_color);
	if(has_text) {
		draw_cool_text(text.c_str(), bound.x + bound.width * 0.5 + text_offset_px, bound.y - 1, BLACK);
	}
	if(percentage_held != 0.0f){
		float shadow_box_h =  bound.height * percentage_held;
		DrawRectangleRec((Rectangle){bound.x, bound.y + bound.height - shadow_box_h, bound.width, shadow_box_h}, (Color){0, 0, 0, 30});
	}
}

bool Button::is_released(){
	return is_being_hovered && is_click_released();
}

bool Button::is_held(){
	return is_being_hovered && is_click_held();
}





Button_Matrix::Button_Matrix(Button base_button, Vector2 gap, Vector2 start_pos, int lin, int col)
		: gap(gap), start_pos(start_pos), lin(lin), col(col) {
	buttons = new Button*[lin * col];

	Rectangle base_button_bound = base_button.get_bound();
	Rectangle new_button_bound = base_button.get_bound();
	Color button_color = base_button.get_color();
	Color button_hover_color = base_button.get_hover_color();
	Color button_click_color = base_button.get_click_color();
	std::string button_text = base_button.get_text();
	int text_offset = base_button.get_text_offset();
	for(int i = 0; i < lin * col; i++){
		int line = i / col;
		int colu = i % col;

		new_button_bound.x = start_pos.x + colu * (gap.x + base_button_bound.width); 
		new_button_bound.y = start_pos.y + line * (gap.y + base_button_bound.height);
		buttons[i] = new Button(new_button_bound, button_text, button_color, button_hover_color, button_click_color, text_offset);
	}
}

Button_Matrix::~Button_Matrix(){
	for(int i = 0; i < lin * col; i++){
		delete buttons[i];
	}

	delete[] buttons;
}

void Button_Matrix::update(){
	for(int i = 0; i < lin * col; i++){
		buttons[i]->update();
	}

}

void Button_Matrix::draw(){
	for(int i = 0; i < lin * col; i++){
		buttons[i]->draw();
	}
}

bool Button_Matrix::is_released(int ind){
	return buttons[ind]->is_released();
}

int Button_Matrix::id_of_button_released(){
	for(int i = 0; i < lin * col; i++){
		if(buttons[i]->is_released()) return i;
	}
	return ERROR_CODE;
}

void Button_Matrix::set_origin(Vector2 new_origin){
	start_pos = new_origin;
	Rectangle base_button_bound = buttons[0]->get_bound();
	for(int i = 0; i < lin * col; i++){
		int line = i / col;
		int colu = i % col;
		Vector2 new_button_bound;
		new_button_bound.x = new_origin.x + colu * (gap.x + base_button_bound.width); 
		new_button_bound.y = new_origin.y + line * (gap.y + base_button_bound.height);
		buttons[i]->set_origin(new_button_bound);
	}
}


int Spell_Matrix::id_of_button_released(){
	for(int i = 0; i < lin * col; i++){
		if(buttons[i]->is_released()) return button_to_ingred_map(i);
	}
	return ERROR_CODE;
}
int Button_Matrix::id_of_button_held(){
	for(int i = 0; i < lin * col; i++){
		if(buttons[i]->is_held()) return i;
	}
	return ERROR_CODE;
}
int Spell_Matrix::id_of_button_held(){
	for(int i = 0; i < lin * col; i++){
		if(buttons[i]->is_held()) return button_to_ingred_map(i);
	}
	return ERROR_CODE;
}

void Spell_Matrix::update_percentage_held(float new_perc_held, int button_id){
	buttons[ingred_to_button_map(button_id)]->percentage_held = new_perc_held;
}


void Spell_Matrix::reset_all_text(){ // Nunca usado, provavelmente
	for(int i = 0; i < lin * col; i++){
		if(i <= 2) {
			buttons[i]->set_text("");
		}
		if(i >= 15){
			buttons[i]->set_text("");
		}
	}
}

void Spell_Matrix::set_text_value_at_ingred(int value, int ingred_id, int time_till_expire){ // time_till_expire default = -1
	// Todo: remove time_till_expire
	int button_id = ingred_to_button_map(ingred_id);
	if(button_id == ERROR_CODE) return;

	if(time_till_expire == EMPTY_HEAP) {
		buttons[button_id]->set_text("0!");
		return;
	}
	buttons[button_id]->is_blocked = time_till_expire == EMPTY_HEAP || value == 0;
	
	std::string new_text = std::to_string(value);
	buttons[button_id]->set_text(new_text);
}

void Spell_Matrix::change_height(int button_id, int new_height){
	buttons[button_id]->bound.height = new_height;
}

bool is_click_released(){
	return IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_RIGHT);
}

bool is_click_held(){
	return IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
}

int button_to_ingred_map(int id_b){ // Muda os lugar dos ingredientes. Ingredientes preparados ficam no canto
	if(id_b > 17 || id_b < 0) return ERROR_CODE;
	return id_b - 3;
}

int ingred_to_button_map(int id_b){
	if(id_b > 14 || id_b < -3) return ERROR_CODE;
	return id_b + 3;
}


