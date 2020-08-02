#include "stdafx.h"
#include "GObject.h"


GMap *GObject::pStage = NULL;

int GObject::Getline()
{
	return line;
}

int GObject::Getcol()
{
	return col;
}

int GObject::transform(int k)
{
	return (k - (pStage->Wall_L) / 2) / pStage->Wall_L;

}

//判断物体是否到达逻辑坐标位置
bool GObject::Arrive()
{
	int n = (center.x - pStage->Wall_L / 2) % pStage->Wall_L; // 计算x坐标的余数
	int k = (center.y - pStage->Wall_L / 2) % pStage->Wall_L; // 计算y坐标的余数
	bool l = (n == 0 && k == 0);         // 如果两个余数都为0,说明到达中心位置
	return l;
}

//如果到达逻辑坐标位置则更新
void GObject::update()
{
	if (Arrive()) {
		col = transform(center.x);  //更新列
		line = transform(center.y);  //更新行
	}
}

//画领域（每个对象都有一个矩形领域，用来作meet检测
void GObject::Paintfield(HDC &hdc)
{
	// 申请资源,并交给智能指针处理
	HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
	std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {
		// 离开 DrawBlank函数时,会自动调用释放资源
		DeleteObject(hbr);
	});
	RECT rect;
	rect.top = Y - RD;
	rect.left = X - RD;
	rect.right = X + RD;
	rect.bottom = Y + RD;
	// 埴充
	FillRect(hdc, &rect, *phbr);
}

// 设置中心位置
void GObject::Setcenter(int R, int A)
{
	line = R;
	col = A;
	this->center.y = line * pStage->Wall_L + pStage->Wall_L / 2;
	this->center.x = col * pStage->Wall_L + pStage->Wall_L / 2;
}

//相遇检测
bool GObject::meet()
{
	bool b = false;
	update();//更新
	//边界溢出检测
	if (col < 0 || line < 0 || col > MAPLEN - 1
		|| line > MAPLEN - 1) {
		b = true;
	}
	else if (Arrive()) {
		switch (cmd) {  //判断行进的方向
		case LEFT:
			//判断下一个格子是否能够通行
			if (col > 0 &&
				!pStage->walltable[line][col - 1]) {
				b = true;//指令无效
			}
			break;
			//以下方向的判断原理相同
		case RIGHT:
			if (col < MAPLEN - 1 &&
				!pStage->walltable[line][col + 1]) {
				b = true;
			}
			break;
		case UP:
			if (line > 0 &&
				!pStage->walltable[line - 1][col]) {
				b = true;
			}
			break;
		case DOWN:
			if (line < MAPLEN - 1 &&
				!pStage->walltable[line + 1][col]) {
				b = true;
			}
			break;
		}
		if (!b) {
			dir = cmd; //没撞墙,指令成功
		}
	}	
	//依照真实的方向位移
	X = center.x;
	Y = center.y;
	int MAX = pStage->Wall_L * MAPLEN + pStage->Wall_L / 2;
	int MIN = pStage->Wall_L / 2;
	switch (dir) {  //判断行进的方向
	case LEFT:
		//判断下一个格子是否能够通行
		if (col > 0 &&
			!pStage->walltable[line][col - 1]) {
			b = true;
			break;
		}
		center.x -= vel;
		if (center.x < MIN) {
			center.x = MAX;
		}

		break;
		//以下方向的判断原理相同
	case RIGHT:
		if (col < MAPLEN - 1 &&
			!pStage->walltable[line][col + 1]) {
			b = true;
			break;
		}
		center.x += vel;
		if (center.x > MAX) {
			center.x = MIN;
		}

		break;
	case UP:
		if (line > 0 &&
			!pStage->walltable[line - 1][col]) {
			b = true;
			break;
		}
		center.y -= vel;
		if (center.y < MIN) {
			center.y = MAX;
		}
		break;
	case DOWN:
		if (line < MAPLEN - 1 &&
			!pStage->walltable[line + 1][col]) {
			b = true;
			break;
		}
		center.y += vel;
		if (center.y > MAX) {
			center.y = MIN;
		}
		break;
	}
	return b;
}



//Player成员定义:
void Player::update()
{
	GObject::update();
	if (Arrive()) {
		if (line >= 0 && line < MAPLEN &&
			col >= 0 && col < MAPLEN) {  //边界溢出检查
			if (pStage->beantable[line][col]) {
				pStage->beantable[line][col] = false;
			}
		}
	}
}

void Player::behavior()
{
	meet();
}

void Player::Setcmd(order command)
{
	cmd = command;
}

bool Player::ifend()
{
	return dir == END;
}

bool Player::ifwin()
{
	for (int i = 0; i <= MAPLEN; i++) {
		for (int j = 0; j <= MAPLEN; j++) {
			if (pStage->beantable[i][j] == true) {
				return false; //存在任意一个豆子,没取得胜利
			}
		}
	}
	return true;//没有豆子,胜利
}

POINT Player::Getcenter()
{
	return center;
}

void Player::Setend()
{
	dir = END;
}

//画自己
void Player::paintself(HDC &memDC)
{
	if (dir == END) {
		return;
	}
	else if (Frame % 2 == 0) {  //第4帧动画与第2帧动画（圆弧+两条线段）表半张嘴
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + OFFSET; //弧弦交点
		int offsetY = DISTANCE / 2 + OFFSET; //弧弦交点
		//确定交点坐标
		switch (dir) {
		case UP:
			x1 = center.x - offsetX;
			x2 = center.x + offsetX;
			y2 = y1 = center.y - offsetY;
			break;
		case DOWN:
			x1 = center.x + offsetX;
			x2 = center.x - offsetX;
			y2 = y1 = center.y + offsetY;
			break;
		case LEFT:
			x2 = x1 = center.x - offsetX;
			y1 = center.y + offsetY;
			y2 = center.y - offsetY;
			break;
		case RIGHT:
			x2 = x1 = center.x + offsetX;
			y1 = center.y - offsetY;
			y2 = center.y + offsetY;
			break;

		}
		//画圆
		Arc(memDC, center.x - DISTANCE, center.y - DISTANCE,
			center.x + DISTANCE, center.y + DISTANCE,
			x1, y1,
			x2, y2);
		//画线段
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, center.x, center.y);
		LineTo(memDC, x2, y2);
	}
	else if (Frame % 3 == 0) {//画圆形表示没张嘴
		Ellipse(memDC, center.x - DISTANCE, center.y - DISTANCE,
			center.x + DISTANCE, center.y + DISTANCE);
	}
	else {//画半圆和一条直径线段表示全张嘴
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		switch (dir) {
		case UP:
			x1 = center.x - DISTANCE;
			x2 = center.x + DISTANCE;
			y2 = y1 = center.y;
			break;
		case DOWN:
			x1 = center.x + DISTANCE;
			x2 = center.x - DISTANCE;
			y2 = y1 = center.y;
			break;
		case LEFT:
			x2 = x1 = center.x;
			y1 = center.y + DISTANCE;
			y2 = center.y - DISTANCE;
			break;
		case RIGHT:
			x2 = x1 = center.x;
			y1 = center.y - DISTANCE;
			y2 = center.y + DISTANCE;
			break;

		}

		Arc(memDC, center.x - DISTANCE, center.y - DISTANCE,
			center.x + DISTANCE, center.y + DISTANCE,
			x1, y1,
			x2, y2);
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, center.x, center.y);
		LineTo(memDC, x2, y2);
	}

	Frame++;//绘制下一祯
}

//Enermy成员定义:
std::shared_ptr<Player> Enemy::player = nullptr;

// 抓住，游戏结束
void Enemy::Seize()
{
	int DX = center.x - player->Getcenter().x;
	int DY = center.y - player->Getcenter().y;
	if ((-RD < DX && DX < RD) && (-RD < DY && DY < RD)) {
		player->Setend();
	}
}

//画敌人
void Enemy::paintself(HDC &hdc)
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, center.x - DISTANCE, center.y - DISTANCE,
		center.x + DISTANCE, center.y + DISTANCE,
		center.x + DISTANCE, center.y,
		center.x - DISTANCE, center.y); //半圆型的头
	int const LEGLENTH = (DISTANCE) / (enemy_feet);
	//根据帧数来绘制身体和“腿部”，有两帧
	if (Frame % 2 == 0) {
		//矩形的身子
		MoveToEx(hdc, center.x - DISTANCE, center.y, NULL);
		LineTo(hdc, center.x - DISTANCE,
			center.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, center.x + DISTANCE, center.y, NULL);
		LineTo(hdc, center.x + DISTANCE,
			center.y + DISTANCE - LEGLENTH);
		for (int i = 0; i < enemy_feet; i++) {  //从左往右绘制“腿部”
			Arc(hdc,
				center.x - DISTANCE + i * 2 * LEGLENTH,
				center.y + DISTANCE - 2 * LEGLENTH,
				center.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				center.y + DISTANCE,
				center.x - DISTANCE + i * 2 * LEGLENTH,
				center.y + DISTANCE - LEGLENTH,
				center.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				center.y + DISTANCE - LEGLENTH
			);
		}
	}
	else {
		MoveToEx(hdc, center.x - DISTANCE, center.y, NULL);   //绘制身体
		LineTo(hdc, center.x - DISTANCE, center.y + DISTANCE);
		MoveToEx(hdc, center.x + DISTANCE, center.y, NULL);
		LineTo(hdc, center.x + DISTANCE, center.y + DISTANCE);
		//从左往右绘制“腿部”
		//先画两个小三角形再画四条腿，使得有连续动态感
		MoveToEx(hdc, center.x - DISTANCE,
			center.y + DISTANCE, NULL);
		LineTo(hdc, center.x - DISTANCE + LEGLENTH,
			center.y + DISTANCE - LEGLENTH);

		for (int i = 0; i < enemy_feet - 1; i++) {
			Arc(hdc,
				center.x - DISTANCE + (1 + i * 2)*LEGLENTH,
				center.y + DISTANCE - 2 * LEGLENTH,
				center.x - DISTANCE + (3 + i * 2)*LEGLENTH,
				center.y + DISTANCE,
				center.x - DISTANCE + (1 + i * 2)*LEGLENTH,
				center.y + DISTANCE - LEGLENTH,
				center.x - DISTANCE + (3 + i * 2)*LEGLENTH,
				center.y + DISTANCE - LEGLENTH
			);
		}

		MoveToEx(hdc, center.x + DISTANCE, center.y + DISTANCE, NULL);
		LineTo(hdc, center.x + DISTANCE - LEGLENTH,
			center.y + DISTANCE - LEGLENTH);
	}
	//根据方向绘制眼睛
	int R = DISTANCE / 5; //眼睛的半径
	switch (dir) {
	case UP:
		Ellipse(hdc, center.x - 2 * R, center.y - 2 * R,
			center.x, center.y);
		Ellipse(hdc, center.x, center.y - 2 * R,
			center.x + 2 * R, center.y);
		break;
	case DOWN:
		Ellipse(hdc, center.x - 2 * R, center.y,
			center.x, center.y + 2 * R);
		Ellipse(hdc, center.x, center.y,
			center.x + 2 * R, center.y + 2 * R);
		break;
	case LEFT:
		Ellipse(hdc, center.x - 3 * R, center.y - R,
			center.x - R, center.y + R);
		Ellipse(hdc, center.x - R, center.y - R,
			center.x + R, center.y + R);
		break;
	case RIGHT:
		Ellipse(hdc, center.x - R, center.y - R,
			center.x + R, center.y + R);
		Ellipse(hdc, center.x + R, center.y - R,
			center.x + 3 * R, center.y + R);
		break;
	}

	Frame++; //准备绘制下一祯
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	return;
}

//敌人行为
void Enemy::behavior()
{
	bool b = meet();
	Movement(b);
	Seize();
}



//Randomer成员
void Randomer::paintself(HDC &hdc)
{
	Enemy::paintself(hdc);
}
//随机移动方向函数
void Randomer::Movement(bool b)
{
	//srand(time(0));
	int i = rand();
	if (b) {  //撞到墙壁,改变方向
		//逆时针转向
		if (i % 4 == 0) {
			dir == UP ? cmd = LEFT : cmd = UP;
		}
		else if (i % 3 == 0) {
			dir == DOWN ? cmd = RIGHT : cmd = DOWN;
		}
		else if (i % 2 == 0) {
			dir == RIGHT ? cmd = UP : cmd = RIGHT;
		}
		else {
			dir == LEFT ? cmd = DOWN : cmd = LEFT;
		}
		return;
	}

	if (i % 4 == 0) {
		cmd != UP ? dir == DOWN : cmd == UP;
	}
	else if (i % 3 == 0) {
		dir != DOWN ? cmd = UP : cmd = DOWN;
	}
	else if (i % 2 == 0) {
		dir != RIGHT ? cmd = LEFT : cmd = RIGHT;
	}
	else {
		dir != LEFT ? cmd = RIGHT : cmd = LEFT;
	}

}


//Guard成员定义
void Guard::paintself(HDC &hdc)
{
	Randomer::paintself(hdc);
}
//警戒范围
void Guard::Movement(bool b)
{

	const int DR = this->line - player->Getline();
	const int DA = this->col - player->Getcol();
	if (!b && DR == 0) {
		if (DA <= ALERT && DA > 0) {  //玩家在左侧边警戒范围s
			cmd = LEFT;   //向左移动
			return;
		}
		if (DA < 0 && DA >= -ALERT) {  //右侧警戒范围
			cmd = RIGHT;//向右移动
			return;
		}
	}
	if (!b && DA == 0) {
		if (DR <= ALERT && DR > 0) {  //下方警戒范围
			cmd = UP;
			return;
		}
		if (DR < 0 && DR >= -ALERT) {  //上方警戒范围
			cmd = DOWN;
			return;
		}
	}

	Randomer::Movement(b);  //不在追踪模式时RED行为相同
}


//Tracer成员定义
void Tracer::Movement(bool b)
{
	const int DR = line - player->Getline();      //敌人与玩家行距
	const int DA = col - player->Getcol();  //敌人与玩家列距
	//不撞墙
	if (!b) {
		//竖直方向较远
		if (DR*DR >= DA * DA) {
			if (DR > 0) {//在上面就向上
				cmd = UP;
				return;
			}
			else {
				cmd = DOWN;
				return;
			}
		}
		//水平方向较远
		else {
			if (DA > 0) {//在左边就向左
				cmd = LEFT;
				return;
			}
			else {
				cmd = RIGHT;
				return;
			}
		}
	}

	//当撞墙（b为true）则和父类一样
	Randomer::Movement(b);
}
void Tracer::paintself(HDC &hdc)
{
	Randomer::paintself(hdc);
}