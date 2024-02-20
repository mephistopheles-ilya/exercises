#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <list>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::forward;
using std::move;
using std::list;
using std::map;
using std::vector;
using std::multimap;
using std::fstream;
using std::ostream;



class Man
{
private:
    string name;
    string surname;
    string surname2;
    string account_number;
    string money;
    string currency;
public:
    Man() = default;
    Man(const string& name_, const string& surname_, const string& surname2_, const string& account_number_, const string& money_, const string& currency_):
        name(name_), surname(surname_), surname2(surname2_), account_number(account_number_), money(money_), currency(currency_) {} 
    Man(string&& name_, string&& surname_, string&& surname2_, string&& account_number_, string&& money_, string&& currency_):
        name(forward<string>(name_)), surname(forward<string>(surname_)), surname2(forward<string>(surname2_)), account_number(forward<string>(account_number_)),
        money(forward<string>(money_)), currency(forward<string>(currency_)) {}
    Man(const Man& man): name(man.name), surname(man.surname), surname2(man.surname2), account_number(man.account_number), money(man.money), currency(man.currency) {}
    Man(Man&& man): name(move(man.name)), surname(move(man.surname)), surname2(move(man.surname2)), account_number(move(man.account_number)), money(move(man.money)),
        currency(move(man.currency)) {}
    Man& operator = (const Man& man);
    Man& operator = (Man&& man);
    string& Name() { return name;}
    string& Surname() { return surname;}
    string& Surname2() { return surname2;}
    string& Account_number() { return account_number;}
    string& Money() { return money;}
    string& Currency() { return currency;}
    const string& CName() const { return name;}
    const string& CSurname() const  { return surname;}
    const string& CSurname2() const  { return surname2;}
    const string& CAccount_number() const { return account_number;}
    const string& CMoney() const { return money;}
    const string& CCurrency() const  { return currency;}
 
    friend ostream& operator << (ostream& a, const Man& man);
};

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

class Operations
{
private:
    string my_account_number;
    string money;
    string currency;
    string account_number;
    string data;
    bool receive;
public:
    Operations() = default;
    Operations(const Operations& op): my_account_number(op.my_account_number), money(op.money),
    currency(op.currency), account_number(op.account_number), data(op.data), receive(op.receive) {}
    Operations(Operations&& op): my_account_number(move(op.my_account_number)),
    money(move(op.money)), currency(move(op.currency)),
    account_number(move(op.account_number)), data(move(op.data)), receive(move(op.receive)) {}
    Operations& operator = (const Operations& op);
    Operations& operator = (Operations&& op);
    string& My_account_number() {return my_account_number;}
    string& Money() { return money;}
    string& Currency() { return currency;}
    string& Account_number() { return account_number;}
    string& Data() { return data;}
    bool& Receive() { return receive;}
    const string& CMy_account_number() const { return my_account_number;}
    const string& CMoney() const { return money;}
    const string& CCurrency() const { return currency;}
    const string& CAccount_number() const  { return account_number;}
    const string& CData() const { return data;}
    const bool& CReceive() const  { return receive;}
    
    friend ostream& operator << (ostream& s, const Operations& op);
};



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

class Baza
{
private:
    list<Man> baza_names;
    map<string, list<Man>::iterator> tree_account_numbers;
    multimap<string, list<Man>::iterator> tree_names;
    multimap<string, list<Man>::iterator> tree_surnames;
    multimap<string, list<Man>::iterator> tree_surnames2;
    multimap<double, list<Man>::iterator> tree_money;

    list<Operations> baza_operations;
    multimap<string, list<Operations>::iterator> tree_op_my_account; 
    multimap<double, list<Operations>::iterator> tree_op_money;
    multimap<string, list<Operations>::iterator> tree_op_data;

public:
    Baza(const char* file);
    size_t find_name(vector<list<Man>::iterator>& v, const string& name);
    size_t find_data(vector<list<Operations>::iterator>& v, const string& lower, const string& upper); 
    size_t find_money(vector<list<Man>::iterator>& v, double lower, double upper);

    size_t remove_n(vector<list<Man>::iterator>& v);
    size_t remove_o(vector<list<Operations>::iterator>& v);
    void print_n(ostream& s = cout, size_t n = 10);
    void print_o(ostream& s = cout, size_t n = 10);
    bool add(const Man& man);
    size_t add_file(const char* file);
    size_t write(const char* file);


};


size_t Baza :: find_money(vector<list<Man>::iterator>& v, double lower, double upper)
{
    size_t j = 0;
    for(auto i = tree_money.lower_bound(lower); i != tree_money.upper_bound(upper); ++i, ++j)
        v.push_back((i -> second));
    return j;
};

size_t Baza :: write(const char* file)
{
    fstream file_baza;
    file_baza.open(file, std::ios::out);
    for(auto i = baza_names.cbegin(); i != baza_names.cend(); ++i)
    {
        file_baza << (*i).CName() << ' ' << (*i).CSurname() << ' ' << (*i).CSurname2() << ' '
            << (*i).CAccount_number() << ' ' << (*i).CMoney() << ' ' << (*i).CCurrency() << '\n';
    }
    return baza_names.size();
}

size_t Baza :: add_file(const char* file)
{
    size_t amount = 0;
    fstream file_baza;
    file_baza.open(file, std::ios::in);
    string read_line;
    Man read_man;
    bool insert;
    for(; file_baza >> read_line;)
    {
        read_man.Name() = move(read_line);
        file_baza >> read_line;
        read_man.Surname() = move(read_line);
        file_baza >> read_line;
        read_man.Surname2() = move(read_line);
        file_baza >> read_line;
        read_man.Account_number() = move(read_line);
        file_baza >> read_line;
        read_man.Money() = move(read_line);
        file_baza >> read_line;
        read_man.Currency() = move(read_line);
        file_baza >> read_line;
        baza_names.push_back(read_man);
        auto current_iterator = baza_names.end();
        --current_iterator;
        insert = (tree_account_numbers.insert(std::make_pair(read_man.CAccount_number(), current_iterator))).second;
        if(insert)
        {
            tree_names.insert(std::make_pair(read_man.CName(), current_iterator));
            tree_surnames.insert(std::make_pair(read_man.CSurname(), current_iterator));
            tree_surnames2.insert(std::make_pair(read_man.CSurname(), current_iterator));
            tree_money.insert(std::make_pair(std::stod(read_man.CMoney()), current_iterator));
            ++amount;
        }else
        {
            baza_names.pop_back();
        }
    }
    file_baza.close();
    return amount;
}

bool Baza :: add(const Man& man)
{
    bool insert;
    auto current_iterator = baza_names.end();
    --current_iterator;
    insert = (tree_account_numbers.insert(std::make_pair(man.CAccount_number(), current_iterator))).second;
    if(insert)
    {
        tree_names.insert(std::make_pair(man.CName(), current_iterator));
        tree_surnames.insert(std::make_pair(man.CSurname(), current_iterator));
        tree_surnames2.insert(std::make_pair(man.CSurname(), current_iterator));
        tree_money.insert(std::make_pair(std::stod(man.CMoney()), current_iterator));
        return true;
    }
    baza_names.pop_back();
    return false;
}




void Baza :: print_n(ostream& s, size_t n)
{
    size_t end = std::min(baza_names.size(), n), j = 0;
    for(auto i = baza_names.cbegin(); j < end; ++j, ++i) s << (*i) << '\n';
}

void Baza :: print_o(ostream& s, size_t n)
{
    size_t end = std::min(baza_operations.size(), n), j = 0;
    for(auto i = baza_operations.cbegin(); j < end; ++j, ++i) s << (*i) << '\n';
}

size_t Baza :: remove_n(vector<list<Man>::iterator>& v)
{
    size_t j = 0;
    for(size_t i = 0; i < v.size(); ++i)
    {
        j += tree_account_numbers.erase((*v[i]).CAccount_number());
        j += tree_names.erase((*v[i]).CName());
        j += tree_surnames.erase((*v[i]).CSurname());
        j += tree_surnames2.erase((*v[i]).CSurname2());
        j += tree_money.erase(std::stod((*v[i]).CMoney()));
        baza_names.erase(v[i]);
    }
    v.clear();
    return j;
}

size_t Baza :: remove_o(vector<list<Operations>::iterator>& v)
{
    size_t j = 0;
    for(size_t i = 0; i < v.size(); ++i)
    {
        j += tree_op_my_account.erase((*v[i]).CMy_account_number());
        j += tree_op_money.erase(std::stod((*v[i]).CMoney()));
        j += tree_op_data.erase((*v[i]).CMoney());
        baza_operations.erase(v[i]);
    }
    v.clear();
    return j;
}


size_t Baza :: find_name(vector<list<Man>::iterator>& v, const string& name)
{
    size_t j = 0;
    for(auto i = tree_names.lower_bound(name); i != tree_names.upper_bound(name); ++i, ++j)
        v.push_back((i -> second));
    return j;
}

size_t Baza :: find_data(vector<list<Operations>::iterator>& v, const string& lower, const string& upper)
{
    size_t j = 0;
    for(auto i = tree_op_data.lower_bound(lower); i != tree_op_data.upper_bound(upper); ++i, ++j)
        v.push_back((i -> second));
    return j;
}


Baza :: Baza(const char* file)
{  
    fstream file_baza;
    file_baza.open(file, std::ios::in);
    string read_line;
    Man read_man;
    Operations read_op;
    bool insert;
    for(; file_baza >> read_line;)
    {
        read_man.Name() = move(read_line);
        file_baza >> read_line;
        read_man.Surname() = move(read_line);
        file_baza >> read_line;
        read_man.Surname2() = move(read_line);
        file_baza >> read_line;
        read_man.Account_number() = move(read_line);
        file_baza >> read_line;
        read_man.Money() = move(read_line);
        file_baza >> read_line;
        read_man.Currency() = move(read_line);
        file_baza >> read_line;
        baza_names.push_back(read_man);
        auto current_iterator = baza_names.end();
        --current_iterator;
        insert = (tree_account_numbers.insert(std::make_pair(read_man.CAccount_number(), current_iterator))).second;
        if(insert)
        {
            tree_names.insert(std::make_pair(read_man.CName(), current_iterator));
            tree_surnames.insert(std::make_pair(read_man.CSurname(), current_iterator));
            tree_surnames2.insert(std::make_pair(read_man.CSurname(), current_iterator));
            tree_money.insert(std::make_pair(std::stod(read_man.CMoney()), current_iterator));
        }else
        {
            baza_names.pop_back();
        }
        while(read_line.back() != '#')
        {
            read_op.My_account_number() = read_man.CAccount_number();
            file_baza >> read_line;
            read_op.Money() = move(read_line);
            file_baza >> read_line;
            read_op.Currency() = move(read_line);
            file_baza >> read_line;
            if(read_line[0] == '+') read_op.Receive() = true;
            else read_op.Receive() = false;
            file_baza >> read_line;
            read_op.Account_number() = move(read_line);
            file_baza >> read_line;
            read_op.Data() = move(read_line);
            file_baza >> read_line;
            baza_operations.push_back(read_op);
            auto current_iter = baza_operations.end();
            --current_iter;
            tree_op_my_account.insert(std::make_pair(read_man.CAccount_number(), current_iter));
            tree_op_data.insert(std::make_pair(read_op.CData(), current_iter));
            tree_op_money.insert(std::make_pair(std::stod(read_op.CMoney()), current_iter));
        }
    }
    file_baza.close();
    cout << "created " << baza_names.size() << " records" << endl;
    cout << "created " << baza_operations.size() << " operations" << endl;
} 








