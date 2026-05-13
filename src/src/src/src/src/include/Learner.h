#ifndef LEARNER_H
#define LEARNER_H

#include <string>
#include <vector>
#include <map>

class Learner {
private:
    int id;
    std::string name;
    std::string goal;
    int skillLevel;                             // 1=beginner, 2=intermediate, 3=advanced
    std::map<std::string, float> moduleScores;  // module -> score (0-100)
    std::vector<std::string> completedModules;
    std::vector<std::string> strugglingModules;
    float overallProgress;                      // 0-100

public:
    Learner(int _id, std::string _name, std::string _goal, int _skillLevel);

    void updateModuleScore(std::string module, float score);
    void markModuleComplete(std::string module);
    void detectStruggle(float threshold = 60.0f);
    void calculateOverallProgress(int totalModules);

    // CSV persistence
    static std::vector<Learner> loadFromCSV(const std::string& filepath);
    void saveToCSV(const std::string& filepath) const;  // appends one learner

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getGoal() const;
    int getSkillLevel() const;
    float getOverallProgress() const;
    std::vector<std::string> getStrugglingModules() const;
    std::map<std::string, float> getModuleScores() const;
};

#endif