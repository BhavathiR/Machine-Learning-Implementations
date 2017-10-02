/*
        Stores information about a node of the decision tree
        and provides functionalities like growing, pruning
        and printing the tree or testing a data object of
        adult to check its class.
*/

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
        node();     //Constructor
        virtual ~node();    //Destructor

        void add_pos(adult*);       //Adding positive sample (>50K)
        void add_neg(adult*);       //Adding negative sample (<=50K)
        void setbit(bitset<15>);    //Setting bits for checked attributes
        void clear();               //Removing data from node
        float calc_entropy();       //Function for calculating node entropy
        void split(int);            //Recursive function to calculate class split of decision tree
        float prune();              //Function to prune an existing decision tree using loaded testing data
        void show(int);             //Function to print the current tree
        bool test(adult);           //Testing a tree to return the class of an adult

    protected:

    private:
        float calc_split_disc();       //Calculate best split for discrete attributes
        float calc_split_cont();        //Calculate best split for continuous attributes

        int ite;        //Recurcion variable to maintain indent
        int npost, nnegt;       //Number of positive and negative data samples
        bool split_type_disc,end_of_tree;       //Boolean variables for "split type" and "end of tree"
        bitset<15> checked;     //Mask for bagging attributes
        float entropy;          //Node entropy
        string disc_split_attr, cont_split_attr;    //Split attribute type
        vector < adult* > pos_list, neg_list;       //List of positive and negative data samples
        map <string, node> disc_children;           //Children when split is on a discrete variable
        map <int, pair<node,node>> cont_children;   //Children when split is on a continuous variable
};

#endif // NODE_H
