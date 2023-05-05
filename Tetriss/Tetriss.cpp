#include <iostream>
#include <string>
#include <conio.h>
#include <thread>

using namespace std;


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
		Graphics(Map * map) : map(map) {};

		void show () const {
			system("cls");
			for (int y = 0; y < 16; y++) {
				cout << "\n\t\t";
				for (int x = 0; x < 8; x++) {
					if (this->map->area[y][x])
						cout << "*";
					else
						cout << " ";
				}
			}
		}

	private:
		Map* map;
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
		Mechanic(Map* map, char& ui_key, Figure* figure_colletion) : map(map), ui_key(ui_key), figure_colletion(figure_colletion) {}



	private:
		Map* map;
		Figure* figure_colletion;
		char& ui_key;
};



int main() {
	setlocale(LC_ALL, "ru");
	

	return 0;
}
