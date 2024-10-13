#pragma once
#include <SFML/Graphics.hpp>
#include"Tetramino.h";
#include <vector>

using namespace std;
using namespace sf;

class Interface
{
public:
	virtual void draw() = 0;
	virtual void input(bool x) = 0;
};

class GameEngine :public Interface
{
	// �������� ������ ��������� �� ���� ����������
	unique_ptr<RenderWindow> window = make_unique<RenderWindow>(VideoMode(640, 640), L"Tetris", Style::Close);
	// �������� ������� ������ Tetramino
	Tetramino tetramino = Tetramino(*window, Vector2f(270, -40), Vector2i(10, 23), 28);
	// �������� ������� ������ RectangleShape ��� ������� ���� ����
	RectangleShape background = RectangleShape(Vector2f(640, 640));
	AssetManager manager; // �������� ������� ������ AssetManager ��� ���������� ��������� ����
	Image icon; // �������� ������� ������ Image ��� �������� ������ ����������
	Text text; // �������� ������� ������ Text ��� ������ ���������� � ����� ������
	bool exit = false, pause = false; // �������� ������ ������ �� ���������� � ����� � ����
	Time time; // �������� ������� ������ Time ��� �������� �������
public:
	GameEngine()
	{
		// �������� �������� ������� ���� �� �������� � ��������� �� �� ������ ��������������
		background.setTexture(&AssetManager::GetTexture("image/fon.png"));

		// �������� ������ ���������� �� ����� � ��������� �� �� ���� ����������
		icon.loadFromFile("image/game.png");
		window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// �������� ������ �� �������� � ��������� ��� �� ������ ������
		text.setFont(AssetManager::GetFont("font/212.ttf"));
		text.setFillColor(Color::White);

		tetramino.maket(Vector2f(56, 32)); // ��������� ��������� ���������� ���������
	}

	void input(bool) override // ����� ��� ��������� ����������������� �����
	{
		Event event; // ���������� ������� ������ Event ��� ������ �������
		while (window->pollEvent(event)) // ���� ��� ��������� ���� ������� � �������
		{
			if (event.type == Event::KeyPressed) // ��������� ������� ������
			{
				if (Keyboard::isKeyPressed(Keyboard::Tab)) { pause = !pause; }
				if (Keyboard::isKeyPressed(Keyboard::Escape)) { window->close(); }
				if (Keyboard::isKeyPressed(Keyboard::Q)) { tetramino.restart(); }
				if (!pause) // �������� ���������� ����� � ��������� ������� ������ ���������� ���������
				{
					switch (event.key.code)
					{
					case Keyboard::Left:
						tetramino.tetDirection(Tetramino::direction::left);
						break;

					case Keyboard::Right:
						tetramino.tetDirection(Tetramino::direction::right);
						break;

					case Keyboard::Down:
						tetramino.speed();
						break;

					case Keyboard::Up:
						tetramino.rotate();
						break;

					default:
						break;
					}
				}
			}
		}
	}

	void update(Time const& deltaTime) // ����� ��� ���������� ��������� ����
	{
		// ���������� ��������� ���������, ���� ����� �� ������������
		if (!pause) tetramino.update(deltaTime);
		// �������� ����� ������ �� ���� � ���������� �������
		if (exit)
		{
			time += deltaTime;
			// �������� ����, ���� ������ ����� ������� ����� ��������� ����� ������
			if (time > seconds(1))
			{
				if (exit) window->close();
			}
		}
	}

	void draw() override // ����� ��� ��������� �������
	{
		window->clear(Color(13, 152, 186)); // ������� ������
		tetramino.draw(); // ��������� ���������
		window->draw(background); // ��������� ������� ����

		// ��������� ������ � �����
		text.setPosition(50, 507);
		text.setString(L"����");
		window->draw(text);

		// ��������� �������� ����� ������
		text.setString(to_string(tetramino.getscore()));
		text.setPosition(100 - text.getGlobalBounds().width / 2, 547);
		window->draw(text);

		window->display(); // ���������� ���� ����������
	}

	void run() // ����� ��� ������� �������� �����
	{
		Clock clock; // �������� ������� ������ Clock ��� ��������� �������
		while (window->isOpen()) // ����, ������� �������� ���� ���� ���������� �������
		{
			// ��������� ���������� ������� � ����������� �����
			Time deltaTime = clock.restart();
			input(true); // ��������� ����������������� �����
			update(deltaTime); // ���������� ��������� ����
			draw(); // ��������� ������� �� ���� ����������
		}
	}
};


class GameMenu:public Interface
{
	// ���������� ���� �� X � Y
	double menu_X, menu_Y;
	// ��� ����� �������� ����, ���-�� �������, ������ ������, ��������� ����� ����
	int menu_Step, max_menu, size_font, mainMenuSelected;
	Font font; // ����� ��� ������ ����
	vector<Text> name_menu;  // ������ ��������� ��������� ����
	RenderWindow& window;  // ���� ����������, � ������� ����� ���������� ����
	Color menu_text_color = Color::White; // ���� ����������� ������ ������ ����
	Color chose_text_color = Color::Yellow; // ���� ��������� (����������) ������ ������ ����
	Color border_color = Color::Black;  // ���� ������� ������ ������ ����

	// ��������� ������� ��� ��������� ��������� ���������� ���������� ��������
	void setInitText(Text& text, const String& str, float xpos, float ypos) const
	{
		text.setFont(font);  // ��������� ������ ������
		text.setFillColor(menu_text_color); // ��������� ����� ������
		text.setString(str); // ������� ������
		text.setCharacterSize(size_font); // ��������� ������� ������
		text.setPosition(xpos, ypos); // ��������� ������� �� X � Y
		text.setOutlineThickness(3); // ��������� ������� ������� ������
		text.setOutlineColor(border_color); // ��������� ����� ������� ������
	}
public:
	GameMenu(RenderWindow& window, float menux, float menuy, int sizeFont, int step, const vector<String>& name)
		: menu_X(menux), menu_Y(menuy), menu_Step(step), max_menu(static_cast<int>(name.size())), size_font(sizeFont), name_menu(name.size()), window(window)
	{
		font.loadFromFile("font/212.ttf"); // �������� ������

		// ������ �� ���� ��������� ���� � ��������� ��������� ���������� ��� ������� �� ���
		for (int i = 0, ypos = static_cast<int>(menu_Y); i < max_menu; i++, ypos += menu_Step)
			setInitText(name_menu[i], name[i], menu_X, static_cast<float>(ypos));
		mainMenuSelected = 0; // ��������� ����� ���� �� ��������� - ������
		name_menu[mainMenuSelected].setFillColor(chose_text_color); // ��������� ����� ���������� ������ ����
	}

	// ��������������� ������ ���������� ��� ��������� ��������� ����
	void draw() override { for (auto& menu_item : name_menu) window.draw(menu_item); }

	// ��������������� ������ ���������� ��� ��������� ����������������� �����
	void input(bool moveUp) override
	{
		// ��������� ����� ����������� ������ ��� �������� ���������� ������ ����
		name_menu[mainMenuSelected].setFillColor(menu_text_color);

		if (moveUp) // ���� ������������ ����� ������� "�����"
		{
			// ����� ����������� ������ ���� � ������ ������������
			mainMenuSelected = (mainMenuSelected - 1 + max_menu) % max_menu;
		}
		else // ����� (������������ ����� ������� "����")
		{
			// ����� ���������� ������ ���� � ������ ������������
			mainMenuSelected = (mainMenuSelected + 1) % max_menu;
		}
		// ��������� ����� ��������� ������ ��� ���������� ������ ����
		name_menu[mainMenuSelected].setFillColor(chose_text_color);
	}

	// ���������� ������� setColorTextMenu � ����� �����������: ���� ������ ���� �� ���������, ���� ����������� ������ ���� � ���� ������� ����
	void setColorTextMenu(const Color& menColor, const Color& ChoColor, const Color& BordColor)
	{
		menu_text_color = menColor;
		chose_text_color = ChoColor;
		border_color = BordColor;
		for (auto& menu_item : name_menu)  // �������� ������ �� ���� ������� ����
		{
			menu_item.setFillColor(menu_text_color);  // ������������� ���� ������ ��� ������� ������ ����
			menu_item.setOutlineColor(border_color); // ������������� ���� ������� ��� ������� ������ ����
		}
		// ������������� ���� ���������� ������ ����
		name_menu[mainMenuSelected].setFillColor(chose_text_color);
	}

	// ���������� ������� AlignMenu � ����� ���������� - ���������� ������������ �� ��� X
	void AlignMenu(int posx)
	{
		float nullx = 0; // �������������� ���������� nullx, ������� �������� �� �������� ������� ������ ����

		// �������� ������ �� ���� ������� ����
		for (auto& menu_item : name_menu)
		{
			switch (posx)
			{
			case 0: // ���� ������� ������������ �� ������ ����, ��
				nullx = 0; // �������� nullx ����� 0
				break;
			case 1: // ���� ������� ������������ �� ������� ����, ��
				nullx = menu_item.getLocalBounds().width; // �������� nullx ����� ������ �������� ������ ����
				break;
			case 2:  // ���� ������� ������������ �� ������, ��
				nullx = menu_item.getLocalBounds().width / 2; // �������� nullx ����� �������� ������ �������� ������ ����
				break;
			default:break;
			}
			// ������������� ����� ������� ������� ������ ����, ����������� �� �������� ��� X
			menu_item.setPosition(menu_item.getPosition().x - nullx, menu_item.getPosition().y);
		}
	};

	// ���������� ������������ �������, ������� ���������� ����� ���������� ������ ����
	int getSelectedMenuNumber() const { return mainMenuSelected; }
};