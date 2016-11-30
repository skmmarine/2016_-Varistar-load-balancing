//main.cpp
#include "mainheader.h"

void changeAlbaState(string SmallGroup,alba* emp[],int index)
{
	int time=0;
	if(SmallGroup == "\"espresso\"")
		time = emp[index]->getespressoT();
	else if(SmallGroup == "\"cafemoca\"")
		time = emp[index]->getcafemocaT();
	else if(SmallGroup == "\"icetea\"")
		time = emp[index]->geticeteaT();
	else if(SmallGroup == "\"americano\"")
		time = emp[index]->getamericanoT();
	else if(SmallGroup == "\"cafelatte\"")
		time = emp[index]->getcafelatteT();
	else if(SmallGroup == "\"lemonade\"")
		time = emp[index]->getlemonadeT();
	emp[index]->incmakeTime(time);
	emp[index]->setmakeName(SmallGroup);
}
void printallstate(string enterID,alba* emp[])
{
	cout<<"adminID  "<<enterID<<endl<<endl;
	cout<<" Load Balancing State"<<endl<<endl;
	for(int i=0; i<ALBA_NUM; i++)
	{
		if(emp[i]->getState())
		{
			if(i>=SKILL_ALBA && i<=ALBA_NUM)
				cout<<"skilled Barista "<<setw(2)<<i+1;
			else
				cout<<"    Barista     "<<setw(2)<<i+1;
			cout<<" [ beverage :"<<setw(12)<<emp[i]->getmakeName()<<"]";
			cout<<" Time remaining : "<<emp[i]->getmakeTime()<<endl;
		}
	}
}
void tictok(alba* emp[]) //시간이 1초 지남에 따라 알바의 상태를 변경
{
	Sleep(1000); //1초 기다린 후
	for(int i=0; i<ALBA_NUM; i++) //모든 바리스타에 대하여 검사
	{
		if(emp[i]->getmakeTime() !=0) // 현재 음료를 만드는 중이면
		{
			emp[i]->decmakeTime(); //1초 대기한만큼 음료제조시간이 줄고
			if(emp[i]->getmakeTime() ==0) //제조시간이 0이 되면
			{
				emp[i]->setState(); //상태가 바뀌고
				emp[i]->unsetmakeName(); //현재 음료이름을 제거해준다.
			}
		}
	}
}
bool yetWorking(alba* emp[]) //모든 데이터 로드가 끝난 후
{
	for(int i=0 ;i<ALBA_NUM; i++)
	{
		if(emp[i]->getState()) //누군가 아직 일한다면
			return true;
	}
	return false;
}
void orderProcess(string enterID,alba* emp[], int MenuCount, string SmallGroup, int allwork) //////// whenMySQL, whenText 의 중첩된 while문 함수로 처리
{
	while(MenuCount)
	{
		int index = emp[index]->getIndex(); //음료를 분배할 바리스타의 순서

		if(!(emp[index]->getState() )) //현재 인덱스 바리스타 쉬고 있으면
		{
			emp[index]->setState();                //바리스타 일하는 상태로 변경
			changeAlbaState(SmallGroup,emp,index); //바리스타 객체에 음료이름 전달
			emp[index]->incIndex();                //다음 바리스타를 가리킴
			emp[index]->setIndex();				   //index가 바리스타를 초과했을때 초기화
			MenuCount--;
		}
		else //만약 현재 바리스타가 일하면
		{
			emp[index]->incIndex(); //다음 바리스타를 가리킴
			emp[index]->setIndex(); //index가 바리스타를 초과했을때 초기화
			allwork++;
		}

		if(allwork==ALBA_NUM) //모든 바리스타가 일하는 중이면
		{
			system("cls");
			printallstate(enterID,emp);
			tictok(emp);
			allwork=0; //think all barista state is false
		}
	}
}
int whenText(string enterID,alba* emp[])
{
	ifstream inStream;
	inStream.open("input.txt");
	string BigGroup,openData,closeData,SmallGroup,tmp;
	int MenuCount;

	inStream>>BigGroup>>BigGroup;
	while(true)
	{
		inStream>>openData>>tmp>>SmallGroup>>tmp>>tmp>>tmp>>MenuCount>>closeData;
		// BigGroup 주문큰종류 / SmallGroup 음료종류 / MenuCount 주문 수s
		// 마지막에 한번더 인풋 받아서 } 이면 종료한다.
		if(openData == "]") // 만약 인풋이 종료 기호이면
		{
			while(yetWorking(emp))
			{
				tictok(emp);
				system("cls");
				printallstate(enterID,emp);
			}
			system("cls");
			cout<<"Load balancing has ended."<<endl;
			return 0;
		}
		int allwork=0;

		orderProcess(enterID,emp, MenuCount, SmallGroup, allwork); // 한 줄 주문 완료
	}
}
int whenMySQL(string enterID,alba* emp[])
{
	string str_query;
	MYSQL *conn;
	char *server = "localhost";
	char *user = "root";
	char *password = "qkr9303362"; //root계정 비밀번호
	char *database = "cafe_test"; //005강에서 생성한 데이터베이스
	int port = 3307; //포트번호 ★ 지금은 3307
	conn = mysql_init(NULL);

	/* Connect to SQL */
	if (!mysql_real_connect(conn, server, user, password, database, port, NULL, 0)) {
		fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}

	str_query = "select * from cafe";  // 쿼리문 str_query에 저장

	/* send SQL query */
	// str_query문에 입력된 쿼리문을 mysql에 보냄. 
	// cafe테이블에 있는 모든 데이터를 가져옴
	if (mysql_query(conn, str_query.c_str())) {
		//fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}

	// mysql_store_result() 함수를 사용해 결과셋을 가져옴
	// MYSQL_RES는 결과셋을 담는 구조체
	MYSQL_RES *result = mysql_store_result(conn);
	// 테이블에서 가져온 결과셋의 객수(컬럼)을 저장한다
	// 이는 mysql_num_fields() 함수로 수행
	int num_fields = mysql_num_fields(result);
	// 몇줄의 결과가 발생한지는 mysql_num_rows()함수로 알아냄


	// 테이블의 한줄(ROW)에 해당하는 것을 담고있는 구조체가 MYSQL_ROW 구조체이다.

	MYSQL_ROW row;

	string BigGroup, SmallGroup; // BigGroup은 필요 없어보이네
	int MenuCount;

	// while문은 결과셋을 화면에 출력하는 반복문
	// 데이터가 없으면 null을 출력한다. (관용구적인 표현)
	// 결과셋에서 한줄을 가져오는데 가장 마지막 줄까지 읽었다면 NULL을 리턴해서 while 반복문 종료
	while( (row = mysql_fetch_row(result)) )
	{
		BigGroup = row[0];
		SmallGroup = row[1];
		MenuCount = atoi(row[2]); // atoi 는 문자열을 정수로 변환해주는 함수(iostream 라이브러리에 포함)

		int allwork=0;

		orderProcess(enterID,emp, MenuCount, SmallGroup, allwork);  // 한 줄 주문 완료
	}
	//★ 나는 데이터를 튜플단위로 받아와서 배열에 저장되어있음. 0번 인덱스에 bigGroup, 1번 인덱스에 smallGroup, 2번 인덱스에 음료주문수

	while(yetWorking(emp))
	{
		tictok(emp);
		system("cls");
		printallstate(enterID,emp);
	}
	system("cls");
	cout<<"Load balancing has ended."<<endl;

	// 결과셋을 메모리에 보관하기 때문에(mysql_store_result()), 사용을 마친 결과셋은 최대한 해제시켜야 한다.
	mysql_free_result(result); 

	/* close connection */
	mysql_close(conn);

	return 0;

}
void setalba(alba* emp[])
{
	if(ALBA_NUM<SKILL_ALBA)
	{
		cout<<"waring, #define ALBA is too small. "<<endl;
		Sleep(3000);
		while(true){}
	}
	for(int i=0; i<SKILL_ALBA; i++) //전체 바리스타의 수
		emp[i] = new alba();
	for(int i=SKILL_ALBA; i<ALBA_NUM; i++) //숙련된 바리스타
		emp[i] = new skilled();
}
string checkMaster(string enterID)
{
	while(true)
	{
		ifstream inStream;
		string id, passWord ,enterPassWord;
		//id, password - IDPW file 에서 불러올 관리자 계정 
		inStream.open("IDPW.txt");

		cout<<"Enter Administrator ID : ";
		getline(cin,enterID);
		cout<<"Enter Administrator PW : ";
		getline(cin,enterPassWord);
		while(!inStream.eof())
		{
			inStream>>id>>passWord;
			if(id==enterID && passWord==enterPassWord)
			{
				cout<<"success to login"<<endl;
				Sleep(2000);
				return enterID;
			}
		}
		cout<<"Wrong ID or Password"<<endl<<"Try Again";
		Sleep(1500);
		system("cls");
	}
}
void inputMenual(string enterID,alba* emp[])
{
   while(true)
   {
      int dataType = 0;
      system("cls");
      cout<<"adminID  "<<enterID<<endl<<endl;
      cout << "(1) Data load from txt file   (2) Data load from MySQL  (3) Exit"  << endl<< "choose : ";
      cin >> dataType;
      switch(dataType)
      {      
      case 1: //text file input
         whenText(enterID,emp);
         break;
      case 2:
         whenMySQL(enterID,emp);
         break;
      case 3:
         system("cls");
         return;
      default:
         cout << "Wrong Command" << endl;
      }
      Sleep(1500);
   }
}
int main()
{
	alba* emp[ALBA_NUM]; //바리스타
	string enterID;		//관리자
	enterID = checkMaster(enterID);
	setalba(emp);


	inputMenual(enterID,emp);
	cout << "Exit Program. Thank You" << endl; 
	//inputMenual 에서 출력할수 있지만 메인 루틴에서 종료를 알림

	for(int i=0 ; i<ALBA_NUM; i++)
		delete emp[i];
	return 0;
}