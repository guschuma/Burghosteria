#pragma once // Inclua esse header apenas uma vez
#include "raylib.h"
#include <string>


bool is_click_released();
bool is_click_held();

class Button {
public:
	Button(Rectangle bound, std::string text, Color normal_color, Color hover_color, Color clicked_color, int text_offset_px); // Construtor
	void update();
	void draw();
	bool is_released();
	bool is_held();
	Rectangle get_bound() {return bound;};
	Color get_color() {return color;};
	Color get_hover_color(){return hover_color;};
	Color get_click_color(){return clicked_color;};
	std::string get_text(){return text;};
	int get_text_offset(){return text_offset_px;};
	void set_text(std::string new_str){text = new_str; has_text = !new_str.empty();};

	float percentage_held = 0.0f; // Usado para itens com preparo. 1.0f = 100% segurado
	Rectangle bound;
private:
	
	Color color, hover_color, clicked_color;
	std::string text;
	bool has_text = false;
	
	int text_offset_px;

	
	bool is_being_hovered = false;
	Color current_color = color;
};

class Button_Matrix {
public:
	Button_Matrix(Button base_button, Vector2 gap, Vector2 start_pos, int lin, int col); // Construtor. Forma matriz lin x col de botoes
	~Button_Matrix(); // Destrutor
	void update();
	void draw();
	bool is_released(int ind);
	int id_of_button_released();
	int id_of_button_held();

	//void update_percentage_held(float new_perc_held, int button_id);
protected:
	int lin, col;
	Vector2 gap, start_pos;
	Button **buttons; // Vetor de ponteiros para botões
};


class Spell_Matrix : public Button_Matrix{
public:
	using Button_Matrix::Button_Matrix;

	void update_percentage_held(float new_perc_held, int button_id);
	int id_of_button_released();
	int id_of_button_held();
	void reset_all_text();// Seta texto para tudo zero
	void set_text_value_at_ingred(int value, int ingred_id, int time_till_expire = -1);
	void change_height(int button_id, int new_height);
private:
	
};

int button_to_ingred_map(int id_b);
int ingred_to_button_map(int id_b);