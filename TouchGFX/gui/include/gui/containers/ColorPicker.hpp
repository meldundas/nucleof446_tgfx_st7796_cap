#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

#include <gui_generated/containers/ColorPickerBase.hpp>
#include <touchgfx/Color.hpp>

#define SELECTOR_RADIUS 10

class ColorPicker : public ColorPickerBase
{
public:
    ColorPicker();
    virtual ~ColorPicker() {}
    virtual void initialize();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);



    colortype getRgbColor();

private:
    uint32_t colorDistances(uint32_t x, uint32_t y);
    void setCurrentColor(uint32_t x, uint32_t y);

    // Distances used for calculating the color selected in the color wheel.
    enum
    {
        MID_RADIUS = 65,
        MAX_RADIUS = 76,
        MAX_DIST = MAX_RADIUS * MAX_RADIUS,
        MIN_RADIUS = 55,
        MIN_DIST = MIN_RADIUS * MIN_RADIUS
    };

    int32_t color_center_x, color_center_y; // Center of the color picker
    bool isPressed; // Used for informing handleDragEvent
    colortype selectedColor;
};

#endif // COLORPICKER_HPP
