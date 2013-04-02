#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct event {
	int id;
	int time;
	int score;
	int height;
	event() {}
	event(int i, int t, int s, int h): id(i), time(t), score(s), height(h) {}
};

class idList {
public:
	vector<int> v;
	bool operator <(const idList& other) const {
		if (v.size() < other.v.size()) return true;
		else if (v.size() > other.v.size()) return false;
		else {
			for (int i = 0; i < v.size(); i++) {
				if (v[i] < other.v[i]) return true;
				else if (v[i] > other.v[i]) return false;
			}
			return true;
		}
	}
};

bool Unequal(const int listHead, const int listEnd, const int H, const event* list, int &maxScore, vector<int> &maxList, int** B, int** Score) {
	for (int m = 0; m <= listEnd-listHead+1; m++) {
		for (int n = 0; n <= H; n++) {
			Score[m][n] = 0;
			B[m][n] = 0;
		}
	}
	for (int i = listHead; i <= listEnd; i++) {
		for (int j = 1; j <= H; j++) {
			if (list[i].height > j) { //not chosen
				Score[i-listHead+1][j] = Score[i-listHead][j];
				B[i-listHead+1][j] = B[i-listHead][j];
			} else {
				if (Score[i-listHead][j-list[i].height] + list[i].score > Score[i-listHead][j]) {//chosen
					if (Score[i-listHead][j-list[i].height] != 0 || j-list[i].height == 0) {
						Score[i-listHead+1][j] = Score[i-listHead][j-list[i].height] + list[i].score;
						B[i-listHead+1][j] = list[i].id;
					}
				} else if (Score[i-listHead][j-list[i].height] + list[i].score < Score[i-listHead][j]) {//not chosen
					Score[i-listHead+1][j] = Score[i-listHead][j];
					B[i-listHead+1][j] = B[i-listHead][j];
				} else {
					return false; // equal scores
				}
			}
		}
	}
	int maxHeight;
	for (int m = 1; m <= H; m++) {
		if (Score[listEnd-listHead+1][m] > maxScore) {
			maxScore = Score[listEnd-listHead+1][m];
			maxHeight = m;
		}
	}
	int q = listEnd-listHead+1;
	int p = maxHeight;
	while (q >= 1 && p >= 0 && B[q][p] != 0) {
		q = B[q][p];
		maxList.insert(maxList.begin(), q);
		p -= list[q-1].height;
		q -= listHead;
		q--;
	}
	return true;
}

void Equal (const int listHead, const int listEnd, const int H, const event* list, int &maxScore, vector<int> &maxList, int** Score) {
	for (int m = 0; m <= listEnd-listHead+1; m++) {
		for (int n = 0; n <= H; n++)
			Score[m][n] = 0;
	}
	vector<int> BT[listEnd-listHead+2][H+1];
	idList aList, bList;
	for (int i = listHead; i <= listEnd; i++) {
		for (int j = 1; j <= H; j++) {
			if (list[i].height > j) { //not chosen
				Score[i-listHead+1][j] = Score[i-listHead][j];
				BT[i-listHead+1][j] = BT[i-listHead][j];
			} else {
				if (Score[i-listHead][j-list[i].height] + list[i].score > Score[i-listHead][j]) {//chosen
					Score[i-listHead+1][j] = Score[i-listHead][j-list[i].height] + list[i].score;
					BT[i-listHead+1][j] = BT[i-listHead][j-list[i].height];
					BT[i-listHead+1][j].push_back(list[i].id);
				} else if (Score[i-listHead][j-list[i].height] + list[i].score < Score[i-listHead][j]) {//not chosen
					Score[i-listHead+1][j] = Score[i-listHead][j];
					BT[i-listHead+1][j] = BT[i-listHead][j];
				} else { // equal scores
					aList.v = BT[i-listHead][j-list[i].height];
					aList.v.push_back(list[i].id);
					bList.v = BT[i-listHead][j];
					if (aList < bList) {//chosen
						Score[i-listHead+1][j] = Score[i-listHead][j-list[i].height] + list[i].score;
						BT[i-listHead+1][j] = BT[i-listHead][j-list[i].height];
						BT[i-listHead+1][j].push_back(list[i].id);
					} else { //not chosen
						Score[i-listHead+1][j] = Score[i-listHead][j];
						BT[i-listHead+1][j] = BT[i-listHead][j];
					}
				}
			}
		}
	}
	maxScore = Score[listEnd-listHead+1][H];
	maxList = BT[listEnd-listHead+1][H];
}

void reload() {
	int N, W, H, ti, sc, he, maxScore, maxHeight, listHead, listEnd;
	char type;
	// N: number of events; W: time window; H: browser height
	cin >> N >> W >> H;
	event* list = new event[N];
	int** Score = new int*[W+1];
	int** B = new int*[W+1];
	for (int m = 0; m <= W; m++) {
		B[m] = new int[H+1];
		Score[m] = new int[H+1];
	}
	int ID = 1;
	vector<int> maxList;
	listHead = 0; // earliest event index that's not expired
	listEnd = -1; // latest event index
	bool list_updated = false; // event(s) added/expired in the event list
	bool MAX_updated = false; // event(s) added/expired in maxList
	vector<int>::const_iterator listIter;
	for (int a = 0; a < N; a++) {
		cin >> type >> ti;
		if (type == 'S') {
			cin >> sc >> he;
			list[ID-1] = event(ID++, ti, sc, he);
			list_updated = true;
			listEnd++;
		}
		else if (type == 'R') {
			if (listHead > listEnd) continue;
			if (!maxList.empty()) {
				for (int m = 0; m < maxList.size(); m++)
					if (ti - list[maxList[m]-1].time > W) {
						MAX_updated = true;
						break;
				}
			}
			for (int m = listHead; m <= listEnd; m++) {
				if (ti - list[m].time > W) {
					listHead++;
					list_updated = true;
				}
				else break;
			}
			if (listHead > listEnd) continue;
			if (list_updated || MAX_updated) {
				maxScore = 0;
				maxList.clear();
				if (!Unequal(listHead, listEnd, H, list, maxScore, maxList, B, Score)) {
					maxScore = 0;
					maxList.clear();
					Equal(listHead, listEnd, H, list, maxScore, maxList, Score);
				}
			}
			cout << maxScore << " " << maxList.size();
			for (listIter = maxList.begin(); listIter != maxList.end(); ++listIter) {
				cout << " " << *listIter;
			}
			cout << endl;
			list_updated = false;
			MAX_updated = false;
		}
	}
	for (int n = 0; n <= W; n++) {
		delete [] Score[n];
		delete [] B[n];
	}
	delete [] Score;
	delete [] B;
	delete [] list;
}

int main() {
	reload();
}



