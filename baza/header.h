#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>

void fill_file(std::fstream* baza_file, std::fstream* female_names, std::fstream* female_surnames, std::fstream* female_surnames2,
        std::fstream* male_names, std::fstream* male_surnames, std::fstream* male_surnames2);
void fill_vector(std::fstream* file, std::vector<std::string>& v);

