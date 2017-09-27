#include "adult.h"

adult::adult(int age, string workclass, int fnlwgt, string education, int education_num, string marital_status,
             string occupation, string relationship, string race, string sex, int capital_gain, int capital_loss,
             int hours_per_week, string native_country, string salary)
{
        cont_attr["age"] = age;
        disc_attr["workclass"] = workclass;
        cont_attr["fnlwgt"] = fnlwgt;
        disc_attr["education"] = education;
        cont_attr["education-num"] = education_num;
        disc_attr["marital_status"] = marital_status;
        disc_attr["occupation"] = occupation;
        disc_attr["relationship"] = relationship;
        disc_attr["race"] = race;
        disc_attr["sex"] = sex;
        cont_attr["capital-gain"] = capital_gain;
        cont_attr["capital-loss"] = capital_loss;
        cont_attr["hours-per_week"] = hours_per_week;
        disc_attr["native-country"] = native_country;
        disc_attr["salary"] = salary;
}

adult::~adult()
{
    //dtor
}
string adult::ask_disc(string s)
{
    if(disc_attr.count(s))
        return disc_attr[s];
    return "ERR";
}

int adult::ask_cont(string s)
{
    if(cont_attr.count(s))
        return cont_attr[s];
    return 0;
}
bool adult::ask_class()
{
    return dclassify[disc_attr["salary"]];
}
