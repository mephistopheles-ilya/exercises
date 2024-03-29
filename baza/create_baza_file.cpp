#include "header.h"


void fill_vector(std::fstream* file, std::vector<std::string>& v)
{
    std::string line;
    while((*file) >> line)
    {
        v.push_back(line);
    }
}

void fill_file(std::fstream* file_currency, std::fstream* baza_file, std::fstream* female_names, std::fstream* female_surnames, std::fstream* female_surnames2,
        std::fstream* male_names, std::fstream* male_surnames, std::fstream* male_surnames2)
{
    std::vector<std::string> fn, fs, fs2, mn, ms, ms2;
    fill_vector(female_names, fn);
    fill_vector(female_surnames, fs);
    fill_vector(female_surnames2, fs2);
    fill_vector(male_names, mn);
    fill_vector(male_surnames, ms);
    fill_vector(male_surnames2, ms2);
    std::vector<std::string> currency{"RUB", "USD", "EUR", "AUD", "BYN", "BGN", "CAD", "NOK", "SEK" };
    std::string sgn = "-+";

    std::srand(std::time(nullptr));
    *file_currency << std::setprecision(2);
    *file_currency << std::fixed;
    std::string current_currency, next_currency;
    for(size_t  i = 0; i < currency.size(); ++i)
    {
        current_currency = currency[i];
        for(size_t j = i; j < currency.size(); ++j)
        {
            double cur = 0.1 +  static_cast<double>(rand()%3) + static_cast<double>(rand())/RAND_MAX;
            next_currency = currency[j];
            if(i == j) cur = 1;
            *file_currency << current_currency << ' ' << "to" << ' '  << next_currency << ' ' << cur << '\n';
            *file_currency << next_currency << ' ' << "to" << ' '  << current_currency << ' ' << 1/cur << '\n';
        }
    }



    int fn_size = fn.size(), fs_size = fs.size(), fs2_size = fs2.size()/*, mn_size = mn.size(), ms_size = ms.size(), ms2_size = ms2.size()*/;
    int currency_size = currency.size();
    *baza_file << std::setprecision(2);
    *baza_file << std::fixed;
    for(int i = 0; i < 100'000; ++i)
    {
        int gender = rand()%2;
        //int gender = 1;
        if(gender == 0)
        {
            int name = rand()%(fn_size);
            int surname = rand()%(fs_size);
            int surname2 = rand()%(fs2_size);
            *baza_file << fn[name] << ' ' << fs[surname] << ' ' << fs2[surname2];
        }else
        {
            /*int name = rand()%(mn_size);
            int surname = rand()%(ms_size);
            int surname2 = rand()%(ms2_size);*/
            int name  = rand()%(10);
            int surname = rand()%(10);
            int surname2 = rand()%(10);
            *baza_file << mn[name] << ' ' << ms[surname] << ' ' << ms2[surname2];
        }
        *baza_file << ' ' << (1'000'000 + rand()%9'000'000) << (1'000'000 + rand()%9'000'000) << (1'000'00 + rand()%9'000'00)
                << ' ' <<  static_cast<double>(rand()%100) + (static_cast<double>(rand())/RAND_MAX) * static_cast<double>(rand()%10000) << 
                ' ' << currency[rand()%(currency_size)] << " ;";
       for(int j = 0, k = 1 + rand()%5; j < k; ++j)
        {
            int month = 1 + rand()%12, year = 2020 + rand()%4, hour = rand()%24, minute = rand()%60, second = rand()%60, day;
            if(((month <= 7) && (month%2 == 1)) || ((month > 7) && (month%2 == 0))) day = 1 + rand()%31;
            else if(month != 2) day = 1 + rand()%30;
            else if(year%4 == 0) day = 1 + rand()%29;
            else day = 1 + rand()%28;
            *baza_file << ' ' <<  static_cast<double>(rand()%100) + (static_cast<double>(rand())/RAND_MAX) * static_cast<double>(rand()%10000) << ' ' <<
                currency[rand()%(currency_size)] << ' ' << sgn[rand()%2] << ' ' << (1'000'000 + rand()%9'000'000) << (1'000'000 + rand()%9'000'000) << (1'000'00 + rand()%9'000'00)
                << ' ' << year << '/';
                if(month < 10) *baza_file << 0;
                *baza_file << month << '/';
                if(day < 10) *baza_file << 0;
                *baza_file << day << '/';
                if(hour < 10) *baza_file << 0;
                *baza_file << hour << '/';
                if(minute < 10) *baza_file << 0;
                *baza_file << minute << '/';
                if(second < 10) *baza_file << 0;
                *baza_file << second << " ;";
       }
       *baza_file << '#' << '\n';
 
    }
}


