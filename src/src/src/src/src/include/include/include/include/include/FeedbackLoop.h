#ifndef FEEDBACKLOOP_H
#define FEEDBACKLOOP_H

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <functional>

struct FeedbackEntry {
    std::string learnerId;
    std::string courseId;
    std::string feedbackType;
    std::string message;
    int rating;
    std::chrono::system_clock::time_point timestamp;
    bool resolved;
};

struct PerformanceMetrics {
    double avgScore;
    double completionRate;
    double engagementScore;
    double satisfactionScore;
    int completedModules;
    int timeSpentHours;
};

class FeedbackLoop {
private:
    std::vector<FeedbackEntry> feedbackQueue;
    std::map<std::string, std::vector<FeedbackEntry>> learnerFeedback;
    std::map<std::string, std::vector<PerformanceMetrics>> coursePerformanceHistory;
    
    double criticalFeedbackThreshold;
    int maxFeedbackQueueSize;
    bool autoAdjustEnabled;
    
    std::function<void(const std::string&, const std::string&)> onDifficultyAdjust;
    std::function<void(const std::string&, const std::string&)> onContentRecommendation;
    std::function<void(const std::string&)> onLearnerIntervention;
    
    void processUrgentFeedback();
    void analyzeFeedbackPatterns();
    double calculateCourseSatisfaction(const std::string& courseId);
    std::vector<std::string> extractCommonIssues();
    void triggerAutoAdjustment(const FeedbackEntry& feedback);
    
public:
    FeedbackLoop();
    ~FeedbackLoop();
    
    void submitFeedback(const std::string& learnerId, const std::string& courseId,
                       const std::string& feedbackType, const std::string& message, int rating);
    void processFeedbackQueue();
    void resolveFeedback(const std::string& learnerId, const std::string& courseId);
    
    void updatePerformanceMetrics(const std::string& courseId, const PerformanceMetrics& metrics);
    std::vector<PerformanceMetrics> getCoursePerformanceHistory(const std::string& courseId);
    
    std::map<std::string, double> generateFeedbackReport();
    std::vector<FeedbackEntry> getUnresolvedFeedback();
    double getCourseRating(const std::string& courseId);
    std::vector<std::string> getTopComplaints(int count = 5);
    
    void optimizeCourseContent(const std::string& courseId);
    void suggestInstructorActions();
    void updateRecommendationAlgorithms(const std::map<std::string, double>& feedbackData);
    
    void runQualityCheck();
    void generateContinuousImprovementPlan();
    
    void setAutoAdjustEnabled(bool enabled);
    void setCriticalThreshold(double threshold);
    void registerAdjustmentCallback(std::function<void(const std::string&, const std::string&)> callback);
};

#endif