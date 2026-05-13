#include "AdaptiveEngine.h"
#include <algorithm>
#include <vector>
#include <cmath>

AdaptiveEngine::AdaptiveEngine() {}

std::string AdaptiveEngine::recommendNextModule(Learner* learner, Course* course) {
    std::vector<std::string> completed;
    auto scores = learner->getModuleScores();
    for (auto& pair : scores) {
        completed.push_back(pair.first);
    }

    std::vector<std::string> nextModules = course->getNextModules(completed);

    if (nextModules.empty()) {
        return "COURSE_COMPLETED";
    }

    if (!learner->getStrugglingModules().empty()) {
        return "REVIEW_" + learner->getStrugglingModules()[0];
    }

    return nextModules[0];
}

int AdaptiveEngine::adjustDifficulty(Learner* learner) {
    float avgScore = 0;
    auto scores = learner->getModuleScores();

    if (scores.empty()) return learner->getSkillLevel();

    for (auto& pair : scores) {
        avgScore += pair.second;
    }
    avgScore /= scores.size();

    if (avgScore > 85 && learner->getSkillLevel() < 5) {
        return learner->getSkillLevel() + 1;
    }
    else if (avgScore < 50 && learner->getSkillLevel() > 1) {
        return learner->getSkillLevel() - 1;
    }

    return learner->getSkillLevel();
}

std::vector<std::string> AdaptiveEngine::generatePersonalizedPath(Learner* learner, std::vector<Course*> availableCourses) {
    std::vector<std::string> path;
    std::string goal = learner->getGoal();
    int skillLevel = learner->getSkillLevel();

    for (auto* course : availableCourses) {
        if (course == nullptr) continue;

        std::string courseCategory = course->getCategory();
        int courseDifficulty = course->getDifficultyLevel();

        // Match based on goal
        bool goalMatches = false;

        if (goal == "Data Science") {
            if (courseCategory == "Data") goalMatches = true;
        }
        else if (goal == "AI/ML") {
            if (courseCategory == "AI" || courseCategory == "Data") goalMatches = true;
        }
        else if (goal == "Web Development") {
            if (courseCategory == "Web") goalMatches = true;
        }
        else if (goal == "Data Engineering") {
            if (courseCategory == "Data") goalMatches = true;
        }

        // Also show Programming category courses (Python for Beginners)
        if (courseCategory == "Programming") {
            goalMatches = true;
        }

        // Match skill level (beginner can take level 1-2 courses)
        bool skillMatches = false;
        if (skillLevel == 1 && courseDifficulty <= 2) skillMatches = true;
        else if (skillLevel == 2 && courseDifficulty <= 3) skillMatches = true;
        else if (skillLevel >= 3 && std::abs(courseDifficulty - skillLevel) <= 1) skillMatches = true;

        if (goalMatches && skillMatches) {
            path.push_back(course->getName());
        }
    }

    // If no matches found, recommend beginner-friendly courses
    if (path.empty()) {
        for (auto* course : availableCourses) {
            if (course != nullptr && course->getDifficultyLevel() <= 2) {
                path.push_back(course->getName());
            }
        }
    }

    return path;
}

bool AdaptiveEngine::shouldReviewModule(Learner* learner, std::string moduleName) {
    auto scores = learner->getModuleScores();
    if (scores.find(moduleName) != scores.end()) {
        return scores.at(moduleName) < 60.0f;
    }
    return false;
}

float AdaptiveEngine::predictSuccessRate(Learner* learner, Course* course) {
    float rate = 0.7f;

    if (learner->getSkillLevel() >= course->getDifficultyLevel()) {
        rate += 0.2f;
    }
    else {
        rate -= 0.1f * (course->getDifficultyLevel() - learner->getSkillLevel());
    }

    if (rate > 1.0f) rate = 1.0f;
    if (rate < 0.0f) rate = 0.0f;

    return rate * 100;
}