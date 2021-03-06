// Guangzhou_subway.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

string linename[16]= { "地铁一号线","地铁二号线", "地铁三号线", "地铁三号线北延段","地铁四号线" ,"地铁五号线", "地铁六号线", "地铁七号线", "地铁八号线",
					"地铁九号线", "地铁十三号线", "地铁十四号线", "地铁二十一号线", "地铁广佛线","地铁APM线", "地铁十四号线支线" };
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
		cout << "站点序号:" << order << " ";
		cout << "站点名称:" << name << " ";
		cout << "所属线路:";
		for (int i = 0; i < linenum; i++)
		{
			cout << line[i] << " ";
		}
		cout << "相连站点序号:";
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
	int findNext(int line, vector<station> list, int lastSta)//返回一个与输入点不一样的站点序号
	{
		for (int i = 0; i < adjacentnum; i++) {
			if (lastSta == adjacentsite[i])
				continue;
			if (list[adjacentsite[i]].isOnLine(line))
				return adjacentsite[i];
		}
		return -1;//如果没有返回-1 
	}
	vector<int> findNext(int line, vector<station> list)//返回这条线上与此站点相邻的站点
	{
		vector<int> nextSta;
		for (int i = 0; i < adjacentnum; i++) {
			if (list[adjacentsite[i]].isOnLine(line))
				nextSta.push_back(adjacentsite[i]);
		}
		return nextSta;
	}
	int findNext(int line, vector<station> list, vector<int> used) //找一个邻接点与已经遍历过的点不一样的
	{
		for (int i = 0; i < adjacentnum; i++) {
			unsigned int j;
			for (j = 0; j < used.size() && used[j] != adjacentsite[i]; j++);
			if (j != used.size())
				continue;
			if (list[adjacentsite[i]].isOnLine(line))
				return adjacentsite[i];
		}
		return -1;//如果没有返回-1 
	}
	void reachableSta(vector<int>& staList, vector<int> lineSta[])
	{
		for (int i = 0; i < linenum; i++) {//循环该站点的几条线路
			for (unsigned int j = 0; j < lineSta[line[i]].size(); j++) {//循环该线路上的所有点
				unsigned int k;
				for (k = 0; k < staList.size(); k++) {//查找该点是否和已加入该集合的所有点都不同
					if (staList[k] == lineSta[line[i]][j])
						break;
				}
				if (k != staList.size()) {//该点已存在
					continue;
				}
				staList.push_back(lineSta[line[i]][j]);
			}
		}
	}



private:
	int order;//站点序号
	string name;//站点名字
	int linenum;//所在地铁线路个数
	int adjacentnum;//相邻站点个数
	vector<int> line;//所属线路标号序列
	vector<int> adjacentsite;//相邻站点个数
};

vector<station> list;//存储所有站点的容器
vector<vector<int> > matrix(DATA);//存储点图的邻接表
vector<vector<int> >matrix_2(16);
vector<int> lineStation[16];//存储每条线路所有的站点
int startStation[16] = { 0, 38, 39, 54, 66, 108, 109, 38, 138, 160, 170, 180, 192, 210, 65, 185 };//记录各线路起点下标
vector<int> transfer[10];//存储计算换乘时的矩阵,以下几个量每次计算完都要清零
vector<int> route;//存储目前找到的最短的路线
int shortestLength = MAX;//目前找到的最短的路线长度
int num;//迭代层数



int getnum(string);
vector<int> shortestPath(int begin, int end,vector<vector<int> > matrix);
vector<int> publicline(station sta_1, station sta_2);

void getfiledata();




void printShortest_path(string sta_1,string sta_2) {
	int begin = getnum(sta_1);//获得序号
	int end = getnum(sta_2);
	vector<int> path = shortestPath(begin, end, matrix);

	int currentline = publicline(list[path[path.size() - 1]], list[path[path.size() - 2]])[0];
	cout << "请在" << linename[currentline] << " " << list[path[path.size() - 1]].getname() << "上车" << endl;
	for (int i = path.size() - 2; i > 0; i--) {

		int nextline = publicline(list[path[i]], list[path[i - 1]])[0];
		if (currentline != nextline) {
			cout << list[path[i]].getname() << " 换乘 " << linename[nextline] << endl;
		}
		else cout << list[path[i]].getname() << endl;
		currentline = nextline;
	}
	cout << list[path[0]].getname() << endl;
	cout << "途径：" << path.size() << "站" << endl;
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
}//得到每条线上的所有站点

void allStations(string name)//打印所给地铁线路上所有站点
{

	int line;
	if (name == "地铁一号线") {
		line = 0;
	}
	else if (name == "地铁二号线") {
		line = 1;
	}
	else if (name == "地铁三号线") {
		line = 2;
	}
	else if (name == "地铁三号线北延段") {
		line = 3;
	}
	else if (name == "地铁四号线") {
		line = 4;
	}
	else if (name == "地铁五号线") {
		line = 5;
	}
	else if (name == "地铁六号线") {
		line = 6;
	}
	else if (name == "地铁七号线") {
		line = 7;
	}
	else if (name == "地铁八号线") {
		line = 8;
	}
	else if (name == "地铁九号线") {
		line = 9;
	}
	else if (name == "地铁十三号线") {
		line = 10;
	}
	else if (name == "地铁十四号线") {
		line = 11;
	}
	else if (name == "地铁二十一号线") {
		line = 12;
	}
	else if (name == "地铁广佛线") {
		line = 13;
	}
	else if (name == "地铁APM线") {
		line = 14;
	}
	else if (name == "地铁十四号线支线") {
		line = 15;
	}
	else {
		cout << "输入错误，请重试" << endl;
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
					cout << list[sta1].getname() << " 换乘 " << linename[common[i]] << endl;
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
			cout << list[rou[minNo][0]].getname() << " 换乘 " << linename[rouLine[minNo]] << endl;
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
	cout << "途径: " <<(shortestLength + 1) << "站" << endl;

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

//递归
void transfer3(vector<int> sta, vector<int> ro, int m, int length)
{
	//如果m为0,结束递归
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
		//对于sta中的每个点sta[i],求出sta[i]所在线路与transfer[m]的交集
		//递归时的路线
		vector<int> rou;
		for (unsigned int j = 0; j < ro.size(); j++)
			rou.push_back(ro[j]);
		//计算从ro.back()到sta[i]的距离再加上原来的距离，如果太远就不考虑了
		int l = length + directLength(sta[i], ro.back());
		if (l > shortestLength)
			continue;
		rou.push_back(sta[i]);
		//sta[i]能直接到达的站点
		vector<int> sta1;
		list[sta[i]].reachableSta(sta1, lineStation);
		//求sta1和transfer[m-1]的交集
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

//换乘最少
void transfer2(int sta1, int sta2)
{
	//每换乘一次，将可到达的站点填入新的矩阵
	transfer[0].push_back(sta1);
	list[sta1].reachableSta(transfer[1], lineStation);
	unsigned int n;
	for (n = 0; n < transfer[1].size(); n++)
		if (sta2 == transfer[1][n])
			break;
	if (n != transfer[1].size()) {//如果sta2在transfer[1]里，可以直接到达
		route.push_back(sta2);
		route.push_back(sta1);
		shortestLength = directLength(sta1, sta2);
		printTransfer();
		return;
	}

	//stopSta为目的地站点可直接到达的站点
	vector<int> stopSta;
	list[sta2].reachableSta(stopSta, lineStation);
	int m = 1;//m为目前迭代的层数
	vector<int> intersection;
	while (true) {
		//求transfer[m]和stopSta的交集
		//若交集为空，则继续迭代
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
				//在transfer[m+1]中除去transfer[i]中有的元素
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
		cerr << "输入错误，请重试" << endl;
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
		cout << "---------------广州地铁查询系统---------------" << endl;
		cout << "1.查询地铁线路站点信息" << endl;
		cout << "2.最短路程方案" << endl;
		cout << "3.最少换乘方案" << endl;
        cout << "4.退出系统" << endl;
		int choose;
		cout << "请输入您的选择：";
		cin >> choose;
		if (choose == 1) {
			string linename;
			cout << "请输入查询线路名称：";
			cin >> linename;
			allStations(linename);
		}
		else if (choose == 2) {
			string sta1, sta2;
			cout << "请输入起始点:";
			cin >> sta1;
			cout << "请输入终点:";
			cin >> sta2;
			/*vector<int> tmp = shortestPath(getnum(sta1), getnum(sta2), matrix);
			for (int i = 0; i < tmp.size(); i++) {
				cout << tmp[i] << list[tmp[i]].getname() << endl;
			}*/
			printShortest_path(sta1, sta2);

		}
		else if (choose == 3) {
			string sta1, sta2;
			cout << "请输入起始点:";
			cin >> sta1;
			cout << "请输入终点:";
			cin >> sta2;
			transfer1(sta1, sta2);
		}
        else if(choose == 4){
        	cout << "感谢使用！" << endl; 
            exit(0);
        }
		else {
			cout << "输入有误，请重新输入！";
			continue;
		}
	}
	return 0;
}

int getnum(string name)//根据输入站点名称确定站点序号，输入错误则返回-1
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
		if (tmp == end) break;//！！！！！！
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

}//根据BFS算法求得最短路径

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
		cerr << "打开文件出错" << endl;
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
		order = sum;//读取站点序号
		for (; ch[k] == ' ' || ch[k] == '\t'; k++);
		//略去空格和制表符
		string name = "";
		for (; ch[k] != ' ' && ch[k] != '\t'; k++) {
			name += ch[k];
		}//读取站点名称
		for (; ch[k] == ' ' || ch[k] == '\t'; k++);
		//略去空格和制表符
		for (sum = 0; isdigit(ch[k]) && k < 100; k++) {
			sum = sum * 10 + (ch[k] - '0');
		}
		linenum = sum;//读取所属线路个数
		for (int i = 0; i < linenum; i++) {
			//略去空格和制表符
			for (; (ch[k] == ' ' || ch[k] == '\t') && k < 100; k++);

			for (sum = 0; isdigit(ch[k]) && k < 100; k++)
				sum = sum * 10 + (ch[k] - '0');
			line.push_back(sum);
		}//读取所属线路标号
		for (; (ch[k] == ' ' || ch[k] == '\t') && k < 100; k++);
		//略去空格和制表符
		for (sum = 0; isdigit(ch[k]) && k < 100; k++)
			sum = sum * 10 + (ch[k] - '0');
		adjacentnum = sum;//读取相邻站点个数
		for (int i = 0; i < adjacentnum; i++) {
			//略去空格和制表符
			for (; (ch[k] == ' ' || ch[k] == '\t') && k < 100; k++);

			for (sum = 0; isdigit(ch[k]) && k < 100; k++)
				sum = sum * 10 + (ch[k] - '0');
			adjacentsite.push_back(sum);
		}//读取相邻站点序号
		for (unsigned int i = 0; i < adjacentsite.size(); i++)
			matrix[order].push_back(adjacentsite[i]);
		//设置邻接表
		station s(order, name, linenum, adjacentnum, line, adjacentsite);
		list.push_back(s);
		//读取完毕加入站点列表
	}
	in.close();
}

