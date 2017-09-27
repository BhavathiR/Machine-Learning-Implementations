#include "adult_data.h"
#include <iostream>
#include <fstream>
#include <my_ctype.h>

using namespace std;


adult_data::adult_data(string file)
{
    n=0;
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
        int a[15]={0};
//        int i=0;

    while(!fin.eof())
    {
        fin>>age>>workclass>>fnlwgt>>education>>education_num>>marital_status>>occupation>>relationship>>race>>sex>>capital_gain>>capital_loss>>hours_per_week>>native_country>>salary;
//        cout<<++i<<age<<workclass<<fnlwgt<<education<<education_num<<marital_status<<occupation<<relationship<<race<<sex<<capital_gain<<capital_loss<<hours_per_week<<native_country<<salary<<endl;

        adult ad(age,workclass,fnlwgt,education,education_num,marital_status,occupation,relationship,race,sex,capital_gain,capital_loss,hours_per_week,native_country,salary);
        data.push_back(ad);
        n++;
    }
        for(int i=0; i<15;++i) cout<<a[i]<<endl;

//    cout<<n;
    fin.close();
        cout<<data.size()<<endl;
}

adult_data::~adult_data()
{
    //dtor
}

void adult_data::parse()
{
    node root;
    for(vector<adult>::iterator it=data.begin(); it!=data.end(); ++it)
    {
 //       cout<<it->ask_disc("sex")<<endl;
        if(it->ask_class())
            root.add_pos(&(*it));
        else
            root.add_neg(&(*it));
    }
    root.calc_entropy();
    root.calc_split();
}
