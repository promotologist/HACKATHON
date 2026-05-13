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
    int skillLevel;
    std::map<std::string, float> moduleScores;
    std::vector<std::string> completedModules;
    std::vector<std::string> strugglingModules;
    float overallProgress;
    std::vector<std::string> enrolledCourses;

public:
    Learner(int _id, std::string _name, std::string _goal, int _skillLevel);

    void updateModuleScore(std::string module, float score);
    void markModuleComplete(std::string module);
    void detectStruggle(float threshold = 60.0f);
    void calculateOverallProgress(int totalModules);
    void enrollInCourse(std::string courseName);
    void saveToCSV();

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getGoal() const;
    int getSkillLevel() const;
    float getOverallProgress() const;
    std::vector<std::string> getStrugglingModules() const;
    std::map<std::string, float> getModuleScores() const;
    std::vector<std::string> getEnrolledCourses() const;

    // Setters
    void setSkillLevel(int level);
};

#endif