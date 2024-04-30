#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<easyx.h>
#include<graphics.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
//闪退
using namespace std;
#define ROW 10
#define COL 10
#define size 40
int map[ROW][COL] = { 0 };
IMAGE img[12];//

void initmap(int map[ROW][COL]);
void loadimg(IMAGE img[]);
void drawmap(int map[ROW][COL], IMAGE img[]);
void mouse(int map[ROW][COL]);
void openNull(int map[ROW][COL], int row, int col);
void judge(int map[ROW][COL], int row, int col);
bool isover = false;
bool iswin = false;
void show(int map[ROW][COL])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++) {
			cout << map[i][j] << " ";

		}
		cout << endl;
	}
	cout << endl;
}
int main()
{
	initgraph(COL * size, ROW * size, EX_SHOWCONSOLE);




	initmap(map);
	loadimg(img);
	show(map);
	while (true) {
		mouse(map);
		drawmap(map, img);
		if (isover) {
			int ret = MessageBox(GetHWnd(), "你做了错饭", "you,lose", MB_OKCANCEL);
			if (ret == IDOK)
			{
				initmap(map);
				isover = false;
			}
			else {
				exit(0);
			}
		}
		else if (iswin) {
			int ret = MessageBox(GetHWnd(), "你做了对饭", "you,lose", MB_OKCANCEL);
			if (ret == IDOK)
			{
				initmap(map);
				iswin = false;
			}
			else {
				exit(0);
			}
		}
	}
	getchar();
	return 0;
}

void initmap(int map[ROW][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] = 0;
		}
	}
	mciSendString("close start", NULL, 0, NULL);
	mciSendString("open ./images/start.mp3 alias start", NULL, 0, NULL);
	mciSendString("play ./images/start.mp3", NULL, 0, NULL);
	srand((unsigned)time(NULL));
	for (int i = 0; i < 10; ) {
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;//只有成功设置雷计数才加 
		}
	}
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1)//lei
			{
				for (int r = i - 1; r <= i + 1; r++) {
					for (int c = j - 1; c <= j + 1; c++) {
						if (r >= 0 && r < ROW && c >= 0 && c < COL && map[r][c] != -1) {//数组越界 
							map[r][c]++;
						}
					}
				}
			}
		}
	}
	//
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] += 20;
		}
	}
}
void loadimg(IMAGE img[]) {
	//加载图片
	for (int i = 0; i < 12; i++) {
		char imgName[50] = { 0 };
		sprintf_s(imgName, "./images/%d.jpg", i);
		loadimage(img + i, imgName, size, size);
	}
}
void drawmap(int map[ROW][COL], IMAGE img[]) {
	//贴图
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] >= 0 && map[i][j] <= 8) {
				putimage(j * 40, i * 40, &img[map[i][j]]);
			}
			else if (map[i][j] == -1) {
				putimage(j * 40, i * 40, img + 9);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) {
				putimage(j * 40, i * 40, img + 10);
			}
			else if (map[i][j] >= 29) {
				putimage(j * 40, i * 40, img + 11);
			}
		}
	}
}
void mouse(int map[ROW][COL]) {
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		int r = msg.y / 40;
		int c = msg.x / 40;
		//左键点击解码-20
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			mciSendString("close click", NULL, 0, NULL);
			mciSendString("open ./images/click.wav alias click", NULL, 0, NULL);
			mciSendString("play ./images/click.wav", NULL, 0, NULL);
			if (map[r][c] >= 19 && map[r][c] <= 28) {
				map[r][c] -= 20;
				if (map[r][c] == 0) {
					mciSendString("close search", NULL, 0, NULL);
					mciSendString("open ./images/search.wav alias search", NULL, 0, NULL);
					mciSendString("play ./images/search.wav", NULL, 0, NULL);
				}
				openNull(map, r, c);
				judge(map, r, c);
				show(map);
			}
		}
		//右键标记
		else if (msg.uMsg == WM_RBUTTONDOWN) {
			mciSendString("close rightClick", NULL, 0, NULL);
			mciSendString("open ./images/rightClick.wav alias rightClick", NULL, 0, NULL);
			mciSendString("play ./images/rightClick.wav", NULL, 0, NULL);
			if (map[r][c] >= 19 && map[r][c] <= 28)
			{
				map[r][c] += 20;
				show(map);
			}
			else if (map[r][c] > 28)//标记过了，取消
			{
				map[r][c] -= 20;
			}
		}
	}
}
void openNull(int map[ROW][COL], int row, int col) {

	if (map[row][col] == 0) {
		for (int i = row - 1; i <= row + 1; i++) {
			for (int j = col - 1; j <= col + 1; j++) {
				//如果周围的格子没有被打开
				if ((i >= 0 && i < ROW && j >= 0 && j < COL) && map[i][j] > 19 && map[i][j] <= 28) {
					map[i][j] -= 20;
					openNull(map, i, j);
				}
			}
		}
	}
}
void judge(int map[ROW][COL], int row, int col) {
	//点到了雷
	if (map[row][col] == -1) {
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++) {

				if (map[i][j] == 19) {
					map[i][j] -= 20;
				}
				else if (map[i][j] == 39) {
					map[i][j] -= 40;
				}
			}
		}

		isover = true;
		return;
	}
	//全部成功
	int cnt = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++) {
			if (map[i][j] >= 0 && map[i][j] <= 8) {
				cnt++;
			}
		}
	}
	if (cnt == ROW * COL - 10) {

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++) {

				if (map[i][j] == 19) {
					map[i][j] -= 20;
				}

				else if (map[i][j] == 39) {
					map[i][j] -= 40;
				}
			}
		}

		iswin = true;
	}
}