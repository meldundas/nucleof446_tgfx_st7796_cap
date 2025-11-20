#include <gui/screen_screen/screenView.hpp>
#include <stdio.h>

screenView::screenView()
{
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
        // Also update the AARRGGBB display when slider value changes
        updateColor(colorPicker1.getRgbColor());
    }
}

void screenView::setCount(int count)
{
    Unicode::snprintf(textAreaCountBuffer, TEXTAREACOLOR_SIZE, "%02d", count);
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
    // This function seems to be for another text area, not AARRGGBB display
    // Unicode::snprintf(textAreaColorBuffer, TEXTAREACOLOR_SIZE, "%03d", value);
    // textAreaColor.invalidate();
}

void screenView::updateColor(colortype color)
{
    box1.setColor(color);
    box1.invalidate();
    textAreaColor.setColor(color);

    uint8_t r = touchgfx::Color::getRed(color);
    uint8_t g = touchgfx::Color::getGreen(color);
    uint8_t b = touchgfx::Color::getBlue(color);
    uint8_t a = slider.getValue(); // Get alpha from slider

    // Use a temporary char buffer for sprintf
    char tempBuffer[TEXTAREACOLOR_SIZE]; // TEXTAREACOLOR_SIZE is 10 for "#AARRGGBB\0"
    sprintf(tempBuffer, "#%02X%02X%02X%02X", a, r, g, b);

    // Copy the char string to the UnicodeChar buffer
    Unicode::strncpy(textAreaColorBuffer, tempBuffer, TEXTAREACOLOR_SIZE);

    // printf("updateColor: %s\r\n", tempBuffer); // Debug print from char buffer - Removed
    textAreaColor.setWildcard(textAreaColorBuffer);
    textAreaColor.invalidate();
}

void screenView::setDht(DHT_data *dhtVal)
{
        printf("screenView::setDht() - Temp: %.1f, Hum: %.1f\r\n", dhtVal->temp, dhtVal->hum); // Debug print
        Unicode::snprintfFloat(textTemperatureBuffer, TEXTTEMPERATURE_SIZE, "%0.1f", dhtVal->temp);
        Unicode::snprintf(textHumidityBuffer, TEXTHUMIDITY_SIZE, "%d", (int)dhtVal->hum);

        textTemperature.invalidate();
        textHumidity.invalidate();

        imageProgress.setValue(dhtVal->hum * 0.88); //244pixels / 280 pixels
        imageProgress.invalidate();

         //.setColor(colorPicker1.getRgbColor());
}
