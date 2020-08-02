#pragma once

#include <list>

const int MAPLEN = 19;  
const int P_line = 10;
const int P_col = 9;
const int E_line = 8;
const int E_col = 9;

using std::list;

//����
class GMap
{

protected:
	static int Wall_L;//ǽ��
	static int bean_R;//��radius
	void Initbean();//��ʼ��
	bool walltable[MAPLEN][MAPLEN];//ǽ�߼���
	bool beantable[MAPLEN][MAPLEN];//���߼���
	COLORREF color;
public:
	//���ƺ���
	void  paintwall(HDC &hdc);
	void  paintbean(HDC &hdc);
	virtual ~GMap();
	GMap()
	{
	}

	//����GObject��pacman����GMap��
	friend class GObject;
	friend class Player;
};

//��һ��
class chapter_1 : public GMap
{
private:
	bool static setchapter[MAPLEN][MAPLEN];
public:
	chapter_1();
};

//�ڶ���
class chapter_2 : public GMap
{
private:
	bool static setchapter[MAPLEN][MAPLEN];
public:
	chapter_2();
};

// ������
class chapter_3 : public GMap
{
private:
	bool static setchapter[MAPLEN][MAPLEN];
public:
	chapter_3();
};