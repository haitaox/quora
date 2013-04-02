#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class event {
public:
	int id;
	int time;
	int score;
	int height;
	event(int i, int t, int s, int h): id(i), time(t), score(s), height(h) {}
};

class output {
public:
	int score;
	int num; // number of events;
	int height; // used height
	vector<int> idList;
	output() {}
	output(int s, int n, int h, int l): score(s), num(n), height(h) {idList.push_back(l);}
	void print() const {
		cout << score << " " << num << " ";
		for (int i = 0; i < idList.size(); i++)
			cout << idList[i] << " ";
		cout << endl;
	}
};

int main() {
	ofstream outData;
	outData.open("largeInput.txt");
	char command;
	int N = 10000; // # of events
	int W = 2000; // time window
	int H = 2000; // window height
	int ti = 10;
	int score, height, temp;
	outData << N << " " << W << " " << H << endl;
	srand(time(NULL));
	for (int i = 0; i < N; i++) {
		temp = rand();
		ti += (temp%80 + 1);
		if (temp%2 == 0) {
			command = 'S';
			score = temp%1000000;
			height = temp%50;
			outData << command << " " << ti << " " << score << " " << height << endl;
		} else {
			command = 'R';
			outData << command << " " << ti << endl;
		}
	}
}




