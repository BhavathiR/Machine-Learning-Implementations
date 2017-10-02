/*
    Stores an object of adult type and provides various abstractions on it
*/

#ifndef ADULT_H
#define ADULT_H
#include<attribute_values.h>
#include<vector>
#include<string>
#include <map>

using namespace std;

class adult
{
    public:
        adult(int, string, int, string, int, string, string, string, string, string, int, int, int, string, string);    //Constructor: Create Adult object with passed values
        virtual ~adult();           //Destructor
        string ask_disc(string);    //Return value of discrete attribute
        int ask_cont(string);       //Return value of continuous attribute
        bool ask_class();           //Return class: true for >50K, false for <=50K

    protected:

    private:
        map <string, string> disc_attr;     //Map from discrete attribute type to its value
        map <string, int> cont_attr;        //Map from continuous attribute type to its value
};
#endif // ADULT_H
