#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
#include<easyx.h>
#include<graphics.h>
#include<stdio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
#define row 10
#define col 10
#define gridw 40
int map[row][col] = { 0 };
IMAGE img[12];//保存所有图片
int cnt = 0;//当前点开非雷格数量
bool isover = false;
bool iswin = false;
/*
void playmusic(const char filename[]) {
	char str[50] = " ";
	sprintf_s(str, "close./image/%s", filename);
	mciSendString(str, NULL, 0, NULL);
	sprintf_s(str, "open./image/%s", filename);
	mciSendString(str, NULL, 0, NULL);
	sprintf_s(str, "play./image/%s", filename);
	mciSendString(str, NULL, 0, NULL);
}
void playmusic() {
	mciSendString("close ji", NULL, 0, NULL);
	mciSendString("open ./images/ji.mp3 alias ji", NULL, 0, NULL);
	mciSendString("play ./images/ji.mp3", NULL, 0, NULL);

}
*/
void show() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << std::left << setw(4) << map[i][j];//输出对齐 
		}
		cout << endl;
	}
	cout << endl;
}
void gameinit() {
	//playmusic("start.mp3");
	mciSendString("close start", NULL, 0, NULL);
	mciSendString("open ./images/start.mp3 alias start", NULL, 0, NULL);
	mciSendString("play ./images/start.mp3", NULL, 0, NULL);
	srand((unsigned)time(NULL));//随机数种子;//
	for (int i = 0; i < 10; ) {
		int r = rand() % row;
		int c = rand() % col;
		if (map[r][c] == 0) {
			map[r][c] = -1;
			i++;//只有成功设置雷计数才加 
		}


	} //埋雷 

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j] == -1) {
				for (int r = i - 1; r <= i + 1; r++) {
					for (int c = j - 1; c <= j + 1; c++) {
						if ((r >= 0 && r < row && c < col) && map[r][c] != -1) {//数组越界 
							map[r][c]++;
						}

					}
				}
			}
		}
	}//九宫格计数 
	for (int i = 0; i < 12; i++) {
		char str[50] = " ";
		sprintf_s(str, "./images/%d.jpg", i);
		loadimage(&img[i],str,gridw,gridw);
		
	}//加载图片
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j]+=20;
		}
	}//加密，=+20
}
void gamedraw() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j] >= 0 && map[i][j] <= 8) {
				putimage(j * gridw, i * gridw, &img[map[i][j]]);
			}
			else if (map[i][j] == -1) {
				putimage(j * gridw, i * gridw, &img[9]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) {
				putimage(j * gridw, i * gridw, &img[10]);
			}
			else if (map[i][j] > 28) {
				putimage(j * gridw, i * gridw, &img[11]);
			}
		}
	}
	
}

void opennull(int map[row][col], int Row, int Col);
void judge(int map[row][col],int Row, int Col);
void mouse() {
	MOUSEMSG msg = GetMouseMsg();//获取鼠标消息
	int Row = msg.y / gridw;
	int Col = msg.x / gridw;//把鼠标当前点击的坐标转成二维数组下标
	if (msg.uMsg == WM_LBUTTONDOWN) {//左键解码
		//playmusic("click.wav");
		mciSendString("close click", NULL, 0, NULL);
		mciSendString("open ./images/click.wav alias click", NULL, 0, NULL);
		mciSendString("play ./images/click.wav", NULL, 0, NULL);
		if (map[Row][Col] >= 19) {
			map[Row][Col] -= 20;
			cnt++;
			if (map[Row][Col] == 0) {
				//playmusic("search.wav");
				mciSendString("close search", NULL, 0, NULL);
				mciSendString("open ./images/search.wav alias search", NULL, 0, NULL);
				mciSendString("play ./images/search.wav", NULL, 0, NULL);
			}
			opennull(map,Row,Col);
			judge(map,Row, Col);
		}
		
	}
	else if (msg.uMsg == WM_RBUTTONDOWN) {//右键标记
		//playmusic("rightClick.wav");
		mciSendString("close rightClick", NULL, 0, NULL);
		mciSendString("open ./images/rightClick.wav alias rightClick", NULL, 0, NULL);
		mciSendString("play ./images/rightClick.wav", NULL, 0, NULL);
		if (map[Row][Col] >= 19 && map[Row][Col] <= 28) {//没打开才能标记
			map[Row][Col] += 20;
		}
		else if (map[Row][Col] > 28) {//已标记，那么取消标记
			map[Row][Col] -= 20;
		}
		
	}
	




}
void opennull(int map[row][col],int Row,int Col) {//点到空格炸开一片
	if (map[Row][Col] == 0) {
		for (int i = Row - 1; i <=Row + 1; i++) {
			for (int j = Col - 1; j <= Col + 1; j++) {
				if ((i>=0 && i<row && j>=0 && j< col)&&map[i][j]> 19&&map[i][j]<=28) {
					cnt++;
					map[i][j] -= 20;//如果是空格或数字就直接打开
					opennull(map, i, j);
				}
			}
		}
	}
}
void judge(int map[row][col],int Row, int Col) {
	if (map[Row][Col] == -1) {
		for (int i = 0; i < row; i++)
		{
			for (int k = 0; k < col; k++) {

				if (map[i][k] == 19) {
					map[i][k] -= 20;
				}
				else if (map[i][k] == 39) {
					map[i][k] -= 40;
				}
			}
		}
		isover = true;
		//MessageBox(GetHWnd(), "你做了错饭", "you,lose", MB_OK);//lose
		return;
	}
	

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j] != 19 || map[i][j] != 39) {//不是雷
				cnt++;
				cout << cnt << endl;
			}
		}
	}
	if (cnt == (row * col - 10)) {
		for (int i = 0; i < row; i++)
		{
			for (int k = 0; k < col; k++) {

				if (map[i][k] == 19) {
					map[i][k] -= 20;
				}
				else if (map[i][k] == 39) {
					map[i][k] -= 40;
				}
			}
		}
		iswin = true;
		return;
		//MessageBox(GetHWnd(), "你做了对饭","you,win",MB_OK);
	}
}
int main() {
	initgraph(col * gridw, row * gridw);//创建图形窗口 
	
	
	gameinit();
	
	
	while (1) {
		gamedraw();
		if (isover) {
			int ret = MessageBox(GetHWnd(), "你干嘛，哎呦！", "游戏结束，再来一把吗？", MB_OKCANCEL);
			if (ret == IDOK)
			{
				//gameinit();
				//isover = false;
				exit(0);
			}
			else {
				exit(0);
			}
		}
		else if (iswin) {
			int ret = MessageBox(GetHWnd(), "只因你太美！", "你过关了，再来一把吗？", MB_OKCANCEL);
			if (ret == IDOK)
			{
				exit(0);
				
				
			}
			else {
				exit(0);
			}
		}
		mouse();
		
	}
		





	
	return 0;
}
