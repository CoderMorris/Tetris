#include "Interface.h"
#include <Windows.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <codecvt>

// ������� �������
void Gam�Start()
{
    auto myGame = make_unique<GameEngine>();
    myGame->run();
}

void Records()
{
    RenderWindow window(VideoMode(640, 640), L"Tetris");
    Image icon;
    icon.loadFromFile("image/game.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("image/menu.jpg");
    RectangleShape background(Vector2f(640, 640));
    background.setTexture(&backgroundTexture);
    Font font;
    font.loadFromFile("font/212.ttf");
    Text title(L"�������", font, 40);
    title.setPosition(201, 40);
    Text options(L"�������� ������ - Delete", font, 30);
    options.setPosition(46, 550);
    map<int, wstring> recordMap;
    wifstream fin("records.txt");
    fin.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
    int score;
    wstring name;
    while (fin >> score && getline(fin, name))
    {
        recordMap[score] = name;
    }
    fin.close();
    vector<Text> recordTexts;
    int i = 1;
    for (auto it = recordMap.rbegin(); it != recordMap.rend() && i <= 10; ++it)
    {
        Text text(to_wstring(i) + L". " + it->second + L"  " + to_wstring(it->first), font, 30);
        text.setPosition(170, 100 + (i - 1) * 45);
        recordTexts.push_back(text);
        ++i;
    }
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) { window.close(); }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Delete)
            {
                string fileName = "records.txt";
                ofstream newFile(fileName);
                newFile.close();
                window.close();
            }
        }
        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(options);
        for (const auto& text : recordTexts) { window.draw(text); }
        window.display();
    }
};

// �������� ����
void About_Game()
{
    RenderWindow About(sf::VideoMode(640, 640), L"Tetris");
    Image icon;
    icon.loadFromFile("image/game.png");
    About.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    RectangleShape background_ab(sf::Vector2f(640, 640));
    Texture texture_ab;
    texture_ab.loadFromFile("image/menu.jpg");
    background_ab.setTexture(&texture_ab);
    Font font;
    font.loadFromFile("font/212.ttf");
    Text text(L"������� ����", font, 40);
    text.setPosition(140, 40);
    vector<wstring> rules = {
        L"������� ��������� ������ ������ � ������������� 10*20.",
        L"�  ������ �����  �����  ������������ ������� �� 90� � ",
        L"������� �� ee �����������.����� ����� �������� �������",
        L"�������.������� ����� �� ��� ���, ���� �� ��������� ��",
        L"������ ������ ��� �� ������ �� ��� ��������������.����",
        L"����������� ��� �� 10 ������, �� �� ��������� � ������",
        L"������  5  �����. �  �������  �����  ���� ������������",
        L"���������  �������.  ����  �������������, �����  �����",
        L"������� ������� �� ������� ������� �������� ����. ����",
        L"������ ������������ � ������ ����� ����.",
    };
    vector<Text> ruleTexts;
    for (size_t i = 0; i < rules.size(); ++i)
    {
        Text ruleText(rules[i], font, 15);
        ruleText.setPosition(10, 100 + i * 40);
        const float maxWidth = 620.f;
        const auto& lineEnd = rules[i].end() - 1;
        const Vector2f endPos = ruleText.findCharacterPos(lineEnd - rules[i].begin());
        const float xOffset = (maxWidth - endPos.x) / 2.f;
        const float lineHeight = 35.f;
        ruleText.setPosition(10.f + xOffset, text.getPosition().y + 70.f + i * lineHeight);
        ruleTexts.push_back(ruleText);
    }
    while (About.isOpen())
    {
        Event event_opt;
        while (About.pollEvent(event_opt))
        {
            if (event_opt.type == Event::KeyPressed)
            {
                if (event_opt.key.code == Keyboard::Escape) About.close();
            }
        }
        About.clear();
        About.draw(background_ab);
        About.draw(text);
        for (const auto& ruleText : ruleTexts)
        {
            About.draw(ruleText);
        }
        About.display();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    RenderWindow window(VideoMode(640, 640), "Tetris");
    //window.setMouseCursorVisible(false); //��������� ��������� �������
    Image icon;
    icon.loadFromFile("image/game.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //�������� ��������
    Texture texthome;
    texthome.loadFromFile("Image/z1.png");
    RectangleShape homecls(Vector2f(640, 640));
    homecls.setTexture(&texthome);
    window.draw(homecls);
    window.display();


    clock_t start_time = clock();
    while ((clock() - start_time) / CLOCKS_PER_SEC < 5);
    vector<String> name_menu{ L"�����", L"����� ������", L"�������", L"�������", L"�����" };
    GameMenu mymenu(window, 335, 100, 50, 100, name_menu);
    mymenu.setColorTextMenu(Color(54, 171, 217), Color(22, 70, 89), Color::Black);
    mymenu.AlignMenu(2);

    // ������� ��������� �� �������
    Texture texture_back;
    texture_back.loadFromFile("image/t.jpg");
    Sprite backgroundBlack;
    backgroundBlack.setColor(Color(255, 255, 255, 255));
    backgroundBlack.setTexture(texture_back);
    backgroundBlack.setTextureRect(IntRect(0, 0, 640, 640));
    float alpha = 255;


    RectangleShape background(Vector2f(640, 640));
    Texture texture_window;
    texture_window.loadFromFile("image/menu.jpg");
    background.setTexture(&texture_window);
    Font font;
    font.loadFromFile("font/212.ttf");
    ChoicePlayer();
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                // ������� ����� ������� ����
                if (event.key.code == Keyboard::Up) { mymenu.input(true); }       // �����
                if (event.key.code == Keyboard::Down) { mymenu.input(false); }  // ����
                if (event.key.code == Keyboard::Return)                                     // ����
                {
                    // ��������� �� ��������� ����� ����
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:
                        window.setVisible(false);
                        Gam�Start();
                        window.setVisible(true);
                        break;
                    case 1:
                        ChoicePlayer();
                        break;
                    case 2:
                        window.setVisible(false);
                        Records();
                        window.setVisible(true);
                        break;
                    case 3:
                        window.setVisible(false);
                        About_Game();
                        window.setVisible(true);
                        break;
                    case 4:
                        window.close();
                        break;
                    default:break;
                    }
                }
            }
        }
        // ������� ���������� ������ ����
        if (alpha > 0)
        {
            alpha -= 0.2f;
            backgroundBlack.setColor(Color(255, 255, 255, static_cast<unsigned char>(alpha)));
        }
        window.clear();
        window.draw(background);
        mymenu.draw();
        window.draw(backgroundBlack);
        window.display();
    }
    return 0;
}