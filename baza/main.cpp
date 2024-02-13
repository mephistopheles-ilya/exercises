#include "header.h"

int main(void)
{
    std::fstream female_names, female_surnames, female_surnames2, male_names, male_surnames, male_surnames2, file_baza;

    female_names.open("female_names", std::ios::in);
    female_surnames.open("female_surnames", std::ios::in);
    female_surnames2.open("female_surnames2", std::ios::in);
    male_names.open("male_names", std::ios::in);
    male_surnames.open("male_surnames", std::ios::in);
    male_surnames2.open("male_surnames2", std::ios::in);
    file_baza.open("file_baza", std::ios::out);

    if(female_names.is_open() && female_surnames.is_open() && female_surnames2.is_open() &&
            male_names.is_open() &&  male_surnames.is_open() &&  male_surnames2.is_open() &&  file_baza.is_open()) std::cout << "All is open" << std::endl; 

    fill_file(&file_baza, &female_names, &female_surnames, &female_surnames2, &male_names, &male_surnames, &male_surnames2);

    file_baza.close();
    male_surnames2.close();
    male_surnames.close();
    male_names.close();
    female_surnames2.close();
    female_surnames.close();
    female_names.close();

    return 0;
}
