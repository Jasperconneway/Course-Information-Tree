#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * Jasper Conneway
 * CS 499 Category Two: Algorithms and Data Structure enhancement
 * Original project from CS 300
 * Completed: 06/22/2025
 */

// define a course struct
struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisite;
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) : Node() {
		course = aCourse;
	}

};

class CourseInformationTree {
private:
	Node* root;
	bool isPrerequisiteCourse(vector<string> courseNumbers, string prerequisite);
	void loadTree(Course course);
	void addCourseToTree(Node* node, Course course);
	void printCourse(Node* node);
	bool isAlphabeticallySmaller(string str1, string str2);
	void destroyTree(Node* node);

public:
	CourseInformationTree();
	virtual ~CourseInformationTree();
	bool readFile(string file);
	void LoadStructure(string file);
	Course FindCourse(Node* node, string courseNumber);
	void PrintCourseList();
	void PrintCourse(string courseNumber);
	vector<Course> Merge(vector<Course> num1, vector<Course> num2);
	vector<Course> MergeSort(vector<Course> courses, int lo, int hi);

};

// Constructor
CourseInformationTree::CourseInformationTree() {
	// root is equal to nullptr
	this->root = nullptr;
}

void CourseInformationTree::destroyTree(Node* node) {
	if (node != nullptr) {
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}
}

// Destructor
CourseInformationTree::~CourseInformationTree() {
	// recursively delete tree
	destroyTree(root);
}

/**
 * Read file and verify course information
 * @param file contains a course in each line
 * Each course has a course number, course title. Some may have prerequisite course numbers
 * Verify prerequisite course numbers are true prerequisites
 */
bool CourseInformationTree::readFile(string file) {
	vector<string> courseNumbers;
	string line;
	bool noErrors = false;

	// open file
	ifstream fileStream(file);

	// verify file is open
	if (!fileStream.is_open()) {
		cerr << "Error: Could not open file " << file << endl;
		return noErrors;
	}

	// read each line in file
	while (getline(fileStream, line)) {
		// Create string stream from line in file
		stringstream ss(line);
		string courseItem;
		vector<string> courseItems;

		// Parse line by comma and store in courseItem
		while (getline(ss, courseItem, ',')) {
			// Store each course item in the vector courseItems
			courseItems.push_back(courseItem);
		}

		// Add course number to courseNumbers
		courseNumbers.push_back(courseItems[0]);

		// if there is less than two course items
		if (courseItems.size() < 2) {
			cout << "Error: File does not contain enough course information." << endl;
			return noErrors;
		}
		// if there are more than two course items
		else if (courseItems.size() > 2) {
			// loop through the prerequisite course numbers
			for (int i = 2; i < courseItems.size(); ++i) {
				// if course number is not a prerequisite, print error
				if (!isPrerequisiteCourse(courseNumbers, courseItems[i])) {
					cout << "Error: Course number is not a prerequisite." << endl;
					return noErrors;
				}
			}
		}
	}

	cout << "Did not locate any issues with the file." << endl;
	noErrors = true;

	// close file
	fileStream.close();

	return noErrors;
}

/**
 * Determine if prerequisite course number is in the vector of course numbers
 */
bool CourseInformationTree::isPrerequisiteCourse(vector<string> courseNumbers, string prerequisite) {
	// for each course number in vector
	for (string courseNumber : courseNumbers) {
		// check if course number matches the prerequisite paramater
		if (courseNumber == prerequisite) {
			// if found return true
			return true;
		}
	}
	// return false if prerequisite is not found
	return false;
}

/**
 * Load tree with the course provided
 * @param course course to add to CourseInformationTree
 */
void CourseInformationTree::loadTree(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		addCourseToTree(root, course);
	}
}

/**
 * Recursively add a new course to the CourseInformationTree
 */
void CourseInformationTree::addCourseToTree(Node* node, Course course) {
	// if course is smaller than @param node
	if (node->course.courseNumber > course.courseNumber) {
		// add course to the left if null
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		// else recursive call with the left node and course
		else {
			addCourseToTree(node->left, course);
		}
	}
	// if course is larger than @param node
	else if (node->course.courseNumber < course.courseNumber) {
		// add course to the right if null
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		// else recursive call with the right node and course
		else {
			addCourseToTree(node->right, course);
		}
	}
}

/**
 * Load each line into the structure
 * File has been checked by the read file method
 */
void CourseInformationTree::LoadStructure(string file) {
	Course course;
	string line;

	// open file
	ifstream fileStream(file);

	// verify file is open
	if (!fileStream.is_open()) {
		cerr << "Error: Could not open file " << file << endl;
		return;
	}


	// read each line in file
	while (getline(fileStream, line)) {
		// clear old data before parsing new course
		course.prerequisite.clear();

		// Create string stream from line in file
		stringstream ss(line);
		string courseItem;
		vector<string> courseItems;

		// Parse line by comma and store in courseItem
		while (getline(ss, courseItem, ',')) {
			// Store each course item in the vector courseItems
			courseItems.push_back(courseItem);
		}

		// add course number to course
		course.courseNumber = courseItems[0];
		// add course title to course
		course.courseTitle = courseItems[1];

		// if any prerequisites
		if (courseItems.size() > 2) {
			// loop each prerequisite and add to course
			for (int i = 2; i < courseItems.size(); ++i) {
				course.prerequisite.push_back(courseItems[i]);
			}
		}

		// load current course into the binary search tree
		loadTree(course);
	}

	// close file
	fileStream.close();
}

/**
 * Find course recursively with course number
 * @param courseNumber course number of the course to be found
 */
Course CourseInformationTree::FindCourse(Node* node, string courseNumber) {
	Course course;

	// if the current node course number equals @param courseNumber
	if (node->course.courseNumber == courseNumber) {
		// return the course found
		return node->course;
	}
	// if course is smaller than @param node
	else if (node->course.courseNumber > course.courseNumber) {
		// if left node is null, course does not exist
		if (node->left == nullptr) {
			cout << "Course does not exist." << endl;
			return course; // return empty course
		}
		// else recursive call with the left node and courseNumber
		else {
			return FindCourse(node->left, courseNumber);
		}
	}
	// if course is larger than @param node
	else if (node->course.courseNumber < course.courseNumber) {
		// if right node is null, course does not exist
		if (node->right == nullptr) {
			cout << "Course does not exist." << endl;
		}
		// else recursive call with the right node and courseNumber
		else {
			return FindCourse(node->right, courseNumber);
		}
	}
}

/**
 * Call private recursive printCourse starting at root node
 */
void CourseInformationTree::PrintCourseList() {
	printCourse(root);

}

/**
 * Print a course with the given course number
 * @param courseNumber Course Number to find course
 */
void CourseInformationTree::PrintCourse(string courseNumber) {
	Course course;
	// recurse find course starting at root
	course = FindCourse(root, courseNumber);

	// if course is found, print courseNumber, title
	if (!course.courseNumber.empty()) {
		cout << course.courseNumber << ", " << course.courseTitle;
		// and any prerequisites
		if (!course.prerequisite.empty()) {
			for (string prerequisite : course.prerequisite) {
				cout << ", " << prerequisite;
			}
		}
		// print newline
		cout << endl;
	}
	else { // if course was not found
		cout << "Course was not found. " << endl;
	}

}

/**
 * Recursively print every course starting at node
 * @param node Current node in the tree
 */
void CourseInformationTree::printCourse(Node* node) {
	// if node is not null
	if (node != nullptr) {
		printCourse(node->left);
		// print the nodes contents
		cout << node->course.courseNumber << ", " << node->course.courseTitle;
		// if there are prerequisites, print them
		if (!node->course.prerequisite.empty()) {
			for (string prerequisite : node->course.prerequisite) {
				cout << ", " << prerequisite;
			}
		}// newline
		cout << endl;
		// recursively call right
		printCourse(node->right);
	}
}

/**
 * Comparison between two strings
 * @return return true if str1 is smaller or false otherwise
 */
bool CourseInformationTree::isAlphabeticallySmaller(string str1, string str2) {
	transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
	transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
	if (str1 < str2) {
		return true;
	}
	return false;
}

/**
 * Part of MergeSort
 * @return return the merged vector
 */
vector<Course> CourseInformationTree::Merge(vector<Course> num1, vector<Course> num2) {
	int m = num1.size();
	int n = num2.size();
	vector<Course> num3;

	int idx = 0;

	int i = 0;
	int j = 0;
	// check each course between two vectors if smaller
	while (i < m && j < n) {
		if (isAlphabeticallySmaller(num1[i].courseNumber,
			num2[j].courseNumber)) {
			// if num1 is smaller put in num3
			num3.push_back(num1[i]);
			i++;
			idx++;
		}
		else { // else put num2 in num3
			num3.push_back(num2[j]);
			j++;
			idx++;
		}
	}
	// put each course into num3
	while (i < m) {
		num3.push_back(num1[i]);
		i++;
		idx++;
	}
	// put each course into num3
	while (j < n) {
		num3.push_back(num2[j]);
		j++;
		idx++;
	}
	// return the merged vector
	return num3;
}

/**
 * MergeSort takes the given vector courses and splits them into two vectors
 * @param courseNumbers
 * @param lo Low position
 * @param hi High position
 * @return the sorted vector courses
 */
vector<Course> CourseInformationTree::MergeSort(vector<Course> courses, int lo,
	int hi) {
	vector<Course> num1;
	vector<Course> num2;
	vector<Course> num3;

	// if there is only one course in the vector, return it
	if (lo == hi) {
		vector<Course> temp = { courses[lo] };
		return temp;
	}
	// set the midpoint
	int mid = lo + (hi - lo) / 2;
	// recurse call with courses, low point, and mid point
	num1 = MergeSort(courses, lo, mid);
	// recurse call with courses, mid point + 1, and high point
	num2 = MergeSort(courses, mid + 1, hi);
	// call merge and set merged vector num3
	num3 = Merge(num1, num2);
	// then return it
	return num3;

}

int main() {
	string filename = "CourseInformation.txt";
	CourseInformationTree* cst = new CourseInformationTree();
	if (cst->readFile(filename)) {
		cst->LoadStructure(filename);
		cst->PrintCourseList();
	}
}
