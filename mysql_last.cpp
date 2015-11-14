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
			cout<<"login failure"<<endl;
			cout<<"please login again"<<endl;
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
		cout<<year<<" "<<month<<" "<<day;
		int quarter = getQuarter(day,month);

	}
private:
	int userId;
	char password[10];
	int getQuarter(int day, int month) {
		if 
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	conn = mysql_init ( NULL );
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
