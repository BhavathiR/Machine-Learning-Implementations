/*
    Provides various functionalities on provided data like
    building decision trees and random forests, pruning
    and testing given data against existing structures
*/

#ifndef ADULT_DATA_H
#define ADULT_DATA_H
#include<vector>
#include<string>
#include<node.h>

class adult_data
{
    public:
        virtual ~adult_data();          //Constructor
        void parse(string);             //Training a tree on training data given in file
        void prune (string);            //Pruning a tree on validation data given in file
        void random_forest(int, string);//Creating a random forest with given number of trees
        void test_forest(string);       //Testing trees with data given in file and returning accuracy
    protected:

    private:
        void load(string);              //Loading data stored in comma seperates vector (csv) form from a file
        node root;                      //Tree root node
        vector <node> forest;           //Forest root nodes
        vector <adult> data;            //Training/Validation/Testing Data
};

#endif // ADULT_DATA_H
