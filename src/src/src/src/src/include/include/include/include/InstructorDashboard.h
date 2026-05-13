#ifndef INSTRUCTOR_DASHBOARD_H
#define INSTRUCTOR_DASHBOARD_H

#include "Learner.h"
#include <vector>

class InstructorDashboard {
private:
    std::vector<Learner*> learners;
    
public:
    void addLearner(Learner* learner);
    void showAtRiskLearners(float struggleThreshold = 60.0);
    void showClassProgress();
    void showModulePerformance(std::string moduleName);
    void generateReport();
};

#endif