#include "inc/RenderWindow.h"
#include <iostream>
#include <GL/glut.h>
#include <Cg/cg.h> 
#include <GL/FreeImage.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* RenderWindow::InputData;
CGcontext RenderWindow::CgContext;
CGprofile RenderWindow::CgVertexProfile;
CGprofile RenderWindow::CgFragmentProfile;
CGprogram RenderWindow::CgVertexProgram;
CGprogram RenderWindow::CgFragmentProgram;
CGparameter RenderWindow::CgFragmentParam;
std::unique_ptr<GLubyte[]> RenderWindow::Pixels;
int RenderWindow::width, RenderWindow::height;
int RenderWindow::PosX, RenderWindow::PosY;
std::string  RenderWindow::inputFileName;

RenderWindow::RenderWindow(int width_, int height_, int PosX_, int PosY_, std::string inputFileName_)
{
    width = width_;
    height = height_;
    PosX = PosX_;
    PosY = PosY_;
    inputFileName = inputFileName_;
    Pixels = std::make_unique<GLubyte[]>(3 * width*height);
}

void RenderWindow::Init()
{
    int argc = 1;
    char *argv[1] = { (char*)"" };
    glutInitWindowSize(width, height);
    glutInitWindowPosition(PosX, PosY);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInit(&argc, argv);
}

void RenderWindow::Create()
{
    glutCreateWindow("PixelArt");
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void RenderWindow::LoadInputData()
{
    int picWidth, picHeight, picChannels;
    InputData = stbi_load(inputFileName.c_str(), &picWidth, &picHeight, &picChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glBindTexture(GL_TEXTURE_2D, 666);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8,
        picWidth, picHeight, GL_RGB, GL_UNSIGNED_BYTE, InputData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_LINEAR_MIPMAP_LINEAR);
    stbi_image_free(InputData);
}

void RenderWindow::DrawVertices()
{
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  glVertex2f(-0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);  glVertex2f(0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);  glVertex2f(0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);  glVertex2f(-0.5f, -0.5f);
    glEnd();
}

void RenderWindow::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cgGLBindProgram(CgVertexProgram);
    cgGLEnableProfile(CgVertexProfile);
    cgGLBindProgram(CgFragmentProgram);
    cgGLEnableProfile(CgFragmentProfile);
    cgGLEnableTextureParameter(CgFragmentParam);
    DrawVertices();
    cgGLDisableProfile(CgVertexProfile);
    cgGLDisableProfile(CgFragmentProfile);
    cgGLDisableTextureParameter(CgFragmentParam);
    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.get());
    glutSwapBuffers();
}

void RenderWindow::SavePicture()
{
    FIBITMAP* image = FreeImage_ConvertFromRawBits(Pixels.get(), width, height, 3 * width, 24,
        0x0000FF, 0xFF0000, 0x00FF00, false);
    FreeImage_Save(FIF_BMP, image, "Save/Picture.bmp", 0);
    FreeImage_Unload(image);
    std::cout << "Image has been saved to the PixelArt/Save/";
}

void RenderWindow::Keyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 27:
        exit(0);
        break;
    case 115:
        glutFullScreen();
        break;
    case 100:
        glutReshapeWindow(width, height);
        break;
    case 97:
        SavePicture();
        break;
    }
}

void RenderWindow::AddGlutFunc()
{
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutCreateMenu(Menu);
    glutAddMenuEntry("Save", static_cast<int>(Options::SAVE));
    glutAddMenuEntry("FULL_SCREEN", static_cast<int>(Options::FULL_SCREEN));
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void RenderWindow::SetCGContextAndProfile()
{
    CgContext = cgCreateContext();
    cgGLSetDebugMode(CG_FALSE);
    cgSetParameterSettingMode(CgContext, CG_DEFERRED_PARAMETER_SETTING);
    CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    cgGLSetOptimalOptions(CgVertexProfile);
}

void RenderWindow::SetCgVertexProgram()
{
    CgVertexProgram = cgCreateProgramFromFile(CgContext, CG_SOURCE,
        "CG/varying.cg",
        CgVertexProfile, "C3E2v_varying", NULL);
    cgGLLoadProgram(CgVertexProgram);
    CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(CgFragmentProfile);
}

void RenderWindow::SetCgFragmentProgram(const char * FragmentProgramFileName)
{
    CgFragmentProgram = cgCreateProgramFromFile(CgContext, CG_SOURCE,
        FragmentProgramFileName,
        CgFragmentProfile,
        "C3E3f_texture", NULL);
    cgGLLoadProgram(CgFragmentProgram);
    CgFragmentParam = cgGetNamedParameter(CgFragmentProgram, "decal");
    cgGLSetTextureParameter(CgFragmentParam, 666);
}

void RenderWindow::SetColor(Color color) 
{
    switch (color)
    {
    case Color::RED:
        SetCgFragmentProgram("CG/red_texture.cg");
        break;
    case Color::GREEN:
        SetCgFragmentProgram("CG/green_texture.cg");
        break;
    case Color::BLUE:
        SetCgFragmentProgram("CG/blue_texture.cg");
        break;
    }
}

void RenderWindow::Menu(int value)
{
    switch (value)
    {
    case (static_cast<int>(Options::FULL_SCREEN)):
        glutFullScreen();
        break;
    case (static_cast<int>(Options::SAVE)):
        SavePicture();
        break;
    }
}

void RenderWindow::Build(Color color)
{
    Init();
    Create();
    LoadInputData();
    AddGlutFunc();
    SetCGContextAndProfile();
    SetCgVertexProgram();
    SetColor(color);
    glutMainLoop();
}
