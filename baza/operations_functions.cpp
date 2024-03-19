#include "classes.h"


ostream& operator << (ostream& s, const vector<list<Operations>::iterator>& v)
{
    for(size_t i = 0; i < v.size(); ++i)
    {
        s << (*v[i]) << '\n';
    }
    return s;
}

string Operations :: create_string() const
{
    string s;
    s += my_account_number + ' ' + std::to_string(money) + ' ' + currency + ' ';
    if(receive) s += '+' + ' ';
    else s += '-' + ' ';
    s += account_number + ' ' + data;
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

