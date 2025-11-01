#include <gui/screen_screen/screenView.hpp>

screenView::screenView()
{

}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
}

void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}

void screenView::setCount(int count)
{
    Unicode::snprintf(textAreaCountBuffer, TEXTAREACOUNT_SIZE, "%03d", count);
    textAreaCount.invalidate();
}

void screenView::changeCount(char op)
{
    if (op == '+')
    {
        presenter->incrementCount();
    }
    else if (op == '-')
    {
        presenter->decrementCount();
    }
}
