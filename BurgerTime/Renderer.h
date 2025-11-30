#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>
#include <gl/GL.h>
#include <string>
#include <map>

#pragma comment(lib, "opengl32.lib")

class Renderer {
public:
    Renderer(int width, int height, const char* title, bool fullscreen = false);
    ~Renderer();

    bool handleMessages();
    void beginFrame();
    void endFrame();
    
    void drawSprite(const std::string& textureName, float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    void loadTexture(const std::string& name, const std::string& path);
    void drawNeonText(const std::string& text, float x, float y, float size, float r, float g, float b);
    void drawText(const std::string& text, float x, float y, const std::string& fontName, int size, float r, float g, float b);

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    void initOpenGL();
    void buildFont(const std::string& fontName, int size);
    
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width, height;
    
    std::map<std::string, GLuint> textures;
    std::map<std::string, GLuint> fonts; // Map "Name_Size" to display list base
};

#endif
