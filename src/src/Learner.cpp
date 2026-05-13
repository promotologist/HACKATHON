#include "Learner.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

Learner::Learner(int _id, std::string _name, std::string _goal, int _skillLevel)
    : id(_id), name(_name), goal(_goal), skillLevel(_skillLevel), overallProgress(0.0f) {}

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
    for (const auto& pair : moduleScores) {
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
    overallProgress = (static_cast<float>(completedModules.size()) / totalModules) * 100.0f;
}

// ========== CSV LOAD ==========
std::vector<Learner> Learner::loadFromCSV(const std::string& filepath) {
    std::vector<Learner> learners;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << filepath << std::endl;
        return learners;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        // Columns: id,name,goal,skillLevel,completedModules,moduleScores
        int id, skill;
        std::string name, goal;
        std::getline(ss, token, ','); id = std::stoi(token);
        std::getline(ss, name, ',');
        std::getline(ss, goal, ',');
        std::getline(ss, token, ','); skill = std::stoi(token);

        Learner learner(id, name, goal, skill);

        // Read completed modules (separated by ';')
        if (std::getline(ss, token, ',')) {
            std::stringstream modStream(token);
            std::string mod;
            while (std::getline(modStream, mod, ';')) {
                if (!mod.empty()) learner.completedModules.push_back(mod);
            }
        }

        // Read module scores (format: module:score;module:score...)
        if (std::getline(ss, token, ',')) {
            std::stringstream scoreStream(token);
            std::string pair;
            while (std::getline(scoreStream, pair, ';')) {
                size_t colon = pair.find(':');
                if (colon != std::string::npos) {
                    std::string mod = pair.substr(0, colon);
                    float score = std::stof(pair.substr(colon + 1));
                    learner.moduleScores[mod] = score;
                }
            }
        }

        learners.push_back(learner);
    }
    return learners;
}

// ========== CSV SAVE (appends one learner) ==========
void Learner::saveToCSV(const std::string& filepath) const {
    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << filepath << " for writing" << std::endl;
        return;
    }

    file.seekp(0, std::ios::end);
    if (file.tellp() == 0) {
        file << "id,name,goal,skillLevel,completedModules,moduleScores\n";
    }

    file << id << "," << name << "," << goal << "," << skillLevel << ",";

    // Completed modules (; separated)
    for (size_t i = 0; i < completedModules.size(); ++i) {
        if (i > 0) file << ";";
        file << completedModules[i];
    }
    file << ",";

    // Module scores (module:score;...)
    bool first = true;
    for (const auto& pair : moduleScores) {
        if (!first) file << ";";
        file << pair.first << ":" << pair.second;
        first = false;
    }
    file << "\n";
}

// ========== GETTERS ==========
int Learner::getId() const { return id; }
std::string Learner::getName() const { return name; }
std::string Learner::getGoal() const { return goal; }
int Learner::getSkillLevel() const { return skillLevel; }
float Learner::getOverallProgress() const { return overallProgress; }
std::vector<std::string> Learner::getStrugglingModules() const { return strugglingModules; }
std::map<std::string, float> Learner::getModuleScores() const { return moduleScores; }