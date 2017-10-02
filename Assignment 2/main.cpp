/*
    Main Function:
    ***Results***
    Decision Tree: 84.91% accuracy
    Random Forests: 83.47% accuracy
*/

#include <adult_data.h>
#include <hyperparameters.h>

using namespace std;

int main()
{
    adult_data list;
    list.parse("adult.data");
    list.prune("adult.test");
    list.random_forest(FOREST_SIZE, "adult.data");
    list.test_forest("adult.test");

            return 0;
}


