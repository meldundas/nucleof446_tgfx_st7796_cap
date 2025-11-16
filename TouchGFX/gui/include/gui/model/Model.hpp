#ifndef MODEL_HPP
#define MODEL_HPP

#include "dht11.h"

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



        void getDHTValue(DHT11_Data_TypeDef *dhtVal);



    



    protected:

    ModelListener* modelListener;

    int count;

};

#endif // MODEL_HPP
