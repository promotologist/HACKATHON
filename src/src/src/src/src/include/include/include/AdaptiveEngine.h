#ifndef ADAPTIVEENGINE_H
#define ADAPTIVEENGINE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Learner.h"
#include "Course.h"

class AdaptiveEngine {
private:
    std::map<std::string, std::unique_ptr<Learner>> learners;
    std::map<std::string, std::unique_ptr<Course>> courses;
    std::map<std::string, std::vector<std::string>> learnerRecommendations;
    
    double performanceThreshold;
    double engagementWeight;
    double difficultyAdjustmentRate;
    
    double calculateLearnerProficiency(const Learner& learner);
    std::vector<Course*> filterCoursesByLevel(const Learner& learner);
    double calculateCourseMatchScore(const Learner& learner, const Course& course);
    void adjustCourseDifficulty(Course& course, const std::vector<double>& learnerPerformances);
    
public:
    AdaptiveEngine();
    ~AdaptiveEngine();
    
    void registerLearner(std::unique_ptr<Learner> learner);
    void addCourse(std::unique_ptr<Course> course);
    std::vector<std::string> generateLearningPath(const std::string& learnerId);
    std::vector<std::string> recommendNextCourses(const std::string& learnerId, int count = 3);
    
    void updateLearnerProgress(const std::string& learnerId, const std::string& courseId, double score);
    double predictSuccessRate(const std::string& learnerId, const std::string& courseId);
    
    void adjustPacing(const std::string& learnerId, double completionRate);
    void modifyContentDifficulty(const std::string& courseId, const std::vector<double>& studentScores);
    
    std::map<std::string, double> getLearnerAnalytics(const std::string& learnerId);
    std::vector<std::string> getStrugglingLearners(double threshold = 60.0);
    void generateAdaptiveInsights();
    
    void setPerformanceThreshold(double threshold);
    void setEngagementWeight(double weight);
    double getAdaptiveEfficiency() const;
};

#endif