#ifndef NODE_H
#define NODE_H
#include <adult.h>
#include <utility>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

class node
{
    public:
        node(vector < adult* >, vector < adult* > , int , int );
        node();
        virtual ~node();
        void calc_split();
        void add_pos(adult*);
        void add_neg(adult*);
        float calc_entropy();

    protected:

    private:

        int npost, nnegt;
        bitset<15> checked;
        float entropy;
        string split_attr;
        vector < adult* > pos_list, neg_list;
        map <string, node> children;

        void children_split();
        void setbit(bitset<15>);


};

#endif // NODE_H
