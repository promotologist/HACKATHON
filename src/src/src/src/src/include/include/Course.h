#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <map>

class Course {
private:
    int id;
    std::string name;
    std::string category;
    int difficultyLevel;
    int duration; // in months
    std::vector<std::string> modules;
    std::map<std::string, int> modulePrerequisites;

public:
    Course(int _id, std::string _name, std::string _category, int _difficultyLevel, int _duration);

    void addModule(std::string moduleName);
    void addPrerequisite(std::string module, int requiredModuleIndex);

    int getId() const;
    std::string getName() const;
    std::string getCategory() const;
    int getDifficultyLevel() const;
    int getDuration() const;
    std::vector<std::string> getModules() const;
    std::vector<std::string> getNextModules(std::vector<std::string> completedModules) const;
    int getTotalModules() const;
};

#endif