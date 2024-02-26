#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdint>

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
using std::sort;

class Man;
class Operations;
class Baza;


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



class Baza
{
private:
    list<Man> baza_names;
    map<string, list<Man>::iterator> tree_account_numbers;
    multimap<string, list<Man>::iterator> tree_names;
    multimap<string, list<Man>::iterator> tree_surnames;
    multimap<string, list<Man>::iterator> tree_surnames2;
    multimap<double, list<Man>::iterator> tree_money;
    multimap<string, list<Man>::iterator> tree_currency;

    list<Operations> baza_operations;
    multimap<string, list<Operations>::iterator> tree_op_my_account; 
    multimap<double, list<Operations>::iterator> tree_op_money;
    multimap<string, list<Operations>::iterator> tree_op_data;
    multimap<string, list<Operations>::iterator> tree_op_currency;

public:
    Baza(const char* file);
    const list<Man>::iterator find_man(const string& account_number);
    size_t find_name(vector<list<Man>::iterator>& v, const string& name);
    size_t find_surname(vector<list<Man>::iterator>& v, const string& name);
    size_t find_surname2(vector<list<Man>::iterator>& v, const string& name);
    size_t find_data(vector<list<Operations>::iterator>& v, const string& lower, const string& upper); 
    size_t find_money_n(vector<list<Man>::iterator>& v, double lower, double upper);
    size_t find_money_o(vector<list<Operations>::iterator>& v, double lower, double upper);
    size_t find_operations(vector<list<Operations>::iterator>& v, const string& account_number);
    size_t find_currency_n(vector<list<Man>::iterator>& v, const string& currency);
    size_t find_currency_o(vector<list<Operations>::iterator>& v, const string& currency);

    size_t remove_n(vector<list<Man>::iterator>& v);
    size_t remove_o(vector<list<Operations>::iterator>& v);
    void print_n(ostream& s = cout, size_t n = 10);
    void print_o(ostream& s = cout, size_t n = 10);
    bool add_n(const Man& man);
    bool add_o(const Operations& op);

    size_t find_n(vector<list<Man>::iterator>& v, const string& name = "-", const string& surname = "-", const string& surname2 = "-",
            double lower_money = std::numeric_limits<double>::infinity(), double upper_money = std::numeric_limits<double>::infinity(), const string& currency = "-", const string& account_number = "-");
    size_t find_o(vector<list<Operations>::iterator>& v, const string& account_number, double lower_money, double upper_money, const string& currency, const string& lower_data, const string& upper_data);


};








