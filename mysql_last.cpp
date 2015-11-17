// mysql_last.cpp : 定义控制台应用程序的入口点。
//

#include <mysql.h>
#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include <unordered_set>
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
        try{
            char* sql = new char[100];
            conn = mysql_init ( NULL );
            sprintf(sql, "ALTER TABLE uosoffering ADD status VARCHAR(60)");
            mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
            mysql_query(conn, sql);
        }
        catch(int e){
            cout << "already modified"<< endl;
        }
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
        int result;
        if (numrows == 1) {
            cout<<"login successful"<<endl;
            result = studentMenu();
            while(result){
                result = studentMenu();
            }
        } else {
            cout<<"login failure!"<<endl;
            cout<<"please login again..."<<endl;
            login();
        }
    }

    int studentMenu() {
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);
        int year = t_tm->tm_year + 1900;
        int month = t_tm->tm_mon + 1;
        int day = t_tm->tm_mday;
        int quarter = getQuarter(day, month);
        char* combine = new char[1024];
        sprintf(combine,"SELECT UoSCode, UoSName FROM unitofstudy join transcript using(UoSCode)  WHERE StudId = %d AND Grade is null AND Semester = 'Q%d' AND Year = '%d';", userId, quarter, year);
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int a = mysql_query(conn,combine);//return int; 0 is true, 1 is false

        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        // Display results
        cout<<"CourseID:    "<<"CourseName: "<<endl;
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
            int choose,result;
            cin>>choose;
            switch(choose) {
                case 1:
                    cout<<"Transcript"<<endl;
                    result = transcript();
                    return result;
                case 2:
                    pass = false;
                    Enroll();
                    break;
                case 3:
                    cout<<"Withdraw"<<endl;
                    result = withdraw();
                    return result;
                case 4:
                    cout<<"Personal Details"<<endl;
                    result = updateDetails();
                    return result;
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

    int transcript(){
        char courseNum[10];
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        char* sql = new char[1024];
        sprintf(sql, "select UoSCode, Grade, UoSName from transcript inner join unitofstudy using(UoSCode) where studid = %d;", userId);
        int status = mysql_query(conn,sql);
        if(status){
            cout << "cannot connet to the mysql server";
            mysql_close(conn);
            exit(0);
        }
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row(res_set);
            if (row != NULL) {
                cout << "Course Id : " << row[0];
                if (row[1]) {
                    cout << " Course Name: " << row[2] << " Grade: " << row[1] << endl;
                } else {
                    cout << " Course Name: " << row[2] << " Grade: null" << endl;
                }
            }
        }
        //mysql_close(conn);
        while(1) {
            cout << "1.go back to student menu" << endl;
            cout << "2.check details of a course" << endl;
            cout << ">>";
            int chooseTrans;
            cin >> chooseTrans;
            if (cin.fail()) {
                cout << "invalid input " << endl;
                cin.clear();
                while(cin.get() != '\n');
            }
            if(chooseTrans == 1){
                //mysql_free_result(res_set);
                //mysql_close(conn);
                return 1;
            } else if (chooseTrans == 2){
                cout << "please enter the course number: ";
                cin >> courseNum;
                sprintf(sql, "select transcript.uoscode, unitofstudy.uosname, "
                        "transcript.year, transcript.semester, uosoffering.enrollment, "
                        "uosoffering.maxenrollment, faculty.name, transcript.grade "
                        "from transcript inner join unitofstudy using(UoSCode) "
                        "inner join uosoffering on transcript.uoscode = uosoffering.uoscode "
                        "and transcript.year = uosoffering.year inner join faculty "
                        "on faculty.id = uosoffering.instructorid "
                        "where transcript.studid = %d and transcript.uoscode = \"%s\";", userId, courseNum);
                status = mysql_query(conn,sql);
                if(status){
                    cout << "course number might be wrong" << endl;
                    continue;
                }
                res_set = mysql_store_result(conn);
                row = mysql_fetch_row(res_set);
                if (!row){
                    cout << "this course does not exist" << endl;
                    continue;
                }
                cout << "couse number: " << row[0] << " course title: " << row[1];
                cout << " year : " << row[2] << " quarter :" << row[3];
                cout << " number of enrolled students: " << row[4];
                cout << " maximun enrolled students: " << row[5];
                cout << " lectuer name: " << row[6];
                if(row[7]){
                    cout << "grade: " << row[7] << endl;
                } else {
                    cout << "grade: null" << endl;
                }
            } else {
                cout << "please enter the right number" << endl;
            }
        }
    }

    void Enroll() {
        cout<<"Do u want to enroll in some courses? Y/N"<<endl;
        string answer;
        cin>>answer;
        if (answer == "N") {
            return ;
        }
        answer = "";
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);
        int year = t_tm->tm_year + 1900;
        int month = t_tm->tm_mon + 1;
        int day = t_tm->tm_mday;
        int quarter = getQuarter(day, month);
        int followingquarter = quarter%4 + 1;
        char* combine = new char[1024];
        sprintf(combine,"SELECT UoSCode, Credits,Semester, UoSName FROM unitofstudy join whenoffered using(UoSCode) join uosoffering using(UoSCode, Semester) WHERE Semester = 'Q%d' OR Semester = 'Q%d' order by Semester;",quarter, followingquarter);
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int a = mysql_query(conn,combine);//return int; 0 is true, 1 is false

        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        unordered_set<string> xulihao2;
        // Display results
        cout<<"CourseID:  "<<"Credits:  "<<"Semester:  "<<"CourseName: "<<endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row( res_set );
            if( row != NULL )
            {
                xulihao2.insert(row[0]);
                cout << row[0] <<"   "<< row[1] <<"         "<< row[2]<<"         "<< row[3]<< endl;
                xulihao2.insert(reverseCase(row[0]));
            }
        }
        // free resources
        cout<<"Which course? Please show us the CourseID"<<endl<<">>";
        cin>>answer;
        if (xulihao2.find(answer) != xulihao2.end()) {
            cout<<"find it"<<endl;
        }


        mysql_free_result( res_set );
        mysql_close( conn );
    }

    int withdraw() {
        //print the courses the student has enrolled in with null grade
        char* sql = new char[1024];
        sprintf(sql, "select unitofstudy.uosname, transcript.uoscode, semester, year from transcript inner join unitofstudy using(uoscode) where grade is null and studid = %d;", userId);
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int status = mysql_query(conn,sql);//return int; 0 is true, 1 is false
        if(status){
            cout << "cannot connet to the mysql server";
            mysql_close(conn);
            exit(0);
        }
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row(res_set);
            if (row != NULL) {
                cout << "Course Id : " << row[1];
                cout << " Course Name : " << row[0];
                cout << " Semester: " << row[2];
                cout << " Year: " << row[3] << endl;
            }
        }
        mysql_free_result(res_set);
        cout << "1.go back to student menu" << endl;
        cout << "2.withdraw an enrolled class" << endl;
        cout << ">>";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cout << "invalid input " << endl;
            cin.clear();
            while(cin.get() != '\n');
        }
        if(choice == 1){
            return 1;
        } else if (choice == 2){
            string courseNum;
            string semester;
            time_t timer;
            time(&timer);
            tm* t_tm = localtime(&timer);
            int year = t_tm->tm_year + 1900;
            int month = t_tm->tm_mon + 1;
            int day = t_tm->tm_mday;
            int quarter = getQuarter(day, month);
            int followingquarter = quarter%4 + 1;
            int followingyear = quarter == 4? year+1:year;
            cout << "please enter the course number:" << endl;
            cout << ">>";
            cin >> courseNum;
            cout << "please enter semester of this course, e.g Q1:" << endl;
            cout << ">>";
            cin >> semester;
            int semNum = checkStr(semester);
            cout << " semNum value is: " << semNum << endl;
            sprintf(sql, "select * from transcript where studid = %d and uoscode = '%s' and semester = '%s';", userId, courseNum.c_str(), semester.c_str());
            status = mysql_query(conn,sql);
            if(status){
                cout << "course number might be wrong" << endl;
                return 1;
            }
            res_set = mysql_store_result(conn);
            row = mysql_fetch_row(res_set);
            if (!row){
                cout << "this course does not exist" << endl;
                return 1;
            }
            if (quarter == semNum) {
                cout << " year: " << year << "quarter :"<< quarter << endl;
                sprintf(sql, "CALL withdraw_class('%s', '%s', %d, %d)", courseNum.c_str(), semester.c_str(), year, userId);
                cout << "p1 break2" << endl;
                status = mysql_query(conn,sql);
                if(status){
                    cout << " cannot connet to the mysql server";
                    mysql_close(conn);
                    exit(0);
                }
                res_set = mysql_store_result(conn);
                mysql_free_result(res_set);
                query_warning(courseNum, semester, year);
            } else {
                cout << " year: " << followingyear << "quarter :"<< followingquarter << endl;
                sprintf(sql, "CALL withdraw_class('%s', '%s', %d, %d)", courseNum.c_str(), semester.c_str(), followingyear, userId);
                status = mysql_query(conn,sql);
                if(status){
                    cout << " cannot connet to the mysql server";
                    mysql_close(conn);
                    exit(0);
                }
                res_set = mysql_store_result(conn);
                mysql_free_result(res_set);
                query_warning(courseNum, semester, followingyear);
            }
        } else {
            cout << "you entered an invalid number, jumps to sutdent menu now" << endl;
            return 1;
        }
        return 1;
    }

    int updateDetails(){
        char* sql = new char[1024];
        sprintf(sql, "select * from student where id = %d", userId);
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int status = mysql_query(conn,sql);//return int; 0 is true, 1 is false
        if(status){
            cout << "cannot connet to the mysql server";
            mysql_close(conn);
            exit(0);
        }
        res_set = mysql_store_result(conn);
        row = mysql_fetch_row(res_set);
        cout << "id : " << row[0] << " name: " << row[1] << " password: " << row[2] << " address: " << row[3]<<endl;
        cout << "1.go back to student menu" << endl;
        cout << "2.update personal detail" << endl;
        cout << ">>";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cout << "invalid input " << endl;
            cin.clear();
            while(cin.get() != '\n');
        }
        if(choice == 1){
            return 1;
        } else{
            cout << "1.update password" << endl;
            cout << "2.update address" << endl;
            cout << ">>";
            int update_choice;
            cin >> update_choice;
            if (update_choice == 1){
                char* password = new char[100];
                cout << "please enter new password:" << endl;
                cout << ">>";
                cin >> password;
                sprintf(sql, "update student set password = '%s' where id = %d;", password, userId);
                mysql_query(conn, sql);
                cout << "password has been changed"<< endl;
            } else if (update_choice == 2){
                char* address = new char[100];
                cout << "please enter new address:" << endl;
                cout << ">>";
                cin >> address;
                sprintf(sql, "update student set address = '%s' where id = %d;", address, userId);
                mysql_query(conn, sql);
                cout << "address has been changed" << endl;
            } else {
                cout << "you entered an invalid number, jumps to sutdent menu now" << endl;
                return 1;
            }
        }
        return 1;
    }

    int checkStr(string input){
        if (input.length() != 2){
            return -1;
        } else {
            return atoi(input.substr(1,1).c_str());
        };
    }

private:
    int userId;
    char password[10];

    int getQuarter(int day,int month) {
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

    char* reverseCase(char* strings){
        int length = strlen(strings);
        for (int i=0; i<length; i++) {
            if(strings[i] >= 'A' && strings[i] <= 'Z') {
                strings[i] = strings[i] + 32;
            }
        }
        return strings;
    }

    void query_warning(string courseNum, string semester, int year){
        conn = mysql_init (NULL);
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        char* sql = new char[1024];
        char* status= new char[20];
        sprintf(status,"warning");
        sprintf(sql, "select status from uosoffering where uoscode = '%s' and semester = '%s' and year = %d;", courseNum.c_str(), semester.c_str(), year);
        try {
            mysql_query(conn, sql);//return int; 0 is true, 1 is false
            res_set = mysql_store_result(conn);
            row = mysql_fetch_row(res_set);
            if (strcmp(row[0],"warning")==0){
                cout << "the enrollment number of course " << courseNum << " is less than 50% of max enrollment: "  << endl;
            }
        }
        catch(int e){
            cout << "exception : "<< e << endl;
        }
    }
};

int main()
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
