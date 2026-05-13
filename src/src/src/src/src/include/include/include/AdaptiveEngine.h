#ifndef ADAPTIVE_ENGINE_H
#define ADAPTIVE_ENGINE_H

#include "Learner.h"
#include "Course.h"
#include <string>
#include <vector>

class AdaptiveEngine {
public:
    AdaptiveEngine();

    std::string recommendNextModule(Learner* learner, Course* course);
    int adjustDifficulty(Learner* learner);
    std::vector<std::string> generatePersonalizedPath(Learner* learner, std::vector<Course*> availableCourses);
    bool shouldReviewModule(Learner* learner, std::string moduleName);
    float predictSuccessRate(Learner* learner, Course* course);
};

#endif