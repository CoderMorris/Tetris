#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;
using namespace sf;

//  ласс дл€ управлени€ ресурсами игры
class AssetManager
{
	// јссоциативные контейнеры дл€ хранени€ ресурсов по строковому ключу
	map<string, Texture> m_Textures;
	map<string, Font> m_Fonts;
	static AssetManager* sInstance; //статическа€ переменна€ класса дл€ хранени€ экземпл€ра класса AssetManager
public:
	//конструктор класса AssetManager
	AssetManager() { sInstance = this; }

	//статический метод дл€ получени€ текстуры по названию файла
	static Texture& GetTexture(string const& filename)
	{
		auto& texMap = sInstance->m_Textures; //ссылка на ассоциативный контейнер с текстурами текущего экземпл€ра класса AssetManager
		auto pairFound = texMap.find(filename); //искомый элемент в ассоциативном контейнере
		if (pairFound != texMap.end()) //если элемент найден в контейнере
		{
			return pairFound->second; //возвращаем найденную текстуру
		}
		else //иначе
		{
			auto& texture = texMap[filename]; //добавл€ем новую текстуру в контейнер по ключу, равному названию файла
			texture.loadFromFile(filename); //загружаем текстуру из файла
			return texture; //возвращаем загруженную текстуру
		}
	}

	//статический метод дл€ получени€ шрифта по названию файла
	static Font& GetFont(string const& filename)
	{
		auto& fontMap = sInstance->m_Fonts; //ссылка на ассоциативный контейнер с шрифтами текущего экземпл€ра класса AssetManager
		auto pairFound = fontMap.find(filename); //искомый элемент в ассоциативном контейнере
		if (pairFound != fontMap.end()) //если элемент найден в контейнере
		{
			return pairFound->second; //возвращаем найденный буфер звука
		}
		else //иначе
		{
			auto& font = fontMap[filename]; //добавл€ем новый шрифт в контейнер по ключу, равному названию файла
			font.loadFromFile(filename); //загружаем буфер звука из файла
			return font; //возвращаем загруженный буфер звука
		}
	}
};
AssetManager* AssetManager::sInstance = nullptr; //инициализаци€ статической переменной класса