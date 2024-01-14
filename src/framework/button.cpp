#include "button.h"

Button::Button(const char *filename, Vector2 position, ButtonType type) // Constructor
{
    if (!image.LoadPNG(filename))
    {
        std::cout << "Error al cargar la imagen " << std::endl;
    }
    this->position = position;
    this->type = type;
}

void Button::Render(Image &framebuffer)
{
    if (image.pixels)
        framebuffer.DrawImage(this->image, this->position.x, this->position.y, true);
}

bool Button::IsMouseInside(Vector2 mousePosition)
{
    // Devolvemos el booleano que calcula si el raton esta dentro del boton
    return mousePosition.x >= position.x &&
           mousePosition.x < (position.x + image.width) &&
           mousePosition.y >= position.y &&
           mousePosition.y < (position.y + image.height);
}

Image &Button::GetImage()
{
    return image;
}

int Button::GetWidth()
{
    return image.width;
}

ButtonType Button::GetType()
{
    return type;
}

Vector2 Button::GetPosition()
{
    return position;
}
