#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C"
{
extern DHT11_Data_TypeDef dht11_data;
}


void Model::getDHTValue(DHT11_Data_TypeDef *dhtVal)
{
#ifdef SIMULATOR
//     dhtVal = &dht11_data;


#endif
    dhtVal->humidity = dht11_data.humidity;
    dhtVal->temperature = dht11_data.temperature;
}

Model::Model() : modelListener(0), count(0), tickCounter(0)
{

}

void Model::tick()
{
    tickCounter++;
    if (tickCounter >= 60)
    {
        if(modelListener != 0)
        {
            modelListener->newDHTValue(&dht11_data);
        }
        tickCounter = 0;
    }
}