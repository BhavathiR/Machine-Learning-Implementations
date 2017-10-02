#include "adult_data.h"
#include <fstream>
#include <my_ctype.h>
#include <iostream>
#include <bitset>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <hyperparameters.h>

using namespace std;

//Loading data stored in comma seperates vector (csv) form from a file
void adult_data::load(string file)
{
    data.clear();
    ifstream fin(file);
    locale x(locale::classic(), new my_ctype);
    fin.imbue(x);
        int age;
        string workclass;
        int fnlwgt;
        string education;
        int education_num;
        string marital_status;
        string occupation;
        string relationship;
        string race;
        string sex;
        int capital_gain, capital_loss, hours_per_week;
        string native_country;
        string salary;

    while(!fin.eof())
    {
        fin>>age>>workclass>>fnlwgt>>education>>education_num>>marital_status>>occupation>>relationship>>race>>sex>>capital_gain>>capital_loss>>hours_per_week>>native_country>>salary;
//        cout<<' '<<age<<workclass<<fnlwgt<<education<<education_num<<marital_status<<occupation<<relationship<<race<<sex<<capital_gain<<capital_loss<<hours_per_week<<native_country<<salary<<endl;     //Debugging Comment
        adult ad(age,workclass,fnlwgt,education,education_num,marital_status,occupation,relationship,race,sex,capital_gain,capital_loss,hours_per_week,native_country,salary);
        data.push_back(ad);
    }

    fin.close();
//        cout<<data.size()<<endl;  ////Debugging Comment
}

adult_data::~adult_data()
{
    //dtor
}

//Training a tree on training data given in file
void adult_data::parse(string file)
{
    load(file);
    for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it)
    {
        if(it->ask_class())
            root.add_pos(&(*it));
        else
            root.add_neg(&(*it));
    }
    root.calc_entropy();
    root.split(0);
}

//Pruning a tree on validation data given in file
void adult_data::prune(string file)
{
    load (file);
    root.clear();
    for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it)
    {
        if(it->ask_class())
            root.add_pos(&(*it));
        else
            root.add_neg(&(*it));
    }
    cout<<endl<<"Decision Tree Accuracy (after pruning): "<< root.prune()*100<<"%"<<endl;
    cout<<"\n\nPRUNED TREE\n\n";
    root.show(1);
}

//Creating a random forest with given number of trees
void adult_data::random_forest(int n, string file)
{
    load(file);

    for(int i=0;i<n;++i)    //Planting seeds
    {
//        cout<<"\nTree "<<i;   //Debugging Comment
        node tree;
        bitset<15> b("111111111111111");
        for(int j=0;j<14;++j)
            if((rand()%3)==0)       //Masking attributes, selecting about root(p) attributes per tree, where p is the total number of attributes
                b.reset(j);
//        cout<<" : "<<b;       //Debugging Comment
        tree.setbit(b);
        for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it)
        {
            if(it->ask_class())
            {
                if((rand()%3))      //Randomly takes 2/3rd of the positive testing data per tree in the forest
                {
                    tree.add_pos(&(*it));
//                    cout<<"\nInputting positive";     //Debugging Comment
                }
            }
            else
            {
                if((rand()%3))      //Randomly takes 2/3rd of the negative testing data per tree in the forest
                {
                    tree.add_neg(&(*it));
//                    cout<<"\nInputting negative";     //Debugging Comment
                }
            }
        }
        forest.push_back(tree);
    }

    for(vector<node>::iterator it=forest.begin(); it!=forest.end(); ++it)   //Growing trees
    {
//        cout<<"\nTree:";

        it->calc_entropy();
        it->split(0);
    }
}


//Testing trees with data given in file and returning accuracy
void adult_data::test_forest(string file)
{
        load(file);
        float accuracy=0;
        int pos, neg;
        bool result;
        for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it)
        {
            pos=0; neg=0;
            for(vector<node>::iterator jt=forest.begin(); jt!=forest.end(); ++jt)
            {
                if(jt->test(*it))
                    pos++;
                else neg++;
            }
            if(pos>=neg)
                result=true;
            else
                result=false;
            if(result==it->ask_class())
                accuracy++;
//            cout<<endl<<result<<" : "<<accuracy;      //Debugging Comment
        }
        accuracy/=data.size();
        cout<<"\n\n\nRandom Forest Accuracy: "<<accuracy*100<<"%";
}
