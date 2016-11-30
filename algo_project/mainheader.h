//header.h
#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ws2_32.lib")

// mysql ���� �������
// ������� ���� �ݵ�� �̼����� ( my_global ��������� windows.h���� �ؿ� ������ ������  http://sacstory.tistory.com/entry/MySQL%EC%9D%98-Lib-%EC%82%AC%EC%9A%A9%EB%B2%95)
#include <my_global.h>
#include <mysql.h>
//
#include <iostream>
#include <fstream>
#include <cstdlib> //ȭ�� �ʱ�ȭ�� ���� ���
#include <iomanip> //��� �������� ���
#include <string>
#include "windows.h" //�ð��� ������Ű�� ���� ���

#define ALBA_NUM 20
#define SKILL_ALBA 17
using namespace std;
class alba
{
private :
   bool state;
   static int index;
   int makeTime;
   string makeName;
   const static int americanoT=5;
   const static int espressoT = 5;
   const static int cafemocaT=5;
   const static int iceteaT=5;
   const static int cafelatteT=5; 
   const static int lemonade=5;
public:
   alba():state(false),makeTime(0),makeName(""){} //�ٸ���Ÿ���� �ʱ� ����
   //������� �ð� ��ȯ
   virtual int getamericanoT(){return americanoT;}
   virtual int getespressoT(){return espressoT;}
   virtual int getcafemocaT(){return cafemocaT;}
   virtual int geticeteaT(){return iceteaT;}
   virtual int getcafelatteT(){return cafelatteT;}
   virtual int getlemonadeT(){return lemonade;}
   //������� �̸� ����
   void setmakeName(string tmp) { makeName=tmp;}
   string getmakeName() {return makeName;}
   void unsetmakeName() {makeName="";}
   //�ٸ���Ÿ ����, �ε��� get / set
   bool getState() { return state;}
   void setState() { state ? state=false : state =true;}
   static int getIndex() {return index;}
   static void incIndex() {index++;}
   void setIndex() {if(index==ALBA_NUM) index=0;}
   //�ٸ���Ÿ ���ѽð�
   int getmakeTime() {return makeTime;}
   void incmakeTime(int hour) { makeTime +=hour;}
   void decmakeTime(){makeTime -=1;}
};  int alba::index=0;

class skilled : public alba
{
private :
   static int americanoT;
   static int espressoT;
   static int cafemocaT;
   static int iceteaT;
   static int cafelatteT; 
   static int lemonadeT;
public :
   int getamericanoT(){return americanoT;}
   int getespressoT(){return espressoT;}
   int getcafemocaT(){return cafemocaT;}
   int geticeteaT(){return iceteaT;}
   skilled() :alba(){} //����Ŭ������ ������ �̴ϼȶ����� ���� ������
};
int skilled::americanoT=2;
int skilled::espressoT=2;
int skilled::cafemocaT=2;
int skilled::iceteaT=2;
int skilled::cafelatteT=2;
int skilled::lemonadeT=2;
