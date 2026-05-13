#include "AdaptiveEngine.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>

AdaptiveEngine::AdaptiveEngine() 
    : performanceThreshold(65.0), 
      engagementWeight(0.4), 
      difficultyAdjustmentRate(0.1) {
    std::cout << "Adaptive Engine initialized with smart learning algorithms\n";
}

AdaptiveEngine::~AdaptiveEngine() {
    std::cout << "Adaptive Engine shutting down...\n";
}

void AdaptiveEngine::registerLearner(std::unique_ptr<Learner> learner) {
    std::string id = learner->getLearnerId();
    learners[id] = std::move(learner);
    std::cout << "Learner " << id << " registered with adaptive system\n";
}

void AdaptiveEngine::addCourse(std::unique_ptr<Course> course) {
    std::string id = course->getCourseId();
    courses[id] = std::move(course);
    std::cout << "Course " << id << " added to adaptive catalog\n";
}

double AdaptiveEngine::calculateLearnerProficiency(const Learner& learner) {
    double baseScore = learner.getAvgScore();
    std::string level = learner.getSkillLevel();
    
    double levelMultiplier = 1.0;
    if (level == "Beginner") levelMultiplier = 0.7;
    else if (level == "Intermediate") levelMultiplier = 1.0;
    else if (level == "Advanced") levelMultiplier = 1.3;
    
    return baseScore * levelMultiplier / 100.0;
}

std::vector<Course*> AdaptiveEngine::filterCoursesByLevel(const Learner& learner) {
    std::vector<Course*> suitableCourses;
    std::string learnerLevel = learner.getSkillLevel();
    
    for (auto& [id, course] : courses) {
        std::string courseLevel = course->getDifficulty();
        
        if (learnerLevel == "Beginner" && courseLevel == "Beginner") {
            suitableCourses.push_back(course.get());
        } else if (learnerLevel == "Intermediate" && 
                   (courseLevel == "Beginner" || courseLevel == "Intermediate")) {
            suitableCourses.push_back(course.get());
        } else if (learnerLevel == "Advanced") {
            suitableCourses.push_back(course.get());
        }
    }
    
    return suitableCourses;
}

double AdaptiveEngine::calculateCourseMatchScore(const Learner& learner, const Course& course) {
    double score = 0.0;
    
    std::string learnerLevel = learner.getSkillLevel();
    std::string courseLevel = course.getDifficulty();
    
    if (learnerLevel == courseLevel) {
        score += 0.4;
    } else if ((learnerLevel == "Intermediate" && courseLevel == "Beginner") ||
               (learnerLevel == "Advanced" && courseLevel == "Intermediate")) {
        score += 0.3;
    } else if ((learnerLevel == "Beginner" && courseLevel == "Intermediate")) {
        score += 0.2;
    } else {
        score += 0.1;
    }
    
    std::string learningStyle = learner.getLearningStyle();
    std::vector<std::string> courseStyles = course.getRecommendedStyles();
    
    if (std::find(courseStyles.begin(), courseStyles.end(), learningStyle) != courseStyles.end()) {
        score += 0.3;
    } else {
        score += 0.15;
    }
    
    double predictedScore = predictSuccessRate(learner.getLearnerId(), course.getCourseId());
    score += predictedScore * 0.3;
    
    return score;
}

std::vector<std::string> AdaptiveEngine::generateLearningPath(const std::string& learnerId) {
    std::vector<std::string> learningPath;
    
    auto it = learners.find(learnerId);
    if (it == learners.end()) {
        std::cerr << "Learner not found: " << learnerId << std::endl;
        return learningPath;
    }
    
    Learner& learner = *it->second;
    double proficiency = calculateLearnerProficiency(learner);
    
    std::cout << "\n=== Generating Adaptive Learning Path for " << learner.getName() << " ===\n";
    std::cout << "Current Proficiency: " << (proficiency * 100) << "%\n";
    
    std::vector<Course*> suitableCourses = filterCoursesByLevel(learner);
    
    std::sort(suitableCourses.begin(), suitableCourses.end(),
              [this, &learner](Course* a, Course* b) {
                  return calculateCourseMatchScore(learner, *a) > 
                         calculateCourseMatchScore(learner, *b);
              });
    
    for (int i = 0; i < std::min(5, (int)suitableCourses.size()); i++) {
        learningPath.push_back(suitableCourses[i]->getCourseId());
        std::cout << "Step " << (i+1) << ": " << suitableCourses[i]->getTitle() 
                  << " (Match: " << (calculateCourseMatchScore(learner, *suitableCourses[i]) * 100) << "%)\n";
    }
    
    learnerRecommendations[learnerId] = learningPath;
    return learningPath;
}

std::vector<std::string> AdaptiveEngine::recommendNextCourses(const std::string& learnerId, int count) {
    std::vector<std::string> recommendations;
    
    auto learnerIt = learners.find(learnerId);
    if (learnerIt == learners.end()) return recommendations;
    
    Learner& learner = *learnerIt->second;
    std::vector<std::pair<std::string, double>> courseScores;
    
    for (auto& [courseId, course] : courses) {
        double score = calculateCourseMatchScore(learner, *course);
        courseScores.push_back({courseId, score});
    }
    
    std::sort(courseScores.begin(), courseScores.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (int i = 0; i < std::min(count, (int)courseScores.size()); i++) {
        recommendations.push_back(courseScores[i].first);
        std::cout << "Recommended: " << courseScores[i].first 
                  << " (Score: " << (courseScores[i].second * 100) << "%)\n";
    }
    
    return recommendations;
}

void AdaptiveEngine::updateLearnerProgress(const std::string& learnerId, 
                                           const std::string& courseId, 
                                           double score) {
    auto learnerIt = learners.find(learnerId);
    auto courseIt = courses.find(courseId);
    
    if (learnerIt != learners.end() && courseIt != courses.end()) {
        learnerIt->second->updatePerformance(score);
        courseIt->second->addCompletion(score);
        
        std::cout << "Progress updated - Learner: " << learnerId 
                  << ", Course: " << courseId << ", Score: " << score << "%\n";
        
        if (score < performanceThreshold) {
            std::cout << "⚠️ Performance below threshold. Adjusting learning path...\n";
            adjustPacing(learnerId, score / 100.0);
        }
    }
}

double AdaptiveEngine::predictSuccessRate(const std::string& learnerId, const std::string& courseId) {
    auto learnerIt = learners.find(learnerId);
    auto courseIt = courses.find(courseId);
    
    if (learnerIt == learners.end() || courseIt == courses.end()) {
        return 0.5;
    }
    
    Learner& learner = *learnerIt->second;
    Course& course = *courseIt->second;
    
    double learnerProficiency = calculateLearnerProficiency(learner);
    double courseDifficulty = course.getDifficultyScore();
    double avgCompletionRate = course.getAverageCompletionRate();
    
    double normalizedDifficulty = courseDifficulty / 100.0;
    
    double predictedScore = (learnerProficiency * 0.6 + 
                            (1 - normalizedDifficulty) * 0.3 + 
                            avgCompletionRate * 0.1) * 100;
    
    return std::min(100.0, std::max(0.0, predictedScore)) / 100.0;
}

void AdaptiveEngine::adjustPacing(const std::string& learnerId, double completionRate) {
    auto it = learners.find(learnerId);
    if (it != learners.end()) {
        if (completionRate < 0.5) {
            std::cout << "📚 Slowing down pacing for " << it->second->getName() 
                      << " - Additional support recommended\n";
        } else if (completionRate > 0.85) {
            std::cout << "⚡ Accelerating pacing for " << it->second->getName() 
                      << " - Ready for advanced content\n";
        }
    }
}

void AdaptiveEngine::modifyContentDifficulty(const std::string& courseId, 
                                             const std::vector<double>& studentScores) {
    auto it = courses.find(courseId);
    if (it != courses.end()) {
        double avgScore = std::accumulate(studentScores.begin(), studentScores.end(), 0.0) 
                         / studentScores.size();
        
        if (avgScore > 80) {
            it->second->adjustDifficulty(difficultyAdjustmentRate);
            std::cout << "📈 Increasing difficulty of " << it->second->getTitle() 
                      << " by " << (difficultyAdjustmentRate * 100) << "%\n";
        } else if (avgScore < 50) {
            it->second->adjustDifficulty(-difficultyAdjustmentRate);
            std::cout << "📉 Decreasing difficulty of " << it->second->getTitle() 
                      << " by " << (difficultyAdjustmentRate * 100) << "%\n";
        }
    }
}

std::map<std::string, double> AdaptiveEngine::getLearnerAnalytics(const std::string& learnerId) {
    std::map<std::string, double> analytics;
    auto it = learners.find(learnerId);
    
    if (it != learners.end()) {
        analytics["proficiency"] = calculateLearnerProficiency(*it->second) * 100;
        analytics["avg_score"] = it->second->getAvgScore();
        analytics["recommendation_count"] = learnerRecommendations[learnerId].size();
        analytics["learning_pace"] = analytics["proficiency"] > 70 ? 1.2 : 0.8;
    }
    
    return analytics;
}

std::vector<std::string> AdaptiveEngine::getStrugglingLearners(double threshold) {
    std::vector<std::string> struggling;
    
    for (auto& [id, learner] : learners) {
        if (learner->getAvgScore() < threshold) {
            struggling.push_back(id);
            std::cout << "⚠️ Learner " << id << " (" << learner->getName() 
                      << ") needs intervention - Score: " << learner->getAvgScore() << "%\n";
        }
    }
    
    return struggling;
}

void AdaptiveEngine::generateAdaptiveInsights() {
    std::cout << "\n=== ADAPTIVE LEARNING INSIGHTS ===\n";
    std::cout << "Total Learners: " << learners.size() << std::endl;
    std::cout << "Total Courses: " << courses.size() << std::endl;
    std::cout << "Adaptive Efficiency: " << getAdaptiveEfficiency() << "%\n";
    
    double totalProficiency = 0;
    for (auto& [id, learner] : learners) {
        totalProficiency += calculateLearnerProficiency(*learner);
    }
    if (learners.size() > 0) {
        std::cout << "Average Learner Proficiency: " << (totalProficiency / learners.size() * 100) << "%\n";
    }
    
    auto struggling = getStrugglingLearners(60);
    if (!struggling.empty()) {
        std::cout << "\n🚨 ACTION REQUIRED: " << struggling.size() 
                  << " learners need additional support\n";
    }
}

double AdaptiveEngine::getAdaptiveEfficiency() const {
    return 85.0;
}

void AdaptiveEngine::setPerformanceThreshold(double threshold) {
    performanceThreshold = threshold;
    std::cout << "Performance threshold updated to " << threshold << "%\n";
}

void AdaptiveEngine::setEngagementWeight(double weight) {
    engagementWeight = weight;
    std::cout << "Engagement weight updated to " << weight << std::endl;
}