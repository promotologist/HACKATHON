# 🎓 Atomcamp Adaptive Learning Management System (LMS)

## 📌 Project Title
**Atomcamp Adaptive LMS - Personalized Learning Journey Platform**

---

## 📝 Problem Statement

atomcamp is an emerging tech education ecosystem offering programs, bootcamps, and learning resources across Pakistan and beyond. As the learner base grows, atomcamp faces a critical challenge: **no two learners are alike — yet most learning systems treat them as if they are.**

Today, learners navigate a static catalogue of content with little personalization, instructors have limited visibility into who is struggling and why, and the platform lacks a unified system that ties together goals, progress, feedback, and outcomes.

**Our solution:** A Smart Adaptive Learning Management System that personalizes each student's learning journey from onboarding to course completion, while giving instructors and administrators actionable intelligence to improve outcomes at scale.

---

## ✨ Features

### For Students 👨‍🎓
- Personalized onboarding with career goal selection (Data Science, AI/ML, Web Development, Data Engineering)
- Smart course recommendations based on skill level (1-5)
- Individual course selection (YES/NO for each course)
- Quiz system with instant feedback
- Real-time progress tracking
- View lecturer comments and recommendations
- Module completion status

### For Instructors 👨‍🏫
- Class progress dashboard
- At-risk student detection (scores below 60%)
- Module performance analytics
- Add personalized comments for students
- Add recommendations for students
- Comprehensive class reports

### For Admins 👑
- View all students with details
- Reset student progress
- Change student skill levels
- Delete student profiles
- ADD NEW COURSES with custom modules
- REMOVE existing courses
- View system statistics

### System Features 🔧
- Data persistence (CSV and TXT files)
- Adaptive difficulty adjustment
- Feedback loop for continuous improvement
- Input validation and error handling

---

## 🛠️ Tech Stack

| Technology | Purpose |
|------------|---------|
| C++17 | Core programming language |
| STL (Vector, Map, Algorithm) | Data structures |
| File I/O (fstream) | Data persistence |
| OOP (Classes) | Modular code structure |
| Makefile | Build automation |

---

## 📁 Project Structure
atomcamp-adaptive-lms/
├── include/
│ ├── Learner.h
│ ├── Course.h
│ ├── AdaptiveEngine.h
│ ├── InstructorDashboard.h
│ ├── FeedbackLoop.h
│ └── LecturerDashboard.h
├── src/
│ ├── main.cpp
│ ├── Learner.cpp
│ ├── Course.cpp
│ ├── AdaptiveEngine.cpp
│ ├── InstructorDashboard.cpp
│ ├── FeedbackLoop.cpp
│ └── LecturerDashboard.cpp
├── data/
│ ├── learners.csv
│ ├── comments.txt
│ └── recommendations.txt
├── Makefile
└── README.md
## 🚀 Steps to Run the Project

### Prerequisites
- C++17 compiler (g++ 7+, Visual Studio 2019+, or MinGW)

### Option 1: Linux/Mac
### Option 2: Windows (MinGW)
### Option 3: Visual Studio
1. Create new Console App project
2. Add all .h files to Header Files
3. Add all .cpp files to Source Files
4. Set C++ Language Standard to C++17
5. Press Ctrl+F5 to run

### Option 4: Manual Compilation
## 🎮 How to Use

### Main Menu Options
New Learner Onboarding
Continue as Learner
Instructor Dashboard
ADMIN
Exit

### Option 1: New Student Onboarding
- Enter your name
- Select career goal (1-4)
- Rate skill level (1-5)
- Review recommended courses
- Choose YES/NO for each course
- Start learning!

### Option 2: Continue as Learner
- Select your profile
- View lecturer feedback
- Select a course
- Take quizzes
- Track progress

### Option 3: Instructor Dashboard
- View class progress
- See at-risk students
- Add comments on students
- Add recommendations

### Option 4: ADMIN Dashboard
- Manage students
- Add new courses
- Remove courses
- View system stats

---

## 👥 Team Members

| Name | Role |
|------|------|
| **Wania Zahra** | Team Lead - Core Engine & Data Structures |
| **Sophiya Khayyam** | Adaptive Logic & Personalization Algorithm |
| **Muhammad Saad Nawaz** | Dashboards & Integration |

---

## 📊 Submission Requirements Checklist

| Requirement | Status | Location |
|-------------|--------|----------|
| GitHub Repository Link | ✅ | https://github.com/YOUR_USERNAME/atomcamp-adaptive-lms |
| Project ZIP File | ✅ | atomcamp-adaptive-lms.zip |
| Project Demonstration Video | ✅ | atomcamp_lms_demo.mp4 |
| README File | ✅ | This file |

---

## 🏆 AIHACJATHON 2026

**Event:** AIHACJATHON by AUREX'26  
**Head Organizer:** Salah ud din Sani  
**Submission Date:** April 2026  
**Time:** Before 8:00 PM

### Problem Statement Satisfied ✅
- ✅ Personalized learning from onboarding to completion
- ✅ Instructor dashboard with actionable intelligence
- ✅ Feedback loop tying goals, progress, and outcomes
- ✅ Built on atomcamp's real course offerings
- ✅ Admin panel for course management
- ✅ Lecturer comments and recommendations
- ✅ Data persistence across sessions

---

## 🎯 Sample Output
