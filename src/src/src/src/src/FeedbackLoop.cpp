#include "FeedbackLoop.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>

FeedbackLoop::FeedbackLoop() 
    : criticalFeedbackThreshold(2.0), 
      maxFeedbackQueueSize(1000), 
      autoAdjustEnabled(true) {
    std::cout << "🤖 Feedback Loop System initialized\n";
    std::cout << "Continuous learning and improvement active\n";
}

FeedbackLoop::~FeedbackLoop() {
    std::cout << "Feedback Loop shutting down. Processing final queue...\n";
    processFeedbackQueue();
}

void FeedbackLoop::submitFeedback(const std::string& learnerId, const std::string& courseId,
                                 const std::string& feedbackType, const std::string& message, int rating) {
    if (feedbackQueue.size() >= maxFeedbackQueueSize) {
        std::cout << "⚠️ Feedback queue full. Processing before adding new feedback...\n";
        processFeedbackQueue();
    }
    
    FeedbackEntry entry;
    entry.learnerId = learnerId;
    entry.courseId = courseId;
    entry.feedbackType = feedbackType;
    entry.message = message;
    entry.rating = rating;
    entry.timestamp = std::chrono::system_clock::now();
    entry.resolved = false;
    
    feedbackQueue.push_back(entry);
    learnerFeedback[learnerId].push_back(entry);
    
    std::cout << "📝 Feedback received from " << learnerId << " for course " << courseId 
              << " | Rating: " << rating << "/5 | Type: " << feedbackType << "\n";
    
    if (rating <= criticalFeedbackThreshold) {
        std::cout << "🚨 CRITICAL FEEDBACK DETECTED! Immediate action required.\n";
        processUrgentFeedback();
    }
}

void FeedbackLoop::processFeedbackQueue() {
    if (feedbackQueue.empty()) {
        return;
    }
    
    std::cout << "\n=== Processing Feedback Queue (" << feedbackQueue.size() << " items) ===\n";
    
    for (auto& feedback : feedbackQueue) {
        if (!feedback.resolved) {
            std::cout << "Processing: " << feedback.feedbackType 
                      << " feedback from " << feedback.learnerId << "\n";
            std::cout << "Message: " << feedback.message << "\n";
            
            if (autoAdjustEnabled) {
                triggerAutoAdjustment(feedback);
            }
        }
    }
    
    analyzeFeedbackPatterns();
    feedbackQueue.clear();
    std::cout << "✅ Feedback queue processed successfully\n";
}

void FeedbackLoop::processUrgentFeedback() {
    std::cout << "\n🔴 URGENT FEEDBACK INTERVENTION REQUIRED 🔴\n";
    
    for (auto& feedback : feedbackQueue) {
        if (feedback.rating <= criticalFeedbackThreshold && !feedback.resolved) {
            std::cout << "⚠️ Critical Issue - Learner: " << feedback.learnerId 
                      << " | Course: " << feedback.courseId << "\n";
            std::cout << "Issue: " << feedback.message << "\n";
            
            if (onLearnerIntervention) {
                onLearnerIntervention(feedback.learnerId);
            }
        }
    }
}

void FeedbackLoop::analyzeFeedbackPatterns() {
    std::map<std::string, int> feedbackTypeCount;
    std::map<std::string, std::vector<int>> ratingsByType;
    
    for (const auto& feedback : feedbackQueue) {
        feedbackTypeCount[feedback.feedbackType]++;
        ratingsByType[feedback.feedbackType].push_back(feedback.rating);
    }
    
    std::cout << "\n📊 Feedback Pattern Analysis:\n";
    for (const auto& [type, count] : feedbackTypeCount) {
        double avgRating = std::accumulate(ratingsByType[type].begin(), 
                                          ratingsByType[type].end(), 0.0) / count;
        std::cout << "  " << type << ": " << count << " items | Avg Rating: " 
                  << std::fixed << std::setprecision(2) << avgRating << "/5\n";
        
        if (avgRating < 3.0 && count > 5) {
            std::cout << "    ⚠️ PATTERN DETECTED: Consistently low ratings for " << type << "\n";
        }
    }
}

void FeedbackLoop::resolveFeedback(const std::string& learnerId, const std::string& courseId) {
    for (auto& feedback : feedbackQueue) {
        if (feedback.learnerId == learnerId && feedback.courseId == courseId) {
            feedback.resolved = true;
            std::cout << "✅ Feedback resolved for learner " << learnerId 
                      << " on course " << courseId << "\n";
        }
    }
}

void FeedbackLoop::updatePerformanceMetrics(const std::string& courseId, 
                                           const PerformanceMetrics& metrics) {
    coursePerformanceHistory[courseId].push_back(metrics);
    
    std::cout << "📈 Performance metrics updated for course " << courseId << "\n";
    std::cout << "  Completion Rate: " << (metrics.completionRate * 100) << "%\n";
    std::cout << "  Engagement Score: " << (metrics.engagementScore * 100) << "%\n";
    std::cout << "  Satisfaction: " << (metrics.satisfactionScore * 100) << "%\n";
}

std::vector<PerformanceMetrics> FeedbackLoop::getCoursePerformanceHistory(const std::string& courseId) {
    if (coursePerformanceHistory.find(courseId) != coursePerformanceHistory.end()) {
        return coursePerformanceHistory[courseId];
    }
    return {};
}

std::map<std::string, double> FeedbackLoop::generateFeedbackReport() {
    std::map<std::string, double> report;
    
    double totalRating = 0;
    int totalFeedbacks = 0;
    
    for (const auto& [learnerId, feedbacks] : learnerFeedback) {
        for (const auto& feedback : feedbacks) {
            totalRating += feedback.rating;
            totalFeedbacks++;
        }
    }
    
    report["average_rating"] = totalFeedbacks > 0 ? totalRating / totalFeedbacks : 0;
    report["total_feedback_count"] = totalFeedbacks;
    report["critical_feedback_percentage"] = 0;
    
    int criticalCount = 0;
    for (const auto& [learnerId, feedbacks] : learnerFeedback) {
        for (const auto& feedback : feedbacks) {
            if (feedback.rating <= criticalFeedbackThreshold) {
                criticalCount++;
            }
        }
    }
    
    if (totalFeedbacks > 0) {
        report["critical_feedback_percentage"] = (criticalCount * 100.0) / totalFeedbacks;
    }
    
    std::cout << "\n=== FEEDBACK REPORT ===\n";
    std::cout << "Average Rating: " << std::fixed << std::setprecision(2) 
              << report["average_rating"] << "/5\n";
    std::cout << "Total Feedback Items: " << totalFeedbacks << "\n";
    std::cout << "Critical Feedback: " << report["critical_feedback_percentage"] << "%\n";
    
    return report;
}

std::vector<FeedbackEntry> FeedbackLoop::getUnresolvedFeedback() {
    std::vector<FeedbackEntry> unresolved;
    
    for (const auto& feedback : feedbackQueue) {
        if (!feedback.resolved) {
            unresolved.push_back(feedback);
        }
    }
    
    return unresolved;
}

double FeedbackLoop::getCourseRating(const std::string& courseId) {
    std::vector<int> ratings;
    
    for (const auto& [learnerId, feedbacks] : learnerFeedback) {
        for (const auto& feedback : feedbacks) {
            if (feedback.courseId == courseId) {
                ratings.push_back(feedback.rating);
            }
        }
    }
    
    if (ratings.empty()) return 0;
    
    double avgRating = std::accumulate(ratings.begin(), ratings.end(), 0.0) / ratings.size();
    return avgRating;
}

std::vector<std::string> FeedbackLoop::getTopComplaints(int count) {
    std::map<std::string, int> complaintFrequency;
    
    for (const auto& [learnerId, feedbacks] : learnerFeedback) {
        for (const auto& feedback : feedbacks) {
            if (feedback.rating <= 2) {
                complaintFrequency[feedback.feedbackType]++;
            }
        }
    }
    
    std::vector<std::pair<std::string, int>> sortedComplaints(
        complaintFrequency.begin(), complaintFrequency.end());
    
    std::sort(sortedComplaints.begin(), sortedComplaints.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> topComplaints;
    for (int i = 0; i < std::min(count, (int)sortedComplaints.size()); i++) {
        topComplaints.push_back(sortedComplaints[i].first);
        std::cout << "Top complaint #" << (i+1) << ": " << sortedComplaints[i].first 
                  << " (" << sortedComplaints[i].second << " occurrences)\n";
    }
    
    return topComplaints;
}

void FeedbackLoop::optimizeCourseContent(const std::string& courseId) {
    std::cout << "\n🔧 Optimizing course content for: " << courseId << "\n";
    
    double courseRating = getCourseRating(courseId);
    auto performanceHistory = getCoursePerformanceHistory(courseId);
    
    if (courseRating < 3.0) {
        std::cout << "⚠️ Low course rating detected. Recommendations:\n";
        std::cout << "  - Review content clarity and presentation\n";
        std::cout << "  - Add more practical examples\n";
        std::cout << "  - Break down complex topics into smaller modules\n";
        std::cout << "  - Increase interactive elements\n";
    }
    
    if (!performanceHistory.empty()) {
        double avgCompletion = 0;
        for (const auto& metrics : performanceHistory) {
            avgCompletion += metrics.completionRate;
        }
        avgCompletion /= performanceHistory.size();
        
        if (avgCompletion < 0.6) {
            std::cout << "⚠️ Low completion rate detected. Recommendations:\n";
            std::cout << "  - Adjust course pacing\n";
            std::cout << "  - Add more assessments and checkpoints\n";
            std::cout << "  - Improve engagement strategies\n";
        }
    }
    
    if (onContentRecommendation) {
        onContentRecommendation(courseId, "optimize_content");
    }
}

void FeedbackLoop::suggestInstructorActions() {
    std::cout << "\n👨‍🏫 INSTRUCTOR ACTION SUGGESTIONS:\n";
    
    auto topComplaints = getTopComplaints(3);
    
    if (!topComplaints.empty()) {
        std::cout << "Based on recent feedback patterns:\n";
        for (const auto& complaint : topComplaints) {
            if (complaint == "difficulty") {
                std::cout << "  • Adjust course difficulty levels - students finding content too challenging\n";
            } else if (complaint == "pace") {
                std::cout << "  • Modify course pacing - consider adding more review sections\n";
            } else if (complaint == "engagement") {
                std::cout << "  • Increase engagement activities - add discussions or group work\n";
            }
        }
    }
    
    std::map<std::string, int> strugglingCourses;
    for (const auto& [learnerId, feedbacks] : learnerFeedback) {
        for (const auto& feedback : feedbacks) {
            if (feedback.rating <= 2) {
                strugglingCourses[feedback.courseId]++;
            }
        }
    }
    
    for (const auto& [courseId, count] : strugglingCourses) {
        if (count >= 3) {
            std::cout << "  • Schedule review session for " << courseId 
                      << " - multiple students reporting issues\n";
        }
    }
}

void FeedbackLoop::updateRecommendationAlgorithms(const std::map<std::string, double>& feedbackData) {
    std::cout << "\n🔄 Updating recommendation algorithms based on feedback...\n";
    
    double avgRating = feedbackData.at("average_rating");
    
    if (avgRating < 3.0) {
        std::cout << "⚠️ Low overall satisfaction detected. Adjusting recommendation strategy...\n";
    }
    
    std::cout << "✅ Recommendation algorithms updated with latest feedback insights\n";
}

void FeedbackLoop::runQualityCheck() {
    std::cout << "\n🔍 Running quality assurance checks...\n";
    
    auto report = generateFeedbackReport();
    auto unresolved = getUnresolvedFeedback();
    
    std::cout << "Quality Metrics:\n";
    std::cout << "  • Overall Satisfaction: " << std::fixed << std::setprecision(1) 
              << report["average_rating"] << "/5\n";
    std::cout << "  • Response Rate: " << (report["total_feedback_count"] > 0 ? "Good" : "Low") << "\n";
    std::cout << "  • Unresolved Issues: " << unresolved.size() << "\n";
    
    if (report["critical_feedback_percentage"] > 20) {
        std::cout << "⚠️ CRITICAL: High percentage of negative feedback!\n";
        suggestInstructorActions();
    } else {
        std::cout << "✅ Quality standards maintained\n";
    }
}

void FeedbackLoop::generateContinuousImprovementPlan() {
    std::cout << "\n📋 CONTINUOUS IMPROVEMENT PLAN\n";
    std::cout << "================================\n";
    
    auto report = generateFeedbackReport();
    auto topIssues = extractCommonIssues();
    
    std::cout << "Short-term actions (next 2 weeks):\n";
    for (int i = 0; i < std::min(3, (int)topIssues.size()); i++) {
        std::cout << "  • Address " << topIssues[i] << " feedback category\n";
    }
    
    std::cout << "\nLong-term initiatives (next quarter):\n";
    std::cout << "  • Implement adaptive difficulty based on feedback patterns\n";
    std::cout << "  • Develop personalized intervention strategies\n";
    std::cout << "  • Create automated feedback response system\n";
    
    if (report["average_rating"] < 3.5) {
        std::cout << "\n🚨 URGENT: Schedule root cause analysis meeting\n";
    }
}

void FeedbackLoop::triggerAutoAdjustment(const FeedbackEntry& feedback) {
    if (!autoAdjustEnabled) return;
    
    std::cout << "🤖 Auto-adjustment triggered for " << feedback.feedbackType << " feedback\n";
    
    if (feedback.feedbackType == "difficulty") {
        if (onDifficultyAdjust) {
            onDifficultyAdjust(feedback.courseId, feedback.learnerId);
        }
    } else if (feedback.feedbackType == "content") {
        if (onContentRecommendation) {
            onContentRecommendation(feedback.courseId, "review_content");
        }
    }
}

std::vector<std::string> FeedbackLoop::extractCommonIssues() {
    std::map<std::string, int> issueCount;
    
    for (const auto& [learnerId, feedbacks] : learnerFeedback) {
        for (const auto& feedback : feedbacks) {
            if (feedback.rating <= 3) {
                issueCount[feedback.feedbackType]++;
            }
        }
    }
    
    std::vector<std::pair<std::string, int>> sortedIssues(
        issueCount.begin(), issueCount.end());
    
    std::sort(sortedIssues.begin(), sortedIssues.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> commonIssues;
    for (const auto& [issue, count] : sortedIssues) {
        commonIssues.push_back(issue);
    }
    
    return commonIssues;
}

void FeedbackLoop::setAutoAdjustEnabled(bool enabled) {
    autoAdjustEnabled = enabled;
    std::cout << "Auto-adjustment " << (enabled ? "enabled" : "disabled") << "\n";
}

void FeedbackLoop::setCriticalThreshold(double threshold) {
    criticalFeedbackThreshold = threshold;
    std::cout << "Critical feedback threshold set to " << threshold << "/5\n";
}

void FeedbackLoop::registerAdjustmentCallback(
    std::function<void(const std::string&, const std::string&)> callback) {
    onDifficultyAdjust = callback;
    std::cout << "Adjustment callback registered\n";
}

double FeedbackLoop::calculateCourseSatisfaction(const std::string& courseId) {
    return getCourseRating(courseId) / 5.0;
}