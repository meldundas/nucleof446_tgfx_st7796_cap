#ifndef SCREENPRESENTER_HPP
#define SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class screenView;

class screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    screenPresenter(screenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

        virtual ~screenPresenter() {}

    

        void incrementCount();

        void decrementCount();

                void slideChange(int newVal);

        

                void newDHTValue(DHT11_Data_TypeDef *dhtVal);

            

            private:
    screenPresenter();

    screenView& view;
};

#endif // SCREENPRESENTER_HPP
