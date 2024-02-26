#include "classes.h"


bool cmp_o(const list<Operations>::iterator& a, const list<Operations>::iterator& b)
{
    return &(*a) < &(*b);
}


bool cmp_o1(const vector<list<Operations>::iterator>* a, const vector<list<Operations>::iterator>* b)
{
    return (*a).size() < (*b).size();
}



size_t Baza :: find_o(vector<list<Operations>::iterator>& v, const string& account_number, double lower_money, double upper_money, const string& currency, const string& lower_data, const string& upper_data)
{
    size_t t = 0;
    vector<list<Operations>::iterator> v_account_number, v_money, v_data, v_currency;
    vector<vector<list<Operations>::iterator>*> vv;
    if(account_number != "-") 
    {
        find_operations(v_account_number, account_number);
        sort(v_account_number.begin(), v_account_number.end(), cmp_o);
        vv.push_back(&v_account_number);
    }
    if(lower_money != std::numeric_limits<double>::infinity() && upper_money != std::numeric_limits<double>::infinity())
    {
        find_money_o(v_money, lower_money, upper_money);
        sort(v_money.begin(), v_money.end(), cmp_o);
        vv.push_back(&v_money);
    }
    if(lower_data != "-" && upper_data != "-")
    {
        find_data(v_data, lower_data, upper_data);
        sort(v_data.begin(), v_data.end(), cmp_o);
        vv.push_back(&v_data);
    }
    if(currency != "-")
    {
        find_currency_o(v_currency, currency);
        sort(v_currency.begin(), v_currency.end(), cmp_o);
        vv.push_back(&v_currency);
    }


    if(vv.size() == 0) return 0;
    sort(vv.begin(), vv.end(), cmp_o1);
    vector<size_t> help_v(vv.size(), 0);
    for(help_v[0] = 0; help_v[0] < (*vv[0]).size();)
    {
        for(int j = 1; j < vv.size(); ++j)
        {
            while(help_v[0] < (*vv[0]).size() && help_v[j] < (*vv[j]).size() && &(*((*vv[0])[help_v[0]])) != &(*((*vv[j])[help_v[j]])))
            {
                if(&(*((*vv[0])[help_v[0]])) < &(*((*vv[j])[help_v[j]]))) ++(help_v[0]);
                else ++help_v[j];
            }

        }
        int j = 1;
        for(j = 0; j < vv.size(); ++j) if(help_v[j] >= ((*vv[j]).size())) break;
        if(j != vv.size()) break;
        for(j = 1; j < vv.size(); ++j)
        {
            if(&(*((*vv[0])[help_v[0]])) != &(*((*vv[j])[help_v[j]]))) break;
        }
        if(j == vv.size())
        {
            v.push_back((*vv[0])[help_v[0]]);
            ++help_v[0];
        }
    }
    return v.size();
}



bool cmp_n(const list<Man>::iterator& a, const list<Man>::iterator& b)
{
    return &(*a) < &(*b);
}


bool cmp_n1(const vector<list<Man>::iterator>* a, const vector<list<Man>::iterator>* b)
{
    return (*a).size() < (*b).size();
}



size_t Baza :: find_n(vector<list<Man>::iterator>& v, const string& name, const string& surname, const string& surname2, double lower_money, double upper_money
       , const string& currency,  const string& account_number)
{
    size_t t = 0;
    vector<list<Man>::iterator> v_name, v_surname, v_surname2, v_money, v_currency;
    vector<vector<list<Man>::iterator>*> vv;
    if(account_number != "-") 
    {
        v.push_back(find_man(account_number));
        return 1;
    }
    if(name != "-")
    {
        find_name(v_name, name);
        sort(v_name.begin(), v_name.end(), cmp_n);
        vv.push_back(&v_name);
    }        
    if(surname != "-") 
    {
        find_surname(v_surname, surname);
        sort(v_surname.begin(), v_surname.end(), cmp_n);
        vv.push_back(&v_surname);
    }
    if(surname2 != "-")
    {
        find_surname2(v_surname2, surname2);
        sort(v_surname2.begin(), v_surname2.end(), cmp_n);
        vv.push_back(&v_surname2);
    }
    if(lower_money != std::numeric_limits<double>::infinity() && upper_money != std::numeric_limits<double>::infinity())
    {
        find_money_n(v_money, lower_money, upper_money);
        sort(v_money.begin(), v_money.end(), cmp_n);
        vv.push_back(&v_money);
    }
    if(currency != "-")
    {
        find_currency_n(v_currency, currency);
        sort(v_currency.begin(), v_currency.end(), cmp_n);
        vv.push_back(&v_currency);
    }
    if(vv.size() == 0) return 0;
    sort(vv.begin(), vv.end(), cmp_n1);
    vector<size_t> help_v(vv.size(), 0);
    for(help_v[0] = 0; help_v[0] < (*vv[0]).size();)
    {
        for(int j = 1; j < vv.size(); ++j)
        {
            while(help_v[0] < (*vv[0]).size() && help_v[j] < (*vv[j]).size() && &(*((*vv[0])[help_v[0]])) != &(*((*vv[j])[help_v[j]])))
            {
                if(&(*((*vv[0])[help_v[0]])) < &(*((*vv[j])[help_v[j]]))) ++(help_v[0]);
                else ++help_v[j];
            }

        }
        int j = 1;
        for(j = 0; j < vv.size(); ++j) if(help_v[j] >= ((*vv[j]).size())) break;
        if(j != vv.size()) break;
        for(j = 1; j < vv.size(); ++j)
        {
            if(&(*((*vv[0])[help_v[0]])) != &(*((*vv[j])[help_v[j]]))) break;
        }
        if(j == vv.size())
        {
            v.push_back((*vv[0])[help_v[0]]);
            ++help_v[0];
        }
    }
    return v.size();
}

const list<Man>::iterator Baza :: find_man(const string& account_number)
{
    return (tree_account_numbers.find(account_number) -> second);
}

size_t Baza :: find_operations(vector<list<Operations>::iterator>& v, const string& account_number)
{
   size_t j = 0;
   for(auto i = tree_op_my_account.lower_bound(account_number); i != tree_op_my_account.upper_bound(account_number); ++i, ++j)
       v.push_back((i -> second));
   return j;
}

size_t Baza :: find_currency_n(vector<list<Man>::iterator>& v, const string& currency)
{
    size_t j = 0;
    for(auto i = tree_currency.lower_bound(currency); i != tree_currency.upper_bound(currency); ++i, ++j)
        v.push_back((i -> second));
    return j;
}

size_t Baza :: find_currency_o(vector<list<Operations>::iterator>& v, const string& currency)
{
    size_t j = 0;
    for(auto i = tree_op_currency.lower_bound(currency); i != tree_op_currency.upper_bound(currency); ++i, ++j)
        v.push_back((i -> second));
    return j;
}



size_t Baza :: find_money_o(vector<list<Operations>::iterator>& v, double lower, double upper)
{
    size_t j = 0;
    multimap<double, list<Operations>::iterator>::iterator a, b;
    if(lower == std::numeric_limits<double>::infinity()) a = tree_op_money.begin();
    else a = tree_op_money.lower_bound(lower);
    if(upper == std::numeric_limits<double>::infinity()) b = tree_op_money.end();
    else b = tree_op_money.upper_bound(upper);
    for(; a != b; ++a, ++j)
        v.push_back((a -> second));
    return j;
}


size_t Baza :: find_money_n(vector<list<Man>::iterator>& v, double lower, double upper)
{
    size_t j = 0;
    multimap<double, list<Man>::iterator>::iterator a, b;
    if(lower == std::numeric_limits<double>::infinity()) a = tree_money.begin();
    else a = tree_money.lower_bound(lower);
    if(upper == std::numeric_limits<double>::infinity()) b = tree_money.end();
    else b = tree_money.upper_bound(upper);
    for(; a != b; ++a, ++j)
        v.push_back((a -> second));
    return j;
};



bool Baza :: add_o(const Operations& op)
{
    baza_operations.push_back(op);
    auto current_iterator = baza_operations.end();
    --current_iterator;
    tree_op_my_account.insert(std::make_pair(op.CMy_account_number(), current_iterator));
    tree_op_money.insert(std::make_pair(std::stod(op.CMoney()), current_iterator));
    tree_op_data.insert(std::make_pair(op.CData(), current_iterator));
    return true;
}




bool Baza :: add_n(const Man& man)
{
    bool insert;
    baza_names.push_back(man);
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
    size_t j = v.size();
    for(size_t i = 0; i < v.size(); ++i)
    {
        tree_account_numbers.erase((*v[i]).CAccount_number());
        for(auto k = tree_names.lower_bound((*v[i]).CName()); k != tree_names.upper_bound((*v[i]).CName()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_names.erase(k);
                break;
            }
        for(auto k = tree_surnames.lower_bound((*v[i]).CSurname()); k != tree_surnames.upper_bound((*v[i]).CSurname()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_surnames.erase(k);
                break;
            }
        for(auto k = tree_surnames2.lower_bound((*v[i]).CSurname2()); k != tree_surnames2.upper_bound((*v[i]).CSurname2()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_surnames2.erase(k);
                break;
            }
        for(auto k = tree_money.lower_bound(std::stod((*v[i]).CMoney())); k != tree_money.upper_bound(std::stod((*v[i]).CMoney())); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_money.erase(k); 
                break;
            }
        for(auto k = tree_currency.lower_bound((*v[i]).CCurrency()); k != tree_currency.upper_bound((*v[i]).CCurrency()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_currency.erase(k);
                break;
            }
        baza_names.erase(v[i]);
    }
    v.clear();
    return j;
}

size_t Baza :: remove_o(vector<list<Operations>::iterator>& v)
{
    size_t j = v.size();
    for(size_t i = 0; i < v.size(); ++i)
    {
        for(auto k = tree_op_my_account.lower_bound((*v[i]).CMy_account_number()); k != tree_op_my_account.upper_bound((*v[i]).CAccount_number()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_op_my_account.erase(k);
                break;
            }
        for(auto k = tree_op_money.lower_bound(std::stod((*v[i]).CMoney())); k != tree_op_money.upper_bound(std::stod((*v[i]).CMoney())); ++k)
            if(&(*(k -> second)) == &(*v[i])) 
            {
                tree_op_money.erase(k);
                break;
            }
        for(auto k = tree_op_data.lower_bound((*v[i]).CData()); k != tree_op_data.upper_bound((*v[i]).Data()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_op_data.erase(k);
                break;
            }
        for(auto k = tree_op_currency.lower_bound((*v[i]).CCurrency()); k != tree_op_currency.upper_bound((*v[i]).CCurrency()); ++k)
            if(&(*(k -> second)) == &(*v[i]))
            {
                tree_op_currency.erase(k);
                break;
            }

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

size_t Baza :: find_surname(vector<list<Man>::iterator>& v, const string& name)
{
    size_t j = 0;
    for(auto i = tree_surnames.lower_bound(name); i != tree_surnames.upper_bound(name); ++i, ++j)
        v.push_back((i -> second));
    return j;
}

size_t Baza :: find_surname2(vector<list<Man>::iterator>& v, const string& name)
{
    size_t j = 0;
    for(auto i = tree_surnames2.lower_bound(name); i != tree_surnames2.upper_bound(name); ++i, ++j)
        v.push_back((i -> second));
    return j;
}

size_t Baza :: find_data(vector<list<Operations>::iterator>& v, const string& lower, const string& upper)
{
    size_t j = 0;
    multimap<string, list<Operations>::iterator>::iterator a, b;
    if(lower == "-") a = tree_op_data.begin();
    else a = tree_op_data.lower_bound(lower);
    if(upper == "-") b = tree_op_data.end();
    else b = tree_op_data.upper_bound(upper);
    for(; a != b; ++a, ++j)
        v.push_back((a -> second));
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
            tree_surnames2.insert(std::make_pair(read_man.CSurname2(), current_iterator));
            tree_money.insert(std::make_pair(std::stod(read_man.CMoney()), current_iterator));
            tree_currency.insert(std::make_pair(read_man.CCurrency(), current_iterator));
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
            tree_op_currency.insert(std::make_pair(read_op.CCurrency(), current_iter));
        }
    }
    file_baza.close();
    cout << "created " << baza_names.size() << " records" << endl;
    cout << "created " << baza_operations.size() << " operations" << endl;
} 
