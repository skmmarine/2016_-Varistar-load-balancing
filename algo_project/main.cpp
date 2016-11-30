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
void tictok(alba* emp[]) //�ð��� 1�� ������ ���� �˹��� ���¸� ����
{
	Sleep(1000); //1�� ��ٸ� ��
	for(int i=0; i<ALBA_NUM; i++) //��� �ٸ���Ÿ�� ���Ͽ� �˻�
	{
		if(emp[i]->getmakeTime() !=0) // ���� ���Ḧ ����� ���̸�
		{
			emp[i]->decmakeTime(); //1�� ����Ѹ�ŭ ���������ð��� �ٰ�
			if(emp[i]->getmakeTime() ==0) //�����ð��� 0�� �Ǹ�
			{
				emp[i]->setState(); //���°� �ٲ��
				emp[i]->unsetmakeName(); //���� �����̸��� �������ش�.
			}
		}
	}
}
bool yetWorking(alba* emp[]) //��� ������ �ε尡 ���� ��
{
	for(int i=0 ;i<ALBA_NUM; i++)
	{
		if(emp[i]->getState()) //������ ���� ���Ѵٸ�
			return true;
	}
	return false;
}
void orderProcess(string enterID,alba* emp[], int MenuCount, string SmallGroup, int allwork) //////// whenMySQL, whenText �� ��ø�� while�� �Լ��� ó��
{
	while(MenuCount)
	{
		int index = emp[index]->getIndex(); //���Ḧ �й��� �ٸ���Ÿ�� ����

		if(!(emp[index]->getState() )) //���� �ε��� �ٸ���Ÿ ���� ������
		{
			emp[index]->setState();                //�ٸ���Ÿ ���ϴ� ���·� ����
			changeAlbaState(SmallGroup,emp,index); //�ٸ���Ÿ ��ü�� �����̸� ����
			emp[index]->incIndex();                //���� �ٸ���Ÿ�� ����Ŵ
			emp[index]->setIndex();				   //index�� �ٸ���Ÿ�� �ʰ������� �ʱ�ȭ
			MenuCount--;
		}
		else //���� ���� �ٸ���Ÿ�� ���ϸ�
		{
			emp[index]->incIndex(); //���� �ٸ���Ÿ�� ����Ŵ
			emp[index]->setIndex(); //index�� �ٸ���Ÿ�� �ʰ������� �ʱ�ȭ
			allwork++;
		}

		if(allwork==ALBA_NUM) //��� �ٸ���Ÿ�� ���ϴ� ���̸�
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
		// BigGroup �ֹ�ū���� / SmallGroup �������� / MenuCount �ֹ� ��s
		// �������� �ѹ��� ��ǲ �޾Ƽ� } �̸� �����Ѵ�.
		if(openData == "]") // ���� ��ǲ�� ���� ��ȣ�̸�
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

		orderProcess(enterID,emp, MenuCount, SmallGroup, allwork); // �� �� �ֹ� �Ϸ�
	}
}
int whenMySQL(string enterID,alba* emp[])
{
	string str_query;
	MYSQL *conn;
	char *server = "localhost";
	char *user = "root";
	char *password = "qkr9303362"; //root���� ��й�ȣ
	char *database = "cafe_test"; //005������ ������ �����ͺ��̽�
	int port = 3307; //��Ʈ��ȣ �� ������ 3307
	conn = mysql_init(NULL);

	/* Connect to SQL */
	if (!mysql_real_connect(conn, server, user, password, database, port, NULL, 0)) {
		fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}

	str_query = "select * from cafe";  // ������ str_query�� ����

	/* send SQL query */
	// str_query���� �Էµ� �������� mysql�� ����. 
	// cafe���̺� �ִ� ��� �����͸� ������
	if (mysql_query(conn, str_query.c_str())) {
		//fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}

	// mysql_store_result() �Լ��� ����� ������� ������
	// MYSQL_RES�� ������� ��� ����ü
	MYSQL_RES *result = mysql_store_result(conn);
	// ���̺��� ������ ������� ����(�÷�)�� �����Ѵ�
	// �̴� mysql_num_fields() �Լ��� ����
	int num_fields = mysql_num_fields(result);
	// ������ ����� �߻������� mysql_num_rows()�Լ��� �˾Ƴ�


	// ���̺��� ����(ROW)�� �ش��ϴ� ���� ����ִ� ����ü�� MYSQL_ROW ����ü�̴�.

	MYSQL_ROW row;

	string BigGroup, SmallGroup; // BigGroup�� �ʿ� ����̳�
	int MenuCount;

	// while���� ������� ȭ�鿡 ����ϴ� �ݺ���
	// �����Ͱ� ������ null�� ����Ѵ�. (���뱸���� ǥ��)
	// ����¿��� ������ �������µ� ���� ������ �ٱ��� �о��ٸ� NULL�� �����ؼ� while �ݺ��� ����
	while( (row = mysql_fetch_row(result)) )
	{
		BigGroup = row[0];
		SmallGroup = row[1];
		MenuCount = atoi(row[2]); // atoi �� ���ڿ��� ������ ��ȯ���ִ� �Լ�(iostream ���̺귯���� ����)

		int allwork=0;

		orderProcess(enterID,emp, MenuCount, SmallGroup, allwork);  // �� �� �ֹ� �Ϸ�
	}
	//�� ���� �����͸� Ʃ�ô����� �޾ƿͼ� �迭�� ����Ǿ�����. 0�� �ε����� bigGroup, 1�� �ε����� smallGroup, 2�� �ε����� �����ֹ���

	while(yetWorking(emp))
	{
		tictok(emp);
		system("cls");
		printallstate(enterID,emp);
	}
	system("cls");
	cout<<"Load balancing has ended."<<endl;

	// ������� �޸𸮿� �����ϱ� ������(mysql_store_result()), ����� ��ģ ������� �ִ��� �������Ѿ� �Ѵ�.
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
	for(int i=0; i<SKILL_ALBA; i++) //��ü �ٸ���Ÿ�� ��
		emp[i] = new alba();
	for(int i=SKILL_ALBA; i<ALBA_NUM; i++) //���õ� �ٸ���Ÿ
		emp[i] = new skilled();
}
string checkMaster(string enterID)
{
	while(true)
	{
		ifstream inStream;
		string id, passWord ,enterPassWord;
		//id, password - IDPW file ���� �ҷ��� ������ ���� 
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
	alba* emp[ALBA_NUM]; //�ٸ���Ÿ
	string enterID;		//������
	enterID = checkMaster(enterID);
	setalba(emp);


	inputMenual(enterID,emp);
	cout << "Exit Program. Thank You" << endl; 
	//inputMenual ���� ����Ҽ� ������ ���� ��ƾ���� ���Ḧ �˸�

	for(int i=0 ; i<ALBA_NUM; i++)
		delete emp[i];
	return 0;
}