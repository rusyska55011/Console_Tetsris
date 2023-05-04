#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Figure {

	public:
		Figure(bool * figure, unsigned size, string name) : figure(figure), size(size), name(name) {
			
			// Делаем указатель на первый элемент массива, массив пришлось инициализировать в классе, чтобы не брать его аргументом в контрукторе
			this->figure_sided = sided[0];
			fill_figure();

			// Конструктор подкласса реализауется НЕ после конца кода этого конструктора, а сразу после инициализации переменных конструктора, не успев начать тело
			// конструктора Figure. Тут либо надо передавать figure_sided как аргумент этому конструктору, что я считаю не правильным, потому что это копия с другим
			// адресом памяти. Поэтому мы повторно инициализируем конструктор, поскольку указатель figure_sided вообще не передается без этого!
			this->rotation = Rotation{ this->figure, this->figure_sided, &this->size }; 
			
		};
		
		virtual string* get_name() {
			return &this->name;
		}

		virtual unsigned* get_size() {
			return &this->size;
		}

		virtual bool* get_figure() {
			return this->figure;
		}

		virtual bool* get_figure_sided() {
			return this->figure_sided;
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
			bool* p = this->figure;
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->size; j++) {
					cout << *(p++) << " ";
				}
				cout << "\n";
			}
		}

		virtual void show_figure_sided() const {
			bool* p = this->figure_sided;
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

				Rotation(bool * figure, bool * figure_sided, unsigned * size) : figure(figure), figure_sided(figure_sided), size(size) {};

				void to_normal() {
					int index;
					int size = *this->size;

					for (int y = 0; y < size; y++) {
						for (int x = 0; x < size; x++) {
							index = (y * size) + x;

							*(this->figure_sided + index) = *(this->figure + index);
						}
					}
				}

				void to_degree180(){
					int index, reversed_index;
					int size = *this->size;

					for (int y = 0; y < size; y++) {
						for (int x = 0; x < size; x++) {
							index = (y * size) + x;
							reversed_index = (size * size - 1) - index;
							
							*(this->figure_sided + index) = *(this->figure + reversed_index);		
						}
					}
				}

				void to_degree90(){
					int index, degree90_index;
					int size = *this->size;

					for (int y = 0; y < size; y++) {
						for (int x = 0; x < size; x++) {
							index = (y * size) + x;
							degree90_index = (size - (1 + y)) + (size * x);
							
							*(this->figure_sided + index) = *(this->figure + degree90_index);
						}
					}
				}

				void to_degree270() {
					int index, degree270_index;
					int size = *this->size;

					for (int y = 0; y < size; y++) {
						for (int x = 0; x < size; x++) {
							index = (y * size) + x;
							degree270_index = (size * size - (y + 1)) - (size * x);

							*(this->figure_sided + index) = *(this->figure + degree270_index);
						}
					}
				}

			protected:
				bool * figure;
				bool * figure_sided;
				unsigned * size;

			};

	private:
		unsigned size;

		bool* figure;
		bool* figure_sided;
		bool sided[4][4];

		string name;

		static rotate_angle angle;
		Rotation rotation{ this->figure, this->figure_sided, &this->size };

		void fill_figure() {
			for (int i = 0; i < this->size * this->size; i++)
				*(this->figure_sided + i) = *(this->figure + i);
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
		static bool box[4][4];
	public:
		Box() : Figure(*box, 4, "Box") {};
};
bool Box::box[4][4] = {
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
		AngleLine() : Figure(*anglline, 4, "Triangle") {};
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



class Mechanic {
	public:
		Mechanic(Map* map, Graphics* graphics, Figure* figure_colletion) : map(map), graphics(graphics), figure_colletion(figure_colletion) {}


	private:
		Map* map;
		Graphics* graphics;
		Figure* figure_colletion;
};



int main() {
	setlocale(LC_ALL, "ru");
	

	return 0;
}