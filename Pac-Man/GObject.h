#include "stdafx.h"
#include <time.h>

#include "GMap.h"
const int pacman_v = 6;            
const int enemy_v = 4;            
const int enemy_feet = 5;              
const int DISTANCE = 10; 

const int ALERT = 8;             //�����߾��䷶Χ

const int OFFSET = 2;            
const int RD =(DISTANCE + OFFSET); 

//ö��
enum Order {
	UP,    
	DOWN,  
	LEFT,  
	RIGHT, 
	END, 
};

// ������ : ����͵��˵ĸ���
class GObject
{
public:
	GObject(int R, int A)
	{
		// ֡��
		Frame = 1;
		// ��ǰ�ؿ�
		pStage = NULL;
		// ��
		this->line = R;
		// ����
		this->col = A;
		// ����λ��
		this->center.y = line * pStage->Wall_L + pStage->Wall_L / 2;
		this->center.x = col * pStage->Wall_L + pStage->Wall_L / 2;

		this->X = center.x;
		this->Y = center.y;
	}

	// ����λ��
	void Setcenter(int R, int A);
	
	void Paintfield(HDC &hdc);
	void virtual paintself(HDC &hdc) = 0; //���ƺ���
	void virtual behavior() = 0;//���ݱ���ı���

	int Getline();
	int Getcol();

	static GMap *pStage; //ָ���ͼ���ָ��,����Ϊ��̬,ʹ������������ܹ�ʹ����ͬ�ĵ�ͼ

protected:
	int X;
	int Y;
	//ָ���
	Order cmd;
	//��ǰ״̬
	Order dir;
	//��������
	POINT center;
	//�߼�������
	int line;
	//�߼�������
	int col;
	//�ٶ�
	int vel;
	//֡��
	int Frame;
	//�ж������Ƿ񵽴��߼�����λ��
	bool Arrive();
	//�߼���ײ���,������ڷŵ������λ��
	bool meet();
	//��ʵ������ת��Ϊ�߼�����
	int transform(int k);
	//�����߼�����������
	virtual void update();
};

//��Ҷ���
class Player : public GObject
{
protected:
	virtual void update();//��д�麯��

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

//���˻���
class Enemy : public GObject
{
protected:
	void Seize();//�Ƿ�ץס���
	void virtual Movement(bool b) = 0;//AIʵ���˶�����
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

// ���ֵ���

//�����ߣ�����ƶ�
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

//�����ߣ��о�������
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

//׷���ߣ�����������ǿ
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