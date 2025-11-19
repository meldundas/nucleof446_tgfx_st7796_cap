#include <gui/screen_screen/screenView.hpp>
#include <stdio.h>

screenView::screenView()
{
    textAreaColor.setXY(10, 10); // Example position, adjust as needed
    textAreaColor.setWildcard(textAreaColorBuffer);
    add(textAreaColor); // Add to the container
}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
    counter = 0;
    slider.setValue(counter);
    setCount(counter);

    static touchgfx::Callback<screenView, colortype> colorConfirmedCallback(this, &screenView::updateColor);
    colorPicker1.setColorConfirmedCallback(colorConfirmedCallback);
}

void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}

void screenView::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonWithIconUp)
    {
        buttonClickUp();
    }
    else if (&src == &buttonWithIconDown)
    {
        buttonClickDown();
    }
}

void screenView::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &slider)
    {
        sliderValueChanged(value);
    }
}

void screenView::setCount(int count)
{
    Unicode::snprintf(textAreaCountBuffer, TEXTAREACOUNT_SIZE, "%02d", count);
    textAreaCount.invalidate();
}

void screenView::sliderValueChanged(int value)
{
    if (counter != value)
    {
        counter = value;
        setCount(counter);
    }
}

void screenView::setColorText(int value)
{
    Unicode::snprintf(textAreaColorBuffer, TEXTAREACOLOR_SIZE, "%03d", value);
    textAreaColor.invalidate();
}

void screenView::updateColor(colortype color)
{
    box1.setColor(color);
    box1.invalidate();
    textAreaColor.setColor(color);
    uint8_t r = touchgfx::Color::getRed(color);
    uint8_t g = touchgfx::Color::getGreen(color);
    uint8_t b = touchgfx::Color::getBlue(color);

    Unicode::snprintf(textAreaColorBuffer, TEXTAREACOLOR_SIZE, "#%02X%02X%02X", r, g, b);
    textAreaColor.setWildcard(textAreaColorBuffer);
    textAreaColor.invalidate();
}

void screenView::setDht(DHT11_Data_TypeDef *dhtVal)
{
        Unicode::snprintfFloat(textTemperatureBuffer, TEXTTEMPERATURE_SIZE, "%0.1f", dhtVal->temperature);
        Unicode::snprintf(textHumidityBuffer, TEXTHUMIDITY_SIZE, "%d", dhtVal->humidity);

        textTemperature.invalidate();
        textHumidity.invalidate();

        imageProgress.setValue(dhtVal->humidity * 0.88); //244pixels / 280 pixels
        imageProgress.invalidate();

         //.setColor(colorPicker1.getRgbColor());
}


