#include "easyx.h"
#include "graphics.h"
#include <strstream>
#include <iostream>
#include <ctime>
#pragma comment(lib, "Winmm.lib")
#pragma warning(disable:4996)
#define OPP(c) ((c == 'B') ? 'W' : 'B')
using namespace std;

/*******************************定义全局变量*****************************/
const int difficult = 6;		// 难度，递归次数
const int Move[8][2] = {{-1, 0}, {1, 0}, 
						{0, -1}, {0, 1},
						{-1, -1}, {1, -1}, 
						{1, 1}, {-1, 1}};
								// 八个方向扩展
char map[8][8];					// 棋盘
IMAGE img[5];					// 保存pic
int black, white;				// 双方的棋子数
int X, Y;						// 白棋的下子点
int Round = 1;					// 开局数
/**********************************函数声明*****************************/
void load(void);				// 加载素材
void draw(void);				// 画棋盘，同时记录双方棋子个数
void put_down(int, int, char);	// 下当前子，同时翻转
int judge(int, int, char);		// 判断当前是否可以落下，返回可吃棋数，有棋可吃才可下
bool has_chance(char);			// 判断整盘棋局是否有可下位置
bool is_lose(char);				// 判断是否有棋存活
bool ask(void);					// 弹出对话框
int DP_with_minmax(char, int);	// 动态规划，本质minmax，找到规定步数内，我方能够吃子最多的下一步
void play_b(void);				// 作为黑棋先手
void play_w(void);				// 作为白棋后手

/**********************************定义函数*****************************/
void load(void)		// 加载素材
{
	// 加载pic
	loadimage(&img[0], "pic\\Blank.bmp");
	loadimage(&img[1], "pic\\Black.bmp");
	loadimage(&img[2], "pic\\White.bmp");
	loadimage(&img[3], "pic\\Black1.bmp");
	loadimage(&img[4], "pic\\White1.bmp");
	//初始化棋盘
	initgraph(340, 340);
	IMAGE board;
	loadimage(&board, "pic\\Board.bmp");
	putimage(0, 0, &board);
	setorigin(26, 26);
	SetWindowText(GetHWnd(), "Reversi");
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(GetHWnd(), &rect);
	//重新设置rect里的值
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	SetWindowPos(GetHWnd(), HWND_TOP, rect.left, rect.top, 356, 379, SWP_SHOWWINDOW);
}

void draw(void)	// 画棋盘,同时记录双方棋子个数
{
	int x, y;
	black = white = 0;
	for(x = 0; x < 8; x++)
		for(y = 0; y < 8; y++)
			switch(map[x][y]){
				case 0:
						putimage(37 * y, 37 * x, &img[0]);
					break;
				case 'B':
						putimage(37 * y, 37 * x, &img[1]);
					black++;
					break;
				case 'W':
						putimage(37 * y, 37 * x, &img[2]);
					white++;
					break;
			}
}

void put_down(int x, int y, char a)	// 下当前子
{
	char b = OPP(a);// 敌方子
	int i, x1, y1, x2, y2;
	bool sign;			
	for (i = 0; i < 8; i++){
		sign = false;
		x1 = x + Move[i][0];
		y1 = y + Move[i][1];
		while (0 <= x1 && x1 < 8 && 0 <= y1 && y1 < 8 && map[x1][y1]){//在棋盘内，且该位置有子
			if(map[x1][y1] == b)
				sign = true;
			else{//找到了本方棋子，开始翻转范围内的敌方棋子
				if(sign){
					x1 -= Move[i][0];
					y1 -= Move[i][1];
					x2 = x + Move[i][0];
					y2 = y + Move[i][1];
					while (((x <= x2 && x2 <= x1) || (x1 <= x2 && x2 <= x)) && ((y <= y2 && y2 <= y1) || (y1 <= y2 && y2 <= y))){
						map[x2][y2] = a;
						x2 += Move[i][0];
						y2 += Move[i][1];
					}
				}
				break;
			}
			x1 += Move[i][0];
			y1 += Move[i][1];
		}
	}
	map[x][y] = a;
}

int judge(int x, int y, char a)	// 判断当前是否可以落下，返回可吃棋数，有棋可吃才可下
{
	if(map[x][y])// 如果当前不是空的返回0值
		return 0;
	char b = OPP(a);//敌方下子
	int i, x1, y1;
	int n = 0, sign;
	for (i = 0; i < 8; i++){//向八个方向扩展搜索
		sign = 0;
		x1 = x + Move[i][0];
		y1 = y + Move[i][1];
		while (0 <= x1 && x1 < 8 && 0 <= y1 && y1 < 8 && map[x1][y1]){//一直沿着某一个方向扩展
			if(map[x1][y1] == b)//遇到敌方棋子，sign加一
				sign++;
			else{//沿着这个方向找到第一个本方棋子(while中的map[x1][y1]判断保证了这一点)，n记录下总共遇到的敌方棋子数
				n += sign;
				break;
			}
			x1 += Move[i][0];
			y1 += Move[i][1];
		}
	}
	return n;// 返回可吃棋数
}

bool has_chance(char c)	// 判断整盘棋局是否有可下位置
{
	int x, y;
	for(x = 0; x < 8; x++)//只要存在一个可下的地方就返回true
		for(y = 0; y < 8; y++)
			if(judge(x, y, c))
				return true;
	return false;
}

bool is_lose(char c)// 判断是否有棋存活
{
	//bool b = false, w = false;
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++){
			if(map[i][j] == c)
				return false;
		}
	return true;
}

bool ask(void)	// 弹出对话框
{
	HWND wnd = GetHWnd();
	int key;
	char str[50];
	ostrstream strout(str, 50);
	strout <<"黑：" <<black <<"  白：" <<white <<endl;
	if (black == white)
		strout <<"平局!";
	else if(black > white)
		strout <<"你赢了!";
	else
		strout <<"你输了!";
	strout <<"\n再来一局吗？" <<ends;
	if(black == white)
		key = MessageBox(wnd, str, "和局", MB_YESNO | MB_ICONQUESTION);
	else if(black > white)
		key = MessageBox(wnd, str, "黑胜", MB_YESNO | MB_ICONQUESTION);
	else
		key = MessageBox(wnd, str, "白胜", MB_YESNO | MB_ICONQUESTION);
	if (key == IDYES) {
		Round++;
		return true;
	}
	else return false;
}

int DP_with_minmax(char c, int step)//下一步能吃到最多的敌方棋子为最优
{
	// 判断是否结束递归
	if (step > difficult)
		return 0;
	if (!has_chance(c)){
		if (has_chance(OPP(c)))
			return -DP_with_minmax(OPP(c), step);
		else
			return 0;
	}

	int i, j, max = 0, temp, x, y;
	bool ans = false;

	// 建立临时数组,保留当前棋局
	char **t = new char *[8];
	for (i = 0; i < 8; i++)
		t[i] = new char [8];
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			t[i][j] = map[i][j];

	// 搜索解法
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)//N^2
			if (temp = judge(i, j, c)){//N^2
				put_down(i, j, c);//N^3
				temp -= DP_with_minmax(OPP(c), step + 1);
				if (temp > max || !ans){
					max = temp;
					x = i;
					y = j;
					ans = true;
				}
				for (int k = 0; k < 8; k++)
					for (int l = 0; l < 8; l++)
						map[k][l] = t[k][l];
			}

	// 撤销空间
	for (i = 0; i < 8; i++)
		delete[] t[i];
	delete[] t;

	// 如果是第一步则标识白棋下子点
	if (step == 1){
		X = x;
		Y = y;
	}

	return max;// 返回最优解
}

void play_b(void)// 游戏过程
{
	// 初始化棋盘
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
			map[i][j] = 0;
	map[3][4] = map[4][3] = 'B';
	map[3][3] = map[4][4] = 'W';
	draw();
	MOUSEMSG m;
	int total_time = 0;//AI总用时
	// 开始游戏
	do
	{
		if (has_chance('B'))// 如果玩家有下子位置							
		{
			int x, y;
			while (true) {
				while (true){
					m = GetMouseMsg();// 获取鼠标消息
					if (m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
						// 如果左键点击
						break;
				}
				x = (m.y - 26) / 37;
				y = (m.x - 26) / 37;
				if (judge(x, y, 'B')){// 如果当前位置有效
					put_down(x, y, 'B');//下子，同时翻转
					draw();
					putimage(37 * y, 37 * x, &img[3]);// 标识下子点
					break;
				}
			}
			if (is_lose('W'))// 计算机是否失败
				break;
		}

		if (has_chance('W')){// 如果计算机有下子位置
			clock_t start, end;
			start = clock();
			DP_with_minmax('W', 1);// 搜索解法
			end = clock();
			std::cout << "AI: " << '(' << X + 1 << ',' << char(Y + 65) << ')'
				<< " with " << end - start << "ms" << endl;
			total_time += end - start;
			put_down(X, Y, 'W');
			draw();
			putimage(37 * Y, 37 * X, &img[4]);	// 标识下子点
			if (is_lose('B'))// 玩家是否失败
				break;
		}
	}while (has_chance('B') || has_chance ('W'));
	std::cout << "Total time: " << 
		(double)total_time / CLOCKS_PER_SEC << "sec" << endl;
}

void play_w(void) {
	// 初始化棋盘
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			map[i][j] = 0;
	map[3][4] = map[4][3] = 'B';
	map[3][3] = map[4][4] = 'W';
	draw();
	MOUSEMSG m;
	int total_time = 0;//AI总用时
					   // 开始游戏
	do
	{
		if (has_chance('B')) {// 如果计算机有下子位置
			clock_t start, end;
			start = clock();
			DP_with_minmax('B', 1);// 搜索解法
			end = clock();
			std::cout << "AI: " << '(' << X + 1 << ',' << char(Y + 65) << ')'
				<< " with " << end - start << "ms" << endl;
			total_time += end - start;
			put_down(X, Y, 'B');
			draw();
			putimage(37 * Y, 37 * X, &img[3]);	// 标识下子点
			if (is_lose('W'))// 玩家是否失败
				break;
		}

		if (has_chance('W'))// 如果玩家有下子位置							
		{
			int x, y;
			while (true) {
				while (true) {
					m = GetMouseMsg();// 获取鼠标消息
					if (m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
						// 如果左键点击
						break;
				}
				x = (m.y - 26) / 37;
				y = (m.x - 26) / 37;
				if (judge(x, y, 'W')) {// 如果当前位置有效
					put_down(x, y, 'W');//下子，同时翻转
					draw();
					putimage(37 * y, 37 * x, &img[4]);// 标识下子点
					break;
				}
			}
			if (is_lose('B'))// 计算机是否失败
				break;
		}
	} while (has_chance('B') || has_chance('W'));
	std::cout << "Total time: " <<
		(double)total_time / CLOCKS_PER_SEC << "sec" << endl;
}
// 主函数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	load();
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
	do{
		std::cout << "**** " << "Round " << Round << " ****" << endl;
		HWND wnd = GetHWnd();
		int key;
		key = MessageBox(wnd, "我方先手？", "选择先后手", MB_YESNO | MB_ICONQUESTION);
		if (key == IDYES)	play_b();
		else				play_w();
	} while(ask());
	closegraph();
	FreeConsole();
	return 0;
}