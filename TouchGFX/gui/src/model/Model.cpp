#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C"
{
extern DHT_data dht_data;
}


void Model::getDHTValue(DHT_data *dhtVal)
{
#ifdef SIMULATOR
//     dhtVal = &dht11_data;


#endif
    dhtVal->hum = dht_data.hum;
    dhtVal->temp = dht_data.temp;
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
            modelListener->newDHTValue(&dht_data);
        }
        tickCounter = 0;
    }
}