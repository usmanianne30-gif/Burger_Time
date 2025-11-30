#include "Renderer.h"
#include <windows.h>
#include <gl/gl.h>
#include <iostream>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Renderer::Renderer(int width, int height, const char* title, bool fullscreen) : width(width), height(height) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BurgerTimeWindow";
    wc.style = CS_OWNDC;

    RegisterClass(&wc);

    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    if (fullscreen) {
        DEVMODE dmScreenSettings = {};
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        dwStyle = WS_POPUP | WS_VISIBLE;
        ShowCursor(FALSE);
    }

    hwnd = CreateWindowEx(
        0, "BurgerTimeWindow", title,
        dwStyle,
        0, 0, width, height,
        NULL, NULL, hInstance, NULL
    );

    initOpenGL();
}

Renderer::~Renderer() {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
}

void Renderer::initOpenGL() {
    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, format, &pfd);

    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

bool Renderer::handleMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Renderer::beginFrame() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {
    SwapBuffers(hdc);
}

void Renderer::loadTexture(const std::string& name, const std::string& path) {
    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 4);
    
    if (data) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        textures[name] = textureID;
        stbi_image_free(data);
        std::cout << "Loaded texture: " << name << " from " << path << std::endl;
    } else {
        std::cerr << "Failed to load texture: " << path << " Reason: " << stbi_failure_reason() << std::endl;
    }
}

void Renderer::drawSprite(const std::string& textureName, float x, float y, float width, float height, float r, float g, float b) {
    if (textures.find(textureName) != textures.end()) {
        glBindTexture(GL_TEXTURE_2D, textures[textureName]);
        glColor3f(r, g, b);
        
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + width, y);
        glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
        glTexCoord2f(0, 1); glVertex2f(x, y + height);
        glEnd();
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}

void Renderer::drawNeonText(const std::string& text, float x, float y, float size, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D);
    
    // Draw glow effect
    for (int glow = 8; glow > 0; glow -= 2) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glColor4f(r, g, b, 0.15f);
        
        float charWidth = size * 0.6f;
        for (size_t i = 0; i < text.length(); i++) {
            if (text[i] == ' ') continue;
            float charX = x + i * charWidth;
            
            glBegin(GL_QUADS);
            glVertex2f(charX - glow, y - glow);
            glVertex2f(charX + charWidth * 0.8f + glow, y - glow);
            glVertex2f(charX + charWidth * 0.8f + glow, y + size + glow);
            glVertex2f(charX - glow, y + size + glow);
            glEnd();
        }
        glDisable(GL_BLEND);
    }
    
    // Draw main text
    glColor3f(r * 1.5f, g * 1.5f, b * 1.5f);
    
    float charWidth = size * 0.6f;
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] == ' ') continue;
        
        float charX = x + i * charWidth;
        
        glBegin(GL_QUADS);
        glVertex2f(charX, y);
        glVertex2f(charX + charWidth * 0.8f, y);
        glVertex2f(charX + charWidth * 0.8f, y + size);
        glVertex2f(charX, y + size);
        glEnd();
    }
    
    glEnable(GL_TEXTURE_2D);
}

void Renderer::buildFont(const std::string& fontName, int size) {
    std::string key = fontName + "_" + std::to_string(size);
    if (fonts.find(key) != fonts.end()) return;

    HFONT hFont;
    GLuint base = glGenLists(96);

    hFont = CreateFont(
        -size,                  // Height of font
        0,                      // Width of font
        0,                      // Angle of escapement
        0,                      // Orientation angle
        FW_BOLD,                // Font weight
        FALSE,                  // Italic
        FALSE,                  // Underline
        FALSE,                  // Strikeout
        ANSI_CHARSET,           // Character set identifier
        OUT_TT_PRECIS,          // Output precision
        CLIP_DEFAULT_PRECIS,    // Clipping precision
        ANTIALIASED_QUALITY,    // Output quality
        FF_DONTCARE | DEFAULT_PITCH, // Family and pitch
        fontName.c_str()        // Font name
    );

    SelectObject(hdc, hFont);
    wglUseFontBitmaps(hdc, 32, 96, base);
    
    fonts[key] = base;
    DeleteObject(hFont);
}

void Renderer::drawText(const std::string& text, float x, float y, const std::string& fontName, int size, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D);
    
    std::string key = fontName + "_" + std::to_string(size);
    if (fonts.find(key) == fonts.end()) {
        buildFont(fontName, size);
    }
    
    glColor3f(r, g, b);
    glRasterPos2f(x, y + size); // Adjust for baseline
    
    glPushAttrib(GL_LIST_BIT);
    glListBase(fonts[key] - 32);
    glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
    glPopAttrib();
    
    glEnable(GL_TEXTURE_2D);
}
