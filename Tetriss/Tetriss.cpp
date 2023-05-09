﻿#include <iostream>
#include <string>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;


template <typename Type>
Type* get_index_pointer_of_double_array(Type *pointer, const unsigned sizey, const unsigned sizex, const unsigned y, const unsigned x){
	return sizey >= sizex ? (pointer + (y * sizey) + x) : (pointer + (y * sizex) + x);
}


class Figure {

	public:
		Figure(const bool * figure, const unsigned size, const string name) : figure(figure), size(size), name(name) {
			
			fill_figure();

			// Конструктор подкласса реализауется НЕ после конца кода этого конструктора, а сразу после инициализации переменных конструктора, не успев начать тело
			// конструктора Figure. Тут либо надо передавать figure_sided как аргумент этому конструктору, что я считаю не правильным, потому что это копия с другим
			// адресом памяти. Поэтому мы повторно инициализируем конструктор, поскольку указатель figure_sided вообще не передается без этого!
			this->rotation = Rotation{ this->figure, this->figure_sided[0], &this->size};
			
		};
		
		virtual const string* get_name() {
			return &this->name;
		}

		virtual const unsigned* get_size() {
			return &this->size;
		}

		virtual const bool* get_figure() {
			return this->figure;
		}

		virtual bool* get_figure_sided() {
			return this->figure_sided[0];
		}

		enum class rotate_angle { Normal = 0, Degree90 = 1, Degree180 = 2, Degree270 = 3 }; // Перенести объект в Rotation не получится, поскольку не будет аргументов для rotate_figure

		virtual void rotate_figure(rotate_angle angle) {
			if (this->angle != angle)
				this->do_rotate(angle);
		}

		virtual void rotate_figure() {
			Figure::rotate_angle angle = this->angle;

			if (angle != rotate_angle::Degree270) {
				this->rotate_figure(static_cast<Figure::rotate_angle>(static_cast<int>(angle) + 1));
				return;
			}
			else {
				this->rotate_figure(static_cast<Figure::rotate_angle>(0));
			}
		}
		
		virtual void show_figure() const {
			const bool* p = this->figure;
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->size; j++) {
					cout << *(p++) << " ";
				}
				cout << "\n";
			}
		}

		virtual void show_figure_sided() const {
			const bool* p = this->figure_sided[0];
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->size; j++) {
					cout << *(p++) << " ";
				}
				cout << "\n";
			}
		}

		virtual void show_angle() const {
			cout << static_cast<int>(this->angle) << "\n";
		}

	protected:
		class Rotation {
			public:

				Rotation(const bool * figure, bool * figure_sided, const unsigned * size) : figure(figure), figure_sided(figure_sided), size(size) {};


				void to_normal() {
					auto degree_default_by_step_replace{
						[](int index, int size, int y, int x) {return index;}
					};
					this->redraw_figure_sided_by_figure(degree_default_by_step_replace);
				}

				void to_degree180() {
					auto degree180_by_step_replace{
						[](int index, int size, int y, int x) {return (size * size - 1) - index;}
					};
					this->redraw_figure_sided_by_figure(degree180_by_step_replace);
				}

				void to_degree90() {
					auto degree90_by_step_replace{
						[](int index, int size, int y, int x) {return (size - (1 + y)) + (size * x);}
					};
					this->redraw_figure_sided_by_figure(degree90_by_step_replace);
				}

				void to_degree270() {
					auto degree270_by_step_replace{ 
						[](int index, int size, int y, int x) {return (size * size - (y + 1)) - (size * x);}
					};
					this->redraw_figure_sided_by_figure(degree270_by_step_replace);
				}

			protected:
				const bool * figure;
				bool * figure_sided;
				const unsigned * size;
			
			private:
				void redraw_figure_sided_by_figure(int (*to_do_by_step)(int index, int size, int y, int x)) {
					int index, sided_index;
					const int size = *this->size;

					for (int y = 0; y < size; y++) {
						for (int x = 0; x < size; x++) {
							index = (y * size) + x;
							sided_index = to_do_by_step(index, size, y, x);

							*(this->figure_sided + index) = *(this->figure + sided_index);
						}
					}
				}
			};

	private:
		const unsigned size;

		const bool* figure;
		bool figure_sided[4][4];

		const string name;

		static rotate_angle angle;
		Rotation rotation{ this->figure, this->figure_sided[0], &this->size};

		void fill_figure() {
			for (int i = 0; i < this->size * this->size; i++)
				*(this->figure_sided[0] + i) = *(this->figure + i);
		}

		void do_rotate(rotate_angle angle) {
			switch (angle) {
				case rotate_angle::Normal:
					this->rotation.to_normal();
					break;
				case rotate_angle::Degree90:
					this->rotation.to_degree90();
					break;
				case rotate_angle::Degree180:
					this->rotation.to_degree180();
					break;
				case rotate_angle::Degree270:
					this->rotation.to_degree270();
					break;
			}
			this->angle = angle;		
		}
};
Figure::rotate_angle Figure::angle = Figure::rotate_angle::Normal;



class Box : public Figure {
	private:
		const static bool box[4][4];
	public:
		Box() : Figure(*box, 4, "Box") {};
};
const bool Box::box[4][4] = {
	{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0},
};



class Line : public Figure {
	private:
		static bool line[4][4];
	public:
		Line() : Figure(*line, 4, "Line") {};
};
bool Line::line[4][4] = {
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
};



class ZigZag : public Figure {
	private:
		static bool zigzag[4][4];
	public:
		ZigZag() : Figure(*zigzag, 4, "ZigZag") {};
};
bool ZigZag::zigzag[4][4] = {
	{0, 0, 0, 0},
	{1, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0},
};



class Triangle : public Figure {
	private:
		static bool triangle[4][4];
	public:
		Triangle() : Figure(*triangle, 4, "Triangle") {};
};
bool Triangle::triangle[4][4] = {
	{0, 0, 0, 0},
	{0, 1, 0, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0},
};



class AngleLine : public Figure {
	private:
		static bool anglline[4][4];
	public:
		AngleLine() : Figure(*anglline, 4, "AngleLine") {};
};
bool AngleLine::anglline[4][4] = {
	{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
};



class Map {
	public:
		static bool area[16][8];
};
bool Map::area[16][8] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
};



class Graphics {
	public:
		Graphics(Map* map, Figure*& selected_figure, unsigned* figure_map_position_yx) : map(map), selected_figure(selected_figure), figure_map_position_yx(figure_map_position_yx) {};

		void render () const {
			unsigned figure_posisition_y = this->figure_map_position_yx[0];
			unsigned figure_posisition_x = this->figure_map_position_yx[1];
			const unsigned figure_size = *this->selected_figure->get_size();
			const bool* figure = this->selected_figure->get_figure_sided();

			unsigned map_size_yx[2]{ 16, 8 };
			unsigned tabulation = 1;

			system("cls");

			for (int y = 0; y < map_size_yx[0]; y++) {
		
				this->start_field(tabulation);
				for (int x = 0; x < map_size_yx[1]; x++) {
					
					if (this->map->area[y][x]) {
						this->draw_pixel();
					} else if ((figure_posisition_y <= y) && (figure_posisition_y + figure_size > y)) {
						
						if ((figure_posisition_x <= x) && (figure_posisition_x + figure_size > x)) {
							
							bool pixel = *get_index_pointer_of_double_array(figure, figure_size, figure_size, y - figure_posisition_y, x - figure_posisition_x);
							if (pixel)
								this->draw_pixel();
							else
								this->draw_void();

						} else
							this->draw_void();
					} else
						this->draw_void();
				}
				this->end_field();
			}
		}

	private:

		void do_tabulation(unsigned num) const {
			for (int i = 0; i < num; i++) {
				cout << "\t";
			}
		}

		void draw_pixel() const {
			cout << " *";
		}

		void draw_void() const {
			cout << "  ";
		}

		void start_field(unsigned tabulation) const {
			this->do_tabulation(tabulation);
			cout << " |";
		}

		void end_field() const {
			cout << " |\n";
		}

		void draw_horizontal_line(unsigned size, unsigned tabulation) const {
			this->do_tabulation(tabulation);
			for (int i = 0; i < size; i++) {
				cout << "--";
			}
			cout << "\n";
		}

		Map* map;
		Figure*& selected_figure;
		unsigned* figure_map_position_yx;
};



class UserInput {
	public:
		UserInput(char& key) : key(key){};
		
		void read_keys_infinite() {
			this->process = 1;

			char getted = 0;
			while (this->process) {
				getted = _getch();
				if (getted) {
					this->key = getted;
				}
			}
		}

		void stop() {
			this->process = 0;
		}

		void start() {
			this->process = 1;
		}

	private:
		char& key;
		static bool process;
};
bool UserInput::process = 0;



class Mechanic {
	public:

		Mechanic(Map* map, char& ui_key, Figure* figure_colletion[]) : map(map), ui_key(ui_key) {
			this->figure_map_position_yx[0] = 0;
			this->figure_map_position_yx[1] = 2;

			this->fill_figure_collection(figure_colletion);
			this->selected_figure = figure_colletion[rand() % 5];
		}
		
		Figure::rotate_angle rotation;

		unsigned* get_figure_position() {
			return this->figure_map_position_yx;
		}

		Figure*& get_selected_figure() {
			return this->selected_figure;
		}

		void select_new_figure() {
			this->selected_figure = this->figure_colletion[rand() % 5];
			this->fixing_figure_sided_in_map_pixel_or_out_off_map();
		}

		void figure_go_down() {
			this->figure_map_position_yx[0] += 1;
			this->fixing_figure_sided_in_map_pixel_or_out_off_map();
		}

		void try_set_figure() {
			if (this->is_setted()) {
				bool* figure_sided = this->selected_figure->get_figure_sided();

				int figure_map_position_y = this->figure_map_position_yx[0];
				int figure_map_position_x = this->figure_map_position_yx[1];

				for (int figure_pixel_y = 0; figure_pixel_y < 4; figure_pixel_y++) {
					for (int figure_pixel_x = 0; figure_pixel_x < 4; figure_pixel_x++) {
						bool checked_pixel_figure = *get_index_pointer_of_double_array(figure_sided, 4, 4, figure_pixel_y, figure_pixel_x);

						int figure_pixel_map_position_y = figure_map_position_y + figure_pixel_y;
						int figure_pixel_map_position_x = figure_map_position_x + figure_pixel_x;
						
						if (checked_pixel_figure) {
							this->map->area[figure_pixel_map_position_y][figure_pixel_map_position_x] = true;
						}
					}
				}
				this->figure_map_position_yx[0] = 0;
				this->figure_map_position_yx[1] = 3;
			}
		}

		void figure_rotate() {
			this->selected_figure->rotate_figure();
			this->fixing_figure_sided_in_map_pixel_or_out_off_map();
		}

		void delete_full_rows() {
			int row;
			while (true) {
				row = this->find_full_row();
				if (row >= 0) {
					for (int x = 0; x < 16; x++) {
						this->map->area[row][x] = 0;
					}
				} else {
					break;
				}
			}
		}

		bool is_game_over() {
			for (int x = 0; x < 16; x++) {
				if (this->map->area[0][x])
					return true;
			}
			return false;
		}

	protected:

		Figure* selected_figure;
		Figure* figure_colletion[5];

		Map* map;
		char& ui_key;
		unsigned figure_map_position_yx[2];
		
		bool is_setted() {
			bool* figure_sided = this->selected_figure->get_figure_sided();

			int figure_map_position_y = this->figure_map_position_yx[0];
			int figure_map_position_x = this->figure_map_position_yx[1];

			for (int figure_pixel_y = 0; figure_pixel_y < 4; figure_pixel_y++) {
				for (int figure_pixel_x = 0; figure_pixel_x < 4; figure_pixel_x++) {

					bool checked_pixel_figure = *get_index_pointer_of_double_array(figure_sided, 4, 4, figure_pixel_y, figure_pixel_x);

					int figure_pixel_map_position_y = figure_map_position_y + figure_pixel_y;
					int figure_pixel_map_position_x = figure_map_position_x + figure_pixel_x;

					if (checked_pixel_figure) {
						if (this->map->area[figure_pixel_map_position_y + 1][figure_pixel_map_position_x] == true) {
							return true;
						}
						if (figure_pixel_map_position_y + 1 >= 16) {
							return true;
					}
				}
			}
			}
			return false;
		}
	
		bool is_figure_sided_in_map_pixel_or_out_off_map() {
			bool* figure_sided = this->selected_figure->get_figure_sided();

			int figure_map_position_y = this->figure_map_position_yx[0];
			int figure_map_position_x = this->figure_map_position_yx[1];

			for (int figure_pixel_y = 0; figure_pixel_y < 4; figure_pixel_y++) {
				for (int figure_pixel_x = 0; figure_pixel_x < 4; figure_pixel_x++) {

					bool checked_pixel_figure = *get_index_pointer_of_double_array(figure_sided, 4, 4, figure_pixel_y, figure_pixel_x);

					int figure_pixel_map_position_y = figure_map_position_y + figure_pixel_y;
					int figure_pixel_map_position_x = figure_map_position_x + figure_pixel_x;

					if (checked_pixel_figure) {
						if (this->map->area[figure_pixel_map_position_y][figure_pixel_map_position_x] == true) {
							return true;
						}
						if ((figure_pixel_map_position_y > 15) || (figure_pixel_map_position_x > 7)) {
							return true;
						}
					}
				}
			}
			return false;
		}

		void fixing_figure_sided_in_map_pixel_or_out_off_map() {
			unsigned default_y = this->figure_map_position_yx[0];
			unsigned default_x = this->figure_map_position_yx[1];

			if (this->is_figure_sided_in_map_pixel_or_out_off_map()) {

				this->figure_map_position_yx[0] -= 1;

				if (this->is_figure_sided_in_map_pixel_or_out_off_map()) {

					this->figure_map_position_yx[0] = default_y;

					for (int i = 1; i < *this->selected_figure->get_size() * 2 - 2; i++) {

						if (this->is_figure_sided_in_map_pixel_or_out_off_map()) {
							this->figure_map_position_yx[1] = default_x;

							this->figure_map_position_yx[1] += i;

							if (this->is_figure_sided_in_map_pixel_or_out_off_map()) {
								this->figure_map_position_yx[1] -= i + i;
							}
							else
								return;

						}
						else
							return;
					}
				}
			}
		}

		int find_full_row() {
			for (int y = 0; y < 16; y++) {
				for (int x = 0; x < 8; x++) {
					if (!(this->map->area[y][x]))
						break;
						
					if (x == 7)
						return y;
				}
			}
			return -1;
		}

	private:

		void fill_figure_collection(Figure* new_figure_collection[]) {
			for (int i = 0; i < 5; i++) {
				this->figure_colletion[i] = new_figure_collection[i];
			}
		}
};



class Game {
	public:
		static void run() {
			Map map;
			char key;

			Box box;
			Line line;
			AngleLine angleline;
			ZigZag zigzag;
			Triangle triangle;

			Figure* figure_collection[] = { &box, &line, &angleline, &zigzag, &triangle };

			Mechanic mechanic{ &map, key, figure_collection};
			
			Graphics graphics{ &map, mechanic.get_selected_figure(), mechanic.get_figure_position()};

			graphics.render();
			while (true) {
				
				mechanic.figure_go_down();

				mechanic.try_set_figure();

				graphics.render();

				mechanic.figure_rotate();

				this_thread::sleep_for(chrono::milliseconds(1000));
			}

		}
};

int main() {
	setlocale(LC_ALL, "ru");
	
	Game::run();

	return 0;
}
