#include <gui/containers/ColorPicker.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <math.h>

ColorPicker::ColorPicker() :
    colorConfirmedCallback(0),
    color_center_x(0),
    color_center_y(0),
    isPressed(false),
    selectedColor(0)
{

}

void ColorPicker::initialize()
{
    ColorPickerBase::initialize();
    setTouchable(true);
    color_center_x = colorCircle.getX() + colorCircle.getWidth() / 2;
    color_center_y = colorCircle.getY() + colorCircle.getHeight() / 2;
    setCurrentColor(selector.getX() + selector.getWidth() / 2, selector.getY() + selector.getHeight() / 2);
}

void ColorPicker::handleClickEvent(const ClickEvent& evt)
{
    int offsetX = evt.getX();
    int offsetY = evt.getY();

    if (evt.getType() == ClickEvent::PRESSED)
    {
        const uint32_t dist = colorDistances(offsetX, offsetY);
        if (dist < MAX_DIST && dist > MIN_DIST) // If the color palette circle is pressed
        {
            isPressed = true;
            float angle = float(atan2(float(offsetY - color_center_y), float(offsetX - color_center_x)));
            setCurrentColor(uint32_t(MID_RADIUS * cosf(angle) + color_center_x), uint32_t(MID_RADIUS * sinf(angle) + color_center_y));
        }
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        if (isPressed)
        {
            isPressed = false;
            if (colorConfirmedCallback && colorConfirmedCallback->isValid())
            {
                colorConfirmedCallback->execute(selectedColor);
            }
        }
    }
}

void ColorPicker::handleDragEvent(const DragEvent& evt)
{
    if (isPressed)
    {
        int offsetX = evt.getNewX();
        int offsetY = evt.getNewY();
        float angle = float(atan2(float(offsetY - color_center_y), float(offsetX - color_center_x)));
        setCurrentColor(uint32_t(MID_RADIUS * cosf(angle) + color_center_x), uint32_t(MID_RADIUS * sinf(angle) + color_center_y));
    }
}

// Calculate the distance from the pressed position of the color to the center
uint32_t ColorPicker::colorDistances(uint32_t x, uint32_t y)
{
    const int32_t dx = (x - color_center_x);
    const int32_t dy = (y - color_center_y);
    return dx * dx + dy * dy;
}

// Set the color of the circle in the center to the color currently selected
void ColorPicker::setCurrentColor(uint32_t x, uint32_t y)
{
    selector.moveTo(x - selector.getWidth() / 2, y - selector.getHeight() / 2); // Lock the selector to the color palette circle

    const Bitmap bitmapColors = Bitmap(BITMAP_CIRCLECOLORFADE_ID);  // Fetch bitmap
    const uint8_t* pixels = (const uint8_t*)bitmapColors.getData(); // Save bitmap data
    if (!pixels)
    {
        return;
    }
    int16_t _x = (int16_t)x;
    int16_t _y = (int16_t)y;

    _x -= colorCircle.getX();
    _y -= colorCircle.getY();

    pixels += (_x + _y * bitmapColors.getWidth()) * 4;              // Offset the data to start at the position of the selector

    uint8_t b = *pixels++;  // Save the data and move the pointer by 8 bits
    uint8_t g = *pixels++;
    uint8_t r = *pixels++;

    selectedColor = Color::getColorFromRGB(r, g, b); // Convert to touchgfx::colortype

    currentColorPainter.setColor(selectedColor);
    currentColor.invalidate();
}

colortype ColorPicker::getRgbColor()
{
    return selectedColor;
}