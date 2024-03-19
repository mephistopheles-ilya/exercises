#include "classes.h"

ostream& operator << (ostream& a, const Man& man)
{
    a << man.name << ' ' << man.surname << ' ' << man.surname2 << ' ' << man.account_number << ' ' << man.money << ' ' << man.currency;
    return a;
}

string Man :: create_string() const
{
    string s;
    s += name + ' ' + surname + ' ' + surname2 + ' ' + account_number + ' ' + std::to_string(money) + ' ' + currency;
    return s;
}

ostream& operator << (ostream& s, const vector<list<Man>::iterator>& v)
{
    for(size_t i = 0; i < v.size(); ++i)
    {
        s << (*v[i]) << '\n';
    }
    return s;
}


 
