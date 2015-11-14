// mysql_last.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
typedef unsigned int SOCKET;
#ifndef my_socket_defined

#define my_socket SOCKET

#endif
#include "mysql.h"
using namespace std;

MYSQL *conn;
class show {
public:
	void starter(){
		cout<<"1.student login"<<endl;
		cout<< ">>";
	}
	void login() {
		cout<<"userId:";
		cin>>userId;
		cout<<"password:";
		cin>>password;
		char* combine = new char[100];
		sprintf(combine,"SELECT * FROM student WHERE Id = %d AND Password = '%s';", userId, password);
		conn = mysql_init ( NULL );
		mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
		MYSQL_RES *res_set;
		MYSQL_ROW row;
		mysql_query(conn,combine);//return int; 0 is true, 1 is false 
		res_set = mysql_store_result(conn);
		int numrows = (int)mysql_num_rows(res_set);
		mysql_free_result( res_set );
		mysql_close( conn );
		if (numrows == 1) {
			cout<<"login successful"<<endl;
			studentMenu();
		} else {
			cout<<"login failure!"<<endl;
			cout<<"please login again..."<<endl;
			login();
		}
	}
	void studentMenu() {
		time_t timer;
		time(&timer);
		tm* t_tm = localtime(&timer);
		int year = t_tm->tm_year + 1900;
		int month = t_tm->tm_mon + 1;
		int day = t_tm->tm_mday;
		int quarter = getQuarter(day,month);
		char* combine = new char[200];
		sprintf(combine,"SELECT UoSCode, UoSName FROM unitofstudy join transcript using(UoSCode)  WHERE StudId = %d AND Grade is null;", userId);
		conn = mysql_init ( NULL );
		mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
		MYSQL_RES *res_set;
		MYSQL_ROW row;
		int a = mysql_query(conn,combine);//return int; 0 is true, 1 is false 

		res_set = mysql_store_result(conn);
		int numrows = (int)mysql_num_rows(res_set);
		// Display results
		cout<<"UoSCode:     "<<"UoSName: "<<endl;
		for (int i = 0; i < numrows; i++)
		{
			row = mysql_fetch_row( res_set ); 
			if( row != NULL )
			{
				cout << row[0] <<"     "<< row[1] << endl;
			}
		}
		// free resources 
		mysql_free_result( res_set );
		mysql_close( conn );
		cout<<"Options:"<<endl;
		cout<<"1.Transcript"<<endl;
		cout<<"2.Enroll"<<endl;
		cout<<"3.Withdraw"<<endl;
		cout<<"4.Personal Details"<<endl;
		cout<<"5.Logout"<<endl;
		bool pass = true;
		while (pass){
			cout<< ">>";
			int choose;
			cin>>choose;
			switch(choose) {
			case 1:
				cout<<"Transcript"<<endl;
				pass = false;
				break;
			case 2:
				cout<<"Enroll"<<endl;
				pass = false;
				break;
			case 3:
				cout<<"Withdraw"<<endl;
				pass = false;
				break;
			case 4:
				cout<<"Personal Details"<<endl;
				pass = false;
				break;
			case 5:
				cout<<"Logout"<<endl;
				pass = false;
				break;
			default:
				cout<<"unidentified selection"<<endl;
				break;
			}
		}
	}
	void Enroll() {

	}
private:
	int userId;
	char password[10];
	int getQuarter(int day, int month) {
		if (month >= 9 && month <= 12) {
			return 1;
		}
		if (month >= 1 && month <= 2) {
			return 2;
		}
		if (month == 3) {
			if (day <= 26 && day >= 0) {
				return 2;
			}
			if (day>=27 && day <= 31) {
				return 3;
			}
		}
		if (month >= 4 && month <= 5) {
			return 3;
		}
		if (month = 6) {
			if (day <= 19 && day >= 0) {
				return 3;
			}
			if (day >= 20 && day<= 30) {
				return 4;
			}
		}
		return 4;
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	show rafe;
	rafe.starter();
	int choose;
	cin>>choose;
	if (choose == 1) {
		cout<<"next"<<endl;
		rafe.login();
		

	}
	
	/*
	// open connection 
	mysql_real_connect (
		conn, // pointer to connection handler 
		"localhost", // host to connect to
		"user_name", // user name 
		"password", // password  
		"test", // database to use 
		0, // port (default 3306) 
		NULL, // socket or /var/lib/mysql.sock  
		CLIENT_MULTI_RESULTS ); // flags (none) 
	*/
	// close connection 
	conn = mysql_init ( NULL );
	if(mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0))
		cout<<"success"<<endl;
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	mysql_query(conn,"SELECT * FROM student;"); 
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	// Display results
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row( res_set ); 
		if( row != NULL )
		{
			cout << "ID : " << row[0] << endl; cout << "Name: " << row[1] << endl;
		}
	}
	// free resources 
	mysql_free_result( res_set );
	mysql_close( conn );
	system("pause");
	return 0;
}
