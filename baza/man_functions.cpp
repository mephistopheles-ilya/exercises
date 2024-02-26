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

Man& Man :: operator = (const Man& man)
{
    name = man.name;
    surname = man.surname;
    surname2 = man.surname2;
    account_number = man.account_number;
    money = man.money;
    currency = man.currency;
    return *this;
}

Man& Man :: operator = (Man&& man)
{
    name = move(man.name);
    surname = move(man.surname);
    surname2 = move(man.surname2);
    account_number = move(man.account_number);
    money = move(man.money);
    currency = move(man.currency);
    return *this;
}

 
