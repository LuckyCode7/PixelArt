#pragma once
#include <Cg/cgGL.h>
#include <string>
#include <memory>

enum class Color
{
    RED,
    GREEN,
    BLUE
};

enum class Options
{
    SAVE,
    FULL_SCREEN,
};

class RenderWindow final
{
public:
    void Build(Color color);
    RenderWindow(int width_, int height_, int PosX_, int PosY, std::string inputFileName_);
    RenderWindow(const RenderWindow&) = delete;
    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = default;
    RenderWindow& operator=(RenderWindow&&) = default;
private:
    static int width, height;
    static int PosX, PosY;
    static unsigned char* InputData;
    static std::string inputFileName;
    static CGcontext CgContext;
    static CGprofile CgVertexProfile;
    static CGprofile CgFragmentProfile;
    static CGprogram CgVertexProgram;
    static CGprogram CgFragmentProgram;
    static CGparameter CgFragmentParam;
    static std::unique_ptr<GLubyte[]> Pixels;
private:
    static void Init();
    static void Create();
    static void LoadInputData();
    static void DrawVertices();
    static void Display();
    static void SavePicture() ;
    static void Keyboard(unsigned char c, int x, int y);
    static void AddGlutFunc();
    static void SetCGContextAndProfile();
    static void SetCgVertexProgram();
    static void SetCgFragmentProgram(const char* FragmentProgramFileName);
    static void SetColor(Color color);
    static void Menu(int value);
};





















