#pragma once
#include "inc/RenderWindow.h"

class PixelArt final
{
public:
    static void Run();
    PixelArt() = delete;
    PixelArt(const PixelArt&) = delete;
    PixelArt& operator=(const PixelArt&) = delete;
    PixelArt(PixelArt&&) = default;
    PixelArt& operator=(PixelArt&&) = default;
private:
    static std::string Directory;
    static std::unique_ptr<RenderWindow> window;
private:
    static void ShowMenu();
    static void CheckOption(int choice);
    static void ShowFiles();
    static bool checkIfFileExists(const std::string& name);   
};