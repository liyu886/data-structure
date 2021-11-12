// Guangzhou_subway.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <stack>
#include <algorithm>
#define MAX 9999
#define DATA 246
using namespace std;

string linename[16]= { "����һ����","����������", "����������", "���������߱��Ӷ�","�����ĺ���" ,"���������", "����������", "�����ߺ���", "�����˺���",
					"�����ź���", "����ʮ������", "����ʮ�ĺ���", "������ʮһ����", "���������","����APM��", "����ʮ�ĺ���֧��" };
class station {
public:
	station(int order, string name, int linenum, int adjacentnum, vector<int> line, vector<int>adjacentsite)
	{
		this->order = order;
		this->name = name;
		this->linenum = linenum;
		this->adjacentnum = adjacentnum;
		this->line = line;
		this->adjacentsite = adjacentsite;
	}
	~station()
	{
		line.clear();
		adjacentsite.clear();
	}
	void print()
	{
		cout << "վ�����:" << order << " ";
		cout << "վ������:" << name << " ";
		cout << "������·:";
		for (int i = 0; i < linenum; i++)
		{
			cout << line[i] << " ";
		}
		cout << "����վ�����:";
		for (int i = 0; i < adjacentnum; i++) {
			cout << adjacentsite[i] << " ";
		}
		cout << endl;
	}
	void printname() {
		cout << name << endl;
	}
	bool isOnLine(int num)
	{
		for (int i = 0; i < linenum; i++)
		{
			if (num == line[i]) return true;
		}
		return false;
	}
	bool isStation(string name)
	{
		if (name == this->name) return true;
		else return false;
	}
	vector<int> getNextstation() {
		return adjacentsite;
	}
	int getorder() {
		return order;
	}
	string getname() {
		return name;
	}
	vector<int> getline() {
		return line;
	}
	int findNext(int line, vector<station> list, int lastSta)//����һ��������㲻һ����վ�����
	{
		for (int i = 0; i < adjacentnum; i++) {
			if (lastSta == adjacentsite[i])
				continue;
			if (list[adjacentsite[i]].isOnLine(line))
				return adjacentsite[i];
		}
		return -1;//���û�з���-1 
	}
	vector<int> findNext(int line, vector<station> list)//���������������վ�����ڵ�վ��
	{
		vector<int> nextSta;
		for (int i = 0; i < adjacentnum; i++) {
			if (list[adjacentsite[i]].isOnLine(line))
				nextSta.push_back(adjacentsite[i]);
		}
		return nextSta;
	}
	int findNext(int line, vector<station> list, vector<int> used) //��һ���ڽӵ����Ѿ��������ĵ㲻һ����
	{
		for (int i = 0; i < adjacentnum; i++) {
			unsigned int j;
			for (j = 0; j < used.size() && used[j] != adjacentsite[i]; j++);
			if (j != used.size())
				continue;
			if (list[adjacentsite[i]].isOnLine(line))
				return adjacentsite[i];
		}
		return -1;//���û�з���-1 
	}
	void reachableSta(vector<int>& staList, vector<int> lineSta[])
	{
		for (int i = 0; i < linenum; i++) {//ѭ����վ��ļ�����·
			for (unsigned int j = 0; j < lineSta[line[i]].size(); j++) {//ѭ������·�ϵ����е�
				unsigned int k;
				for (k = 0; k < staList.size(); k++) {//���Ҹõ��Ƿ���Ѽ���ü��ϵ����е㶼��ͬ
					if (staList[k] == lineSta[line[i]][j])
						break;
				}
				if (k != staList.size()) {//�õ��Ѵ���
					continue;
				}
				staList.push_back(lineSta[line[i]][j]);
			}
		}
	}



private:
	int order;//վ�����
	string name;//վ������
	int linenum;//���ڵ�����·����
	int adjacentnum;//����վ�����
	vector<int> line;//������·�������
	vector<int> adjacentsite;//����վ�����
};

vector<station> list;//�洢����վ�������
vector<vector<int> > matrix(DATA);//�洢��ͼ���ڽӱ�
vector<vector<int> >matrix_2(16);
vector<int> lineStation[16];//�洢ÿ����·���е�վ��
int startStation[16] = { 0, 38, 39, 54, 66, 108, 109, 38, 138, 160, 170, 180, 192, 210, 65, 185 };//��¼����·����±�
vector<int> transfer[10];//�洢���㻻��ʱ�ľ���,���¼�����ÿ�μ����궼Ҫ����
vector<int> route;//�洢Ŀǰ�ҵ�����̵�·��
int shortestLength = MAX;//Ŀǰ�ҵ�����̵�·�߳���
int num;//��������



int getnum(string);
vector<int> shortestPath(int begin, int end,vector<vector<int> > matrix);
vector<int> publicline(station sta_1, station sta_2);

void getfiledata();




void printShortest_path(string sta_1,string sta_2) {
	int begin = getnum(sta_1);//������
	int end = getnum(sta_2);
	vector<int> path = shortestPath(begin, end, matrix);

	int currentline = publicline(list[path[path.size() - 1]], list[path[path.size() - 2]])[0];
	cout << "����" << linename[currentline] << " " << list[path[path.size() - 1]].getname() << "�ϳ�" << endl;
	for (int i = path.size() - 2; i > 0; i--) {

		int nextline = publicline(list[path[i]], list[path[i - 1]])[0];
		if (currentline != nextline) {
			cout << list[path[i]].getname() << " ���� " << linename[nextline] << endl;
		}
		else cout << list[path[i]].getname() << endl;
		currentline = nextline;
	}
	cout << list[path[0]].getname() << endl;
	cout << ";����" << path.size() << "վ" << endl;
}


void searchAllStations(int line)
{
	int current, next;
	current = startStation[line];
	lineStation[line].push_back(current);
	while (true) {
		next = -1;
		next = list[current].findNext(line, list, lineStation[line]);
		if (next == -1)
			break;
		current = next;
		lineStation[line].push_back(current);
	}
	return;
}//�õ�ÿ�����ϵ�����վ��

void allStations(string name)//��ӡ����������·������վ��
{

	int line;
	if (name == "����һ����") {
		line = 0;
	}
	else if (name == "����������") {
		line = 1;
	}
	else if (name == "����������") {
		line = 2;
	}
	else if (name == "���������߱��Ӷ�") {
		line = 3;
	}
	else if (name == "�����ĺ���") {
		line = 4;
	}
	else if (name == "���������") {
		line = 5;
	}
	else if (name == "����������") {
		line = 6;
	}
	else if (name == "�����ߺ���") {
		line = 7;
	}
	else if (name == "�����˺���") {
		line = 8;
	}
	else if (name == "�����ź���") {
		line = 9;
	}
	else if (name == "����ʮ������") {
		line = 10;
	}
	else if (name == "����ʮ�ĺ���") {
		line = 11;
	}
	else if (name == "������ʮһ����") {
		line = 12;
	}
	else if (name == "���������") {
		line = 13;
	}
	else if (name == "����APM��") {
		line = 14;
	}
	else if (name == "����ʮ�ĺ���֧��") {
		line = 15;
	}
	else {
		cout << "�������������" << endl;
		return;
	}
	for (unsigned int i = 0; i < lineStation[line].size(); i++)
		list[lineStation[line][i]].printname();
	return;
}

void printDirect(int sta1, int sta2, int flag)
{
	vector<int> common = publicline(list[sta1], list[sta2]);
	int minLength = MAX;
	int minNo = -1;
	vector<int> rou[10];
	int rouLine[10];
	int m = 0;
	for (unsigned int i = 0; i < common.size(); i++) {
		vector<int> nextSta = list[sta1].findNext(common[i], list);
		for (unsigned int j = 0; j < nextSta.size(); j++) {
			if (sta2 == nextSta[j]) {
				if (flag == 0) {
					cout << list[sta1].getname() << endl;
				}
				else if (flag == 1) {
					cout << list[sta1].getname() << " ���� " << linename[common[i]] << endl;
				}
				return;
			}
			rouLine[m] = common[i];
			rou[m].push_back(sta1);
			int length = 1;
			int lastSta = sta1;
			int currentSta = nextSta[j];
			rou[m].push_back(currentSta);
			int next = -1;
			while (currentSta != sta2) {
				next = list[currentSta].findNext(common[i], list, lastSta);
				if (next == -1)
					break;
				length++;
				lastSta = currentSta;
				currentSta = next;
				rou[m].push_back(currentSta);
			}
			m++;
			if (next == -1)
				continue;
			if (length < minLength) {
				minLength = length;
				minNo = m - 1;
			}

		}
	}
	if (minNo != -1) {
		if (flag == 0) {
			cout << list[rou[minNo][0]].getname() << endl;
		}
		else {
			cout << list[rou[minNo][0]].getname() << " ���� " << linename[rouLine[minNo]] << endl;
		}
		for (unsigned int i = 1; i < rou[minNo].size() - 1; i++) {
			cout << list[rou[minNo][i]].getname() << endl;
		}
	}
}

void printTransfer()
{

	for (int i = route.size() - 1; i >= 1; i--) {
		if (i == route.size() - 1) {
			printDirect(route[i], route[i - 1], 0);
		}
		else {
			printDirect(route[i], route[i - 1], 1);
		}
	}
	cout << list[route[0]].getname() << endl;
	cout << ";��: " <<(shortestLength + 1) << "վ" << endl;

	for (int i = 0; i < 10; i++)
		transfer[i].clear();
	route.clear();
	shortestLength = MAX;
	num = 0;
}

int directLength(int sta1, int sta2)
{
	vector<int> common = publicline(list[sta1], list[sta2]);
	int minLength = MAX;
	for (unsigned int i = 0; i < common.size(); i++) {
		vector<int> nextSta = list[sta1].findNext(common[i], list);
		for (unsigned int j = 0; j < nextSta.size(); j++) {
			if (sta2 == nextSta[j])
				return 1;
			int length = 1;
			int lastSta = sta1;
			int currentSta = nextSta[j];
			int next = -1;
			while (currentSta != sta2) {
				next = list[currentSta].findNext(common[i], list, lastSta);
				if (next == -1)
					break;
				length++;
				lastSta = currentSta;
				currentSta = next;
			}
			if (next == -1)
				continue;
			if (length < minLength)
				minLength = length;
		}
	}
	return minLength;
}

//�ݹ�
void transfer3(vector<int> sta, vector<int> ro, int m, int length)
{
	//���mΪ0,�����ݹ�
	if (m == -1) {
		int l = directLength(transfer[0][0], ro.back()) + length;
		if (l < shortestLength) {
			shortestLength = l;
			route.clear();
			for (unsigned int i = 0; i < ro.size(); i++)
				route.push_back(ro[i]);
			route.push_back(transfer[0][0]);
		}
		return;
	}

	for (unsigned int i = 0; i < sta.size(); i++) {
		//����sta�е�ÿ����sta[i],���sta[i]������·��transfer[m]�Ľ���
		//�ݹ�ʱ��·��
		vector<int> rou;
		for (unsigned int j = 0; j < ro.size(); j++)
			rou.push_back(ro[j]);
		//�����ro.back()��sta[i]�ľ����ټ���ԭ���ľ��룬���̫Զ�Ͳ�������
		int l = length + directLength(sta[i], ro.back());
		if (l > shortestLength)
			continue;
		rou.push_back(sta[i]);
		//sta[i]��ֱ�ӵ����վ��
		vector<int> sta1;
		list[sta[i]].reachableSta(sta1, lineStation);
		//��sta1��transfer[m-1]�Ľ���
		vector<int> intersection;
		for (unsigned int j = 0; j < sta1.size(); j++) {
			unsigned int k;
			for (k = 0; k < transfer[m].size(); k++) {
				if (transfer[m][k] == sta1[j])
					break;
			}
			if (k != transfer[m].size()) {
				intersection.push_back(sta1[j]);
			}
		}
		transfer3(intersection, rou, m - 1, l);
	}
	if (m == num - 1) {
		printTransfer();
	}
}

//��������
void transfer2(int sta1, int sta2)
{
	//ÿ����һ�Σ����ɵ����վ�������µľ���
	transfer[0].push_back(sta1);
	list[sta1].reachableSta(transfer[1], lineStation);
	unsigned int n;
	for (n = 0; n < transfer[1].size(); n++)
		if (sta2 == transfer[1][n])
			break;
	if (n != transfer[1].size()) {//���sta2��transfer[1]�����ֱ�ӵ���
		route.push_back(sta2);
		route.push_back(sta1);
		shortestLength = directLength(sta1, sta2);
		printTransfer();
		return;
	}

	//stopStaΪĿ�ĵ�վ���ֱ�ӵ����վ��
	vector<int> stopSta;
	list[sta2].reachableSta(stopSta, lineStation);
	int m = 1;//mΪĿǰ�����Ĳ���
	vector<int> intersection;
	while (true) {
		//��transfer[m]��stopSta�Ľ���
		//������Ϊ�գ����������
		intersection.clear();
		for (unsigned int i = 0; i < stopSta.size(); i++) {
			unsigned int j;
			for (j = 0; j < transfer[m].size(); j++) {
				if (stopSta[i] == transfer[m][j])
					break;
			}
			if (j != transfer[m].size()) {
				intersection.push_back(stopSta[i]);
			}
		}

		if (intersection.size() == 0) {
			for (unsigned int i = 0; i < transfer[m].size(); i++) {
				list[transfer[m][i]].reachableSta(transfer[m + 1], lineStation);
			}
			for (int i = 0; i <= m; i++) {
				//��transfer[m+1]�г�ȥtransfer[i]���е�Ԫ��
				for (unsigned int j = 0; j < transfer[m + 1].size(); j++) {
					unsigned int k;
					for (k = 0; k < transfer[i].size(); k++) {
						if (transfer[m + 1][j] == transfer[i][k])
							break;
					}
					if (k != transfer[i].size()) {
						transfer[m + 1].erase(transfer[m + 1].begin() + j);
						j--;
					}
				}
			}
			m++;
		}
		else {
			break;
		}
	}



	vector<int> ro;
	ro.push_back(sta2);
	num = m;
	transfer3(intersection, ro, m - 1, 0);
}


void transfer1(string name1, string name2)
{
	int sta1 = getnum(name1);
	int sta2 = getnum(name2);
	if (sta1 == -1 || sta2 == -1) {
		cerr << "�������������" << endl;
		return;
	}
	transfer2(sta1, sta2);
}

int main() {
	getfiledata();
	for (int i = 0; i < 16; i++) {
		searchAllStations(i);
	}
	while (1) {
		cout << "---------------���ݵ�����ѯϵͳ---------------" << endl;
		cout << "1.��ѯ������·վ����Ϣ" << endl;
		cout << "2.���·�̷���" << endl;
		cout << "3.���ٻ��˷���" << endl;
        cout << "4.�˳�ϵͳ" << endl;
		int choose;
		cout << "����������ѡ��";
		cin >> choose;
		if (choose == 1) {
			string linename;
			cout << "�������ѯ��·���ƣ�";
			cin >> linename;
			allStations(linename);
		}
		else if (choose == 2) {
			string sta1, sta2;
			cout << "��������ʼ��:";
			cin >> sta1;
			cout << "�������յ�:";
			cin >> sta2;
			/*vector<int> tmp = shortestPath(getnum(sta1), getnum(sta2), matrix);
			for (int i = 0; i < tmp.size(); i++) {
				cout << tmp[i] << list[tmp[i]].getname() << endl;
			}*/
			printShortest_path(sta1, sta2);

		}
		else if (choose == 3) {
			string sta1, sta2;
			cout << "��������ʼ��:";
			cin >> sta1;
			cout << "�������յ�:";
			cin >> sta2;
			transfer1(sta1, sta2);
		}
        else if(choose == 4){
        	cout << "��лʹ�ã�" << endl; 
            exit(0);
        }
		else {
			cout << "�����������������룡";
			continue;
		}
	}
	return 0;
}

int getnum(string name)//��������վ������ȷ��վ����ţ���������򷵻�-1
{
	for (unsigned int i = 0; i < list.size(); i++) {
		if (list[i].isStation(name)) return i;
	}
	return -1;
}

vector<int> shortestPath(int begin, int end,vector<vector<int> > matrix)
{
	vector<int> path;
	queue<int> q;
	int parent[DATA] = { 0 };
	int book[DATA] = { 0 };
	parent[begin] = -1;
	q.push(begin);
	while (!q.empty())
	{
		int tmp = q.front();
		q.pop();
		book[tmp] = 1;
		if (tmp == end) break;//������������
		for (unsigned int i = 0; i < matrix[tmp].size(); i++) {
			if (book[matrix[tmp][i]] == 0) {
				q.push(matrix[tmp][i]);
				parent[matrix[tmp][i]] = tmp;
				book[matrix[tmp][i]] = 1;
			}
		}

	}
	int cnt = end;
	while (parent[cnt] != -1)
	{
		path.push_back(cnt);
		cnt = parent[cnt];
	}
	path.push_back(cnt);
	return path;

}//����BFS�㷨������·��

vector<int> publicline(station sta_1, station sta_2)
{
	vector<int> ans;
	vector<int> line_1 = sta_1.getline();
	vector<int> line_2 = sta_2.getline();
	for (unsigned int i = 0; i < line_1.size(); i++) {
		for (unsigned int j = 0; j < line_2.size(); j++) {
			if (line_1[i] == line_2[j]) ans.push_back(line_1[i]);
		}
	}
	return ans;
}


void getfiledata()
{
	ifstream in;

	char file_name[] = "guangzhou-subway.txt";
	in.open(file_name);
	if (!in) {
		cerr << "���ļ�����" << endl;
		return;
	}
	char ch[100];
	while (!in.eof())
	{
		in.getline(ch, 100);
		int order, linenum, adjacentnum;
		vector<int>line, adjacentsite;
		int k = 0, sum;
		for (sum = 0; isdigit(ch[k]) && k < 100; k++) {
			sum = sum * 10 + (ch[k] - '0');
		}
		order = sum;//��ȡվ�����
		for (; ch[k] == ' ' || ch[k] == '\t'; k++);
		//��ȥ�ո���Ʊ��
		string name = "";
		for (; ch[k] != ' ' && ch[k] != '\t'; k++) {
			name += ch[k];
		}//��ȡվ������
		for (; ch[k] == ' ' || ch[k] == '\t'; k++);
		//��ȥ�ո���Ʊ��
		for (sum = 0; isdigit(ch[k]) && k < 100; k++) {
			sum = sum * 10 + (ch[k] - '0');
		}
		linenum = sum;//��ȡ������·����
		for (int i = 0; i < linenum; i++) {
			//��ȥ�ո���Ʊ��
			for (; (ch[k] == ' ' || ch[k] == '\t') && k < 100; k++);

			for (sum = 0; isdigit(ch[k]) && k < 100; k++)
				sum = sum * 10 + (ch[k] - '0');
			line.push_back(sum);
		}//��ȡ������·���
		for (; (ch[k] == ' ' || ch[k] == '\t') && k < 100; k++);
		//��ȥ�ո���Ʊ��
		for (sum = 0; isdigit(ch[k]) && k < 100; k++)
			sum = sum * 10 + (ch[k] - '0');
		adjacentnum = sum;//��ȡ����վ�����
		for (int i = 0; i < adjacentnum; i++) {
			//��ȥ�ո���Ʊ��
			for (; (ch[k] == ' ' || ch[k] == '\t') && k < 100; k++);

			for (sum = 0; isdigit(ch[k]) && k < 100; k++)
				sum = sum * 10 + (ch[k] - '0');
			adjacentsite.push_back(sum);
		}//��ȡ����վ�����
		for (unsigned int i = 0; i < adjacentsite.size(); i++)
			matrix[order].push_back(adjacentsite[i]);
		//�����ڽӱ�
		station s(order, name, linenum, adjacentnum, line, adjacentsite);
		list.push_back(s);
		//��ȡ��ϼ���վ���б�
	}
	in.close();
}

