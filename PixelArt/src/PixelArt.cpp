#include "inc/PixelArt.h"
#include <iostream>
#include <fstream>
#include <filesystem>

std::string PixelArt::Directory = "Pictures/";
std::unique_ptr<RenderWindow> PixelArt::window;

bool PixelArt::checkIfFileExists(const std::string & name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void PixelArt::Run()
{
    ShowFiles();
    std::string pictureName;
    std::cout << "\nType picture's name [name.ext]: ";
    std::cin >> pictureName;
    if (!checkIfFileExists(Directory + pictureName))
        std::cout << "[" << pictureName << "] doesn't exist in " << Directory << "\n";
    else
    {
        window = std::make_unique<RenderWindow>(600, 400, 150, 100, Directory + pictureName);
        int choice;
        ShowMenu();
        std::cout << "Option: ";
        std::cin >> choice;
        CheckOption(choice);
    }
}

void PixelArt::ShowMenu()
{
    std::fstream menu;
    menu.open("menu.txt");
    std::string line;
    do
    {
        std::getline(menu, line);
        std::cout << line << std::endl;
    } while (line != "");
    menu.close();
}

void PixelArt::CheckOption(int choice)
{
    if (choice == 1)
        (*window).Build(Color::RED);
    else if (choice == 2)
        (*window).Build(Color::GREEN);
    else if (choice == 3)
        (*window).Build(Color::BLUE);
    else  exit(0);
}

void PixelArt::ShowFiles()
{
    std::cout << "List of Images:\n";
    int i{ 0 };
    for (const auto & entry : std::experimental::filesystem::directory_iterator("Pictures/"))
    {
        std::cout << "[" << ++i << "] " << entry.path().filename() << std::endl;
    }
}
