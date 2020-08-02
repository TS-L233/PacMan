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

//�ж������Ƿ񵽴��߼�����λ��
bool GObject::Arrive()
{
	int n = (center.x - pStage->Wall_L / 2) % pStage->Wall_L; // ����x���������
	int k = (center.y - pStage->Wall_L / 2) % pStage->Wall_L; // ����y���������
	bool l = (n == 0 && k == 0);         // �������������Ϊ0,˵����������λ��
	return l;
}

//��������߼�����λ�������
void GObject::update()
{
	if (Arrive()) {
		col = transform(center.x);  //������
		line = transform(center.y);  //������
	}
}

//������ÿ��������һ����������������meet���
void GObject::Paintfield(HDC &hdc)
{
	// ������Դ,����������ָ�봦��
	HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
	std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {
		// �뿪 DrawBlank����ʱ,���Զ������ͷ���Դ
		DeleteObject(hbr);
	});
	RECT rect;
	rect.top = Y - RD;
	rect.left = X - RD;
	rect.right = X + RD;
	rect.bottom = Y + RD;
	// ����
	FillRect(hdc, &rect, *phbr);
}

// ��������λ��
void GObject::Setcenter(int R, int A)
{
	line = R;
	col = A;
	this->center.y = line * pStage->Wall_L + pStage->Wall_L / 2;
	this->center.x = col * pStage->Wall_L + pStage->Wall_L / 2;
}

//�������
bool GObject::meet()
{
	bool b = false;
	update();//����
	//�߽�������
	if (col < 0 || line < 0 || col > MAPLEN - 1
		|| line > MAPLEN - 1) {
		b = true;
	}
	else if (Arrive()) {
		switch (cmd) {  //�ж��н��ķ���
		case LEFT:
			//�ж���һ�������Ƿ��ܹ�ͨ��
			if (col > 0 &&
				!pStage->walltable[line][col - 1]) {
				b = true;//ָ����Ч
			}
			break;
			//���·�����ж�ԭ����ͬ
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
			dir = cmd; //ûײǽ,ָ��ɹ�
		}
	}	
	//������ʵ�ķ���λ��
	X = center.x;
	Y = center.y;
	int MAX = pStage->Wall_L * MAPLEN + pStage->Wall_L / 2;
	int MIN = pStage->Wall_L / 2;
	switch (dir) {  //�ж��н��ķ���
	case LEFT:
		//�ж���һ�������Ƿ��ܹ�ͨ��
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
		//���·�����ж�ԭ����ͬ
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



//Player��Ա����:
void Player::update()
{
	GObject::update();
	if (Arrive()) {
		if (line >= 0 && line < MAPLEN &&
			col >= 0 && col < MAPLEN) {  //�߽�������
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
				return false; //��������һ������,ûȡ��ʤ��
			}
		}
	}
	return true;//û�ж���,ʤ��
}

POINT Player::Getcenter()
{
	return center;
}

void Player::Setend()
{
	dir = END;
}

//���Լ�
void Player::paintself(HDC &memDC)
{
	if (dir == END) {
		return;
	}
	else if (Frame % 2 == 0) {  //��4֡�������2֡������Բ��+�����߶Σ��������
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + OFFSET; //���ҽ���
		int offsetY = DISTANCE / 2 + OFFSET; //���ҽ���
		//ȷ����������
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
		//��Բ
		Arc(memDC, center.x - DISTANCE, center.y - DISTANCE,
			center.x + DISTANCE, center.y + DISTANCE,
			x1, y1,
			x2, y2);
		//���߶�
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, center.x, center.y);
		LineTo(memDC, x2, y2);
	}
	else if (Frame % 3 == 0) {//��Բ�α�ʾû����
		Ellipse(memDC, center.x - DISTANCE, center.y - DISTANCE,
			center.x + DISTANCE, center.y + DISTANCE);
	}
	else {//����Բ��һ��ֱ���߶α�ʾȫ����
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

	Frame++;//������һ��
}

//Enermy��Ա����:
std::shared_ptr<Player> Enemy::player = nullptr;

// ץס����Ϸ����
void Enemy::Seize()
{
	int DX = center.x - player->Getcenter().x;
	int DY = center.y - player->Getcenter().y;
	if ((-RD < DX && DX < RD) && (-RD < DY && DY < RD)) {
		player->Setend();
	}
}

//������
void Enemy::paintself(HDC &hdc)
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, center.x - DISTANCE, center.y - DISTANCE,
		center.x + DISTANCE, center.y + DISTANCE,
		center.x + DISTANCE, center.y,
		center.x - DISTANCE, center.y); //��Բ�͵�ͷ
	int const LEGLENTH = (DISTANCE) / (enemy_feet);
	//����֡������������͡��Ȳ���������֡
	if (Frame % 2 == 0) {
		//���ε�����
		MoveToEx(hdc, center.x - DISTANCE, center.y, NULL);
		LineTo(hdc, center.x - DISTANCE,
			center.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, center.x + DISTANCE, center.y, NULL);
		LineTo(hdc, center.x + DISTANCE,
			center.y + DISTANCE - LEGLENTH);
		for (int i = 0; i < enemy_feet; i++) {  //�������һ��ơ��Ȳ���
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
		MoveToEx(hdc, center.x - DISTANCE, center.y, NULL);   //��������
		LineTo(hdc, center.x - DISTANCE, center.y + DISTANCE);
		MoveToEx(hdc, center.x + DISTANCE, center.y, NULL);
		LineTo(hdc, center.x + DISTANCE, center.y + DISTANCE);
		//�������һ��ơ��Ȳ���
		//�Ȼ�����С�������ٻ������ȣ�ʹ����������̬��
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
	//���ݷ�������۾�
	int R = DISTANCE / 5; //�۾��İ뾶
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

	Frame++; //׼��������һ��
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	return;
}

//������Ϊ
void Enemy::behavior()
{
	bool b = meet();
	Movement(b);
	Seize();
}



//Randomer��Ա
void Randomer::paintself(HDC &hdc)
{
	Enemy::paintself(hdc);
}
//����ƶ�������
void Randomer::Movement(bool b)
{
	//srand(time(0));
	int i = rand();
	if (b) {  //ײ��ǽ��,�ı䷽��
		//��ʱ��ת��
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


//Guard��Ա����
void Guard::paintself(HDC &hdc)
{
	Randomer::paintself(hdc);
}
//���䷶Χ
void Guard::Movement(bool b)
{

	const int DR = this->line - player->Getline();
	const int DA = this->col - player->Getcol();
	if (!b && DR == 0) {
		if (DA <= ALERT && DA > 0) {  //��������߾��䷶Χs
			cmd = LEFT;   //�����ƶ�
			return;
		}
		if (DA < 0 && DA >= -ALERT) {  //�Ҳྯ�䷶Χ
			cmd = RIGHT;//�����ƶ�
			return;
		}
	}
	if (!b && DA == 0) {
		if (DR <= ALERT && DR > 0) {  //�·����䷶Χ
			cmd = UP;
			return;
		}
		if (DR < 0 && DR >= -ALERT) {  //�Ϸ����䷶Χ
			cmd = DOWN;
			return;
		}
	}

	Randomer::Movement(b);  //����׷��ģʽʱRED��Ϊ��ͬ
}


//Tracer��Ա����
void Tracer::Movement(bool b)
{
	const int DR = line - player->Getline();      //����������о�
	const int DA = col - player->Getcol();  //����������о�
	//��ײǽ
	if (!b) {
		//��ֱ�����Զ
		if (DR*DR >= DA * DA) {
			if (DR > 0) {//�����������
				cmd = UP;
				return;
			}
			else {
				cmd = DOWN;
				return;
			}
		}
		//ˮƽ�����Զ
		else {
			if (DA > 0) {//����߾�����
				cmd = LEFT;
				return;
			}
			else {
				cmd = RIGHT;
				return;
			}
		}
	}

	//��ײǽ��bΪtrue����͸���һ��
	Randomer::Movement(b);
}
void Tracer::paintself(HDC &hdc)
{
	Randomer::paintself(hdc);
}