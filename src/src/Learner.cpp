#include "Learner.h"
#include <algorithm>
#include <fstream>
#include <iostream>

Learner::Learner(int _id, std::string _name, std::string _goal, int _skillLevel)
    : id(_id), name(_name), goal(_goal), skillLevel(_skillLevel), overallProgress(0.0f) {
}

void Learner::updateModuleScore(std::string module, float score) {
    moduleScores[module] = score;
    detectStruggle();
}

void Learner::markModuleComplete(std::string module) {
    if (std::find(completedModules.begin(), completedModules.end(), module) == completedModules.end()) {
        completedModules.push_back(module);
    }
}

void Learner::detectStruggle(float threshold) {
    strugglingModules.clear();
    for (auto& pair : moduleScores) {
        if (pair.second < threshold) {
            strugglingModules.push_back(pair.first);
        }
    }
}

void Learner::calculateOverallProgress(int totalModules) {
    if (totalModules == 0) {
        overallProgress = 0;
        return;
    }
    overallProgress = (float)completedModules.size() / totalModules * 100;
}

void Learner::enrollInCourse(std::string courseName) {
    enrolledCourses.push_back(courseName);
}

void Learner::saveToCSV() {
    std::ofstream file("learners.csv", std::ios::app);
    file << id << "," << name << "," << goal << "," << skillLevel << "\n";
    file.close();
}

int Learner::getId() const { return id; }
std::string Learner::getName() const { return name; }
std::string Learner::getGoal() const { return goal; }
int Learner::getSkillLevel() const { return skillLevel; }
float Learner::getOverallProgress() const { return overallProgress; }
std::vector<std::string> Learner::getStrugglingModules() const { return strugglingModules; }
std::map<std::string, float> Learner::getModuleScores() const { return moduleScores; }
std::vector<std::string> Learner::getEnrolledCourses() const { return enrolledCourses; }
void Learner::setSkillLevel(int level) { skillLevel = level; }