#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;
using namespace sf;

// ����� ��� ���������� ��������� ����
class AssetManager
{
	// ������������� ���������� ��� �������� �������� �� ���������� �����
	map<string, Texture> m_Textures;
	map<string, Font> m_Fonts;
	static AssetManager* sInstance; //����������� ���������� ������ ��� �������� ���������� ������ AssetManager
public:
	//����������� ������ AssetManager
	AssetManager() { sInstance = this; }

	//����������� ����� ��� ��������� �������� �� �������� �����
	static Texture& GetTexture(string const& filename)
	{
		auto& texMap = sInstance->m_Textures; //������ �� ������������� ��������� � ���������� �������� ���������� ������ AssetManager
		auto pairFound = texMap.find(filename); //������� ������� � ������������� ����������
		if (pairFound != texMap.end()) //���� ������� ������ � ����������
		{
			return pairFound->second; //���������� ��������� ��������
		}
		else //�����
		{
			auto& texture = texMap[filename]; //��������� ����� �������� � ��������� �� �����, ������� �������� �����
			texture.loadFromFile(filename); //��������� �������� �� �����
			return texture; //���������� ����������� ��������
		}
	}

	//����������� ����� ��� ��������� ������ �� �������� �����
	static Font& GetFont(string const& filename)
	{
		auto& fontMap = sInstance->m_Fonts; //������ �� ������������� ��������� � �������� �������� ���������� ������ AssetManager
		auto pairFound = fontMap.find(filename); //������� ������� � ������������� ����������
		if (pairFound != fontMap.end()) //���� ������� ������ � ����������
		{
			return pairFound->second; //���������� ��������� ����� �����
		}
		else //�����
		{
			auto& font = fontMap[filename]; //��������� ����� ����� � ��������� �� �����, ������� �������� �����
			font.loadFromFile(filename); //��������� ����� ����� �� �����
			return font; //���������� ����������� ����� �����
		}
	}
};
AssetManager* AssetManager::sInstance = nullptr; //������������� ����������� ���������� ������