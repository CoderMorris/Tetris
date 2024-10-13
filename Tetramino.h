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
	RenderWindow window(VideoMode(320, 100), L"������� ��� ������", Style::Close);
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
				// ��������, ��� ��������� ������ �������� ������ ��� ������ ������������� ��������� UTF-16
				if (iswalnum(event.text.unicode) || (event.text.unicode >> 8) == 0x04)
				{
					if (player_name.size() < 10) 
					{
						player_name += event.text.unicode; // ���������� ������� � ����� ������
						text.setString(player_name);
						text.setFillColor(Color(54, 171, 217));
					}
				}
				// ��������� ������� Backspace
				else if (event.text.unicode == 8 && player_name.size() > 0)
				{
					player_name.pop_back();
					text.setString(player_name);
					text.setFillColor(Color(54, 171, 217));
				}
				// ��������� ������� ������
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
	enum class direction { left = -1, nuLL, right }; // ������������ ����������� �������� ��������� �� �����������
	enum class ch { x, y, rotation }; // ������������ �������� ��������� �� ������������ � ������������� ��������� ��� ����������� � ��������

	// ����������� ���������
	explicit Tetramino(RenderWindow& window, Vector2f pos, Vector2i square_size, float scale) : height(square_size.y), width(square_size.x), window(window), tet(pos), scale(scale)
	{
		cube->setOutlineColor(Color(22, 70, 89)); // ��������� ����� ������� ������ 
		cube->setOutlineThickness(-1); // ��������� ������� ������� ������
		cube->setSize(Vector2f(scale, scale)); // ��������� ������� ������ 
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

	// ����� ��������������� ������ �������� ���������
	void tetDirection(direction dir) { click_dx = static_cast<float> (dir); }

	void draw() // ����� �������� ��������� � ����������� ����
	{
		if (positionmaket.x >= 0) // �������� �� ����� �� ������� ������
		{
			cube->setFillColor(tetcolor[colTet.y]); // ��������� ����� ���������� ������
			for (int i = 0; i < 4; i++)
			{
				cube->setPosition((figures[typeTet.y][i] % 2) * scale, (static_cast<float>(figures[typeTet.y][i] / 2)) * scale);
				cube->move(positionmaket); // ����������� ������ �� ����� �������
				window.draw(*cube); // ��������� ������ � ����� �������
			}
		}
		for (int i = 0; i < width; i++)  // ������� �� �����������
		{
			for (int j = 0; j < height; j++) // ������� �� ���������
			{
				cube->setFillColor(square[i][j]); // ��������� ����� ���������� ������
				cube->setPosition(static_cast<float>(i) * scale, static_cast<float>(j) * scale);
				cube->move(tet); // ����������� ������ �� ����� �������
				window.draw(*cube); // ��������� ������ � ����� �������
			}
		}
		cube->setFillColor(tetcolor[colTet.x]); // ��������� ����� ���������� ������
		for (int i = 0; i < t.size(); i++)
		{
			cube->setPosition(t[i].x * scale, t[i].y * scale);
			cube->move(tet); // ����������� ������ �� ����� �������
			window.draw(*cube); // ��������� ������ � ����� �������
		}
	}


	void update(Time const& deltaTime) 	// ����� ���������� ������� ������ ���������
	{
		frameRate += deltaTime; // ���������� ������� � ���������� frameRate
		if (frameRate > milliseconds(delay)) // �������� �� ��������� �������
		{
			frameRate = milliseconds(0); // ����� �������
			if (check(ch::x) && click_dx != 0) // �������� ����������� �������� �� �����������
			{
				for (int i = 0; i < 4; i++) t[i].x += click_dx; click_dx = 0; // ����������� ��������� � ����� �������
			}
			// ����������� ��������� ����
			if (check(ch::y)) { for (int i = 0; i < 4; i++)  t[i].y += click_dy; }
			else
			{
				for (int i = 0; i < t.size(); i++)
				{
					// ���� ��������� ����� �� ������� ������� �������� ����
					if (static_cast<int>(t[i].y) == 2)
					{
						Font font;
						font.loadFromFile("font/212.ttf");
						Text scoreText(L"���� ���������. ��� ����: " + to_string(score), font, 28);
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
					// ���������� ��������� � ������ �������� ����
					square[static_cast<size_t>(t[i].x)][static_cast<size_t>(t[i].y)] = Color(tetcolor[colTet.x]);
				}
				int numLine = 0; // ���������� ����������� �����
				for (int j = 0; j < height; j++)
				{
					int line = 0;
					for (int i = 0; i < width; i++)
					{
						if (square[i][j] != Color::Black) line++; // ������� ����������� ������ � ������
						if (line == width) // ���� ��� ������ ����� �������
						{
							lineDead(j); // �������� ������
							numLine++; // ���������� �������� ����������� �����
						}
					}
				}
				if (numLine != 0)
				{
					score += 5 * (numLine * numLine); // ���������� ����� �� �������� �����
				}
				newFigrois(); // ��������� ������ ���������
			}
		}
	}

	void rotate() // ����� �������� ���������
	{
		if (typeTet.x != 6) // ��� �������� ��� ��������� ��������
		{
			if (check(ch::rotation)) // �������� ����������� ��������
			{
				// ���������� ������ ��������
				Vector2f centerRotation = t[1];
				for (int i = 0; i < t.size(); i++)
				{
					float x = t[i].y - centerRotation.y;
					float y = t[i].x - centerRotation.x;
					// ������� ������ �����
					t[i].x = centerRotation.x - x;
					t[i].y = centerRotation.y + y;
				}

			}
		}
	}

	Vector2f getPositio() // ����� ������������ ���������� ������ ���������
	{
		Vector2f pos;
		// ���������� ��������� ������ ��������� �� �����������
		pos.x = t[1].x * scale + tet.x;
		// ���������� ��������� ������ ��������� �� ���������
		pos.y = t[1].y * scale + tet.y;
		return pos;
	}


	void speed() { delay = 10; } // ����� ���������� ������� ���������

	void restart() // ����� ������������ ��� �������� � ��������� �������� - ������� ����
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

	int getscore() const { return score; } // ����� ������������ ���������� ���������� �����
	void maket(Vector2f posmak) { positionmaket = posmak; } // ����� ����������� ������ ���������� ���������

private:
	const int height; // ������ �������� ����
	const int width; // ������ �������� ����
	const  float click_dy = 1.0f; // ��� ����������� ��������� �� y
	vector<vector<Color>> square; // ������ �������� ����

	// ������ ��������� ��������� ������� ��������� 
	array<array<int, 4>, 7> figures
	{ {{1,3,5,7},{2,4,5,7},{3,4,5,6},{3,4,5,7},{2,3,5,7},{3,5,6,7},{2,3,4,5}} };

	array<Vector2f, 4> t; // ��������� �������������� � ���������� ��������� 

	// ������ ����� ��� ���������
	array<Color, 7> tetcolor{ {Color::Blue,Color::Cyan,Color::Yellow,
	Color::Green,Color::Magenta,Color(139,0,255),Color::White} };

	// ������������� ���������
	unique_ptr<RectangleShape> cube = make_unique<RectangleShape>();

	// ������ ���������� �������
	long long seed = chrono::system_clock::now().time_since_epoch().count();

	// ������ ���������� ��������� �����
	default_random_engine rnd = default_random_engine(static_cast<long>(seed));

	// ��������� ��������� ��������� �����
	uniform_int_distribution<int> d = uniform_int_distribution<int>(0, 6);

	// ������ �� ����������� ����
	RenderWindow& window;

	const Vector2f tet; // ��������� ���������� ���������
	Time frameRate; // �������� ���������� ������� ������ 
	Vector2i typeTet; // ��� ��������� 
	Vector2i colTet; // ���� ��������� 
	void newFigrois()  // ����� ��������� 
	{
		typeTet.x = typeTet.y; // ��������� ������� ��� ��������� ��� ����������
		colTet.x = colTet.y;  // ��������� ������� ���� ��������� ��� ����������

		// ���������� ����� ��������� � ��������� ����� � ������
		for (int i = 0; i < t.size(); i++)
		{
			t[i].x = figures[typeTet.x][i] % 2 + static_cast<float>(floor(width / 2));
			t[i].y = static_cast<float>(figures[typeTet.x][i] / 2);
		}
		typeTet.y = d(rnd); // �������� ����� ��� ��������� ��������� �������
		colTet.y = d(rnd);  // �������� ����� ���� ��������� ��������� �������
		delay = 250; // ������������� �������� �� ���������
	}

	void lineDead(int g) // ����������� ������� ��������� ��� ���������� ���� �� ����������� 
	{
		// �������� ��� ������ ���� ����������� ����� �� ���� ������ ����, ����� "�������" ����������� �����
		for (int i = g; i > 0; i--)
		{
			for (int j = 0; j < width; j++)
			{
				square[j][i] = square[j][static_cast<size_t>(i - 1)];
			}
		}
	}

	bool check(ch ch) // �������� ��������� ���������
	{
		switch (ch)
		{
		// �������� ����������� ��������� �� �����������
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
		// �������� ������� ���������
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
		// �������� �������� ���������
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
	Int32 delay; // �������� ��������� ������� ������
	float click_dx; // ��� ����������� ��������� �� x
	int score; // ���� ��������
	float scale; // ������� ������
	Vector2f positionmaket = Vector2f(-1, -1); // �������� ��������� ������ ���������
};
