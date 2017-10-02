#include "my_ctype.h"

//Adding ',' and '.' as delimiter characters
my_ctype::my_ctype(size_t refs)  : ctype<char>(&my_table[0], false, refs)
{
        std::copy_n(classic_table(), table_size, my_table);
        my_table[','] = (mask)space;
        my_table['.'] = (mask)space;

}

my_ctype::~my_ctype()
{
    //dtor
}
