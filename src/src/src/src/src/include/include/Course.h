#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <map>

class Course {
private:
    std::string name;
    std::string category;           // e.g., "Python", "Data Science"
    int difficultyLevel;            // 1=beginner, 2=intermediate, 3=advanced
    std::vector<std::string> modules;
    // Map: module name -> list of prerequisite module names
    std::map<std::string, std::vector<std::string>> modulePrerequisites;

public:
    Course(std::string _name, std::string _category, int _difficultyLevel);

    void addModule(std::string moduleName);
    void addPrerequisite(std::string module, std::string requiredModule);  // improved!
    void setPrerequisites(const std::map<std::string, std::vector<std::string>>& prereqs);

    // CSV persistence
    static std::vector<Course> loadFromCSV(const std::string& filepath);
    void saveToCSV(const std::string& filepath) const;

    // Getters and helpers
    std::string getName() const;
    std::string getCategory() const;
    int getDifficultyLevel() const;
    std::vector<std::string> getModules() const;
    int getTotalModules() const;
    std::vector<std::string> getNextModules(const std::vector<std::string>& completedModules) const;
    std::vector<std::string> getPrerequisitesForModule(const std::string& module) const;
};

#endif