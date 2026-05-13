#include "InstructorDashboard.h"
#include <iostream>
#include <iomanip>

void InstructorDashboard::addLearner(Learner* learner) {
    learners.push_back(learner);
}

void InstructorDashboard::showAtRiskLearners(float struggleThreshold) {
    std::cout << "\n========== AT-RISK LEARNERS ==========\n";
    bool found = false;
    
    for (auto* learner : learners) {
        if (!learner->getStrugglingModules().empty()) {
            found = true;
            std::cout << "⚠️ " << learner->getName() << " (ID: " << learner->getId() << ")\n";
            std::cout << "   Struggling with: ";
            for (const auto& module : learner->getStrugglingModules()) {
                std::cout << module << " ";
            }
            std::cout << "\n   Progress: " << learner->getOverallProgress() << "%\n\n";
        }
    }
    
    if (!found) {
        std::cout << "✅ No at-risk learners found!\n";
    }
}

void InstructorDashboard::showClassProgress() {
    std::cout << "\n========== CLASS PROGRESS ==========\n";
    float totalProgress = 0;
    
    for (auto* learner : learners) {
        std::cout << learner->getName() << ": " 
                  << std::fixed << std::setprecision(1) 
                  << learner->getOverallProgress() << "% complete\n";
        totalProgress += learner->getOverallProgress();
    }
    
    if (!learners.empty()) {
        std::cout << "\n📊 Average Class Progress: " 
                  << totalProgress / learners.size() << "%\n";
    }
}

void InstructorDashboard::showModulePerformance(std::string moduleName) {
    std::cout << "\n========== MODULE PERFORMANCE: " << moduleName << " ==========\n";
    
    for (auto* learner : learners) {
        auto scores = learner->getModuleScores();
        if (scores.find(moduleName) != scores.end()) {
            std::cout << learner->getName() << ": " << scores[moduleName] << "%";
            if (scores[moduleName] < 60) {
                std::cout << " [STRUGGLING]";
            }
            std::cout << "\n";
        }
    }
}

void InstructorDashboard::generateReport() {
    std::cout << "\n========== COMPREHENSIVE REPORT ==========\n";
    showClassProgress();
    showAtRiskLearners();
    std::cout << "\n📋 Recommendation: ";
    
    int atRiskCount = 0;
    for (auto* learner : learners) {
        if (!learner->getStrugglingModules().empty()) atRiskCount++;
    }
    
    if (atRiskCount > (int)learners.size() / 2) {
        std::cout << "High risk detected. Schedule review sessions.\n";
    } else if (atRiskCount > 0) {
        std::cout << "Moderate risk. Provide additional resources to struggling learners.\n";
    } else {
        std::cout << "Class is on track! Keep up the good work.\n";
    }
}