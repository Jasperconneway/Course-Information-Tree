
/**
 * Call private recursive printCourse starting at root node
 */
void CourseTree::PrintCourseList() {
	printCourse(root);

}

/**
 * Print a course with the given course number
 * @param courseNumber Course Number to find course
 */
void CourseTree::PrintCourse(string courseNumber) {
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
	} else { // if course was not found
		cout << "Course was not found. " << endl;
	}

}

/**
 * Recursively print every course starting at node
 * @param node Current node in the tree
 */
void CourseTree::printCourse(Node *node) {
	// if node is not null
	if (node != nullptr) {
		// print the nodes contents
		cout << node->course.courseNumber << ", " << node->course.courseTitle;
		// if there are prerequisites, print them
		if (!node->course.prerequisite.empty()) {
			for (string prerequisite : node->course.prerequisite) {
				cout << ", " << prerequisite;
			}
		}// newline
		cout << endl;
		// recursively call left and right if not null
		if (node->left != nullptr) {
			printCourse(node->left);
		}
		if (node->right != nullptr) {
			printCourse(node->right);
		}
	}
}

/**
 * Comparison between two strings
 * @return return true if str1 is smaller or false otherwise
 */
bool CourseTree::isAlphabeticallySmaller(string str1, string str2) {
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
vector<Course> CourseTree::Merge(vector<Course> num1, vector<Course> num2) {
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
		} else { // else put num2 in num3
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
vector<Course> CourseTree::MergeSort(vector<Course> courses, int lo,
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
