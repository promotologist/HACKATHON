#include "Course.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

// ---------- Constructor ----------
Course::Course(std::string _name, std::string _category, int _difficultyLevel)
    : name(_name), category(_category), difficultyLevel(_difficultyLevel) {}

// ---------- Module management ----------
void Course::addModule(std::string moduleName) {
    modules.push_back(moduleName);
}

void Course::addPrerequisite(std::string module, std::string requiredModule) {
    modulePrerequisites[module].push_back(requiredModule);
}

void Course::setPrerequisites(const std::map<std::string, std::vector<std::string>>& prereqs) {
    modulePrerequisites = prereqs;
}

// ---------- Next modules logic (improved) ----------
std::vector<std::string> Course::getNextModules(const std::vector<std::string>& completedModules) const {
    std::vector<std::string> nextModules;
    for (const auto& module : modules) {
        // Skip already completed
        if (std::find(completedModules.begin(), completedModules.end(), module) != completedModules.end())
            continue;

        // Check prerequisites
        auto it = modulePrerequisites.find(module);
        if (it == modulePrerequisites.end()) {
            // No prerequisites -> ready
            nextModules.push_back(module);
        } else {
            bool allMet = true;
            for (const auto& prereq : it->second) {
                if (std::find(completedModules.begin(), completedModules.end(), prereq) == completedModules.end()) {
                    allMet = false;
                    break;
                }
            }
            if (allMet) {
                nextModules.push_back(module);
            }
        }
    }
    return nextModules;
}

std::vector<std::string> Course::getPrerequisitesForModule(const std::string& module) const {
    auto it = modulePrerequisites.find(module);
    if (it != modulePrerequisites.end())
        return it->second;
    return {};
}

// ---------- CSV Load ----------
std::vector<Course> Course::loadFromCSV(const std::string& filepath) {
    std::vector<Course> courses;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << std::endl;
        return courses;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, category, difficultyStr, modulesStr, prereqStr;

        std::getline(ss, name, ',');
        std::getline(ss, category, ',');
        std::getline(ss, difficultyStr, ',');
        std::getline(ss, modulesStr, ',');
        std::getline(ss, prereqStr, ',');

        int difficulty = std::stoi(difficultyStr);
        Course course(name, category, difficulty);

        // Parse modules (separated by ';')
        std::stringstream moduleStream(modulesStr);
        std::string module;
        while (std::getline(moduleStream, module, ';')) {
            if (!module.empty()) course.addModule(module);
        }

        // Parse prerequisites: format "module1:prereq1,prereq2;module2:prereq3"
        std::stringstream prereqStream(prereqStr);
        std::string pair;
        while (std::getline(prereqStream, pair, ';')) {
            size_t colon = pair.find(':');
            if (colon != std::string::npos) {
                std::string mod = pair.substr(0, colon);
                std::string prereqsList = pair.substr(colon + 1);
                std::stringstream prereqListStream(prereqsList);
                std::string prereq;
                while (std::getline(prereqListStream, prereq, ',')) {
                    if (!prereq.empty()) course.addPrerequisite(mod, prereq);
                }
            }
        }

        courses.push_back(course);
    }
    return courses;
}

// ---------- CSV Save ----------
void Course::saveToCSV(const std::string& filepath) const {
    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << " for writing" << std::endl;
        return;
    }

    // Write header if empty
    file.seekp(0, std::ios::end);
    if (file.tellp() == 0) {
        file << "name,category,difficultyLevel,modules,prerequisites\n";
    }

    // Write modules (separated by ';')
    file << name << "," << category << "," << difficultyLevel << ",";
    for (size_t i = 0; i < modules.size(); ++i) {
        if (i > 0) file << ";";
        file << modules[i];
    }
    file << ",";

    // Write prerequisites: module:prereq1,prereq2;...
    bool firstModule = true;
    for (const auto& module : modules) {
        auto it = modulePrerequisites.find(module);
        if (it != modulePrerequisites.end() && !it->second.empty()) {
            if (!firstModule) file << ";";
            file << module << ":";
            for (size_t i = 0; i < it->second.size(); ++i) {
                if (i > 0) file << ",";
                file << it->second[i];
            }
            firstModule = false;
        }
    }
    file << "\n";
}

// ---------- Getters ----------
std::string Course::getName() const { return name; }
std::string Course::getCategory() const { return category; }
int Course::getDifficultyLevel() const { return difficultyLevel; }
std::vector<std::string> Course::getModules() const { return modules; }
int Course::getTotalModules() const { return static_cast<int>(modules.size()); }
