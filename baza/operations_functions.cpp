#include "classes.h"


ostream& operator << (ostream& s, const vector<list<Operations>::iterator>& v)
{
    for(size_t i = 0; i < v.size(); ++i)
    {
        s << (*v[i]) << '\n';
    }
    return s;
}

ostream& operator << (ostream& a, const Operations& op)
{
    a << op.my_account_number << ' ' << op.money << ' ' << op.currency << ' ';
    if(op.receive) a << '+' << ' ';
    else a << '-' << ' ';
    a << op.account_number << ' ' << op.data;
    return a;
}

