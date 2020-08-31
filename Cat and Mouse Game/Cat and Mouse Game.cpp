// meo and Mouse Game.cpp : Defines the entry point for the console applimeoion.
//

#include "stdafx.h"

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include <math.h>
#include <string.h>
#include <stdarg.h>
#define D2R 0.0174532  // degree to radian
#define PI 3.14159265358979323846
#define SIZE 3

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}

///
int FPS = 100;

//Game Track
int start = 0;
int gv = 0;
int level = 0;

//Track Score
int score = 0;

//Form move track
int roadDivTopMost = 0;
int roadDivTop = 0;
int roadDivMdl = 0;
int roadDivBtm = 0;

//For meod Left / RIGHT
int lrIndex = 0;

//Chướng ngại mèo
//mèo cam
int meo1 = 0;
int lrIndex1 = 0;
bool cat = true;
//mèo xám
int meo2 = +35;
int lrIndex2 = 0;
//mèo kem
int meo3 = +70;
int lrIndex3 = 0;

//Phô mai
int phomai = +75;
int lrIndex4 = 0;
bool food = true;

//khởi tạo góc
float angle;
//đạn
int lrIndex5 = 0;

int right2left = 0;
float theta;

bool fire = true; //Khai báo viên đạn

bool cat1 = true;

typedef struct  //cấu trúc viên đạn
{
	float x, y;
}bullets_t;
bullets_t bullet;
typedef struct //Cấu trúc vị trí tâm mèo
{
	float y, x;
}center_t;
typedef struct //Khai báo cấu trúc mèo
{
	center_t center;

	int r;
	int visible = 1;
}objects_t;
objects_t catx;

//Hiển thị TEXT
const int font1 = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
const int font3 = (int)GLUT_BITMAP_8_BY_13;

char s[30];
void renderBitmapString(float x, float y, void *font, const char *string) {
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

//Cây Hoa
void tree(int x, int y) {
	int newx = x;
	int newy = y;

	//Thân
	glColor3f(0, 0.325, 0.161);
	glBegin(GL_TRIANGLES);
	glVertex2f(newx + 11, newy + 58);
	glVertex2f(newx + 11.5, newy + 55 - 10);
	glVertex2f(newx + 10.5, newy + 55 - 10);
	glEnd();

	glColor3f(0, 0.325, 0.161);
	glBegin(GL_TRIANGLES);
	glVertex2f(newx + 13, newy + 53);
	glVertex2f(newx + 11.5, newy + 55 - 10);
	glVertex2f(newx + 10.5, newy + 55 - 10);
	glEnd();

	glColor3f(0, 0.325, 0.161);
	glBegin(GL_TRIANGLES);
	glVertex2f(newx + 9, newy + 53);
	glVertex2f(newx + 11.5, newy + 55 - 10);
	glVertex2f(newx + 10.5, newy + 55 - 10);
	glEnd();

	//Cánh hoa
	glColor3f(0.99, 0.76, 0.0078);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(newx + 11 + 4 * cos(theta), newy + 55 + 3 + 4 * sin(theta));
	}
	glEnd();

	//Nhụy hoa
	glColor3f(0.424, 0.212, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(newx + 11 + 2 * cos(theta), newy + 55 + 3 + 2 * sin(theta));
	}
	glEnd();
}
void circle(int x, int y, int r)//Hàm vẽ hình tròn
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(x + r * cos(theta), y + r * sin(theta));
	}
	glEnd();
}
void drawFire() // hàm vẽ đạn
{
	glColor3f(1, 0, 0);
	glutSolidSphere(0.08, 10, 100);
	glPopMatrix();
	if (fire) {
		circle(bullet.x, bullet.y, 1);
		if (bullet.x > 77 || bullet.x < 23 || bullet.y > 100 || bullet.y < 13) // xét khoảng cách đường đạn đi được
			fire = !fire;
	}

}
void onClick(int button, int stat, int x, int y) {
	if (start == 1) {
		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
		{
			if (fire == false)
			{
				fire = !fire;
				bullet.x = lrIndex + 28 + 0.7 * cos(theta); // Tọa độ đạn theo trục x
				bullet.y = 13 + 0.7 * sin(theta);//Tọa độ đạn theo trục y
			}
		}
		int i;
		if (fire)
		{
			//Chuyển động viên đạn
			bullet.x +=0;
			bullet.y += 45 ;
			/*if (cat1.visible == 1)
			{
				//Kiểm tra va chạm của viên đạn
				float collision = sqrt((bullet.x - lrIndex5  * cos(theta)) *
					(bullet.x - lrIndex5  * cos(theta)) +
					(bullet.y - meo1 + 100  * sin(theta)) *
					(bullet.y - meo1 + 100  * sin(theta)));
				//lrIndex1 + 28 + 3 * cos(theta), meo1 + 100 - 9 + 3 * sin(theta)
				if (collision <= 1)///Khoảng cách đạn chạm vật thể làm vật thể biến mất
				{
					cat1.visible = 0;//làm mất vật thể

				}
			}*/
			/*if ((lrIndex5 + 28 == lrIndex1 + 38) && (meo1 + 100 - 2 == 13) && fire)
			{
				score += 2;
				fire = false;
			}
			if (meo1 < -100)
			{
				fire = true;
				meo1 = 0;
				lrIndex1 = lrIndex5;
			}*/
		}
		
	}
}
void bulletFire() {
}
void chuot() {

	//Chuột
	//Đầu
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex + 28, 13); //đỉnh mũi
	glVertex2f(lrIndex + 25, 8);
	glVertex2f(lrIndex + 31, 8);
	glEnd();

	//Mắt
	//tròng đen
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON); //hình tròn
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 27.25 + 0.7 * cos(theta), 11 + 0.7 * sin(theta));
	}
	glEnd();

	//tròng trắng
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 27.25 + 0.3 * cos(theta), 11 + 0.3 * sin(theta));
	}
	glEnd();

	//tròng đen
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 28.75 + 0.7 * cos(theta), 11 + 0.7 * sin(theta));
	}
	glEnd();

	//tròng trắng
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 28.75 + 0.3 * cos(theta), 11 + 0.3 * sin(theta));
	}
	glEnd();

	//Thân
	glColor3f(0, 0, 0);
#define DEG2RAD 3.14159/180.0
	void DrawEllipse(float radiusX, float radiusY);
	{
		int i;
		glBegin(GL_POLYGON);//hình ellipse
		for (i = 0; i < 360; i++)
		{
			float rad = i*DEG2RAD;
			glVertex2f(lrIndex + 28 + cos(rad)*2.5,
				6 + sin(rad)*3.5);
		}
		glEnd();
	}

	//Tai
	//ngoài
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 25 + 2 * cos(theta), 8 + 2 * sin(theta));
	}
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 31 + 2 * cos(theta), 8 + 2 * sin(theta));
	}
	glEnd();

	//trong
	glColor3f(1, 0.682, 0.788);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 25 + cos(theta), 8 + sin(theta));
	}
	glEnd();

	glColor3f(1, 0.682, 0.788);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex + 31 + cos(theta), 8 + sin(theta));
	}
	glEnd();

	//Đuôi
	GLfloat lineWidthRange[2] = { 0.0f, 0.0f };
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
	glLineWidth(3.0f);//line đậm
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);//zíc zắc
	glVertex2f(lrIndex + 28, 3);
	glVertex2f(lrIndex + 29, 2);
	glVertex2f(lrIndex + 27, 1);
	glVertex2f(lrIndex + 28, 0);
	glEnd();
}

void Phomai() {
	//Phô Mai
	if (food) {
		glColor3f(0.99, 0.76, 0.0078);
		glBegin(GL_TRIANGLES);
		glVertex2f(lrIndex4 + 42, phomai + 100);
		glVertex2f(lrIndex4 + 37.5, phomai + 100 - 2);
		glVertex2f(lrIndex4 + 36, phomai + 100);
		glEnd();
	}

	//Canh lề
	if (lrIndex4 + 36 < 23)
	{
		food = true;
		lrIndex4 = 59;
	}
	if (lrIndex4 + 40 > 77)
	{
		food = true;
		lrIndex4 = 37;
	}
	phomai--;

	//Đụng phô mai +2 điểm
	if ((lrIndex + 28 == lrIndex4 + 38) && (phomai + 100 - 2 == 13) && food)
	{
		score += 2;
		food = false;
	}
	if (phomai < -100)
	{
		food = true;
		phomai = 0;
		lrIndex4 = lrIndex;
	}
}

void Meo1() {
	if (cat1) {
		//Chướng ngại mèo 1(cam)
		//Tai
		glColor3f(1, 0.65, 0.302);
		glBegin(GL_TRIANGLES);
		glVertex2f(lrIndex1 + 25, meo1 + 100);//đỉnh
		glVertex2f(lrIndex1 + 24.5, meo1 + 100 - 3);
		glVertex2f(lrIndex1 + 28, meo1 + 100 - 2);
		glEnd();

		glColor3f(1, 0.65, 0.302);
		glBegin(GL_TRIANGLES);
		glVertex2f(lrIndex1 + 31, meo1 + 100);//đỉnh
		glVertex2f(lrIndex1 + 31.5, meo1 + 100 - 3);
		glVertex2f(lrIndex1 + 28, meo1 + 100 - 2);
		glEnd();

		//Đầu
		glColor3f(1, 0.65, 0.302);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 28 + 4 * cos(theta), meo1 + 100 - 4 + 2.5 * sin(theta));
		}
		glEnd();

		//Mắt
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 26 + 0.5* cos(theta), meo1 + 100 - 3.5 + 0.5* sin(theta));
		}
		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 30 + 0.5* cos(theta), meo1 + 100 - 3.5 + 0.5* sin(theta));
		}
		glEnd();

		//Mũi
		glColor3f(0, 0, 0);
		glBegin(GL_TRIANGLES);
		glVertex2f(lrIndex1 + 28, meo1 + 100 - 5);
		glVertex2f(lrIndex1 + 27, meo1 + 100 - 4);
		glVertex2f(lrIndex1 + 29, meo1 + 100 - 4);
		glEnd();

		//Chân sau
		glColor3f(0.733, 0.369, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 26 + 1.5* cos(theta), meo1 + 100 - 12 + sin(theta));
		}
		glEnd();

		glColor3f(0.733, 0.369, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 30 + 1.5*cos(theta), meo1 + 100 - 12 + sin(theta));
		}
		glEnd();

		//Đuôi
		glLineWidth(3.0f);//line đậm
		glColor3f(0.733, 0.369, 0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(lrIndex1 + 28, meo1 + 100 - 11);
		glVertex2f(lrIndex1 + 32, meo1 + 100 - 10);
		glVertex2f(lrIndex1 + 31, meo1 + 100 - 8);
		glVertex2f(lrIndex1 + 32, meo1 + 100 - 7);
		glEnd();

		//Thân
		glColor3f(1, 0.65, 0.302);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 28 + 3 * cos(theta), meo1 + 100 - 9 + 3 * sin(theta));
		}
		glEnd();

		//Chân trước
		glColor3f(0.733, 0.369, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 27 + cos(theta), meo1 + 100 - 10 + sin(theta));
		}
		glEnd();

		glColor3f(0.733, 0.369, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i*3.142 / 180;
			glVertex2f(lrIndex1 + 29 + cos(theta), meo1 + 100 - 10 + sin(theta));
		}
		glEnd();
	}
	meo1--;
	if (meo1 < -100) {
		cat1 = true;
		meo1 = 0;
		lrIndex1 = lrIndex;
	}

	//Đụng mèo cam
	if ((abs(lrIndex - lrIndex1) < 8) && (meo1 + 100 < 10)) {
		start = 0;
		gv = 1;
	}
	if ((lrIndex + 28 == lrIndex1 + 28 ) && (meo1 + 100 - 9 == 13 + 45) && cat1 && fire)
	{
		//cat1.visible = 0;
		score += 2; 
		cat1 = false;
	}
}

void Meo2() {
	//Chướng ngại mèo 2(xám)
	//Tai
	glColor3f(0.765, 0.765, 0.765);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex2 + 25, meo2 + 100);
	glVertex2f(lrIndex2 + 24.5, meo2 + 100 - 3);
	glVertex2f(lrIndex2 + 28, meo2 + 100 - 2);
	glEnd();
	glColor3f(0.765, 0.765, 0.765);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex2 + 31, meo2 + 100);
	glVertex2f(lrIndex2 + 31.5, meo2 + 100 - 3);
	glVertex2f(lrIndex2 + 28, meo2 + 100 - 2);
	glEnd();

	//Đầu
	glColor3f(0.765, 0.765, 0.765);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 28 + 4 * cos(theta), meo2 + 100 - 4 + 2.5 * sin(theta));
	}
	glEnd();

	//Mắt
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 26 + 0.5* cos(theta), meo2 + 100 - 3.5 + 0.5* sin(theta));
	}
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 30 + 0.5* cos(theta), meo2 + 100 - 3.5 + 0.5* sin(theta));
	}
	glEnd();

	//Mũi
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex2 + 28, meo2 + 100 - 5);
	glVertex2f(lrIndex2 + 27, meo2 + 100 - 4);
	glVertex2f(lrIndex2 + 29, meo2 + 100 - 4);
	glEnd();

	//Chân sau
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 26 + 1.5* cos(theta), meo2 + 100 - 12 + sin(theta));
	}
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 30 + 1.5*cos(theta), meo2 + 100 - 12 + sin(theta));
	}
	glEnd();

	//Đuôi
	glLineWidth(3.0f);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(lrIndex2 + 28, meo2 + 100 - 11);
	glVertex2f(lrIndex2 + 32, meo2 + 100 - 10);
	glVertex2f(lrIndex2 + 31, meo2 + 100 - 8);
	glVertex2f(lrIndex2 + 32, meo2 + 100 - 7);
	glEnd();

	//Thân
	glColor3f(0.765, 0.765, 0.765);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 28 + 3 * cos(theta), meo2 + 100 - 9 + 3 * sin(theta));
	}
	glEnd();

	//Chân trước
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 27 + cos(theta), meo2 + 100 - 10 + sin(theta));
	}
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex2 + 29 + cos(theta), meo2 + 100 - 10 + sin(theta));
	}
	glEnd();
	meo2--;
	if (meo2 < -100)
	{
		meo2 = 0;
		lrIndex2 = lrIndex;
	}

	//Đụng mèo xám
	if ((abs(lrIndex - lrIndex2) < 8) && (meo2 + 100 < 10)) {
		start = 0;
		gv = 1;
	}
}

void Meo3() {
	//Chướng ngại mèo 3(kem)
	//Tai
	glColor3f(0.937, 0.894, 0.69);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex3 + 25, meo3 + 100);
	glVertex2f(lrIndex3 + 24.5, meo3 + 100 - 3);
	glVertex2f(lrIndex3 + 28, meo3 + 100 - 2);
	glEnd();

	glColor3f(0.937, 0.894, 0.69);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex3 + 31, meo3 + 100);
	glVertex2f(lrIndex3 + 31.5, meo3 + 100 - 3);
	glVertex2f(lrIndex3 + 28, meo3 + 100 - 2);
	glEnd();

	//Dầu
	glColor3f(0.937, 0.894, 0.69);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 28 + 4 * cos(theta), meo3 + 100 - 4 + 2.5 * sin(theta));
	}
	glEnd();

	//Mắt
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 26 + 0.5* cos(theta), meo3 + 100 - 3.5 + 0.5* sin(theta));
	}
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 30 + 0.5* cos(theta), meo3 + 100 - 3.5 + 0.5* sin(theta));
	}
	glEnd();

	//Mũi
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(lrIndex3 + 28, meo3 + 100 - 5);
	glVertex2f(lrIndex3 + 27, meo3 + 100 - 4);
	glVertex2f(lrIndex3 + 29, meo3 + 100 - 4);
	glEnd();

	//Chân sau
	glColor3f(0.596, 0.514, 0.122);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 26 + 1.5* cos(theta), meo3 + 100 - 12 + sin(theta));
	}
	glEnd();

	glColor3f(0.596, 0.514, 0.122);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 30 + 1.5*cos(theta), meo3 + 100 - 12 + sin(theta));
	}
	glEnd();

	//Duoi
	glLineWidth(3.0f);
	glColor3f(0.596, 0.514, 0.122);
	glBegin(GL_LINE_STRIP);
	glVertex2f(lrIndex3 + 28, meo3 + 100 - 11);
	glVertex2f(lrIndex3 + 32, meo3 + 100 - 10);
	glVertex2f(lrIndex3 + 31, meo3 + 100 - 8);
	glVertex2f(lrIndex3 + 32, meo3 + 100 - 7);
	glEnd();

	//Thân
	glColor3f(0.937, 0.894, 0.69);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 28 + 3 * cos(theta), meo3 + 100 - 9 + 3 * sin(theta));
	}
	glEnd();

	//Chân trước
	glColor3f(0.596, 0.514, 0.122);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 27 + cos(theta), meo3 + 100 - 10 + sin(theta));
	}
	glEnd();
	glColor3f(0.596, 0.514, 0.122);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(lrIndex3 + 29 + cos(theta), meo3 + 100 - 10 + sin(theta));
	}
	glEnd();
	meo3--;
	if (meo3 < -100)
	{
		meo3 = 0;
		lrIndex3 = lrIndex;
	}

	//Đụng mèo kem
	if ((abs(lrIndex - lrIndex3) < 8) && (meo3 + 100 < 10))
	{
		start = 0;
		gv = 1;
	}
}




void StartGame() {
	//Đường
	glColor3f(0.376, 0.188, 0);
	glBegin(GL_POLYGON);
	glVertex2f(20, 0);
	glVertex2f(20, 100);
	glVertex2f(80, 100);
	glVertex2f(80, 0);
	glEnd();

	//Lề đường trái
	glColor3f(1.000, 1.000, 1.000);
	glBegin(GL_POLYGON);
	glVertex2f(20, 0);
	glVertex2f(20, 100);
	glVertex2f(23, 100);
	glVertex2f(23, 0);
	glEnd();

	//Lề đường phải
	glColor3f(1.000, 1.000, 1.000);
	glBegin(GL_POLYGON);
	glVertex2f(77, 0);
	glVertex2f(77, 100);
	glVertex2f(80, 100);
	glVertex2f(80, 0);
	glEnd();

	//Cộng 1 điểm
	//Khi chuột vượt qua 3 chướng ngại mèo thì sẽ được cộng 1 điểm
	roadDivBtm--;
	if (roadDivBtm < -20)
	{
		roadDivBtm = 100;
		score++;
	}

	//Bảng điểm
	glColor3f(0.000, 0.000, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(80, 97);
	glVertex2f(100, 97);
	glVertex2f(100, 98 - 8);
	glVertex2f(80, 98 - 8);
	glEnd();

	//In điểm
	char buffer[50];
	sprintf_s(buffer, "SCORE: %d", score);
	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(80.5, 95, (void *)font3, buffer);

	//In tốc độ
	char buffer1[50];
	sprintf_s(buffer1, "SPEED:%dKm/h", FPS);
	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(80.5, 95 - 2, (void *)font3, buffer1);

	//in level
	if (score % 10 == 0) {
		int last = score / 10;
		if (last != level) {
			level = score / 10;
			FPS = FPS + 2;

		}
	}
	char level_buffer[50];
	sprintf_s(level_buffer, "LEVEL: %d", level);
	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(80.5, 95 - 4, (void *)font3, level_buffer);

	//Gọi hàm
	bulletFire();
	drawFire();
	chuot();
	Phomai();
	Meo1();
	Meo2();
	Meo3();
	
}



void TrangChủ() {
	//Trang chủ
	//Nền cỏ
	glColor3f(0.000, 0.392, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(0, 55);
	glVertex2f(100, 55);
	glColor3f(0.604, 0.804, 0.196);
	glVertex2f(100, 50 - 50);
	glVertex2f(0, 50 - 50);
	glEnd();

	//Đường
	glColor3f(0.376, 0.188, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(32 - 2 + 21, 55);
	glVertex2f(32 + 58, 50 - 50);
	glVertex2f(32 - 22, 50 - 50);
	glEnd();

	//Trời
	glColor3f(0.000, 0.749, 1.000);
	glBegin(GL_POLYGON);
	glVertex2f(100, 100);
	glVertex2f(0, 100);
	glColor3f(0.686, 0.933, 0.933);
	glVertex2f(0, 55);
	glVertex2f(100, 55);
	glEnd();

	//Mặt trời
	float theta;
	glColor3f(0.99, 0.76, 0.0078);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(90 + 5 * cos(theta), 90 + 4 * sin(theta));
	}
	glEnd();

	glColor3f(0.99, 0.76, 0.0078);
	glBegin(GL_TRIANGLES);
	glVertex2f(90, 98);
	glVertex2f(92, 95);
	glVertex2f(88, 95);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(90, 82);
	glVertex2f(92, 85);
	glVertex2f(88, 85);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(99.5, 90);
	glVertex2f(96, 92);
	glVertex2f(96, 88);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(80.5, 90);
	glVertex2f(84, 92);
	glVertex2f(84, 88);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(92.5, 95);
	glVertex2f(96, 92.5);
	glVertex2f(96.5, 96);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(92.5, 85);
	glVertex2f(96, 87.5);
	glVertex2f(96.5, 84);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(87.5, 95);
	glVertex2f(84, 92.5);
	glVertex2f(83.5, 96);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(87.5, 85);
	glVertex2f(84, 87.5);
	glVertex2f(83.5, 84);
	glEnd();

	//Đồi 1
	glColor3f(0.753, 0.753, 0.753);
	glBegin(GL_TRIANGLES);
	glVertex2f(20, 55 + 10);
	glVertex2f(20 + 7, 55);
	glVertex2f(0, 55);
	glEnd();

	//Đồi 2
	glColor3f(0.459, 0.459, 0.459);
	glBegin(GL_TRIANGLES);
	glVertex2f(20 + 15, 55 + 12);
	glVertex2f(20 + 20 + 10, 55);
	glVertex2f(0 + 10, 55);
	glEnd();

	//Đồi 3
	glColor3f(0.459, 0.459, 0.459);
	glBegin(GL_TRIANGLES);
	glVertex2f(70, 70);
	glVertex2f(90, 55);
	glVertex2f(50, 55);
	glEnd();

	//Đồi 4
	glColor3f(0.620, 0.620, 0.620);
	glBegin(GL_TRIANGLES);
	glVertex2f(87, 55 + 10);
	glVertex2f(100, 55);
	glVertex2f(60, 55);
	glEnd();

	//Cây hoa bên trái
	//Thân
	glColor3f(0, 0.325, 0.161);
	glBegin(GL_TRIANGLES);
	glVertex2f(11, 58);
	glVertex2f(11.5, 55 - 10);
	glVertex2f(10.5, 55 - 10);
	glEnd();

	glColor3f(0, 0.325, 0.161);
	glBegin(GL_TRIANGLES);
	glVertex2f(13, 53);
	glVertex2f(11.5, 55 - 10);
	glVertex2f(10.5, 55 - 10);
	glEnd();

	glColor3f(0, 0.325, 0.161);
	glBegin(GL_TRIANGLES);
	glVertex2f(9, 53);
	glVertex2f(11.5, 55 - 10);
	glVertex2f(10.5, 55 - 10);
	glEnd();

	//Hoa
	glColor3f(0.99, 0.76, 0.0078);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(11 + 4 * cos(theta), 55 + 3 + 4 * sin(theta));
	}
	glEnd();

	glColor3f(0.424, 0.212, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		theta = i*3.142 / 180;
		glVertex2f(11 + 2 * cos(theta), 55 + 3 + 2 * sin(theta));
	}
	glEnd();

	tree(5, -15);
	tree(9, 5);
	tree(85, 9);
	tree(75, -5);

	//Khung menu
	glColor3f(0.098, 0.098, 0.439);
	glBegin(GL_POLYGON);
	glVertex2f(32 - 7, 50 + 5 + 10);
	glVertex2f(32 + 48, 50 + 5 + 10);
	glVertex2f(32 + 48, 50 - 16 + 10);
	glVertex2f(32 - 7, 50 - 16 + 10);
	glEnd();

	glColor3f(00, 0, 0.000);
	glBegin(GL_POLYGON);
	glVertex2f(32 - 7, 50 + 5 + 10);
	glVertex2f(32 + 48, 50 + 5 + 10);
	glVertex2f(32 + 48, 50 + 4 + 10);
	glVertex2f(32 - 7, 50 + 4 + 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(32 + 47, 50 + 5 + 10);
	glVertex2f(32 + 48, 50 + 5 + 10);
	glVertex2f(32 + 48, 50 - 16 + 10);
	glVertex2f(32 + 47, 50 - 16 + 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(32 - 7, 50 - 15 + 10);
	glVertex2f(32 + 48, 50 - 15 + 10);
	glVertex2f(32 + 48, 50 - 16 + 10);
	glVertex2f(32 - 7, 50 - 16 + 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(32 - 7, 50 + 5 + 10);
	glVertex2f(32 - 8, 50 + 5 + 10);
	glVertex2f(32 - 8, 50 - 16 + 10);
	glVertex2f(32 - 7, 50 - 16 + 10);
	glEnd();

	//Thông tin trên trang chính
	if (gv == 1)
	{
		glColor3f(1.000, 0.000, 0.000);
		renderBitmapString(35, 60 + 10, (void *)font1, "GAME OVER");//thua
		glColor3f(1.000, 0.000, 0.000);
		char buffer2[50];
		sprintf_s(buffer2, "Your Score is : %d", score);
		renderBitmapString(33, 60 - 4 + 10, (void *)font1, buffer2);
	}

	glColor3f(1.000, 1.000, 0.000);
	renderBitmapString(30, 80, (void *)font1, "Cat and Mouse Game ");//tên game

	glColor3f(0.000, 1.000, 0.000);
	renderBitmapString(30, 50 + 10, (void *)font2, "Press SPACE to START");//spacebar bắt đầu
	renderBitmapString(30, 50 - 3 + 10, (void *)font2, "Press ESC to Exit");//esc thoát game

	glColor3f(1.000, 1.000, 1.000);
	renderBitmapString(30, 50 - 6 + 10, (void *)font3, "Press UP to increase Speed");//LÊN tăng tốc
	renderBitmapString(30, 50 - 8 + 10, (void *)font3, "Press DOWN to decrease Speed");//XUỐNG giảm tốc
	renderBitmapString(30, 50 - 10 + 10, (void *)font3, "Press RIGHT to turn Right");//PHẢI qua phải
	renderBitmapString(30, 50 - 12 + 10, (void *)font3, "Press LEFT to turn Left");//TRÁI qua trái
	renderBitmapString(30, 50 - 14 + 10, (void *)font3, "Eat the CHEESE for +2 points!"); // ăn phô mai cộng 2 điểm
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (start == 1)
	{
		glClearColor(0.000, 0.392, 0.000, 1);
		StartGame();
	}
	else {
		TrangChủ();
	}
	glFlush();
	glutSwapBuffers();
}

void reshape(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		if (FPS>(50 + (level * 2)))
			FPS = FPS - 2;
		break;
	case GLUT_KEY_UP:
		FPS = FPS + 2;
		break;

	case GLUT_KEY_LEFT:
		if (lrIndex >= 0) {
			lrIndex = lrIndex - (FPS / 10);
			if (lrIndex<0) {
				lrIndex = -1;
			}
		}
		break;


	case GLUT_KEY_RIGHT:
		if (lrIndex <= 44) {
			lrIndex = lrIndex + (FPS / 10);
			if (lrIndex>44) {
				lrIndex = 45;
			}
		}
		break;

	default:
		break;
	}
}

void inputProcess(unsigned char key, int x, int y) {

	switch (key)
	{
	case ' ':
		if (start == 0) {
			start = 1;
			gv = 0;
			FPS = 50;
			roadDivTopMost = 0;
			roadDivTop = 0;
			roadDivMdl = 0;
			roadDivBtm = 0;
			lrIndex = 0;
			meo1 = 0;
			lrIndex1 = 0;
			meo2 = +35;
			lrIndex2 = 0;
			meo3 = +70;
			lrIndex3 = 0;
			score = 0;
			level = 0;
		}
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void updateWorld(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, updateWorld, 0);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 650);
	glutInitWindowPosition(200, 20);
	init();

	glutCreateWindow("Cat and Mouse Game");

	glutDisplayFunc(display);
	glutSpecialFunc(reshape);
	glutKeyboardFunc(inputProcess);

	glutMouseFunc(onClick);
	glOrtho(0, 100, 0, 100, -1, 1);
	glClearColor(0.184, 0.310, 0.310, 1);

	glutTimerFunc(1000, updateWorld, 0);
	glutMainLoop();

	return 0;
}

