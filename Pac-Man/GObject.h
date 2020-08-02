#include "stdafx.h"
#include <time.h>

#include "GMap.h"
const int pacman_v = 6;            
const int enemy_v = 4;            
const int enemy_feet = 5;              
const int DISTANCE = 10; 

const int ALERT = 8;             //守卫者警戒范围

const int OFFSET = 2;            
const int RD =(DISTANCE + OFFSET); 

//枚举
enum Order {
	UP,    
	DOWN,  
	LEFT,  
	RIGHT, 
	END, 
};

// 物体类 : 大嘴和敌人的父类
class GObject
{
public:
	GObject(int R, int A)
	{
		// 帧数
		Frame = 1;
		// 当前关卡
		pStage = NULL;
		// 行
		this->line = R;
		// 数组
		this->col = A;
		// 中心位置
		this->center.y = line * pStage->Wall_L + pStage->Wall_L / 2;
		this->center.x = col * pStage->Wall_L + pStage->Wall_L / 2;

		this->X = center.x;
		this->Y = center.y;
	}

	// 设置位置
	void Setcenter(int R, int A);
	
	void Paintfield(HDC &hdc);
	void virtual paintself(HDC &hdc) = 0; //绘制函数
	void virtual behavior() = 0;//数据变更的表现

	int Getline();
	int Getcol();

	static GMap *pStage; //指向地图类的指针,设置为静态,使所有自类对象都能够使用相同的地图

protected:
	int X;
	int Y;
	//指令缓存
	Order cmd;
	//当前状态
	Order dir;
	//中心坐标
	POINT center;
	//逻辑横坐标
	int line;
	//逻辑纵坐标
	int col;
	//速度
	int vel;
	//帧数
	int Frame;
	//判断物体是否到达逻辑坐标位置
	bool Arrive();
	//逻辑碰撞检测,将物体摆放到合理的位置
	bool meet();
	//将实际坐标转换为逻辑坐标
	int transform(int k);
	//到达逻辑点后更新数据
	virtual void update();
};

//玩家对象
class Player : public GObject
{
protected:
	virtual void update();//重写虚函数

public:
	POINT Getcenter();
	bool ifend();
	bool ifwin();
	void paintself(HDC &hdc);
	void Setcmd(Order command);
	Player(int x, int y) : GObject(x, y)
	{
		this->vel= pacman_v;
		cmd = dir = LEFT;
	}
	void behavior();
	void Setend();
};

//敌人基类
class Enemy : public GObject
{
protected:
	void Seize();//是否抓住玩家
	void virtual Movement(bool b) = 0;//AI实现运动函数
	COLORREF color;
public:
	static std::shared_ptr<Player> player;
	void virtual paintself(HDC &hdc);

	Enemy(int x, int y) : GObject(x, y)
	{
		this->vel = enemy_v;
		dir = LEFT;
		cmd = UP;
	}
	void virtual behavior();
};

// 三种敌人

//扰乱者，随机移动
class Randomer : public Enemy 
{
protected:
	void virtual Movement(bool b);
public:
	void paintself(HDC &hdc);
	Randomer(int x, int y) : Enemy(x, y)
	{
		color = RGB(255, 0, 0);
	}
};

//守卫者，有警戒能力
class Guard : public Randomer 
{
protected:
	void virtual Movement(bool b);
public:
	void paintself(HDC &hdc);
	Guard(int x, int y) : Randomer(x, y)
	{
		
		color = RGB(0, 0, 255);
	}

};

//追踪者，跟踪能力最强
class Tracer : public Randomer 
{
protected:
	void virtual Movement(bool b);
public:
	void paintself(HDC &hdc);
	Tracer(int x, int y) : Randomer(x, y)
	{
		color = RGB(0, 255, 0);
	}
};