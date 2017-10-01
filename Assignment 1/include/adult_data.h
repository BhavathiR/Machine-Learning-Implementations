#ifndef ADULT_DATA_H
#define ADULT_DATA_H
#include<vector>
#include<string>
#include<node.h>

class adult_data
{
    public:
        virtual ~adult_data();
        void parse(string);
        void prune (string);
        void check(string);
    protected:

    private:
        void load(string);
        int n;
        node root;
        vector <adult> data;
};

#endif // ADULT_DATA_H
