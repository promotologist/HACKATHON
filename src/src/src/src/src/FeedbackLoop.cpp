#include "FeedbackLoop.h"
#include <iostream>

FeedbackLoop::FeedbackLoop(AdaptiveEngine* _engine) : engine(_engine) {}

void FeedbackLoop::processQuizResult(Learner* learner, std::string moduleName, float score, Course* course) {
    std::cout << "\n Processing feedback for " << learner->getName() << "...\n";

    learner->updateModuleScore(moduleName, score);

    if (score >= 70) {
        learner->markModuleComplete(moduleName);
        std::cout << "OK " << moduleName << " completed with score: " << score << "%\n";

        int newLevel = engine->adjustDifficulty(learner);
        if (newLevel > learner->getSkillLevel()) {
            learner->setSkillLevel(newLevel);
            std::cout << " Learner skill level increased to " << newLevel << "\n";
        }
    }
    else {
        std::cout << "WARNING Low score in " << moduleName << " (" << score << "%). Recommending review.\n";
    }

    updateLearningPath(learner, course);
}

void FeedbackLoop::updateLearningPath(Learner* learner, Course* course) {
    std::string next = engine->recommendNextModule(learner, course);

    std::cout << " Next recommendation: " << next << "\n";

    if (next.find("REVIEW") != std::string::npos) {
        std::cout << " Suggestion: Review the module and try again.\n";
    }
    else if (next == "COURSE_COMPLETED") {
        std::cout << " Congratulations! You've completed the course!\n";
    }
    else {
        std::cout << " Proceed to: " << next << "\n";
    }
}

void FeedbackLoop::generateRecommendation(Learner* learner, Course* course) {
    std::cout << "\n Personalized Recommendation for " << learner->getName() << ":\n";
    std::cout << "   Goal: " << learner->getGoal() << "\n";
    std::cout << "   Current Skill Level: " << learner->getSkillLevel() << "/5\n";
    std::cout << "   Progress: " << learner->getOverallProgress() << "%\n";
    std::cout << "   Predicted Success Rate: " << engine->predictSuccessRate(learner, course) << "%\n";

    if (engine->shouldReviewModule(learner, "Python Basics")) {
        std::cout << "   WARNING Needs review: Python Basics\n";
    }
}