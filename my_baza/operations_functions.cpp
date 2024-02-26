#include "classes.h"

Operations& Operations :: operator = (Operations&& op)
{
    my_account_number = move(op.my_account_number);
    money = move(op.money);
    currency = move(op.currency);
    account_number = move(account_number);
    data = move(op.data);
    receive = op.receive;
    return *this;
}

Operations& Operations :: operator = (const Operations& op)
{
    my_account_number = move(op.my_account_number);
    money = op.money;
    currency = op.currency;
    account_number = op.account_number;
    data = op.data;
    receive = op.receive;
    return *this;
}

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

