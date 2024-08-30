/* 
Assignment 2: Marriage Pact

This assignment asks you to discover your one true love using containers and pointers.
There are two parts: implement get_applicants and find_matches.

Submit to Paperless by 11:59pm on 2/1/2024.
*/

#include <iostream>
#include <string>
#include <queue>
#include <set>
#include <fstream>
#include <sstream>
#include <random>

std::set<std::string> get_applicants(std::string filename) {
    std::set<std::string> applicants;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string firstName, lastName;
            ss >> firstName >> lastName;
            applicants.insert(firstName + " " + lastName);
            std::cout << firstName << " " << lastName << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    return applicants;
}

std::queue<const std::string*> find_matches(std::set<std::string> &students) {
    std::queue<const std::string*> matches;
    char firstInitial = 'Z';
    char lastInitial = 'L';

    for (const std::string& name : students) {
        if (name[0] == firstInitial && name.find(' ') != std::string::npos && name[name.find(' ') + 1] == lastInitial) {
            matches.push(&name); 
        }
    }
    return matches;
}



int main() {
    std::set<std::string> applicants = get_applicants("../students.txt"); 
    std::queue<const std::string*> matches = find_matches(applicants);

    if (matches.empty()) {
        std::cout << "NO STUDENT FOUND" << std::endl;
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, matches.size() - 1);
        int randomIndex = distrib(gen);

        for (int i = 0; i < randomIndex; ++i) {
            matches.pop(); 
        }

        std::cout << "Your one true match is: " << *matches.front() << std::endl;
    }
    return 0;
}
