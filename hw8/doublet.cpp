#include <iostream>
#include <fstream>
#include <string>
#include "minheap.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

//so basiclaly going to make a function wehre i search through and fin
//words that are closer to the target, and then Ill make note of them
//and then at the very end of the loop ill calculate out all their 
//a star values and then check which has the lowest and run the function
// on that guy
/*
void converting(string look, string end, string terms[], MinHeap &store, int priority, int iterations) {
	int analysis = 0;
	
	while (look[analysis] == end[analysis]) {
		analysis++;
	}

	if (analysis - 1 == end.size()) {
		return;
	}

	vector<int> possible;
	for (int i = 0; i < int(terms.size()); i++) {
		bool okay = true;
		for (int j = 0; j < analysis) {
			if (terms[i][j] != look[j]) {
				okay = false;
			}
		}
		if (terms[i][analysis] == end[analysis] && okay == true) {
			possible.push_back(i);
		}
	}
	lowest = end.size() + iterations + 5;
	int special = 0;
	
	for (int i = 0; i < int(possible.size()); i++) {
		int wrong = 0;
		for (int j = 0; j < terms[possible]; j++) {
			if (terms[possible][j] != end[j]) {
				wrong++;
			}
		}
		if (iterations + wrong < lowest) {
			lowest = iterations + wrong;
			special = possible;
		}

		else if(iterations + wrong == lowest) {
			int wrong_old = 0;
			for (int j = 0; j < terms[special]; j++){
				if (terms[special][j] != end[j]) {
					wrong_old++;
				}
			}
			if ((iterations + wrong)*(terms[possible].size() + 1) + wrong < lowest*(terms[special].size()+1) + wrong_old) {
				special = possible;
			}
		}
	}
	store.add(terms[special], priority);
	priority++;
	iterations++;
	converting(terms[possible], end, terms, store, priority, iterations);
}
*/

vector<string> get_cousins(string start, vector<string> terms, vector<string> check) {
	vector<string> answers;
	for (int i = 0; i < int(terms.size()); i++) {
		int wrong = 0;
		if (start.size() != terms[i].size()) {
			wrong = 2;
		}
		else {
			for (int j = 0; j < terms[i].size(); j++) {
				if (start[j] != terms[i][j]) {
					wrong++;
				}
			}
		}
		if (wrong == 1 && std::find(check.begin(), check.end(), terms[i]) != vector.end()) {
			answers.push_back(terms[i]);
		}
	}
	return answers;
}

int get_priority(string guy, int step, string end) {
	int wrong = 0;
	for (int i = 0; i < end.size(); i++) {
		if (guy[i] != end[i]) {
			wrong++;
		}
	}
	return (step+wrong)*(end.size() + 1) + wrong;
}

int main (int argc, char* argv[]) {
	if (argc < 4) {
		cout << "Please enter the proper amount of arguments!" << endl;
		return 1;
	}

	int wrong = 0;
	string begin = argv[1];
	string end = argv[2];
	for (int j = 0; j < int(begin.size()); j++) {
		if (begin[j] < 90) {
			begin[j] = begin[j] + 32;
		}
		if (end[j] < 90) {
			end[j] = end[j] + 32;
		}

		if (begin[j] != end[j]) {
			wrong++;
		}
	}

	ifstream file;
	file.open(argv[3]);
	int size;
	file >> size;
	vector<string> terms;
	
	string temp;
	for (int i = 0; i < size; i++) {
		file >> temp;
		for (int j = 0; j < int(temp.size()); i++) {
			if (temp[j] < 90) {
				temp[j] = temp[j] + 32;
			}
		}
		terms.push_back(temp);
	}
	
	MinHeap openlist(2);
	vector<string> closedlist;
	int priority = wrong*(begin.size() + 1) + wrong;
	cool.add(begin, priority);
	map<string,string> backtrack;
	string temp;
	int step = 0;
	int expansion = 1;
	while (!openlist.empty()) {
		step++;
		temp = openlist.peek();
		closedlist.push_back(temp);
		openlist.remove();
		if (temp == end) {
			break;
		}

		vector<string> neighbor = get_cousins(temp, terms, closedlist);
		for (int i = 0; i < neighbor.size(); i++) {
			int prio = get_priority(neighbor[i], step, end);
			openlist.add(std::make_pair(neighbor[i], prio));
			expansion++;
			backtrack.insert(std::pair<string,string>(neighbor[i], temp);
		}
	}





return 0;
}
