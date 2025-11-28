#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <cstdio> // Required for printf

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
    printf("Model::getDHTValue() - Temp: %.1f, Hum: %.1f\r\n", dhtVal->temp, dhtVal->hum); // Debug print
}

Model::Model() : modelListener(0), count(0), tickCounter(0)
{

}

void Model::tick()
{
    tickCounter++;
    //printf("Model::tick() called, tickCounter: %d\r\n", tickCounter); // Debug print
    // Temporarily remove condition for debugging
     if (tickCounter >= 60)
    {
        if(modelListener != 0)
        {
            modelListener->newDHTValue(&dht_data);
        }
        tickCounter = 0; // Don't reset if condition is removed
    }
}
