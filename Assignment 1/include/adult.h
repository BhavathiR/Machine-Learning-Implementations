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
        adult(int, string, int, string, int, string, string, string, string, string, int, int, int, string, string);
        virtual ~adult();
        string ask_disc(string);
        int ask_cont(string);
        bool ask_class();

    protected:

    private:
        map <string, string> disc_attr;
        map <string, int> cont_attr;
};
#endif // ADULT_H
