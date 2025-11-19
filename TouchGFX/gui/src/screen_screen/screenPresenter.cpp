#include <gui/screen_screen/screenView.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

screenPresenter::screenPresenter(screenView& v)
    : view(v)
{

}

void screenPresenter::activate()
{

}

void screenPresenter::deactivate()
{

}

void screenPresenter::incrementCount()
{
    int count = model->getCount();
    count++;
    if (count > 99)
    {
        count = 0;
    }
    model->setCount(count);
    view.setCount(count);
}

void screenPresenter::decrementCount()
{
    int count = model->getCount();
    count--;
    if (count < 0)
    {
        count = 99;
    }
    model->setCount(count);
    view.setCount(count);



}


void screenPresenter::slideChange(int newVal)
{
	view.setCount(newVal);
	model->setCount(newVal);
}


void screenPresenter::newDHTValue(DHT_data *dhtVal)
{
       view.setDht(dhtVal);

}
