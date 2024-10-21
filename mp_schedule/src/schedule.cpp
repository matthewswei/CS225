/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    V2D output;
    // Put all text into a string
    std::string file = file_to_string(filename);
    // Separate the text into rows
    std::vector<std::string> row;
    int rows = split_string(file, '\n', row);

    // Go through each row and split the row's text, dividing them into columns
    for (int i = 0; i<rows; i++) {
        std::vector<std::string> columns;
        std::vector<std::string> col;
        int cols = split_string(row[i], ',', col);

        // Put data onto the 2D output array
        for (int j = 0; j<cols; j++) {
            columns.push_back(trim(col[j]));
        }
        output.push_back(columns);
    }
    return output;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // YOUR CODE HERE
    /*
        Psuedo-code
        
        Create separate lists for cv and student for easier searching (with map)

        1. Go through and get a list of students and which courses they are taking
        (Hash map is easiest way)
        2. Go through list of courses and get which students are in them
        3. Cross-reference the two maps, find students and courses that match, and
        add them to the final list
        4. Set that final list for the course in the 2D vector
    */

    V2D output;
    std::map<std::string, std::vector<std::string>> map1;
    std::map<std::string, std::vector<std::string>> map2;
    std::vector<std::string> courses;

    // Create map of students with what classes they are in
    for (auto stu : student) {
        std::vector<std::string> stu_course;
        for (int i = 1; i<(int)stu.size(); i++) {
            stu_course.push_back(stu[i]);
        }
        map2[stu[0]] = stu_course;
    }

    // Create map of courses with which students they have
    for (auto course: cv) {
        std::vector<std::string> students;
        courses.push_back(course[0]);
        for (int i = 1; i<(int)course.size(); i++) {
            students.push_back(course[i]);
        }
        map1[course[0]] = students;
    }

    // Find student and courses that match from the inputs and create output
    for (auto course : courses) {
        std::vector<std::string> students;
        students.push_back(course);
        for (auto stu : map1[course]) {
            // Checks if student and course are found in the maps
            if (map2.find(stu)!=map2.end()) {
                if (find(map2[stu].begin(), map2[stu].end(), course)!=map2[stu].end()) students.push_back(stu);
            }
        }
        if (students.size()>1) output.push_back(students);
    }

    return output;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    V2D output;
    std::vector<std::string> courses1;
    std::vector<std::string> students;
    std::map<std::string, std::vector<std::string>> map1;
    std::map<int, std::vector<std::string>> map2;
    std::map<std::string, std::set<std::string>> adgList;
    std::map<std::string, int> color;
    int color_high = (int)timeslots.size();
    int degree = 0;

    // Create first map
    for (auto course : courses) {
        std::vector<std::string> list;
        list.push_back(course[0]);
        courses1.push_back(course[0]);
        for (int i = 1; i<(int)course.size(); i++) {
            if (map1.find(course[i])!=map1.end()) map1[course[i]].push_back(course[0]);
            else { students.push_back(course[i]); map1[course[i]] = list;}
        }
    }

    // Create adjacency list for schedule making
    for (auto student : students) {
        std::vector<std::string> list = map1[student];
        for (int i = 0; i<(int)list.size(); i++) {
            for (int j = i + 1 ; j<(int)list.size(); j++) {
                std::set<std::string> course;
                if (adgList.find(list[j])!=adgList.end()) adgList[list[j]].insert(list[i]);
                else { course.insert(list[i]); adgList[list[j]] = course;}
            }
            for (int j = i + 1 ; j<(int)list.size(); j++) {
                std::set<std::string> course;
                if (adgList.find(list[i])!=adgList.end()) adgList[list[i]].insert(list[j]);
                else {course.insert(list[j]); adgList[list[i]] = course;}
            }
        }
    }

    // Create second map (similar to first map creation)
    for (auto course : courses1) {
        int size = (int)adgList[course].size() - 1;
        if (degree<size) degree = size;
        if (map2.find(size)!=map2.end()) map2[size].push_back(course);
        else {
            std::vector<std::string> list; 
            list.push_back(course); 
            map2[size] = list;
        }
    }

    for (int i = degree; 0<=i; i--) {
        for (auto course : map2[i]) {
            std::vector<int> colors;
            colors.resize(color_high, -1);
            for (auto node : adgList[course]) 
                if (color.find(node)!=color.end()) colors[color[node]]++;
            for (int j = 0; j<(int)colors.size(); j++) 
                if (colors[j]==-1) {color[course] = j; break;}
        }
    }

    // Check if no match is found, else output the schedule
    if (courses1.size()!=color.size()) {
        std::vector<std::string> row;
        row.push_back("-1");
        output.push_back(row);
    } else {
        for (int i = 0; i<(int)timeslots.size(); i++) {
            std::vector<std::string> row;
            row.push_back(timeslots[i]);
            for (auto course : courses1) {
                if (color[course]==i) row.push_back(course);
            }
            output.push_back(row);
        }
    }

    return output;
}