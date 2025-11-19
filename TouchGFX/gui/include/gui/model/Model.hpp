#ifndef MODEL_HPP
#define MODEL_HPP

#include "dht.h"

class ModelListener;

class Model

{

public:

    Model();



    void bind(ModelListener* listener)

    {

        modelListener = listener;

    }



    void tick();



        int getCount()



        {



            return count;



        }



    



        void setCount(int value)



        {



            count = value;



        }



        void getDHTValue(DHT_data *dhtVal);



    



    protected:



    



        ModelListener* modelListener;



    



        int count;



    



        int tickCounter;

};

#endif // MODEL_HPP
