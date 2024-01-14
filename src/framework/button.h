#pragma once
#include "image.h"

enum ButtonType
{
    BTN_CLEAR,
    BTN_LOAD,
    BTN_SAVE,
    BTN_ERASER,
    BTN_PAINT,
    BTN_LINE,
    BTN_RECTANGLE,
    BTN_CIRCLE,
    BTN_TRIANGLE,
    BTN_BLACK,
    BTN_WHITE,
    BTN_PINK,
    BTN_YELLOW,
    BTN_RED,
    BTN_BLUE,
    BTN_CYAN,
    BTN_GREEN
};

class Button
{
private:
    Image image;
    Vector2 position;
    ButtonType type;

public:
    Button(const char *filename, Vector2 position, ButtonType type);
    void Render(Image &framebuffer);
    bool IsMouseInside(Vector2 mousePosition);
    Image &GetImage();
    int GetWidth();
    ButtonType GetType();
    Vector2 Button::GetPosition();
};
