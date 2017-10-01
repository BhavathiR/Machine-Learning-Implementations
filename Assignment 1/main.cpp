#include <adult_data.h>

using namespace std;

int main()
{
    adult_data list;
    list.parse("adult.data");
    list.prune("adult.test");
//    list.check("adult.test");
            return 0;
}


