#include "stdafx.h"
#include "GMap.h"

int GMap::Wall_L = 36;
int GMap::bean_R = 3;
//����˫������λ��û�ж��ӳ���
void GMap::Initbean()
{
	beantable[E_line][E_col] = false;
	beantable[P_line][P_col] = false;
}
void GMap::paintwall (HDC &memDC)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	for (int i = 0; i < MAPLEN; i++) {
		for (int j = 0; j < MAPLEN; j++) {
			//����ǽ��
			if (!walltable[i][j]) {
				RECT rect;
				rect.left = j * Wall_L;
				rect.top = i * Wall_L;
				rect.right = (j + 1) * Wall_L;
				rect.bottom = (i + 1) * Wall_L;
				FillRect(memDC, &rect, hBrush);
			}
		}
	}
	DeleteObject(hBrush);
}
void GMap::paintbean(HDC &hdc)
{
	for (int i = 0; i < MAPLEN; i++) {
		for (int j = 0; j < MAPLEN; j++) {
			if (beantable[i][j]) {
				// ��Բ  ��������
				Ellipse(hdc, 
					(Wall_L / 2 - bean_R) + j * Wall_L, 
					(Wall_L / 2 - bean_R) + i * Wall_L, 
					(Wall_L / 2 + bean_R) + j * Wall_L,
					(Wall_L / 2 + bean_R) + i * Wall_L);
			}
		}
	}

	// ��T����
	if (GetAsyncKeyState('T') & 0x8000) {
		MessageBoxA(NULL, "һ��ͨ��", "�Զ�����ʾ", MB_OK);
		for (int i = 0; i < MAPLEN; i++) {
			for (int j = 0; j < MAPLEN; j++) {
				beantable[i][j] = false;
			}
		}
	}
}
GMap::~GMap()
{

}
//chapter_1��Ա����:

bool chapter_1::setchapter[MAPLEN][MAPLEN] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, //1
	0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, //2
	0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, //3
	0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, //4
	0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, //5
	0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, //6
	0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, //7
	0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, //8
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, //9
	0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, //10
	0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, //11
	0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, //12
	0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, //13
	0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, //14
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, //15
	0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, //16
	0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, //17
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, //18
};

chapter_1::chapter_1()
{
	color = RGB(140, 240, 240);
	for (int i = 0; i < MAPLEN; i++) {
		for (int j = 0; j < MAPLEN; j++) {
			this->walltable[i][j] = this->setchapter[i][j];
			this->beantable[i][j] = this->setchapter[i][j];
		}
	}
	//����˫������λ��û�ж��ӳ���
	this->Initbean();
}
//chapter_2��Ա����

bool chapter_2::setchapter[MAPLEN][MAPLEN] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, //0
	1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, //1
	0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, //2
	0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, //3
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, //4
	0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, //5
	0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, //6
	0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, //7
	0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, //8
	1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, //9
	0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, //10
	0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, //11
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, //12
	0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, //13
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, //14
	0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, //15
	0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, //16
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, //17
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, //18
};

chapter_2::chapter_2()
{
	color = RGB(240, 140, 140);
	for (int i = 0; i < MAPLEN; i++) {
		for (int j = 0; j < MAPLEN; j++) {
			this->walltable[i][j] = this->setchapter[i][j];
			this->beantable[i][j] = this->setchapter[i][j];
		}
	}
	//����˫������λ��û�ж��ӳ���
	this->Initbean();
}

//chapter_3��Ա����

bool chapter_3::setchapter[MAPLEN][MAPLEN] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //1
	0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, //2
	0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, //3
	0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, //4
	0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, //5
	0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, //6
	0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, //7
	0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, //8
	0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, //9
	0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, //10
	0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, //11
	0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, //12
	0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, //13
	0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, //14
	0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, //15
	0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, //16
	1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, //17
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, //18
};


chapter_3::chapter_3()
{
	color = RGB(100, 300, 100);
	for (int i = 0; i < MAPLEN; i++) {
		for (int j = 0; j < MAPLEN; j++) {
			this->walltable[i][j] = this->setchapter[i][j];
			this->beantable[i][j] = this->setchapter[i][j];
		}
	}
	//����˫������λ��û�ж��ӳ���
	this->Initbean();
}