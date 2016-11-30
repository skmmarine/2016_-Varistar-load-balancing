//header.h
#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ws2_32.lib")

// mysql 관련 헤더파일
// 헤더파일 순서 반드시 이순서로 ( my_global 헤더파일이 windows.h보다 밑에 있으면 에러남  http://sacstory.tistory.com/entry/MySQL%EC%9D%98-Lib-%EC%82%AC%EC%9A%A9%EB%B2%95)
#include <my_global.h>
#include <mysql.h>
//
#include <iostream>
#include <fstream>
#include <cstdlib> //화면 초기화를 위한 헤더
#include <iomanip> //출력 정렬위한 헤더
#include <string>
#include "windows.h" //시간을 지연시키기 위한 헤더

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
   alba():state(false),makeTime(0),makeName(""){} //바리스타들의 초기 상태
   //음료들의 시간 반환
   virtual int getamericanoT(){return americanoT;}
   virtual int getespressoT(){return espressoT;}
   virtual int getcafemocaT(){return cafemocaT;}
   virtual int geticeteaT(){return iceteaT;}
   virtual int getcafelatteT(){return cafelatteT;}
   virtual int getlemonadeT(){return lemonade;}
   //음료들의 이름 설정
   void setmakeName(string tmp) { makeName=tmp;}
   string getmakeName() {return makeName;}
   void unsetmakeName() {makeName="";}
   //바리스타 상태, 인덱스 get / set
   bool getState() { return state;}
   void setState() { state ? state=false : state =true;}
   static int getIndex() {return index;}
   static void incIndex() {index++;}
   void setIndex() {if(index==ALBA_NUM) index=0;}
   //바리스타 일한시간
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
   skilled() :alba(){} //슈퍼클래스의 생성자 이니셜라이즈 통한 생성자
};
int skilled::americanoT=2;
int skilled::espressoT=2;
int skilled::cafemocaT=2;
int skilled::iceteaT=2;
int skilled::cafelatteT=2;
int skilled::lemonadeT=2;
