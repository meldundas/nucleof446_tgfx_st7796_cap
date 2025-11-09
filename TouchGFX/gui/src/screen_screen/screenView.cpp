#include <gui/screen_screen/screenView.hpp>

screenView::screenView()
{

}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
    counter = 0;
    slider.setValue(counter);
    setCount(counter);
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


