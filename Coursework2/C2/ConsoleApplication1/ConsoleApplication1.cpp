#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "Date.h"
#include <time.h>
#include <random>
#include <fstream>
#include "string"
#include <map>
#include <list>
#include <initializer_list>
#include <thread>
#include <mutex>

#pragma warning (disable : 4996)

using namespace std;

// Date.cpp stuff
Date::Date()
{
	time(&Now);
	struct tm Tm;
	localtime_s(&Tm, &Now);
	Day = Tm.tm_mday; // 1...31
	strcpy_s(Month, MonthNames[Tm.tm_mon]); // 0...11
	iMonth = Tm.tm_mon + 1;
	Year = Tm.tm_year + 1900; // current year - 1900
}
Date::Date(int d, int m, int y)
{
	SetYear(y);
	SetMonth(m);
	SetDay(d);
}
Date::Date(const Date& Original)
{ // copy constructor
	Day = Original.Day;
	Year = Original.Year;
	iMonth = Original.iMonth;
	strcpy_s(Month, Original.Month);
}
Date::~Date()
{
	if (pText)
		delete pText;
}
bool Date::IsLeap(int y)
{ // from https://www.programiz.com/c-programming/examples/leap-year
	if (y % 4 == 0)
	{
		if (y % 100 == 0)
		{	// year is divisible by 400, hence the year is a leap year
			if (y % 400 == 0)
				return true;
			else
				return false;
		}
		else
			return true;
	}
	else
		return false;
}
char* Date::ToString() const
{
	(const_cast<Date*>(this))->pText = new char[12];
	sprintf_s(pText, 12, "%02d %s %d", Day, Month, Year);
	return pText;
}
void Date::SetMonth(int m)
{
	if (m < 1 || m > 12)
		throw exception("Wrong month");
	strcpy_s(Month, MonthNames[m - 1]);
	iMonth = m;
}
int Date::GetMonth(char* pBuf, int nBuf) const
{
	if (!pBuf || nBuf < 4)
		return iMonth;
	strcpy_s(pBuf, nBuf, Month);
	return iMonth;
}
void Date::SetYear(int y)
{
	if (y < 0)
		throw exception("Wrong year");
	Year = y;
}
void Date::SetDay(int d)
{
	if (d < 1 || d > 31)
		throw exception("Wrong day");
	if ((iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11) && d == 31)
		throw exception("Wrong day");
	if (iMonth == 2)
	{
		if (IsLeap(Year))
		{
			if (d > 29)
				throw exception("Wrong day");
		}
		else
		{
			if (d > 28)
				throw exception("Wrong day");
		}
	}
	Day = d;
}
void Date::SetDate(int d, int m, int y)
{
	SetYear(y);
	SetMonth(m);
	SetDay(d);
}
bool Date::operator==(const Date& other) const
{
	if (Day == other.Day && iMonth == other.iMonth && Year == other.Year)
		return true;
	else
		return false;
}
bool Date::operator<(const Date& other) const
{
	if (Year != other.Year)
		return Year < other.Year;
	if (iMonth != other.iMonth)
		return iMonth < other.iMonth;
	return Day < other.Day;
}
Date& Date::operator=(const Date& Right)
{
	//cout << "operator= called\n";
	if (this == &Right) // & - address operator
		return *this; // necessary for expressions like d1 = *pd where pd points to d1
	Day = Right.Day;
	Year = Right.Year;
	iMonth = Right.iMonth;
	strcpy_s(Month, Right.Month);
	return *this;
}
Date Date::CreateRandomDate(Date begin, Date end)
{   // returns a date between begin and end
	// https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
	tm tm_begin, tm_end;
	memset(&tm_begin, 0, sizeof tm_begin);
	memset(&tm_end, 0, sizeof tm_end);
	tm_begin.tm_mday = begin.GetDay();
	tm_end.tm_mday = end.GetDay();
	tm_begin.tm_mon = begin.GetMonth() - 1;
	tm_end.tm_mon = end.GetMonth() - 1;
	tm_begin.tm_year = begin.GetYear() - 1900;
	tm_end.tm_year = end.GetYear() - 1900;
	time_t lower = mktime(&tm_begin);
	time_t upper = mktime(&tm_end);
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr((unsigned int)lower, (unsigned int)upper);
	time_t time_t_rand = distr(eng);
	tm tm_rand;
	localtime_s(&tm_rand, &time_t_rand);
	return Date(tm_rand.tm_mday, tm_rand.tm_mon + 1, tm_rand.tm_year + 1900);
}
// end


int stringCmp(string s1, string s2) {

	int len1 = s1.length();
	int len2 = s2.length();

	if (len1 < len2) {

		for (int i = 0; i < len1; i++) {
			if (s1[i] > s2[i]) {
				return 2;
			}
			else if (s1[i] < s2[i]) {
				return 1;
			}
		}

		return 2;

	}
	else {

		for (int i = 0; i < len2; i++) {
			if (s1[i] > s2[i]) {
				return 2;
			}
			else if (s1[i] < s2[i]) {
				return 1;
			}
		}

		return 1;

	}

}

vector<string> stringSort(vector<string> names) {

	while (true) {

		int change = 0;

		for (int i = 0; i < (names.size() - 1); i++) {

			string s1 = names[i];
			string s2 = names[i + 1];

			if (stringCmp(s1, s2) == 2) {
				string temps = s1;
				names[i] = s2;
				names[i + 1] = temps;

				change = 1;

				//cout << change << endl;
			}

		}

		if (change == 0) { break; }
	}

	return names;

}


int dateSame(Date d1, Date d2) {

	int day1 = d1.GetDay();
	int month1 = d1.GetMonth();
	int year1 = d1.GetYear();

	int day2 = d2.GetDay();
	int month2 = d2.GetMonth();
	int year2 = d2.GetYear();


	if (day1 == day2 && month1 == month2 && year1 == year2) {
		return 0;
	}
	else {
		return 1;
	}


}

int dateCmp(Date d1, Date d2) {

	int day1 = d1.GetDay();
	int month1 = d1.GetMonth();
	int year1 = d1.GetYear();

	int day2 = d2.GetDay();
	int month2 = d2.GetMonth();
	int year2 = d2.GetYear();


	if (year1 < year2) {
		return 1;
	}
	else if (year1 > year2) {
		return 2;
	}
	else {

		if (month1 < month2) {
			return 1;
		}
		else if (month1 > month2) {
			return 2;
		}
		else {
			if (day1 < day2) {
				return 1;
			}
			else if (day1 > day2) {
				return 2;
			}
			else {
				return 1;
			}
		}
	}
}

vector<Date> dateSort(vector<Date> dates){ 

	while (true) {

		int change = 0;

		for (int i = 0; i < (dates.size() - 1); i++) {

			Date d1 = dates[i];
			Date d2 = dates[i + 1];

			if (dateCmp(d1, d2) == 2) {
				Date tempd = d1;
				dates[i] = d2;
				dates[i + 1] = tempd;

				change = 1;
			}

		}

		if (change == 0) { break; }
	}

	return dates;
}


int monthCharToInt(string month) {

	string jan = "Jan";
	string feb = "Feb";
	string mar = "Mar";
	string apr = "Apr";
	string may = "May";
	string jun = "Jun";
	string jul = "Jul";
	string aug = "Aug";
	string sep = "Sep";
	string oct = "Oct";
	string nov = "Nov";
	string dec = "Dec";


	if (!month.compare(jan)) {
		return 1;
	}
	else if (!month.compare(feb)) {
		return 2;
	}
	else if (!month.compare(mar)) {
		return 3;
	}
	else if (!month.compare(apr)) {
		return 4;
	}
	else if (!month.compare(may)) {
		return 5;
	}
	else if (!month.compare(jun)) {
		return 6;
	}
	else if (!month.compare(jul)) {
		return 7;
	}
	else if (!month.compare(aug)) {
		return 8;
	}
	else if (!month.compare(sep)) {
		return 9;
	}
	else if (!month.compare(oct)) {
		return 10;
	}
	else if (!month.compare(nov)) {
		return 11;
	}
	else if (!month.compare(dec)) {
		return 12;
	}

	return 0;
}

//////////////////////////////////////////////////////////////

default_random_engine rando(time(NULL)); // this is a default engine which uses the current time to get pseudo random numbers.

class Item {
private:
	char Group;
	int Subgroup;
	string Name;
	Date Timestamp;

public:
	Item();
	Item(char a, int b, string c, Date d);
	Item(const Item& Original);
	~Item();

	void printAll();

	void chGroup(char c) {
		if (c < 65 || c > 90) {
			printf("Out of group range! (A-Z)");
		}
		else {
			Group = c;
		}
	}
	void chSub(int x) {
		if (x >= 0 && x < 100) {
			Subgroup = x;
		}
		else {
			printf("Subgroup out of range! (0-99)");
		}
	}
	void chName(string s) {
		if (s != "") {
			Name = s;
		}
		else {
			printf("Name can't be an empty string!");
		}
	}
	void chDate(Date d) {
		Timestamp = d;
	}
	
	char getGroup() { 
		return Group; 
	}
	int getSub() {
		return Subgroup;
	}
	string getName() {
		return Name;
	}
	Date getTimestamp() {
		return Timestamp;
	}

};

Item::Item(char G, int S, string N, Date T) {

	if (G < 65 || G > 90) {
		cout << "Out of Group range! (A-Z)" << endl;
	}
	else if (S < 0 || S>99) {
		cout << "Out of Subgroup range! (0-99)" << endl;
	}
	else if (N == "") {
		cout << "Name cannot be an empty string!" << endl;
	}
	else {
		Group = G;
		Subgroup = S;
		Name = N;
		Timestamp = T;
	}
}

Item::Item() {
	Group = char((65 + (rando() % 26)));
	Subgroup = rando() % 100;

	ifstream file("Birds.txt");
	vector<string> birds; // a vector holds elements of the same type, it can GROW DYNAMICALLY, takes more memory than arrays.
	string birdname;

	while (getline(file, birdname)) { // while strings can be written from "file" it will run. This puts everything until the final line.
		
		birds.push_back(birdname); // push_back appends to the vector

	}

	int len = birds.size();
	Name = birds[rando() % len]; // get a random name from vector "birds".
	
	Timestamp = Date::CreateRandomDate(Date(1, 1, 1970), Date(1, 1, 2020));
}

Item::Item(const Item& Original) {
	Group = Original.Group;
	Subgroup = Original.Subgroup;
	Name = Original.Name;
	Timestamp = Original.Timestamp;
}

Item::~Item(){}

void Item::printAll() {
	cout << "Group: " << Group  << endl;
	cout << "Subgroub: " << Subgroup << endl;
	cout << "Name: " << Name << endl;
	cout << "Timestamp: " << Timestamp.GetDay() << "/" << Timestamp.GetMonth() << "/" << Timestamp.GetYear() << endl;
}

/////////////////////////////////////////////////////////////////

int itemCmp(Item i1, Item i2) {

	char g1 = i1.getGroup();
	int sub1 = i1.getSub();
	string n1 = i1.getName();
	Date d1 = i1.getTimestamp();

	char g2 = i2.getGroup();
	int sub2 = i2.getSub();
	string n2 = i2.getName();
	Date d2 = i2.getTimestamp();

	if (g1 == g2 && sub1 == sub2 && n1.compare(n2) == 0 && dateSame(d1, d2) == 0) {
		return 0;
	}
	else {
		return 1;
	}

}

/////////////////////////////////////////////////////////////////

class Data{
private:
	map<char, map<int, list<Item*>*>*> DataStructure;

public:
	Data();
	Data(int n);
	~Data();
	void PrintAll();
	int CountItems();
	map<int, list<Item*>*>* GetGroup(char c);
	void PrintGroup(char c);
	int CountGroupItems(char c);
	list<Item*>* GetSubgroup(char c, int i);
	void PrintSubgroupByNames(char c, int i);
	void PrintSubgroupByDates(char c, int i);
	int CountSubgroupItems(char c, int i);
	Item* GetItem(char c, int i, string s);
	Item* GetItem(char c, int i, Date d);
	void PrintItem(char c, int i, string s);
	Item* InsertItem(char c, int i, string s, Date d);
	/////
	list<Item*>* InsertSubgroup(char s, int i, initializer_list<Item*> items);
	map<int, list<Item*>*>* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items);
	/////
	bool RemoveItem(char g, int sub, string n);
	bool RemoveSubgroup(char c, int i);
	bool RemoveGroup(char c);
};

Data::Data(){}

Data::Data(int n) {

	for (int i = 0; i < n; i++) {
		 
		Item* pItem = new Item;

		char G = pItem->getGroup();
		int S = pItem->getSub();

		if (DataStructure.find(G) == DataStructure.end()) { // this means that it can't find the member

			list<Item*>* tempList = new list<Item*>;
			tempList->push_back(pItem);
			
			map<int, list<Item*>*>* tempMap = new map<int, list<Item*>*>;
			tempMap->insert(make_pair(S, tempList)); // make_pair has to be used to insert something into a map

			DataStructure.insert(make_pair(G, tempMap));
			
		}
		else {
			
			map<int, list<Item*>*>* map2 = DataStructure[G];

			if (map2->find(S) == map2->end()) { // can't find the subgroup, so it just adds it.

				list<Item*>* pList = new list<Item*>;
				pList->push_back(pItem);

				map2->insert(make_pair(S, pList));
			}
			else { // subgroup already exists

				list<Item*>* Lister = (*map2)[S];

				Lister->push_back(pItem);

			}


		}

	}
}

Data::~Data() {

	cout << "\nDestructor Running...." << endl;

	for (auto it1 = DataStructure.begin(); it1 != DataStructure.end(); it1++) {

		map<int, list<Item*>*>* map2 = it1->second;

		for (auto it2 = map2->begin(); it2 != map2->end(); it2++) {

			list<Item*>* lister = it2->second;

			for (auto it3 = lister->begin(); it3 != lister->end(); it3++) {
				delete (*it3);
			}
			
			delete lister;
		}

		delete map2;
	}

	cout << "Target Destructed ;)" << endl;

}

void Data::PrintAll() {
	
	for (auto it1 = DataStructure.begin(); it1 != DataStructure.end(); it1++) {

		char G = it1->first;
		map<int, list<Item*>*> map2 = *(it1->second);

		cout << G << ":" << endl;
		
		for (auto it2 = map2.begin(); it2 != map2.end(); it2++) {

			int S = it2->first;
			cout << "  " << S << ":" << endl;

			list<Item*> lister = *(it2->second);

			for (auto it3 = lister.begin(); it3 != lister.end(); it3++) {

				string N = (*it3)->getName();
				int day = (*it3)->getTimestamp().GetDay();
				int month = (*it3)->getTimestamp().GetMonth();
				int year = (*it3)->getTimestamp().GetYear();

				cout << "    " << N << " " << day << "/" << month << "/" << year << endl;

			}

		}

	}

}

int Data::CountItems() {

	int num=0;

	for (auto it1 = DataStructure.begin(); it1 != DataStructure.end(); it1++) {

		map<int, list<Item*>*> map2 = *(it1->second);

		for (auto it2 = map2.begin(); it2 != map2.end(); it2++) {

			list<Item*> lister = *(it2->second);

			for (auto it3 = lister.begin(); it3 != lister.end(); it3++) {

				num++;

			}

		}

	}

	return num;

}

map<int, list<Item*>*>* Data::GetGroup(char c) {

	for (auto it = DataStructure.begin(); it != DataStructure.end(); it++) {

		if (it->first == c) {
			return it->second;
		}

	}

	return 0;

}

void Data::PrintGroup(char c) {

	auto pMap = this->GetGroup(c);

	if (pMap != 0) {

		auto mappie = *(pMap);

		for (auto it = mappie.begin(); it != mappie.end(); it++) {

			char Sub = it->first;
			list<Item*> lister = *(it->second);

			for (auto it2 = lister.begin(); it2 != lister.end(); it2++) {
				(*it2)->printAll();
			}

		}

	}
	else {
		cout << "Not Found" << endl;
	}

}

int Data::CountGroupItems(char c) {

	int num = 0;

	auto pMap = this->GetGroup(c);

	if (pMap != 0) {

		auto mappie = *(pMap);

		for (auto it = mappie.begin(); it != mappie.end(); it++) {

			char Sub = it->first;
			list<Item*> lister = *(it->second);

			for (auto it2 = lister.begin(); it2 != lister.end(); it2++) {
				num++;
			}

		}

	}

	return num;

}

list<Item*>* Data::GetSubgroup(char c, int i) {

	auto pGroup = this->GetGroup(c);
	if (pGroup == 0) { return 0; }

	for (auto it = pGroup->begin(); it != pGroup->end(); it++) {
		if (it->first == i) {
			return it->second;
		}
	}

	return 0;

}

void Data::PrintSubgroupByNames(char g, int sub) {

	auto pSub = this->GetSubgroup(g,sub);

	if (pSub == 0) {
		cout << "Subgroup Not Found!" << endl;
	}
	else {

		vector<string> names;

		for (auto it = pSub->begin(); it != pSub->end(); it++) {
			names.push_back((*it)->getName());
		}

		names = stringSort(names);

		for (int i = 0; i < names.size(); i++) {

			Item tempItem = *this->GetItem(g, sub, names[i]);
			
			string N = tempItem.getName();
			int day = tempItem.getTimestamp().GetDay();
			int month = tempItem.getTimestamp().GetMonth();
			int year = tempItem.getTimestamp().GetYear();

			cout << sub << ": " << N << " " << day << "/" << month << "/" << year << endl;

		}
	}
} 

void Data::PrintSubgroupByDates(char g, int sub){

	auto pSub = this->GetSubgroup(g, sub);

	if (pSub == 0) {
		cout << "Subgroup Not Found!" << endl;
	}
	else {

		vector<Date> dates;

		for (auto it = pSub->begin(); it != pSub->end(); it++) {
			dates.push_back((*it)->getTimestamp());
		}

		dates = dateSort(dates);

		for (int i = 0; i < dates.size(); i++) {

			Item tempItem = *this->GetItem(g, sub, dates[i]);

			string N = tempItem.getName();
			int day = tempItem.getTimestamp().GetDay();
			int month = tempItem.getTimestamp().GetMonth();
			int year = tempItem.getTimestamp().GetYear();

			cout << sub << ": " << N << " " << day << "/" << month << "/" << year << endl;

		}
	}

}  

int Data::CountSubgroupItems(char c, int i) {

	int n = 0;
	auto pSub = this->GetSubgroup(c, i);

	if (pSub != 0) {
		for (auto it = pSub->begin(); it != pSub->end(); it++) {
			n++;
		}
	}

	return n;

}

Item* Data::GetItem(char c, int i, string s) {

	auto pSub = this->GetSubgroup(c, i);

	if (pSub != 0) {
		
		for (auto it = pSub->begin(); it != pSub->end(); it++) {

			string name = (*it)->getName();

			if (s.compare(name) == 0) {
				return *it;
			}
		}
	}

	return 0;
}

Item* Data::GetItem(char c, int i, Date d) {

	auto pSub = this->GetSubgroup(c, i);

	if (pSub != 0) {

		for (auto it = pSub->begin(); it != pSub->end(); it++) {

			Date timestamp = (*it)->getTimestamp();

			if (dateSame(d,timestamp) == 0) {
				return *it;
			}
		}
	}

	return 0;
}

void Data::PrintItem(char c, int i, string s)
{

	auto pSub = this->GetSubgroup(c, i);

	bool found = 0;

	if (pSub != 0) {

		for (auto it = pSub->begin(); it != pSub->end(); it++) {

			string name = (*it)->getName();

			if (s.compare(name) == 0) {
				(*it)->printAll();
				found = 1;
			}
		}
	}

	if (found == 0) {
		cout << "Item Not Found!" << endl;
	}
}

Item* Data::InsertItem(char G, int S, string str, Date d) {

	Item* pItem = new Item(G,S,str,d);

	if (DataStructure.find(G) == DataStructure.end()) { // this means that it can't find the member

		list<Item*>* tempList = new list<Item*>;
		tempList->push_back(pItem);

		map<int, list<Item*>*>* tempMap = new map<int, list<Item*>*>;
		tempMap->insert(make_pair(S, tempList)); // make_pair has to be used to insert something into a map

		DataStructure.insert(make_pair(G, tempMap));

	}
	else {

		map<int, list<Item*>*>* map2 = DataStructure[G];

		if (map2->find(S) == map2->end()) { // can't find the subgroup, so it just adds it.

			list<Item*>* pList = new list<Item*>;
			pList->push_back(pItem);

			map2->insert(make_pair(S, pList));
		}
		else { // subgroup already exists

			list<Item*>* Lister = (*map2)[S];

			for (auto it = Lister->begin(); it != Lister->end(); it++) {

				if (itemCmp(*(*it), *pItem) == 0) {
					return 0;
				}
			}

			Lister->push_back(pItem);

			return pItem;
		}

	}

}

list<Item*>* Data::InsertSubgroup(char g, int sub, initializer_list<Item*> items) { // (I tHink*) We use initializer lists because they have a fixed size, take up less memory and are efficient. Also the members are constants so we can just copy them.

	map<int, list<Item*>*>* pMap = this->GetGroup(g);

	if (pMap != 0) {

		list<Item*>* pList = this->GetSubgroup(g, sub);

		if (pList != 0) {

			return 0;

		}
		else {
			
			for (auto it = items.begin(); it != items.end(); it++) {

				char grp = (*it)->getGroup();
				int subgrp = (*it)->getSub();
				string name = (*it)->getName();
				Date timestamp = (*it)->getTimestamp();

				this->InsertItem(grp, subgrp, name, timestamp);

			}

		}
	}
	else {

		for (auto it = items.begin(); it != items.end(); it++) {
			
			char grp = (*it)->getGroup();
			int subgrp = (*it)->getSub();
			string name = (*it)->getName();
			Date timestamp = (*it)->getTimestamp();

			this->InsertItem(grp, subgrp, name, timestamp);

		}
		return this->GetSubgroup(g, sub);
	}
} 

map<int, list<Item*>*>* Data::InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items) { 

	map<int, list<Item*>*>* pMap = this->GetGroup(c);

	if (pMap != 0) {
		return 0;
	}
	else {
		
		auto it1 = subgroups.begin();
		auto it2 = items.begin();

		for (; it1 != subgroups.end() && it2 != items.end(); it1++, it2++) {

			int sub = *it1;

			initializer_list<Item*> iList = *it2;

			for (auto it3 = iList.begin(); it3 != iList.end(); it3++) {

				char grp = (*it3)->getGroup();
				int subgrp = (*it3)->getSub();
				string name = (*it3)->getName();
				Date timestamp = (*it3)->getTimestamp();

				this->InsertItem(grp, subgrp, name, timestamp);

			}

		}
		pMap = this->GetGroup(c);
		return pMap;
	}	 
}

bool Data::RemoveItem(char g, int sub, string n) {

	Item* pI = this->GetItem(g, sub, n);

	if (pI != 0) {

		list<Item*>* pList = this->GetSubgroup(g, sub);

		if (this->CountSubgroupItems(g, sub) == 1 && this->CountGroupItems(g) == 1) {

			map<int, list<Item*>*>* pMap = this->GetGroup(g);

			pList->remove(pI);
			delete pI;
			delete pList;
			delete pMap;

			DataStructure.erase(g);
		}
		else if (this->CountSubgroupItems(g, sub) == 1) {
			map<int, list<Item*>*>* pMap = this->GetGroup(g);

			pList->remove(pI);
			delete pI;
			delete pList;
			pMap->erase(sub);
		}
		else {
			pList->remove(pI);
			delete pI;
		}

		return 1;

	}
	else {
		return 0;
	}

}

bool Data::RemoveSubgroup(char c, int i) {

	list<Item*>* pList = this->GetSubgroup(c, i);


	if (pList != 0) {
		
		vector<char> vg;
		vector<int> vsub;
		vector<string> vn;

		for (auto it = pList->begin(); it != pList->end(); it++) {

			char g = (*it)->getGroup();
			int sub = (*it)->getSub();
			string n = (*it)->getName();

			vg.push_back(g);
			vsub.push_back(sub);
			vn.push_back(n);
		}

		for (int i = 0; i < vg.size(); i++) {
			this->RemoveItem(vg[i], vsub[i], vn[i]);
		}
		
		return 1;
	}
	else {
		return 0;
	}


}

bool Data::RemoveGroup(char c) {

	map<int, list<Item*>*>* pMap = this->GetGroup(c);

	if (pMap != 0) {
		vector<int> vsub;

		for (auto it = pMap->begin(); it != pMap->end(); it++) {
			vsub.push_back(it->first);
		}

		for (int i = 0; i < vsub.size(); i++) {
			this->RemoveSubgroup(c,vsub[i]);
		}

		return 1;
	}
	else {
		return 0;
	}
}

/////////////////////////////////////////////////////////////////

// Pipes

/////////////////////////////////////////////////////////////////

HANDLE hpipe; // A handle is used as a reference, it identifies an object

mutex m; // A mutex blocks the memory from being modified by another thread while a thread is using it
int connected = 0;

vector<vector<string>> itemVec;

void InsertReadItems(Data *D) {

	int oldSize = itemVec.size();

	while (connected != -1) {

		int newSize = itemVec.size();

		if (newSize > oldSize) {
			vector<string> tempVec = itemVec[newSize-1];

			char g = tempVec[0][0];
			int sub = stoi(tempVec[1]);
			string n = tempVec[2];

			int day = stoi(tempVec[3]);
			int month = monthCharToInt(tempVec[4]);
			int year = stoi(tempVec[5]);
			Date d = Date::Date(day, month, year);

			//cout << g << " " << sub << " " << n << endl;
			D->InsertItem(g, sub, n, d);
			//cout << "Item inserted" << endl;

			oldSize = newSize;
		}
	}
}

void parseItem(char* arr) {

	// "A 2 <Great Crested Grebe> 29 Nov 2018"

	int count = 0;
	for (int i = 0; TRUE; i++) {
		if (arr[i] == 0) {
			break;
		}
		count++;
	}

	string tempString = "";
	vector<string> tempVec;

	for (int i = 0; i < count; i++) {

		if (arr[i] == 60) {
			while (arr[i + 1] != 62) {
				tempString.push_back(arr[i + 1]);
				i++;
			}

			tempVec.push_back(tempString);
			tempString = "";
			i += 2;
		}
		else if (arr[i] != 32) {
			tempString.push_back(arr[i]);
		}
		else {
			tempVec.push_back(tempString);
			tempString = "";
		}
	}

	tempVec.push_back(tempString); // for the last one

	itemVec.push_back(tempVec);

	for (string i : tempVec) {
		cout << i << " ";
	}
	cout << endl;
}

void pipeReadItem(Data *D) {

	///////////////////////////////////////////////////
	// Read from pipe server
	///////////////////////////////////////////////////

	bool messageRead;

	char itemDesc[81];
	DWORD bytesRead;

	// ReadFile() is used to read from the pipe server
	messageRead = ReadFile(
		hpipe,					// the pipe
		itemDesc,				// message from the pipe
		81,						// length of the buffer
		&bytesRead,				// bytes read ???
		NULL					// not overlapped ?
	);

	if (!messageRead) {
		cout << "Item read error: " << GetLastError() << endl;

		m.lock();
		connected = 0;
		m.unlock();
	}
	else {
		parseItem(itemDesc);
	}
}

void pipeSendStop() {

	bool messageSent;

	char* message = (char*)"stop\0";
	DWORD messageLen = strlen(message) + 1;
	DWORD bytesWritten;

	messageSent = WriteFile(
		hpipe,					// the pipe we're sending a message to
		message,				// the message itself
		messageLen,				// the length of the message
		&bytesWritten,			// bytes written ???
		NULL					// not overlapped ?
	);

	if (!messageSent) {
		cout << "Stop send error: " << GetLastError() << endl;
	}
	else {
		m.lock();
		connected = 0;
		m.unlock();

		cout << "Stop sent" << endl;
	}
}

void pipeSendReady(Data *D) {

	///////////////////////////////////////////////////
	// Send message "ready"
	///////////////////////////////////////////////////

	bool messageSent;

	char* message = (char*)"ready\0";
	DWORD messageLen = strlen(message) + 1; // +1 for terminating \0
	DWORD bytesWritten;


	while (TRUE) {
		if (connected == 1) {
			// WriteFile() is used to send messages to the pipe server
			messageSent = WriteFile(
				hpipe,					// the pipe we're sending a message to
				message,				// the message itself
				messageLen,				// the length of the message
				&bytesWritten,			// bytes written ???
				NULL					// not overlapped ?
			);

			if (!messageSent) {
				cout << "Ready send error: " << GetLastError() << endl;
			}
			else {
				pipeReadItem(D);
			}
		}
		else if (connected == -1) {
			break;
		}
	}
}

void pipeCon() {

	///////////////////////////////////////////////////
	// Connect to the pipe server
	/////////////////////////////////////////////////// 

	// CreateFile() is used to connect to a pipe server
	hpipe = CreateFile(
		L"\\\\.\\pipe\\ICS0025",		// name
		GENERIC_READ | GENERIC_WRITE,	// duplex
		0,								// no share?
		NULL,							// default security
		OPEN_EXISTING,					// opens existing pipe
		0,								// default attributes
		NULL							// no template file?
	);

	if (hpipe == INVALID_HANDLE_VALUE) {
		cout << "Connection error: " << GetLastError() << endl;
	}
	else {
		m.lock();
		connected = 1;
		m.unlock();

		cout << "Connection success" << endl;
	}
}

void comLine(Data *D) {

	thread sr(pipeSendReady, D);

	string input = "";

	while (TRUE) {
		cin >> input;

		if (!input.compare("connect")) {
			if (connected == 0) {
				pipeCon();
			}
			else {
				cout << "already connected to pipe server" << endl;
			}
		}
		else if (!input.compare("stop")) {
			if (connected == 1) {
				pipeSendStop();
			}
			else {
				cout << "there is no connection to the pipe server" << endl;
			}
		}
		else if (!input.compare("exit")) {

			if (connected == 1) {
				pipeSendStop();
			}
			connected = -1;
			sr.join();
			cout << endl << "thread joined" << endl;
			break;
		}
		else {
			cout << "Input must be: connect, stop, exit.\n";
		}
	}
}


/////////////////////////////////////////////////////////////////

int main()
{
	Data D;
	
	cout << "Ready to connect to pipe server\nUse commands: connect, stop, exit" << endl;

	thread insItem(InsertReadItems, &D);
	thread comLine(comLine,&D);

	comLine.join();
	insItem.join();

	InsertReadItems(&D);

	D.PrintAll();
	cout << "Items in DataStructure: " << D.CountItems() << endl << endl;

	cout << endl << "session ended" << endl;

	system("pause");

	return 0;
}
