#include <gui/screen_screen/screenView.hpp>
#include <stdio.h>

screenView::screenView()
{
}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
    counter = 200;
    slider.setValue(counter);
    //setCount(counter);

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


void screenView::setCount(int count)
{
    Unicode::snprintf(textAreaCountBuffer, TEXTAREACOLOR_SIZE, "%02d", count);
    textAreaCount.invalidate();
}


void screenView::sliderValueUpdated(int value)
{
    if (counter != value)
    {
        counter = value;
        //setCount(counter);
    }
    updateColor(colorPicker1.getRgbColor());
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

    uint8_t r = touchgfx::Color::getRed(color);
    uint8_t g = touchgfx::Color::getGreen(color);
    uint8_t b = touchgfx::Color::getBlue(color);
    uint8_t a = slider.getValue(); // Get alpha from slider

    char tempBuffer[TEXTAREACOLOR_SIZE];
    sprintf(tempBuffer, "#%02X%02X%02X%02X", a, r, g, b);

    // Add printf for debugging
    printf("updateColor: %s\r\n", tempBuffer);

    Unicode::strncpy(screenViewBase::textAreaColorBuffer, tempBuffer, TEXTAREACOLOR_SIZE);

    textAreaColor.setWildcard(screenViewBase::textAreaColorBuffer);
    textAreaColor.invalidate();
}

void screenView::setDht(DHT_data *dhtVal)
{
        //printf("screenView::setDht() - Temp: %.1f, Hum: %.1f\r\n", dhtVal->temp, dhtVal->hum); // Debug print
        Unicode::snprintfFloat(textTemperatureBuffer, TEXTTEMPERATURE_SIZE, "%0.1f", dhtVal->temp);
        Unicode::snprintf(textHumidityBuffer, TEXTHUMIDITY_SIZE, "%d", (int)dhtVal->hum);

        textTemperature.invalidate();
        textHumidity.invalidate();

        imageProgress.setValue(dhtVal->hum * 0.88); //244pixels / 280 pixels
        imageProgress.invalidate();

         //.setColor(colorPicker1.getRgbColor());
}
