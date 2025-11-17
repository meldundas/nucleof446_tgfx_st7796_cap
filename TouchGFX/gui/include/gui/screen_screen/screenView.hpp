#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

class screenView : public screenViewBase
{
public:
    screenView();
    virtual ~screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void setCount(int count);


    virtual void scrollWheelUpdateItem(MenuElement& item, int16_t itemIndex)
    {
        item.setNumber(itemIndex);
    }

    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);


    void buttonClickUp();
    void buttonClickDown();
    void sliderValueChanged(int value);

    void setDht(DHT11_Data_TypeDef *dhtVal);

    Unicode::UnicodeChar textAreaColorBuffer[5];
    static const uint16_t TEXTAREACOLOR_SIZE = 5;
    void setColorText(int value);

protected:
    int counter;
    touchgfx::TextAreaWithOneWildcard textAreaColor;
};

#endif // SCREENVIEW_HPP
