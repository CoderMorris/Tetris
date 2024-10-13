#pragma once
#include "AssetManager.h"
#include <vector>
#include <chrono>
#include <random>
#include <math.h>
#include <string>
#include <codecvt>
#include <iostream>
#include <fstream> 
#include <array>

wstring player_name = L"";
void ChoicePlayer()
{
	player_name = L"";
	bool is_input = true;
	Font font;
	font.loadFromFile("font/212.ttf");
	Text text("", font, 20);
	text.setPosition({ 10.f, 40.f });
	RenderWindow window(VideoMode(320, 100), L"Введите имя игрока", Style::Close);
	window.requestFocus();
	window.setMouseCursorVisible(false);
	Image icon;
	icon.loadFromFile("image/game.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	while (is_input)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::TextEntered && window.hasFocus())
			{
				wcout << player_name << endl;
				// Проверка, что введенный символ является буквой или цифрой кириллической кодировки UTF-16
				if (iswalnum(event.text.unicode) || (event.text.unicode >> 8) == 0x04)
				{
					if (player_name.size() < 10) 
					{
						player_name += event.text.unicode; // Добавление символа к имени игрока
						text.setString(player_name);
						text.setFillColor(Color(54, 171, 217));
					}
				}
				// Обработка клавиши Backspace
				else if (event.text.unicode == 8 && player_name.size() > 0)
				{
					player_name.pop_back();
					text.setString(player_name);
					text.setFillColor(Color(54, 171, 217));
				}
				// Обработка клавиши пробел
				else if (event.text.unicode == 32 && player_name.size() > 0)
				{
					is_input = false;
					window.close();
				}
			}
		}
		window.clear(Color(22, 70, 89));
		window.draw(text);
		window.display();
	}
	wcout << player_name << endl;
};

class Tetramino
{
public:
	enum class direction { left = -1, nuLL, right }; // перечисление направлений движения тетрамино по горизонтали
	enum class ch { x, y, rotation }; // перечисление проверки координат на столкновение с установлеными границами при перемещении и вращении

	// конструктор тетрамино
	explicit Tetramino(RenderWindow& window, Vector2f pos, Vector2i square_size, float scale) : height(square_size.y), width(square_size.x), window(window), tet(pos), scale(scale)
	{
		cube->setOutlineColor(Color(22, 70, 89)); // установка цвета обводки кубика 
		cube->setOutlineThickness(-1); // установка толщины обводки кубика
		cube->setSize(Vector2f(scale, scale)); // установка размера кубика 
		for (int i = 0; i < width; i++)
		{
			vector<Color> v;
			for (int j = 0; j < height; j++)
			{
				v.push_back(Color::Black);
			}
			square.push_back(v);
		}
		restart();
	}

	// метод устанавливающий вектор движения тетрамино
	void tetDirection(direction dir) { click_dx = static_cast<float> (dir); }

	void draw() // метод рисующий тетрамино в графическом окне
	{
		if (positionmaket.x >= 0) // проверка на выход за границу экрана
		{
			cube->setFillColor(tetcolor[colTet.y]); // установка цвета заполнения кубика
			for (int i = 0; i < 4; i++)
			{
				cube->setPosition((figures[typeTet.y][i] % 2) * scale, (static_cast<float>(figures[typeTet.y][i] / 2)) * scale);
				cube->move(positionmaket); // перемещение кубика на новую позицию
				window.draw(*cube); // отрисовка кубика в новой позиции
			}
		}
		for (int i = 0; i < width; i++)  // перебор по горизонтали
		{
			for (int j = 0; j < height; j++) // перебор по вертикали
			{
				cube->setFillColor(square[i][j]); // установка цвета заполнения кубика
				cube->setPosition(static_cast<float>(i) * scale, static_cast<float>(j) * scale);
				cube->move(tet); // перемещение кубика на новую позицию
				window.draw(*cube); // отрисовка кубика в новой позиции
			}
		}
		cube->setFillColor(tetcolor[colTet.x]); // установка цвета заполнения кубика
		for (int i = 0; i < t.size(); i++)
		{
			cube->setPosition(t[i].x * scale, t[i].y * scale);
			cube->move(tet); // перемещение кубика на новую позицию
			window.draw(*cube); // отрисовка кубика в новой позиции
		}
	}


	void update(Time const& deltaTime) 	// метод обновления игровой логики тетрамино
	{
		frameRate += deltaTime; // добавление времени к переменной frameRate
		if (frameRate > milliseconds(delay)) // проверка на истечение времени
		{
			frameRate = milliseconds(0); // сброс времени
			if (check(ch::x) && click_dx != 0) // проверка возможности движения по горизонтали
			{
				for (int i = 0; i < 4; i++) t[i].x += click_dx; click_dx = 0; // перемещение тетрамино в новую позицию
			}
			// перемещение тетрамино вниз
			if (check(ch::y)) { for (int i = 0; i < 4; i++)  t[i].y += click_dy; }
			else
			{
				for (int i = 0; i < t.size(); i++)
				{
					// если тетрамино дошло до верхней границы игрового поля
					if (static_cast<int>(t[i].y) == 2)
					{
						Font font;
						font.loadFromFile("font/212.ttf");
						Text scoreText(L"Игра завершена. Ваш счет: " + to_string(score), font, 28);
						scoreText.setPosition(20.f, 260.f);
						scoreText.setFillColor(Color::Red);
						window.draw(scoreText);
						window.display();
						wofstream file("records.txt", std::ios_base::app);
						file.imbue(std::locale(file.getloc(), new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::codecvt_mode(std::consume_header | std::generate_header)>));
						if (file.is_open())
						{
							file << score << " " << player_name << std::endl;
							file.close();
						}
						clock_t start_time = clock();
						while ((clock() - start_time) / CLOCKS_PER_SEC < 2);
						window.close();
					}
					// добавление тетрамино в массив игрового поля
					square[static_cast<size_t>(t[i].x)][static_cast<size_t>(t[i].y)] = Color(tetcolor[colTet.x]);
				}
				int numLine = 0; // количество заполненных линий
				for (int j = 0; j < height; j++)
				{
					int line = 0;
					for (int i = 0; i < width; i++)
					{
						if (square[i][j] != Color::Black) line++; // подсчет заполненных клеток в строке
						if (line == width) // если всю строку можно удалить
						{
							lineDead(j); // удаление строки
							numLine++; // увеличение счетчика заполненных линий
						}
					}
				}
				if (numLine != 0)
				{
					score += 5 * (numLine * numLine); // добавление очков за удаление линий
				}
				newFigrois(); // генерация нового тетрамино
			}
		}
	}

	void rotate() // метод вращения тетрамино
	{
		if (typeTet.x != 6) // для квадрата нет вариантов вращения
		{
			if (check(ch::rotation)) // проверка возможности вращения
			{
				// вычисление центра вращения
				Vector2f centerRotation = t[1];
				for (int i = 0; i < t.size(); i++)
				{
					float x = t[i].y - centerRotation.y;
					float y = t[i].x - centerRotation.x;
					// поворот каждой точки
					t[i].x = centerRotation.x - x;
					t[i].y = centerRotation.y + y;
				}

			}
		}
	}

	Vector2f getPositio() // метод возвращающий координаты центра тетрамино
	{
		Vector2f pos;
		// вычисление координат центра тетрамино по горизонтали
		pos.x = t[1].x * scale + tet.x;
		// вычисление координат центра тетрамино по вертикали
		pos.y = t[1].y * scale + tet.y;
		return pos;
	}


	void speed() { delay = 10; } // метод ускоряющий падение тетрамино

	void restart() // метод сбрасывающий все свойства в начальные значения - рестарт игры
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				square[i][j] = Color::Black;
			}
		}
		typeTet.y = d(rnd);
		colTet.y = d(rnd);
		score = 0;
		newFigrois();
	}

	int getscore() const { return score; } // метод возвращающий количество выигранных очков
	void maket(Vector2f posmak) { positionmaket = posmak; } // метод отображения макета следующего тетрамино

private:
	const int height; // высота игрового поля
	const int width; // ширина игрового поля
	const  float click_dy = 1.0f; // шаг перемещения тетрамино по y
	vector<vector<Color>> square; // массив игрового поля

	// массив локальных координат фигурок тетрамино 
	array<array<int, 4>, 7> figures
	{ {{1,3,5,7},{2,4,5,7},{3,4,5,6},{3,4,5,7},{2,3,5,7},{3,5,6,7},{2,3,4,5}} };

	array<Vector2f, 4> t; // положение прямоугольника в построении тетрамино 

	// массив цвета для тетрамино
	array<Color, 7> tetcolor{ {Color::Blue,Color::Cyan,Color::Yellow,
	Color::Green,Color::Magenta,Color(139,0,255),Color::White} };

	// прямоугольник тетрамино
	unique_ptr<RectangleShape> cube = make_unique<RectangleShape>();

	// момент системного времени
	long long seed = chrono::system_clock::now().time_since_epoch().count();

	// запуск генератора случайных чисел
	default_random_engine rnd = default_random_engine(static_cast<long>(seed));

	// установка диапазона случайных чисел
	uniform_int_distribution<int> d = uniform_int_distribution<int>(0, 6);

	// ссылка на графическое окно
	RenderWindow& window;

	const Vector2f tet; // начальные координаты тетрамино
	Time frameRate; // интервал обновления игровой логики 
	Vector2i typeTet; // тип тетрамино 
	Vector2i colTet; // цвет тетрамино 
	void newFigrois()  // новый тетрамино 
	{
		typeTet.x = typeTet.y; // сохраняем текущий тип тетрамино как предыдущий
		colTet.x = colTet.y;  // сохраняем текущий цвет тетрамино как предыдущий

		// генерируем новое тетрамино с случайным типом и цветом
		for (int i = 0; i < t.size(); i++)
		{
			t[i].x = figures[typeTet.x][i] % 2 + static_cast<float>(floor(width / 2));
			t[i].y = static_cast<float>(figures[typeTet.x][i] / 2);
		}
		typeTet.y = d(rnd); // выбираем новый тип тетрамино случайным образом
		colTet.y = d(rnd);  // выбираем новый цвет тетрамино случайным образом
		delay = 250; // устанавливаем задержку по умолчанию
	}

	void lineDead(int g) // уничтожение полоски тетрамино при заполнении поля по горизонтали 
	{
		// сдвигаем все клетки выше заполненной линии на одну строку вниз, чтобы "удалить" заполненную линию
		for (int i = g; i > 0; i--)
		{
			for (int j = 0; j < width; j++)
			{
				square[j][i] = square[j][static_cast<size_t>(i - 1)];
			}
		}
	}

	bool check(ch ch) // проверка положения тетрамино
	{
		switch (ch)
		{
		// проверка перемещения тетрамино по горизонтали
		case Tetramino::ch::x:
		{
			for (int i = 0; i < t.size(); i++)
			{
				if ((t[i].x + click_dx < 0) || (t[i].x + click_dx > static_cast<float>(width - 1)))
					return false;
				if ((static_cast<int>(t[i].y) >= 0) && (square[static_cast<size_t>(t[i].x + click_dx)][static_cast<size_t>(t[i].y)] != Color::Black))
					return false;
			}
			break;
		}
		// проверка падения тетрамино
		case Tetramino::ch::y:
		{
			for (int i = 0; i < t.size(); i++)
			{
				if ((t[i].y + click_dy) > static_cast<float>(height - 1))
					return false;
				if ((static_cast<int>(t[i].y + click_dy) >= 0) && (square[static_cast<size_t>(t[i].x)][static_cast<size_t>(t[i].y + click_dy)] != Color::Black))
					return false;
			}
			break;
		}
		// проверка вращения тетрамино
		case Tetramino::ch::rotation:
		{
			Vector2f centerRotation = t[1];
			for (int i = 0; i < t.size(); i++)
			{
				float x = t[i].y - centerRotation.y;
				float y = t[i].x - centerRotation.x;
				if (((centerRotation.x - x) < 0) || ((centerRotation.x - x) > static_cast<float>(width - 1)) ||
					((centerRotation.y + y) > static_cast<float>(height - 1)))
					return false;
				if ((static_cast<int>(centerRotation.y + y) >= 0) &&
					(square[static_cast<size_t>(centerRotation.x - x)][static_cast<size_t>(centerRotation.y + y)]
						!= Color::Black))
					return false;
			}
			break;
		}
		default: break;
		}
		return true;
	}
	Int32 delay; // интервал обработки игровой логики
	float click_dx; // шаг перемещения тетрамино по x
	int score; // очки выигрыша
	float scale; // масштаб кубика
	Vector2f positionmaket = Vector2f(-1, -1); // свойство координат макета тетрамино
};
