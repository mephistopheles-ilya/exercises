#include "classes.h"

ostream& operator << (ostream& a, const Man& man)
{
    a << man.name << ' ' << man.surname << ' ' << man.surname2 << ' ' << man.account_number << ' ' << man.money << ' ' << man.currency;
    return a;
}

ostream& operator << (ostream& s, const vector<list<Man>::iterator>& v)
{
    for(size_t i = 0; i < v.size(); ++i)
    {
        s << (*v[i]) << '\n';
    }
    return s;
}


 
