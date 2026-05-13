#define _CRT_SECURE_NO_WARNINGS
#include "LecturerDashboard.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

LecturerDashboard::LecturerDashboard() {
    commentsFile = "comments.txt";
    recommendationsFile = "recommendations.txt";
    loadCommentsFromFile();
    loadRecommendationsFromFile();
}

LecturerDashboard::~LecturerDashboard() {
    saveCommentsToFile();
    saveRecommendationsToFile();
}

std::string getCurrentDateSafe() {
    time_t now = time(0);
    struct tm timeInfo;
    char buffer[80];

    localtime_s(&timeInfo, &now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

    return std::string(buffer);
}

// ========== COMMENT FUNCTIONS ==========

void LecturerDashboard::addComment(Learner* student, std::string lecturerName, std::string comment) {
    if (student == nullptr) {
        std::cout << "[ERROR] Student not found!\n";
        return;
    }

    Comment newComment;
    newComment.lecturerName = lecturerName;
    newComment.studentName = student->getName();
    newComment.studentId = student->getId();
    newComment.comment = comment;
    newComment.date = getCurrentDateSafe();

    allComments.push_back(newComment);
    saveCommentsToFile();

    std::cout << "\n[SUCCESS] Comment added successfully!\n";
    std::cout << "   Student: " << student->getName() << "\n";
    std::cout << "   Comment: " << comment << "\n";
    std::cout << "   Date: " << newComment.date << "\n";
}

void LecturerDashboard::viewStudentComments(Learner* student) {
    if (student == nullptr) {
        std::cout << "[ERROR] Student not found!\n";
        return;
    }

    std::cout << "\n========== COMMENTS FOR " << student->getName() << " ==========\n";
    bool found = false;

    for (const auto& c : allComments) {
        if (c.studentId == student->getId()) {
            found = true;
            std::cout << "\n[" << c.date << "] " << c.lecturerName << ":\n";
            std::cout << "   \"" << c.comment << "\"\n";
            std::cout << "   ------------------------------------\n";
        }
    }

    if (!found) {
        std::cout << "No comments yet for this student.\n";
    }
}

void LecturerDashboard::viewAllComments() {
    std::cout << "\n========== ALL COMMENTS ==========\n";

    if (allComments.empty()) {
        std::cout << "No comments found.\n";
        return;
    }

    for (const auto& c : allComments) {
        std::cout << "\n[" << c.date << "] " << c.lecturerName << " -> " << c.studentName << ":\n";
        std::cout << "   \"" << c.comment << "\"\n";
        std::cout << "   ------------------------------------\n";
    }
}

void LecturerDashboard::saveCommentsToFile() {
    std::ofstream file(commentsFile);
    if (!file.is_open()) {
        return;
    }

    for (const auto& c : allComments) {
        file << c.studentId << "|" << c.studentName << "|"
            << c.lecturerName << "|" << c.comment << "|" << c.date << "\n";
    }
    file.close();
}

void LecturerDashboard::loadCommentsFromFile() {
    std::ifstream file(commentsFile);
    if (!file.is_open()) {
        return;
    }

    allComments.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        Comment c;
        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, '|');
        if (token.empty()) continue;
        c.studentId = std::stoi(token);

        std::getline(ss, c.studentName, '|');
        std::getline(ss, c.lecturerName, '|');
        std::getline(ss, c.comment, '|');
        std::getline(ss, c.date);

        allComments.push_back(c);
    }
    file.close();
}

// ========== RECOMMENDATION FUNCTIONS ==========

void LecturerDashboard::addRecommendation(Learner* student, std::string lecturerName, std::string recommendation) {
    if (student == nullptr) {
        std::cout << "[ERROR] Student not found!\n";
        return;
    }

    Recommendation newRec;
    newRec.lecturerName = lecturerName;
    newRec.studentName = student->getName();
    newRec.studentId = student->getId();
    newRec.recommendation = recommendation;
    newRec.date = getCurrentDateSafe();
    newRec.isRead = false;

    allRecommendations.push_back(newRec);
    saveRecommendationsToFile();

    std::cout << "\n[SUCCESS] Recommendation added for " << student->getName() << "!\n";
    std::cout << "   Recommendation: " << recommendation << "\n";
    std::cout << "   Date: " << newRec.date << "\n";
}

void LecturerDashboard::viewStudentRecommendations(Learner* student) {
    if (student == nullptr) {
        std::cout << "[ERROR] Student not found!\n";
        return;
    }

    std::cout << "\n========== RECOMMENDATIONS FOR " << student->getName() << " ==========\n";
    bool found = false;

    for (auto& r : allRecommendations) {
        if (r.studentId == student->getId()) {
            found = true;
            r.isRead = true;
            std::cout << "\n[" << r.date << "] From: " << r.lecturerName << "\n";
            std::cout << "   \"" << r.recommendation << "\"\n";
            std::cout << "   ------------------------------------\n";
        }
    }

    if (!found) {
        std::cout << "No recommendations yet for this student.\n";
    }

    saveRecommendationsToFile();
}

void LecturerDashboard::viewAllRecommendations() {
    std::cout << "\n========== ALL RECOMMENDATIONS ==========\n";

    if (allRecommendations.empty()) {
        std::cout << "No recommendations found.\n";
        return;
    }

    for (const auto& r : allRecommendations) {
        std::cout << "\n[" << r.date << "] " << r.lecturerName << " -> " << r.studentName << ":\n";
        std::cout << "   \"" << r.recommendation << "\"\n";
        if (!r.isRead) std::cout << "   [UNREAD]\n";
        std::cout << "   ------------------------------------\n";
    }
}

void LecturerDashboard::saveRecommendationsToFile() {
    std::ofstream file(recommendationsFile);
    if (!file.is_open()) {
        return;
    }

    for (const auto& r : allRecommendations) {
        file << r.studentId << "|" << r.studentName << "|"
            << r.lecturerName << "|" << r.recommendation << "|"
            << r.date << "|" << r.isRead << "\n";
    }
    file.close();
}

void LecturerDashboard::loadRecommendationsFromFile() {
    std::ifstream file(recommendationsFile);
    if (!file.is_open()) {
        return;
    }

    allRecommendations.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        Recommendation r;
        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, '|');
        if (token.empty()) continue;
        r.studentId = std::stoi(token);

        std::getline(ss, r.studentName, '|');
        std::getline(ss, r.lecturerName, '|');
        std::getline(ss, r.recommendation, '|');
        std::getline(ss, r.date, '|');
        std::getline(ss, token);
        r.isRead = (token == "1");

        allRecommendations.push_back(r);
    }
    file.close();
}

void LecturerDashboard::showStudentAnalytics(Learner* student) {
    if (student == nullptr) {
        std::cout << "[ERROR] Student not found!\n";
        return;
    }

    std::cout << "\n========== ANALYTICS FOR " << student->getName() << " ==========\n";
    std::cout << "Name: " << student->getName() << "\n";
    std::cout << "ID: " << student->getId() << "\n";
    std::cout << "Goal: " << student->getGoal() << "\n";
    std::cout << "Skill Level: " << student->getSkillLevel() << "/5\n";
    std::cout << "Overall Progress: " << student->getOverallProgress() << "%\n";

    auto scores = student->getModuleScores();
    std::cout << "\nModule Scores:\n";
    if (scores.empty()) {
        std::cout << "   No modules completed yet.\n";
    }
    else {
        float total = 0;
        int count = 0;
        for (const auto& s : scores) {
            std::cout << "   " << s.first << ": " << s.second << "%";
            if (s.second < 60) std::cout << " [NEEDS IMPROVEMENT]";
            std::cout << "\n";
            total += s.second;
            count++;
        }
        if (count > 0) {
            std::cout << "\nAverage Score: " << (total / count) << "%\n";
        }
    }

    auto struggling = student->getStrugglingModules();
    if (!struggling.empty()) {
        std::cout << "\n[ALERT] Struggling with: ";
        for (const auto& m : struggling) {
            std::cout << m << " ";
        }
        std::cout << "\n";
    }

    int commentCount = 0;
    for (const auto& c : allComments) {
        if (c.studentId == student->getId()) commentCount++;
    }
    std::cout << "\nTotal Comments: " << commentCount << "\n";

    int recCount = 0;
    for (const auto& r : allRecommendations) {
        if (r.studentId == student->getId()) recCount++;
    }
    std::cout << "Total Recommendations: " << recCount << "\n";
}