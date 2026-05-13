#ifndef FEEDBACK_LOOP_H
#define FEEDBACK_LOOP_H

#include "Learner.h"
#include "Course.h"
#include "AdaptiveEngine.h"
#include <string>

class FeedbackLoop {
private:
    AdaptiveEngine* engine;

public:
    FeedbackLoop(AdaptiveEngine* _engine);

    void processQuizResult(Learner* learner, std::string moduleName, float score, Course* course);
    void updateLearningPath(Learner* learner, Course* course);
    void generateRecommendation(Learner* learner, Course* course);
};

#endif