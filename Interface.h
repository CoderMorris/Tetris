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
	// Создание умного указателя на окно приложения
	unique_ptr<RenderWindow> window = make_unique<RenderWindow>(VideoMode(640, 640), L"Tetris", Style::Close);
	// Создание объекта класса Tetramino
	Tetramino tetramino = Tetramino(*window, Vector2f(270, -40), Vector2i(10, 23), 28);
	// Создание объекта класса RectangleShape для заднего фона окна
	RectangleShape background = RectangleShape(Vector2f(640, 640));
	AssetManager manager; // Создание объекта класса AssetManager для управления ресурсами игры
	Image icon; // Создание объекта класса Image для загрузки иконки приложения
	Text text; // Создание объекта класса Text для вывода информации о счете игрока
	bool exit = false, pause = false; // Создание флагов выхода из приложения и паузы в игре
	Time time; // Создание объекта класса Time для рассчета времени
public:
	GameEngine()
	{
		// Загрузка текстуры заднего фона из ресурсов и установка ее на объект прямоугольника
		background.setTexture(&AssetManager::GetTexture("image/fon.png"));

		// Загрузка иконки приложения из файла и установка ее на окно приложения
		icon.loadFromFile("image/game.png");
		window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// Загрузка шрифта из ресурсов и установка его на объект текста
		text.setFont(AssetManager::GetFont("font/212.ttf"));
		text.setFillColor(Color::White);

		tetramino.maket(Vector2f(56, 32)); // Установка начальных параметров тетрамино
	}

	void input(bool) override // Метод для обработки пользовательского ввода
	{
		Event event; // Объявление объекта класса Event для работы событий
		while (window->pollEvent(event)) // Цикл для обработки всех событий в очереди
		{
			if (event.type == Event::KeyPressed) // Обработка нажатия клавиш
			{
				if (Keyboard::isKeyPressed(Keyboard::Tab)) { pause = !pause; }
				if (Keyboard::isKeyPressed(Keyboard::Escape)) { window->close(); }
				if (Keyboard::isKeyPressed(Keyboard::Q)) { tetramino.restart(); }
				if (!pause) // Проверка отсутствия паузы и обработка нажатия клавиш управления тетрамино
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

	void update(Time const& deltaTime) // Метод для обновления состояния игры
	{
		// Обновление состояния тетрамино, если пауза не активирована
		if (!pause) tetramino.update(deltaTime);
		// Проверка флага выхода из игры и обновление времени
		if (exit)
		{
			time += deltaTime;
			// Закрытие окна, если прошло более секунды после установки флага выхода
			if (time > seconds(1))
			{
				if (exit) window->close();
			}
		}
	}

	void draw() override // Метод для отрисовки графики
	{
		window->clear(Color(13, 152, 186)); // Очистка экрана
		tetramino.draw(); // Отрисовка тетрамино
		window->draw(background); // Отрисовка заднего фона

		// Отрисовка текста о счете
		text.setPosition(50, 507);
		text.setString(L"Счет");
		window->draw(text);

		// Отрисовка текущего счета игрока
		text.setString(to_string(tetramino.getscore()));
		text.setPosition(100 - text.getGlobalBounds().width / 2, 547);
		window->draw(text);

		window->display(); // Обновление окна приложения
	}

	void run() // Метод для запуска игрового цикла
	{
		Clock clock; // Создание объекта класса Clock для измерения времени
		while (window->isOpen()) // Цикл, который работает пока окно приложения открыто
		{
			// Измерение прошедшего времени с предыдущего кадра
			Time deltaTime = clock.restart();
			input(true); // Обработка пользовательского ввода
			update(deltaTime); // Обновление состояния игры
			draw(); // Отрисовка графики на окне приложения
		}
	}
};


class GameMenu:public Interface
{
	// Координаты меню по X и Y
	double menu_X, menu_Y;
	// Шаг между пунктами меню, кол-во пунктов, размер шрифта, выбранный пункт меню
	int menu_Step, max_menu, size_font, mainMenuSelected;
	Font font; // Шрифт для текста меню
	vector<Text> name_menu;  // Вектор текстовых элементов меню
	RenderWindow& window;  // Окно приложения, в которое будет выводиться меню
	Color menu_text_color = Color::White; // Цвет неактивного текста пункта меню
	Color chose_text_color = Color::Yellow; // Цвет активного (выбранного) текста пункта меню
	Color border_color = Color::Black;  // Цвет обводки текста пункта меню

	// Приватная функция для установки начальных параметров текстового элемента
	void setInitText(Text& text, const String& str, float xpos, float ypos) const
	{
		text.setFont(font);  // Установка шрифта текста
		text.setFillColor(menu_text_color); // Установка цвета текста
		text.setString(str); // Задание текста
		text.setCharacterSize(size_font); // Установка размера шрифта
		text.setPosition(xpos, ypos); // Установка позиции по X и Y
		text.setOutlineThickness(3); // Установка толщины обводки текста
		text.setOutlineColor(border_color); // Установка цвета обводки текста
	}
public:
	GameMenu(RenderWindow& window, float menux, float menuy, int sizeFont, int step, const vector<String>& name)
		: menu_X(menux), menu_Y(menuy), menu_Step(step), max_menu(static_cast<int>(name.size())), size_font(sizeFont), name_menu(name.size()), window(window)
	{
		font.loadFromFile("font/212.ttf"); // Загрузка шрифта

		// Проход по всем элементам меню и установка начальных параметров для каждого из них
		for (int i = 0, ypos = static_cast<int>(menu_Y); i < max_menu; i++, ypos += menu_Step)
			setInitText(name_menu[i], name[i], menu_X, static_cast<float>(ypos));
		mainMenuSelected = 0; // Выбранный пункт меню по умолчанию - первый
		name_menu[mainMenuSelected].setFillColor(chose_text_color); // Установка цвета выбранного пункта меню
	}

	// Переопределение метода интерфейса для отрисовки элементов меню
	void draw() override { for (auto& menu_item : name_menu) window.draw(menu_item); }

	// Переопределение метода интерфейса для обработки пользовательского ввода
	void input(bool moveUp) override
	{
		// Установка цвета неактивного текста для текущего выбранного пункта меню
		name_menu[mainMenuSelected].setFillColor(menu_text_color);

		if (moveUp) // Если пользователь нажал клавишу "вверх"
		{
			// Выбор предыдущего пункта меню с учетом зацикливания
			mainMenuSelected = (mainMenuSelected - 1 + max_menu) % max_menu;
		}
		else // Иначе (пользователь нажал клавишу "вниз")
		{
			// Выбор следующего пункта меню с учетом зацикливания
			mainMenuSelected = (mainMenuSelected + 1) % max_menu;
		}
		// Установка цвета активного текста для выбранного пункта меню
		name_menu[mainMenuSelected].setFillColor(chose_text_color);
	}

	// объявление функции setColorTextMenu с тремя параметрами: цвет текста меню по умолчанию, цвет выделенного пункта меню и цвет обводки меню
	void setColorTextMenu(const Color& menColor, const Color& ChoColor, const Color& BordColor)
	{
		menu_text_color = menColor;
		chose_text_color = ChoColor;
		border_color = BordColor;
		for (auto& menu_item : name_menu)  // проходим циклом по всем пунктам меню
		{
			menu_item.setFillColor(menu_text_color);  // устанавливаем цвет текста для каждого пункта меню
			menu_item.setOutlineColor(border_color); // устанавливаем цвет обводки для каждого пункта меню
		}
		// устанавливаем цвет выбранного пункта меню
		name_menu[mainMenuSelected].setFillColor(chose_text_color);
	}

	// объявление функции AlignMenu с одним параметром - положением выравнивания по оси X
	void AlignMenu(int posx)
	{
		float nullx = 0; // инициализируем переменную nullx, которая отвечает за смещение каждого пункта меню

		// проходим циклом по всем пунктам меню
		for (auto& menu_item : name_menu)
		{
			switch (posx)
			{
			case 0: // если выбрано выравнивание по левому краю, то
				nullx = 0; // смещение nullx равно 0
				break;
			case 1: // если выбрано выравнивание по правому краю, то
				nullx = menu_item.getLocalBounds().width; // смещение nullx равно ширине текущего пункта меню
				break;
			case 2:  // если выбрано выравнивание по центру, то
				nullx = menu_item.getLocalBounds().width / 2; // смещение nullx равно половине ширины текущего пункта меню
				break;
			default:break;
			}
			// устанавливаем новую позицию каждого пункта меню, выравненную по заданной оси X
			menu_item.setPosition(menu_item.getPosition().x - nullx, menu_item.getPosition().y);
		}
	};

	// объявление константного геттера, который возвращает номер выбранного пункта меню
	int getSelectedMenuNumber() const { return mainMenuSelected; }
};