#ifndef LECTURER_DASHBOARD_H
#define LECTURER_DASHBOARD_H

#include "Learner.h"
#include <string>
#include <vector>

struct Comment {
    std::string lecturerName;
    std::string studentName;
    std::string comment;
    std::string date;
    int studentId;
};

struct Recommendation {
    std::string lecturerName;
    std::string studentName;
    std::string recommendation;
    std::string date;
    int studentId;
    bool isRead;
};

class LecturerDashboard {
private:
    std::vector<Comment> allComments;
    std::vector<Recommendation> allRecommendations;
    std::string commentsFile;
    std::string recommendationsFile;

public:
    LecturerDashboard();
    ~LecturerDashboard();

    // Comment functions
    void addComment(Learner* student, std::string lecturerName, std::string comment);
    void viewStudentComments(Learner* student);
    void viewAllComments();
    void saveCommentsToFile();
    void loadCommentsFromFile();
    void showStudentAnalytics(Learner* student);

    // Recommendation functions
    void addRecommendation(Learner* student, std::string lecturerName, std::string recommendation);
    void viewStudentRecommendations(Learner* student);
    void viewAllRecommendations();
    void saveRecommendationsToFile();
    void loadRecommendationsFromFile();
};

#endif