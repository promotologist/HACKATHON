#include <iostream>
#include <vector>
#include "Learner.h"
#include "Course.h"
#include "AdaptiveEngine.h"
#include "InstructorDashboard.h"
#include "FeedbackLoop.h"

int main() {
    std::cout << "\n========================================\n";
    std::cout << "   ATOMCAMP ADAPTIVE LMS SYSTEM\n";
    std::cout << "   AIHACJATHON 2026\n";
    std::cout << "========================================\n\n";
    
    // ========== CREATE COURSES ==========
    Course dataScience("Data Science Bootcamp", "Data", 3);
    dataScience.addModule("Python Basics");
    dataScience.addModule("Pandas & NumPy");
    dataScience.addModule("Data Visualization");
    dataScience.addModule("Machine Learning Intro");
    
    Course webDev("Web Development Bootcamp", "Web", 2);
    webDev.addModule("HTML/CSS");
    webDev.addModule("JavaScript");
    webDev.addModule("React Basics");
    
    std::vector<Course*> courses = {&dataScience, &webDev};
    
    // ========== CREATE LEARNERS ==========
    Learner alice(1, "Alice", "Data Science", 2);
    Learner bob(2, "Bob", "Data Science", 4);
    Learner charlie(3, "Charlie", "Web Development", 1);
    Learner diana(4, "Diana", "AI/ML", 3);
    Learner eve(5, "Eve", "Data Science", 2);
    
    // ========== ADD SCORES AND PROGRESS ==========
    alice.updateModuleScore("Python Basics", 85);
    alice.markModuleComplete("Python Basics");
    
    bob.updateModuleScore("Python Basics", 95);
    bob.markModuleComplete("Python Basics");
    bob.updateModuleScore("Pandas & NumPy", 45);
    
    charlie.updateModuleScore("HTML/CSS", 40);
    
    eve.updateModuleScore("Python Basics", 75);
    eve.markModuleComplete("Python Basics");
    
    // Calculate overall progress
    alice.calculateOverallProgress(dataScience.getTotalModules());
    bob.calculateOverallProgress(dataScience.getTotalModules());
    charlie.calculateOverallProgress(webDev.getTotalModules());
    diana.calculateOverallProgress(dataScience.getTotalModules());
    eve.calculateOverallProgress(dataScience.getTotalModules());
    
    // ========== ADAPTIVE ENGINE ==========
    AdaptiveEngine engine;
    FeedbackLoop feedback(&engine);
    
    // ========== SHOW PERSONALIZED PATHS ==========
    std::cout << "🎯 PERSONALIZED LEARNING PATHS:\n";
    std::cout << "--------------------------------\n";
    
    std::vector<std::string> path1 = engine.generatePersonalizedPath(&alice, courses);
    std::cout << "Alice's path (Data Science, Level 2): ";
    for (const auto& p : path1) std::cout << p << " ";
    std::cout << "\n";
    
    std::vector<std::string> path2 = engine.generatePersonalizedPath(&charlie, courses);
    std::cout << "Charlie's path (Web Dev, Level 1): ";
    for (const auto& p : path2) std::cout << p << " ";
    std::cout << "\n\n";
    
    // ========== QUIZ FEEDBACK SIMULATION ==========
    std::cout << "📝 QUIZ FEEDBACK SIMULATION:\n";
    std::cout << "----------------------------\n";
    feedback.processQuizResult(&bob, "Machine Learning Intro", 55, &dataScience);
    feedback.processQuizResult(&charlie, "JavaScript", 30, &webDev);
    
    // ========== INSTRUCTOR DASHBOARD ==========
    InstructorDashboard dashboard;
    dashboard.addLearner(&alice);
    dashboard.addLearner(&bob);
    dashboard.addLearner(&charlie);
    dashboard.addLearner(&diana);
    dashboard.addLearner(&eve);
    
    dashboard.generateReport();
    dashboard.showModulePerformance("Python Basics");
    
    // ========== INDIVIDUAL RECOMMENDATIONS ==========
    std::cout << "\n📖 INDIVIDUAL RECOMMENDATIONS:\n";
    std::cout << "-------------------------------\n";
    
    std::cout << "\nFor Bob:\n";
    if (engine.shouldReviewModule(&bob, "Machine Learning Intro")) {
        std::cout << "⚠️ Needs to review: Machine Learning Intro (score: 55%)\n";
    }
    
    std::cout << "\nFor Charlie:\n";
    if (engine.shouldReviewModule(&charlie, "JavaScript")) {
        std::cout << "⚠️ Needs to review: JavaScript (score: 30%)\n";
    }
    
    std::cout << "\n========================================\n";
    std::cout << "   🎉 SYSTEM DEMO COMPLETE 🎉\n";
    std::cout << "   Ready for AIHACJATHON submission!\n";
    std::cout << "========================================\n";
    
    return 0;
}