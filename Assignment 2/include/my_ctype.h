#ifndef MY_CTYPE_H
#define MY_CTYPE_H
#include <locale>
#include <algorithm>

using namespace std;

class my_ctype : public ctype <char>
{
        mask my_table[table_size];
    public:
        my_ctype(size_t refs = 0);
        ~my_ctype();
    protected:

    private:
};

#endif // MY_CTYPE_H
