#ifndef MODEL_HPP
#define MODEL_HPP

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



    



    protected:

    ModelListener* modelListener;

    int count;

};

#endif // MODEL_HPP
