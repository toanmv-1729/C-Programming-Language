#include <iostream>
#include <cstring>
#include <fstream>
#include <windows.h>

using namespace std;

short boxx = 6, boxy = 6, boxs = 65; //Vi tri bat dau x, y cua box va box size
bool updated = false; //Kiem tra xem co thay doi du lieu hay khong

/* START Layout */
void gotoxy(short x,short y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x, y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void setcolor(short x) {
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, x);
}

void menu1() {
	setcolor(11);
	gotoxy(18,2);
	cout << "F1: Them tu moi";
	setcolor(14);
	gotoxy(18,3);
	cout << "F2: Ket thuc chuong trinh";
	setcolor(7);
	cout << endl;
}

void dicbox() {
	gotoxy(boxy,boxx); cout << char(201);
	for(int i = 1; i < boxs; i++) {
		cout << char(205);
	}
	cout << char(187);
	gotoxy(boxy,boxx+1); cout << char(186);
	gotoxy(boxy+boxs,boxx+1); cout << char(186);
	gotoxy(boxy,boxx+2); cout << char(204);
	for(int i = 1; i < boxs; i++) {
		cout << char(205);
	}
	cout << char(185);
	for(int i = boxx+3; i < boxx+9; i++) {
		gotoxy(boxy,i);
		cout << char(186);
		cout << i;
		gotoxy(boxy+boxs,i);
		cout << char(186);
	}
}
/* END Layout */

struct word {
	string tu, nghia[3], loai, dongnghia, vidu[3];
	word *left, *right;
};

struct hashtable {
	word *head, *tail;
};

void add(word *&head, word *&tail, word *&newword) {
	word *run = head;
	if(head == NULL) {
		head = tail = newword;	
	}
	else {
		while(run != NULL) {
			if(run->tu < newword->tu) {
				if(run->right == NULL) {
					run->right = newword;
					newword->left = run;
					tail = newword;
					break;
				}
				else {
					run = run->right;
				}		
			}
			else {
				word *tmp = run->left;
				tmp->right = newword;
				newword->left = tmp;
				newword->right = run;
				run->left = newword;
				break;
			}	
		}
	}
	updated = true;
}

void addlast(word *&head, word *&tail, word *&newword) {
	word *run = head;
	if(head == NULL) {
		head = tail = newword;
	}
	else {
		tail->right = newword;
		newword->left = tail;
		tail = newword;
	}
}

void tachchuoi(string line, word *&tmp){
	int d = 0, i = 0;
	string tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->tu = tach;
	tach = "";
	while(line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->loai = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->dongnghia = tach;
	tach = "";

	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[0] = tach;
	tach = "";

	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[1] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->nghia[2] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[0] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[1] = tach;
	tach = "";
	while(line[i] != '@'){
		tach += line[i];
		i++;
	}
	i++;
	tmp->vidu[2] = tach;
}

void importword(hashtable *&tudien, string line) {
	for(int i = 97; i < 124; i++) {
		if(line[0] == char(i)) {
			word *newword = new word();
			tachchuoi(line, newword);
			newword->left = NULL;
			newword->right = NULL;
			addlast(tudien[i - 97].head, tudien[97-i].tail, newword);
			break;
		}
	}
}

void readf(hashtable *&tudien, word *&tuhientai) {
	ifstream fi("data.txt");
	string line;
	if(fi.is_open()) {
		while(getline(fi, line)) {
			importword(tudien, line);
		}
		fi.close();
	}
	for(int i = 0; i < 26; i++) { //Thac mac....
		if(tudien[i].head != NULL) {
			tuhientai = tudien[i].head;
			break;
		}
	}
}

void writef(hashtable *tudien) {
	ofstream fo("data.txt");
	if(fo.is_open()) {
		for(int i=0; i < 26; i++) {
			word *head = tudien[i].head;
			while(head != NULL) {
				fo << head->tu
					<< "@" << head->loai
					<< "@" << head->dongnghia
					<< "@" << head->nghia[0]
					<< "@" << head->nghia[1]
					<< "@" << head->nghia[2]
					<< "@" << head->vidu[0]
					<< "@" << head->vidu[1]
					<< "@" << head->vidu[2] << "@\n";
				head = head->right;
			}
		}
		fo.close();
	}
}

int taptudien(string tu) {
	if(tu != "") {
		char x = tu[0];
		if((x < 123) && (x > 96)) {
			return x - 97;
		}
		return -1;
	}
	return -1;
}

word *leftword(hashtable *tudien, word *current) {
	word *left = current->left;
	int x = taptudien(current->tu);
	if(left != NULL) {
		return left;
	}
	else {
		if(x > 0) {
			while(x >= 0) {
				if(tudien[x - 1].tail != NULL) {
					return tudien[x - 1].tail;
				}
				x--;
			}
			return NULL;
		}
		else {
			return NULL;
		}
	}
}

word *rightword(hashtable *tudien, word *current) {
	word *right = current->right;
	int x = taptudien(current->tu);
	if(right != NULL) {
		return right;
	}
	else {
		if(x < 25) {
			while(x != 26) {
				if(tudien[x + 1].head != NULL) {
					return tudien[x + 1].head;
				}
				x++;
			}
			return NULL;
		}
		else {
			return NULL;
		}
	}
}

bool compare(string cantim, word *dangdo) {
	string x = cantim, y = dangdo->tu;
	if(x.length() > y.length()) {
		return false;
	}
	else {
		for(int i = 0; i < x.length(); i++) {
			if(x[i] != y[i]) {
				return false;
			}
		}
		return true;
	}
}

word *search(hashtable *tudien, string searchword) {
	int x = taptudien(searchword);
	if(x < 0) {
		return NULL;
	}
	else {
		word* head = tudien[x].head;
		while(head != NULL) {
			if(compare(searchword, head)) {
				return head;
			}
			head = head->right;
		}
		return NULL;
	}
}

void deleteword(hashtable *&tudien, word *&xoa) {
	if(xoa != NULL) {
		int x = taptudien(xoa->tu);
		word *head = tudien[x].head, *tail = tudien[x].tail;
		if(head->tu == xoa->tu) {
			head = tail = NULL;
		}
		else if(xoa->tu == tail->tu) {
			tail = tail->left;
			tail->right = NULL;
		}
		else {
			word *left = xoa->left, *right = xoa->right;
			left->right = right;
			right->left = left;
			xoa->left = xoa->right = NULL;
		}
		delete xoa;
		xoa = NULL;
		tudien[x].head = head;
		tudien[x].tail = tail;
	}
	updated = true;
}

void deletewordcurrent(hashtable *&tudien, word *&tuhientai) {
	if(tuhientai != NULL) {
		word *left = leftword(tudien, tuhientai), *right = rightword(tudien, tuhientai), *tmp = tuhientai;
		if((left == NULL) && (right == NULL)) {
			tuhientai = NULL;
		}
		else {
			if(left == NULL) {
				tuhientai = right;
			}
			else {
				tuhientai = left;
			}
		}
		deleteword(tudien, tmp);
	}
}

void printscreen(word *tmp) {
	if(tmp != NULL) {
		cout << tmp->tu << "\nLoai tu: " << tmp->loai << "\nNghia: " << tmp->nghia[0];
		if(tmp->nghia[1] != "") cout << "; " << tmp->nghia[1];
		if(tmp->nghia[2] != "") cout << "; " << tmp->nghia[2];
		if(tmp->dongnghia != "") cout << "\nTu dong nghia: " << tmp->dongnghia;
		if(tmp->vidu[0] != "") cout << "\nVi du: " << tmp->vidu[0];
		if(tmp->vidu[1] != "") cout << "; " << tmp->vidu[1];
		if(tmp->vidu[2] != "") cout << "; " << tmp->vidu[2];
		cout << "\n";
	}
}

int main() {
	SetConsoleTitle("Dictionary 1.0 by Hoang Khoi Nguyen");
	word *tuhientai = NULL;
	hashtable *tudien = new hashtable[26];
	for(int i = 0; i < 26; i++) {
		tudien[i].tail = NULL;
		tudien[i].head = NULL;
	};
	menu1();
	dicbox();
	readf(tudien, tuhientai);
	if(updated) writef(tudien);
	return 0;
}
