#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

#include <gui_generated/containers/ColorPickerBase.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>

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

    void setColorConfirmedCallback(GenericCallback<colortype>& callback)
    {
        colorConfirmedCallback = &callback;
    }

private:
    uint32_t colorDistances(uint32_t x, uint32_t y);
    void setCurrentColor(uint32_t x, uint32_t y);

    GenericCallback<colortype>* colorConfirmedCallback;

    // Distances used for calculating the color selected in the color wheel.
    enum
    {
        MID_RADIUS = 65,
        MAX_RADIUS = 76,
        MAX_DIST = MAX_RADIUS * MAX_RADIUS,
        MIN_RADIUS = 0, // Set to 0 to allow selection to the center
        MIN_DIST = MIN_RADIUS * MIN_RADIUS
    };

    int32_t color_center_x, color_center_y; // Center of the color picker
    bool isPressed; // Used for informing handleDragEvent
    colortype selectedColor;
};

#endif // COLORPICKER_HPP