#ifndef ADULT_DATA_H
#define ADULT_DATA_H
#include<vector>
#include<string>
#include<node.h>

class adult_data
{
    public:
        adult_data(string);
        virtual ~adult_data();
        void parse();
    protected:

    private:
        int n;
        vector <adult> data;
};

#endif // ADULT_DATA_H
