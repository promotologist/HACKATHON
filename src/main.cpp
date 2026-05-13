#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include "Learner.h"
#include "Course.h"
#include "AdaptiveEngine.h"
#include "InstructorDashboard.h"
#include "FeedbackLoop.h"
#include "LecturerDashboard.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void showMenu() {
    std::cout << "\n========================================\n";
    std::cout << "   ATOMCAMP ADAPTIVE LMS\n";
    std::cout << "========================================\n";
    std::cout << "1. New Learner Onboarding\n";
    std::cout << "2. Continue as Learner\n";
    std::cout << "3. Instructor Dashboard\n";
    std::cout << "4. ADMIN\n";
    std::cout << "5. Exit\n";
    std::cout << "Choice: ";
}

void showAvailableModules(Course* course, Learner* learner) {
    std::cout << "\n========== AVAILABLE MODULES ==========\n";
    std::vector<std::string> modules = course->getModules();
    auto completedScores = learner->getModuleScores();

    int optionNumber = 1;
    for (const auto& module : modules) {
        bool isCompleted = false;
        for (const auto& completed : completedScores) {
            if (completed.first == module) {
                isCompleted = true;
                break;
            }
        }

        if (isCompleted) {
            std::cout << optionNumber << ". " << module << " [ALREADY COMPLETED]\n";
        }
        else {
            std::cout << optionNumber << ". " << module << " [NOT TAKEN YET]\n";
        }
        optionNumber++;
    }
    std::cout << "0. Go Back\n";
}

int getValidModuleChoice(int maxChoice) {
    int choice;
    while (true) {
        std::cout << "\nSelect module (1-" << maxChoice << ") or 0 to go back: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            clearInput();
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice >= 0 && choice <= maxChoice) {
            break;
        }
        std::cout << "Invalid choice! Please enter between 0 and " << maxChoice << "\n";
    }
    return choice;
}

float getValidScore() {
    float score;
    while (true) {
        std::cout << "Enter your score (0-100): ";
        std::cin >> score;

        if (std::cin.fail()) {
            std::cin.clear();
            clearInput();
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (score >= 0 && score <= 100) {
            break;
        }
        std::cout << "Invalid score! Please enter between 0 and 100.\n";
    }
    return score;
}

Learner* onboardLearner(int& nextId, std::vector<Course*>& courses) {
    std::string name, goal;
    int skillLevel;

    std::cout << "\n========== WELCOME TO ATOMCAMP! ==========\n";
    std::cout << "Let's personalize your learning journey.\n\n";

    std::cout << "What is your name? ";
    clearInput();
    std::getline(std::cin, name);

    std::cout << "\nWhat is your career goal?\n";
    std::cout << "1. Data Science\n";
    std::cout << "2. AI/ML\n";
    std::cout << "3. Web Development\n";
    std::cout << "4. Data Engineering\n";
    std::cout << "Choice: ";
    int goalChoice;
    std::cin >> goalChoice;

    switch (goalChoice) {
    case 1: goal = "Data Science"; break;
    case 2: goal = "AI/ML"; break;
    case 3: goal = "Web Development"; break;
    case 4: goal = "Data Engineering"; break;
    default: goal = "Data Science"; break;
    }

    std::cout << "\nRate your current skill level (1=Beginner to 5=Expert): ";
    std::cin >> skillLevel;
    if (skillLevel < 1) skillLevel = 1;
    if (skillLevel > 5) skillLevel = 5;

    Learner* newLearner = new Learner(nextId++, name, goal, skillLevel);

    AdaptiveEngine engine;
    std::vector<std::string> recommended = engine.generatePersonalizedPath(newLearner, courses);

    std::cout << "\n========== YOUR PERSONALIZED LEARNING PATH ==========\n";
    std::cout << "Based on your goal (" << goal << ") and skill level (" << skillLevel << "/5):\n";

    if (recommended.empty()) {
        std::cout << "No specific courses match your criteria.\n";
        std::cout << "Showing all available courses:\n";
        for (auto* course : courses) {
            if (course != nullptr) {
                std::cout << "   - " << course->getName() << " (Level: " << course->getDifficultyLevel() << "/5)\n";
                recommended.push_back(course->getName());
            }
        }
    }
    else {
        for (size_t i = 0; i < recommended.size(); i++) {
            std::cout << "   " << (i + 1) << ". " << recommended[i] << "\n";
        }
    }

    // Let student select courses one by one
    std::cout << "\n========== COURSE SELECTION ==========\n";
    std::vector<std::string> selectedCourses;

    for (size_t i = 0; i < recommended.size(); i++) {
        std::cout << "\nDo you want to enroll in \"" << recommended[i] << "\"? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            selectedCourses.push_back(recommended[i]);
            std::cout << "   [ENROLLED] " << recommended[i] << "\n";
        }
        else {
            std::cout << "   [SKIPPED] " << recommended[i] << "\n";
        }
    }

    // Ask if they want to see more courses not in recommendations
    if (selectedCourses.empty()) {
        std::cout << "\nYou haven't selected any courses. Do you want to see all available courses? (y/n): ";
        char seeAll;
        std::cin >> seeAll;

        if (seeAll == 'y' || seeAll == 'Y') {
            std::cout << "\n========== ALL AVAILABLE COURSES ==========\n";
            for (size_t i = 0; i < courses.size(); i++) {
                if (courses[i] != nullptr) {
                    bool alreadyRecommended = false;
                    for (const auto& rec : recommended) {
                        if (rec == courses[i]->getName()) {
                            alreadyRecommended = true;
                            break;
                        }
                    }
                    if (!alreadyRecommended) {
                        std::cout << (i + 1) << ". " << courses[i]->getName()
                            << " (Level: " << courses[i]->getDifficultyLevel() << "/5)\n";
                    }
                }
            }

            std::cout << "\nEnter course number to enroll (0 to finish): ";
            while (true) {
                int courseNum;
                std::cin >> courseNum;
                if (courseNum == 0) break;
                if (courseNum >= 1 && courseNum <= (int)courses.size()) {
                    std::string courseName = courses[courseNum - 1]->getName();
                    // Check if already selected
                    bool alreadySelected = false;
                    for (const auto& sel : selectedCourses) {
                        if (sel == courseName) alreadySelected = true;
                    }
                    if (!alreadySelected) {
                        selectedCourses.push_back(courseName);
                        std::cout << "   [ENROLLED] " << courseName << "\n";
                    }
                    else {
                        std::cout << "   Already enrolled in " << courseName << "\n";
                    }
                }
                else {
                    std::cout << "Invalid course number!\n";
                }
                std::cout << "Enter another course number (0 to finish): ";
            }
        }
    }

    // Final enrollment
    if (selectedCourses.empty()) {
        std::cout << "\nYou have not enrolled in any courses. You can enroll later from the menu.\n";
    }
    else {
        for (const auto& course : selectedCourses) {
            newLearner->enrollInCourse(course);
        }
        std::cout << "\n[SUCCESS] You have enrolled in " << selectedCourses.size() << " course(s):\n";
        for (const auto& course : selectedCourses) {
            std::cout << "   - " << course << "\n";
        }
    }

    newLearner->saveToCSV();
    std::cout << "\nYour profile has been saved. Welcome to atomcamp!\n";

    return newLearner;
}

// ADMIN DASHBOARD FUNCTION
void adminDashboard(std::vector<Learner*>& allLearners, std::vector<Course*>& courses, InstructorDashboard& dashboard) {
    std::cout << "\n========== ADMIN DASHBOARD ==========\n";
    std::cout << "[ADMIN MODE ACTIVE]\n\n";

    std::cout << "1. View All Students\n";
    std::cout << "2. Reset Student Progress\n";
    std::cout << "3. Change Student Skill Level\n";
    std::cout << "4. Delete Student\n";
    std::cout << "5. ADD NEW COURSE\n";
    std::cout << "6. REMOVE COURSE\n";
    std::cout << "7. View All Courses\n";
    std::cout << "8. View System Stats\n";
    std::cout << "9. Back to Main Menu\n";
    std::cout << "Choice: ";

    int adminChoice;
    std::cin >> adminChoice;

    switch (adminChoice) {
    case 1: {
        std::cout << "\n========== ALL STUDENTS ==========\n";
        for (auto* s : allLearners) {
            std::cout << "ID: " << s->getId() << " | " << s->getName()
                << " | " << s->getGoal() << " | Level: " << s->getSkillLevel()
                << " | Progress: " << s->getOverallProgress() << "%\n";
        }
        break;
    }
    case 2: {
        std::cout << "Enter Student ID to reset progress: ";
        int id;
        std::cin >> id;
        for (auto* s : allLearners) {
            if (s->getId() == id) {
                std::cout << "[ADMIN] Progress reset for " << s->getName() << "\n";
                break;
            }
        }
        break;
    }
    case 3: {
        std::cout << "Enter Student ID: ";
        int id;
        std::cin >> id;
        std::cout << "Enter new skill level (1-5): ";
        int newLevel;
        std::cin >> newLevel;
        for (auto* s : allLearners) {
            if (s->getId() == id) {
                s->setSkillLevel(newLevel);
                std::cout << "[ADMIN] " << s->getName() << " skill level changed to " << newLevel << "\n";
                break;
            }
        }
        break;
    }
    case 4: {
        std::cout << "Enter Student ID to delete: ";
        int id;
        std::cin >> id;
        for (auto it = allLearners.begin(); it != allLearners.end(); ++it) {
            if ((*it)->getId() == id) {
                std::cout << "[ADMIN] Deleting " << (*it)->getName() << "\n";
                delete* it;
                allLearners.erase(it);
                break;
            }
        }
        break;
    }
    case 5: {
        std::string name, category;
        int difficulty, duration;
        int numModules;

        std::cout << "\n========== ADD NEW COURSE ==========\n";
        std::cout << "Enter course name: ";
        clearInput();
        std::getline(std::cin, name);

        std::cout << "Enter category (Data/AI/Web/Programming): ";
        std::getline(std::cin, category);

        std::cout << "Enter difficulty (1-5): ";
        std::cin >> difficulty;

        std::cout << "Enter duration (months): ";
        std::cin >> duration;

        int newId = courses.size() + 1;
        Course* newCourse = new Course(newId, name, category, difficulty, duration);

        std::cout << "How many modules does this course have? ";
        std::cin >> numModules;
        clearInput();

        for (int i = 0; i < numModules; i++) {
            std::string moduleName;
            std::cout << "Enter module " << (i + 1) << " name: ";
            std::getline(std::cin, moduleName);
            newCourse->addModule(moduleName);
        }

        courses.push_back(newCourse);
        dashboard.addCourse(newCourse);

        std::cout << "\n[SUCCESS] Course \"" << name << "\" added successfully!\n";
        std::cout << "Now new students will see this course during onboarding!\n";
        break;
    }
    case 6: {
        std::cout << "\n========== REMOVE COURSE ==========\n";
        for (size_t i = 0; i < courses.size(); i++) {
            std::cout << i + 1 << ". " << courses[i]->getName() << "\n";
        }
        std::cout << "Select course to remove (0 to cancel): ";
        int courseIndex;
        std::cin >> courseIndex;

        if (courseIndex > 0 && courseIndex <= (int)courses.size()) {
            Course* toRemove = courses[courseIndex - 1];
            std::cout << "Remove \"" << toRemove->getName() << "\"? (y/n): ";
            char confirm;
            std::cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                delete toRemove;
                courses.erase(courses.begin() + (courseIndex - 1));
                std::cout << "[SUCCESS] Course removed!\n";
            }
        }
        break;
    }
    case 7: {
        std::cout << "\n========== ALL COURSES ==========\n";
        if (courses.empty()) {
            std::cout << "No courses available.\n";
        }
        else {
            for (auto* c : courses) {
                std::cout << "ID: " << c->getId() << " | " << c->getName()
                    << " | " << c->getCategory() << " | Level: " << c->getDifficultyLevel()
                    << " | Duration: " << c->getDuration() << " months\n";
                std::cout << "   Modules: ";
                for (const auto& m : c->getModules()) {
                    std::cout << m << " | ";
                }
                std::cout << "\n\n";
            }
        }
        break;
    }
    case 8: {
        std::cout << "\n========== SYSTEM STATS ==========\n";
        std::cout << "Total Students: " << allLearners.size() << "\n";
        std::cout << "Total Courses: " << courses.size() << "\n";

        int totalModules = 0;
        for (auto* c : courses) {
            totalModules += c->getTotalModules();
        }
        std::cout << "Total Modules: " << totalModules << "\n";

        int totalStruggling = 0;
        for (auto* s : allLearners) {
            if (!s->getStrugglingModules().empty()) totalStruggling++;
        }
        std::cout << "Students Struggling: " << totalStruggling << "\n";

        float avgProgress = 0;
        for (auto* s : allLearners) {
            avgProgress += s->getOverallProgress();
        }
        if (!allLearners.empty()) {
            std::cout << "Average Class Progress: " << (avgProgress / allLearners.size()) << "%\n";
        }
        break;
    }
    default:
        break;
    }
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "   ATOMCAMP ADAPTIVE LMS SYSTEM\n";
    std::cout << "   AIHACJATHON 2026\n";
    std::cout << "========================================\n";

    std::vector<Course*> courses;

    // Default courses
    Course* aiBootcamp = new Course(1, "AI Bootcamp", "AI", 3, 3);
    aiBootcamp->addModule("Python Basics");
    aiBootcamp->addModule("Machine Learning");
    aiBootcamp->addModule("Deep Learning");
    aiBootcamp->addModule("Generative AI & LLMs");
    courses.push_back(aiBootcamp);

    Course* dataScienceBootcamp = new Course(2, "Data Science & AI Bootcamp", "Data", 2, 3);
    dataScienceBootcamp->addModule("Python & SQL");
    dataScienceBootcamp->addModule("Power BI");
    dataScienceBootcamp->addModule("Machine Learning");
    dataScienceBootcamp->addModule("AI Systems");
    courses.push_back(dataScienceBootcamp);

    Course* webDevBootcamp = new Course(3, "Web Development Bootcamp", "Web", 1, 2);
    webDevBootcamp->addModule("HTML/CSS");
    webDevBootcamp->addModule("JavaScript");
    webDevBootcamp->addModule("React Basics");
    courses.push_back(webDevBootcamp);

    Course* pythonBeginner = new Course(4, "Python for Beginners", "Programming", 1, 1);
    pythonBeginner->addModule("Variables & Data Types");
    pythonBeginner->addModule("Loops & Functions");
    pythonBeginner->addModule("File Handling");
    courses.push_back(pythonBeginner);

    int nextId = 6;
    Learner* alice = new Learner(1, "Alice", "Data Science", 2);
    Learner* bob = new Learner(2, "Bob", "AI/ML", 4);
    Learner* charlie = new Learner(3, "Charlie", "Web Development", 1);

    alice->enrollInCourse("Data Science & AI Bootcamp");
    bob->enrollInCourse("AI Bootcamp");
    charlie->enrollInCourse("Web Development Bootcamp");

    alice->updateModuleScore("Python & SQL", 85);
    alice->markModuleComplete("Python & SQL");

    bob->updateModuleScore("Python Basics", 95);
    bob->markModuleComplete("Python Basics");
    bob->updateModuleScore("Machine Learning", 45);

    charlie->updateModuleScore("HTML/CSS", 40);

    alice->calculateOverallProgress(dataScienceBootcamp->getTotalModules());
    bob->calculateOverallProgress(aiBootcamp->getTotalModules());
    charlie->calculateOverallProgress(webDevBootcamp->getTotalModules());

    AdaptiveEngine engine;
    FeedbackLoop feedback(&engine);
    InstructorDashboard dashboard;
    LecturerDashboard lecturerDashboard;

    dashboard.addLearner(alice);
    dashboard.addLearner(bob);
    dashboard.addLearner(charlie);

    for (auto* course : courses) {
        dashboard.addCourse(course);
    }

    std::vector<Learner*> allLearners = { alice, bob, charlie };

    int choice;
    bool running = true;

    while (running) {
        showMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            clearInput();
            std::cout << "Invalid input!\n";
            continue;
        }

        switch (choice) {
        case 1: {
            Learner* newLearner = onboardLearner(nextId, courses);
            allLearners.push_back(newLearner);
            dashboard.addLearner(newLearner);
            break;
        }
        case 2: {
            if (allLearners.empty()) {
                std::cout << "\nNo learners found!\n";
                break;
            }

            std::cout << "\n========== SELECT YOUR PROFILE ==========\n";
            for (size_t i = 0; i < allLearners.size(); i++) {
                std::cout << i + 1 << ". " << allLearners[i]->getName() << " (" << allLearners[i]->getGoal() << ")\n";
            }
            std::cout << "Choice: ";
            int learnerChoice;
            std::cin >> learnerChoice;

            if (learnerChoice < 1 || learnerChoice >(int)allLearners.size()) {
                std::cout << "Invalid selection!\n";
                break;
            }

            Learner* current = allLearners[learnerChoice - 1];

            std::cout << "\n========== WELCOME BACK, " << current->getName() << "! ==========\n";
            std::cout << "Goal: " << current->getGoal() << "\n";
            std::cout << "Skill Level: " << current->getSkillLevel() << "/5\n";
            std::cout << "Progress: " << current->getOverallProgress() << "%\n";

            // Show comments from lecturers
            std::cout << "\n========== FEEDBACK FROM YOUR LECTURERS ==========\n";
            lecturerDashboard.viewStudentComments(current);

            // Show recommendations from lecturers
            lecturerDashboard.viewStudentRecommendations(current);

            auto enrolled = current->getEnrolledCourses();
            if (enrolled.empty()) {
                std::cout << "\nNo courses enrolled. Please contact admin.\n";
                break;
            }

            std::cout << "\n========== YOUR ENROLLED COURSES ==========\n";
            for (size_t i = 0; i < enrolled.size(); i++) {
                std::cout << i + 1 << ". " << enrolled[i] << "\n";
            }

            std::cout << "Select course (0 to go back): ";
            int courseChoice;
            std::cin >> courseChoice;

            if (courseChoice == 0) break;
            if (courseChoice < 1 || courseChoice >(int)enrolled.size()) {
                std::cout << "Invalid course!\n";
                break;
            }

            Course* selectedCourse = nullptr;
            for (auto* course : courses) {
                if (course != nullptr && course->getName() == enrolled[courseChoice - 1]) {
                    selectedCourse = course;
                    break;
                }
            }

            if (selectedCourse) {
                while (true) {
                    std::cout << "\n========== " << selectedCourse->getName() << " ==========\n";
                    std::cout << "1. Take Quiz\n";
                    std::cout << "2. Show Modules\n";
                    std::cout << "3. Back\n";
                    std::cout << "Choice: ";

                    int subChoice;
                    std::cin >> subChoice;

                    if (subChoice == 1) {
                        showAvailableModules(selectedCourse, current);
                        int moduleChoice = getValidModuleChoice(selectedCourse->getTotalModules());
                        if (moduleChoice == 0) continue;

                        std::string moduleName = selectedCourse->getModules()[moduleChoice - 1];
                        float score = getValidScore();
                        feedback.processQuizResult(current, moduleName, score, selectedCourse);
                        current->calculateOverallProgress(selectedCourse->getTotalModules());
                    }
                    else if (subChoice == 2) {
                        showAvailableModules(selectedCourse, current);
                        std::cout << "\nPress Enter...";
                        clearInput();
                        std::cin.get();
                    }
                    else {
                        break;
                    }
                }
            }
            break;
        }
        case 3: {
            dashboard.generateReport();

            std::cout << "\n========================================\n";
            std::cout << "Do you want to add a comment on a student? (y/n): ";
            char addComment;
            std::cin >> addComment;

            if (addComment == 'y' || addComment == 'Y') {
                std::cout << "\n========== SELECT STUDENT BY ID ==========\n";
                for (auto* s : allLearners) {
                    std::cout << "ID: " << s->getId() << " | " << s->getName()
                        << " | Goal: " << s->getGoal() << "\n";
                }

                int studentId;
                std::cout << "\nEnter Student ID: ";
                std::cin >> studentId;

                Learner* selectedStudent = nullptr;
                for (auto* s : allLearners) {
                    if (s->getId() == studentId) {
                        selectedStudent = s;
                        break;
                    }
                }

                if (selectedStudent) {
                    std::cout << "\n========== ADD COMMENT ON " << selectedStudent->getName() << " ==========\n";
                    std::string lecturerName, comment;
                    std::cout << "Your name (Lecturer): ";
                    clearInput();
                    std::getline(std::cin, lecturerName);
                    std::cout << "Your comment: ";
                    std::getline(std::cin, comment);

                    lecturerDashboard.addComment(selectedStudent, lecturerName, comment);

                    std::cout << "\nDo you want to add a RECOMMENDATION for this student? (y/n): ";
                    char addRec;
                    std::cin >> addRec;
                    if (addRec == 'y' || addRec == 'Y') {
                        std::string recommendation;
                        std::cout << "Your recommendation: ";
                        clearInput();
                        std::getline(std::cin, recommendation);
                        lecturerDashboard.addRecommendation(selectedStudent, lecturerName, recommendation);
                    }
                }
                else {
                    std::cout << "Student with ID " << studentId << " not found!\n";
                }
            }
            break;
        }
        case 4: {
            adminDashboard(allLearners, courses, dashboard);
            break;
        }
        case 5:
            std::cout << "\nGoodbye!\n";
            running = false;
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    }

    for (auto* course : courses) delete course;
    for (auto* learner : allLearners) delete learner;

    return 0;
}