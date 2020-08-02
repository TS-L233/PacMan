#pragma once

#include <list>

const int MAPLEN = 19;  
const int P_line = 10;
const int P_col = 9;
const int E_line = 8;
const int E_col = 9;

using std::list;

//基类
class GMap
{

protected:
	static int Wall_L;//墙宽
	static int bean_R;//豆radius
	void Initbean();//初始化
	bool walltable[MAPLEN][MAPLEN];//墙逻辑表
	bool beantable[MAPLEN][MAPLEN];//豆逻辑表
	COLORREF color;
public:
	//绘制函数
	void  paintwall(HDC &hdc);
	void  paintbean(HDC &hdc);
	virtual ~GMap();
	GMap()
	{
	}

	//运行GObject和pacman访问GMap类
	friend class GObject;
	friend class Player;
};

//第一关
class chapter_1 : public GMap
{
private:
	bool static setchapter[MAPLEN][MAPLEN];
public:
	chapter_1();
};

//第二关
class chapter_2 : public GMap
{
private:
	bool static setchapter[MAPLEN][MAPLEN];
public:
	chapter_2();
};

// 第三关
class chapter_3 : public GMap
{
private:
	bool static setchapter[MAPLEN][MAPLEN];
public:
	chapter_3();
};