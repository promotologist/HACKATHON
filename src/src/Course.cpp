#include "Course.h"
#include <algorithm>

Course::Course(int _id, std::string _name, std::string _category, int _difficultyLevel, int _duration)
    : id(_id), name(_name), category(_category), difficultyLevel(_difficultyLevel), duration(_duration) {
}

void Course::addModule(std::string moduleName) {
    modules.push_back(moduleName);
}

void Course::addPrerequisite(std::string module, int requiredModuleIndex) {
    modulePrerequisites[module] = requiredModuleIndex;
}

int Course::getId() const { return id; }
std::string Course::getName() const { return name; }
std::string Course::getCategory() const { return category; }
int Course::getDifficultyLevel() const { return difficultyLevel; }
int Course::getDuration() const { return duration; }
std::vector<std::string> Course::getModules() const { return modules; }
int Course::getTotalModules() const { return (int)modules.size(); }

std::vector<std::string> Course::getNextModules(std::vector<std::string> completedModules) const {
    std::vector<std::string> nextModules;

    for (const auto& module : modules) {
        if (std::find(completedModules.begin(), completedModules.end(), module) != completedModules.end()) {
            continue;
        }

        auto it = modulePrerequisites.find(module);
        if (it == modulePrerequisites.end()) {
            nextModules.push_back(module);
        }
        else {
            int prereqIndex = it->second;
            if (prereqIndex < (int)completedModules.size()) {
                nextModules.push_back(module);
            }
        }
    }

    return nextModules;
}