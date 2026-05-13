#ifndef INSTRUCTOR_DASHBOARD_H
#define INSTRUCTOR_DASHBOARD_H

#include "Learner.h"
#include "Course.h"
#include <vector>

class InstructorDashboard {
private:
    std::vector<Learner*> learners;
    std::vector<Course*> courses;

public:
    void addLearner(Learner* learner);
    void addCourse(Course* course);
    void showAtRiskLearners(float struggleThreshold = 60.0f);
    void showClassProgress();
    void showModulePerformance(std::string moduleName);
    void showCourseAnalytics();
    void generateReport();
    void showAllLearners();
};

#endif