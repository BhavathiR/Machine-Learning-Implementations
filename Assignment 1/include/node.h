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
//        node(vector < adult* >, vector < adult* > , int , int );
        node();
        virtual ~node();
        void split(int);

        void add_pos(adult*);
        void add_neg(adult*);
        float calc_entropy();
        void clear();
        float prune();
        void show(int);

    protected:

    private:
        int ite;
        int npost, nnegt;
        bool split_type_disc,end_of_tree;
        bitset<15> checked;
        float entropy;
        string disc_split_attr, cont_split_attr;
        vector < adult* > pos_list, neg_list;
        map <string, node> disc_children;
        map <int, pair<node,node>> cont_children;
        float calc_split_disc();
        float calc_split_cont();

        void setbit(bitset<15>);


};

#endif // NODE_H
