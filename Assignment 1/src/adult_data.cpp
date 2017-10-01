#include "adult_data.h"
#include <fstream>
#include <my_ctype.h>
#include <iostream>
using namespace std;


void adult_data::load(string file)
{
    n=0;
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
//        int i=0;

    while(!fin.eof())
    {
        fin>>age>>workclass>>fnlwgt>>education>>education_num>>marital_status>>occupation>>relationship>>race>>sex>>capital_gain>>capital_loss>>hours_per_week>>native_country>>salary;
//        cout<<++i<<' '<<age<<workclass<<fnlwgt<<education<<education_num<<marital_status<<occupation<<relationship<<race<<sex<<capital_gain<<capital_loss<<hours_per_week<<native_country<<salary<<endl;

        adult ad(age,workclass,fnlwgt,education,education_num,marital_status,occupation,relationship,race,sex,capital_gain,capital_loss,hours_per_week,native_country,salary);
        data.push_back(ad);
        n++;
    }

    fin.close();
//        cout<<data.size()<<endl;
}

adult_data::~adult_data()
{
    //dtor
}

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
    cout<<root.calc_entropy();
    root.split(0);
}

void adult_data::prune(string file)
{
    load (file);
    cout<<endl<<data.size();
    root.clear();
    for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it)
    {
        if(it->ask_class())
            root.add_pos(&(*it));
        else
            root.add_neg(&(*it));
    }
    cout<<endl<<root.prune()<<endl;
    root.show(0);
}
void adult_data::check(string file)
{
    load(file);
    int i=0;
    for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it, ++i)
    {
        cout<<endl<<i<<' ';
        if(it->ask_disc("salary")==">50K")
            cout<<it->ask_disc("relationship")<<it->ask_disc("education")<<it->ask_cont("capital-gain")<<it->ask_disc("occupation");
   //     cout<<it->ask_disc("salary");

    }

}
