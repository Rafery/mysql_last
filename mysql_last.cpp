// mysql_last.cpp : 定义控制台应用程序的入口点。
//

#include <iomanip>
#include <mysql.h>
#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include <unordered_set>
#include <map>
#include <sstream>
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
        cout<<"1.Student Login"<<endl;
        cout<<"2.Exit"<<endl;
        cout<< ">>";
        int choose;
        cin>>choose;
        int result=0;
        if (choose == 1) {
            result = login();
        }
        while (result) {
            result = login();
        }
        exit(0);
    }

    int login() {
        cout << "[Login] Please input your userId and password: "<< endl;
        cout<<"userId:";
        cin>>userId;
        cout<<"password:";
        cin>>password;
        char* combine = new char[100];
        sprintf(combine,"SELECT * FROM student WHERE Id = '%s' AND Password = '%s';", userId.c_str(), password);
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
            while(result==1){
                result = studentMenu();
            }
            if (result == 0){
                return 0;
            }
        } else {
            cout<<"login failed, your course id or password is incorrect"<<endl;
            cout<<"please login again..."<<endl;
            login();
        }
        return 1;
    }

    int studentMenu() {
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);
        int year = t_tm->tm_year + 1900;
        int month = t_tm->tm_mon + 1;
        int day = t_tm->tm_mday;
        int quarter = getQuarter(day, month);
        char* combine = new char[200];
        sprintf(combine,"SELECT UoSCode,Grade, UoSName FROM unitofstudy join transcript using(UoSCode)  WHERE StudId = '%s' AND Semester = 'Q%d' AND Year = '%d';", userId.c_str(), quarter, year);
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int a = mysql_query(conn,combine);//return int; 0 is true, 1 is false

        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        // Display results
        cout<<"CourseID:  Grade:   "<<"CourseName: "<<endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row( res_set );
            if( row != NULL )
            {
                if (row[1] == NULL){
                    cout << row[0] <<"   "<<"null"<<"     "<< row[2] << endl;
                } else {
                    cout << row[0] <<"   "<<row[1]<<"        "<< row[2] << endl;
                }
            }
        }
        // free resources
        mysql_free_result( res_set );
        mysql_close( conn );
        cout << endl;
        cout << "[Student Menu] Please choose one option: "<< endl;
        cout<<"1.Transcript"<<endl;
        cout<<"2.Enroll"<<endl;
        cout<<"3.Withdraw"<<endl;
        cout<<"4.Personal Details"<<endl;
        cout<<"5.Logout"<<endl;
        cout<<"6.Exit"<<endl;
        bool pass = true;

        while (pass){
            cout<< ">>";
            int choose,result;
            cin>>choose;
            if (cin.fail()) {
                cout << "invalid input " << endl;
                cin.clear();
                while(cin.get() != '\n');
            }
            switch(choose) {
                case 1:
                    result = transcript();
                    return result;
                case 2:
                    pass = false;
                    result = Enroll();
                    return result;
                case 3:
                    result = withdraw();
                    return result;
                case 4:
                    result = updateDetails();
                    return result;
                case 5:
                    cout<<"Logout"<<endl;
                    return -1;
                case 6:
                    cout<<"Exit"<<endl;
                    return 0;
                default:
                    cout<<"unidentified selection"<<endl;
                    cout<<"Please choose again"<<endl;
                    break;
            }
        }
        return 1;
    }

    int transcript(){
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        char* sql = new char[500];
        sprintf(sql, "select UoSCode, Grade, UoSName from transcript inner join unitofstudy using(UoSCode) where studid = '%s';", userId.c_str());
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
        cout << endl;
        cout<<"Course Id:  "<<"Grade:  "<<"Course Name:  "<<endl;
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row(res_set);
            if (row != NULL) {
                cout << left << setw(12) << row[0];
                if (row[1]) {
                    cout << left << setw(8) << row[1] << row[2] << endl;
                } else {
                    cout << left << setw(8) << "null" << row[2] << endl;
                }
            }
        }
        cout << endl;
        //mysql_close(conn);
        while(1) {
            cout << "[Transcript] Please choose one option: "<< endl;
            cout << "1.Check details of a course" << endl;
            cout << "2.Back to student menu" << endl;
            cout << ">>";
            int chooseTrans;
            cin >> chooseTrans;
            if (cin.fail()) {
                cout << "invalid input " << endl;
                cin.clear();
                while(cin.get() != '\n');
            }
            if(chooseTrans == 2){
                //mysql_free_result(res_set);
                //mysql_close(conn);
                return 1;
            } else if (chooseTrans == 1){
                cout << "please enter the course number: ";
                cin >> courseNum;
                sprintf(sql, "select transcript.uoscode, unitofstudy.uosname, "
                        "transcript.year, transcript.semester, uosoffering.enrollment, "
                        "uosoffering.maxenrollment, faculty.name, transcript.grade "
                        "from transcript inner join unitofstudy using(UoSCode) "
                        "inner join uosoffering on transcript.uoscode = uosoffering.uoscode "
                        "and transcript.year = uosoffering.year inner join faculty "
                        "on faculty.id = uosoffering.instructorid "
                        "where transcript.studid = '%s' and transcript.uoscode = \"%s\";", userId.c_str(), courseNum);
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
                cout << endl;
                cout << "Couse Number: " << row[0] << endl;
                cout << "Course Title: " << row[1] << endl;
                cout << "Year : " << row[2] << " quarter :" << row[3] << endl;
                cout << "Number of enrolled students: " << row[4] << endl;
                cout << "Maximun enrolled students: " << row[5] << endl;
                cout << "Lectuer name: " << row[6] << endl;
                if(row[7]){
                    cout << "Grade: " << row[7] << endl;
                } else {
                    cout << "Grade: null" << endl;
                }
                cout << endl;
            } else {
                cout << "please enter the right number" << endl;
            }
        }
    }

    int Enroll() {
        cout<<"Do u want to enroll in some courses? Y/N"<<endl;
        string answer;
        cin>>answer;
        if (answer == "N" || answer == "n") {
            return 1;
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
        int followingyear = quarter == 1? year+1:year;
        char* combine = new char[800];
        sprintf(combine,"SELECT UoSCode, Credits,Semester, UoSName FROM unitofstudy join whenoffered using(UoSCode) join uosoffering using(UoSCode, Semester) WHERE ((Semester = 'Q%d'AND Year = '%d')OR( Semester = 'Q%d' AND Year = '%d')) and not exists (SELECT * FROM transcript WHERE StudId = '%s' AND UoSCode = unitofstudy.UoSCode and Semester = uosoffering.Semester and Year = uosoffering.Year) order by Semester;",quarter,year,followingquarter,followingyear,userId.c_str());
        conn = mysql_init ( NULL );
        mysql_real_connect(conn, "localhost", "root", "103103", "project3-nudb", 0, NULL, 0);
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int a = mysql_query(conn,combine);//return int; 0 is true, 1 is false

        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        map<string,int> currentCourse;
        // Display results
        cout<<"CourseID:  "<<"Credits:  "<<"Semester:  "<<"CourseName: "<<endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row( res_set );
            if( row != NULL )
            {
                if (currentCourse.find(row[0]) == currentCourse.end()) {
                    if (string(row[2]) == "Q1") {
                        currentCourse[row[0]]=1;
                    } else if (string(row[2]) == "Q2") {
                        currentCourse[row[0]]=2;
                    } else if (string(row[2]) == "Q3") {
                        currentCourse[row[0]]=3;
                    } else {
                        currentCourse[row[0]]=4;
                    }
                } else {
                    currentCourse[row[0]]=5;
                }
                cout << row[0] <<"   "<< row[1] <<"         "<< row[2]<<"         "<< row[3]<< endl;
                if (currentCourse.find(reverseCase(row[0])) == currentCourse.end()) {
                    if (string(row[2]) == "Q1") {
                        currentCourse[reverseCase(row[0])]=1;
                    } else if (string(row[2]) == "Q2") {
                        currentCourse[reverseCase(row[0])]=2;
                    } else if (string(row[2]) == "Q3") {
                        currentCourse[reverseCase(row[0])]=3;
                    } else {
                        currentCourse[reverseCase(row[0])]=4;
                    }
                } else {
                    currentCourse[reverseCase(row[0])]=5;
                }
            }
        }
        mysql_free_result( res_set );
        // free resources
        string CourseID;
        cout<<"Which course? Please show us the CourseID"<<endl<<">>";
        cin>>CourseID;
        upCase(CourseID);
        if (currentCourse.find(CourseID) == currentCourse.end()) {
            cout<<"could not find it"<<endl;
            mysql_close( conn );
            return 1;
        }
        string Semester = "9";
        semesterTransfer(Semester);
        stringstream ss;
        string Q1;
        string Q2;
        ss<<quarter;
        ss>>Q1;
        ss.clear();
        ss<<followingquarter;
        ss>>Q2;
        ss.clear();
        if (currentCourse[CourseID] == 5) {
            while (Semester != Q1 && Semester != Q2){
                cout<<"Please choose the semester"<<endl<<">>";
                cin>>Semester;
                semesterTransfer(Semester);
                if (Semester != Q1 && Semester != Q2) {
                    cout << "this Semester does not exist" << endl;
                }
            }
        } else {
            ss<<currentCourse[CourseID];
            ss>>Semester;
            //cout<<currentCourse[CourseID];
            //cout<<Semester;
        }
        semesterTransfer(Semester);
        int currentYear = Semester == Q1? year:followingyear;
        sprintf(combine,"SELECT UoSCode_SELECTED,Semester,Year, Enrollment, MaxEnrollment,UoSCode,grade FROM ("
                "SELECT UoSCode AS UoSCode_SELECTED,Semester,Year, Enrollment, MaxEnrollment,PrereqUoSCode AS UoSCode "
                "FROM uosoffering left join requires using (UoScode)"
                "WHERE uosoffering.UoSCode = '%s' and uosoffering.Semester = 'Q%s' and uosoffering.Year = %d) AS REQUIRED left join (Select UoSCode,Grade from transcript where studID = '%s') as goal using (UoSCode)"
                "WHERE REQUIRED.UoSCode is null or (REQUIRED.UoSCode is not null AND Grade is not null) AND Enrollment < MaxEnrollment;",CourseID.c_str(),Semester.c_str(),currentYear,userId.c_str());
        a = mysql_query(conn,combine);//return int; 0 is true, 1 is false
        res_set = mysql_store_result(conn);
        numrows = (int)mysql_num_rows(res_set);
        mysql_free_result( res_set );
        //cout<<numrows;
        if (numrows == 0) {
            cout<<"You do not meet the requirement for this class or the class is full"<<endl;
            sprintf(combine,"SELECT unitofstudy.UoSName, requires.PrereqUoSCode FROM requires join unitofstudy on requires.PrereqUoSCode = unitofstudy.UoSCode where requires.UoSCode = '%s';",CourseID.c_str());
            a = mysql_query(conn,combine);//return int; 0 is true, 1 is false
            res_set = mysql_store_result(conn);
            numrows = (int)mysql_num_rows(res_set);
            row = mysql_fetch_row( res_set );
            cout<<"before you take the course '"<<CourseID<< "' you should take the course '"<<row[0]<< "'first,"<<endl<<"whose courseId is '"<<row[1]<<"' ."<<endl;
            mysql_free_result( res_set );
            mysql_close( conn );
            return 1;
        }
        sprintf(combine,"CALL enroll_class('%s', 'Q%s', %d, '%s');",CourseID.c_str(),Semester.c_str(),currentYear,userId.c_str());
        a = mysql_query(conn,combine);//return int; 0 is true, 1 is false
        res_set = mysql_store_result(conn);
        numrows = (int)mysql_num_rows(res_set);
        cout<< "below is the course you have enrolled:"<<endl;
        cout<<"CourseID:  "<<"Semester:  "<<"Year:   "<<"CourseName: "<<endl;
        for (int i = 0; i < numrows; i++){
            row = mysql_fetch_row( res_set );
            if( row != NULL ){
                cout << row[0] <<"   "<< row[1] <<"         "<< row[2]<<"    "<< row[3]<< endl;
            }
        }
        // free resources
        mysql_free_result( res_set );
        mysql_close( conn );
        return 1;
    }

    int withdraw() {
        //print the courses the student has enrolled in with null grade
        char* sql = new char[1024];
        sprintf(sql, "select unitofstudy.uosname, transcript.uoscode, semester, year from transcript inner join unitofstudy using(uoscode) where grade is null and studid = '%s';", userId.c_str());
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
        cout << endl;
        int numrows = (int)mysql_num_rows(res_set);
        cout<<"Course Id: " <<"  Semester: "<<"  Year: " <<"  Course Name: " <<endl;
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row(res_set);
            if (row != NULL) {
                cout << left << setw(13) << row[1];
                cout << left << setw(12) << row[2];
                cout << left << setw(8) << row[3];
                cout << left << setw(5) << row[0] << endl;
            }
        }
        cout << endl;
        mysql_free_result(res_set);
        cout << "[Withdraw] Please choose one option: "<< endl;
        cout << "1.Withdraw an enrolled class" << endl;
        cout << "2.Back to student menu" << endl;
        cout << ">>";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cout << "invalid input " << endl;
            cin.clear();
            while(cin.get() != '\n');
        }
        if(choice == 2){
            return 1;
        } else if (choice == 1){
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
            int followingyear = quarter == 1? year+1:year;
            cout << "please enter the course number:" << endl;
            cout << ">>";
            cin >> courseNum;
            cout << "please enter semester of this course, e.g Q1:" << endl;
            cout << ">>";
            cin >> semester;
            int semNum = checkStr(semester);
            sprintf(sql, "select * from transcript where studid = '%s' and uoscode = '%s' and semester = '%s';", userId.c_str(), courseNum.c_str(), semester.c_str());
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
                cout << " year: " << year << " quarter :"<< quarter << endl;
                sprintf(sql, "CALL withdraw_class('%s', '%s', %d, '%s')", courseNum.c_str(), semester.c_str(), year, userId.c_str());
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
                sprintf(sql, "CALL withdraw_class('%s', '%s', %d, '%s')", courseNum.c_str(), semester.c_str(), followingyear, userId.c_str());
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
        sprintf(sql, "select * from student where id = '%s'", userId.c_str());
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
        cout << endl;
        cout << left << setw(6) << "Id :" << left << setw(15) << "Name :";
        cout << left << setw(13) << "Password :" << left << setw(15)<< "Address :" << endl;
        cout << left << setw(6) << row[0] << left << setw(15) << row[1];
        cout << left << setw(13) <<row[2] << left << setw(15) << row[3] << endl;
        cout << endl;
        cout << "[Personal Details] please choose one option:"<<endl;
        cout << "1.Update personal detail" << endl;
        cout << "2.Back to student menu" << endl;
        cout << ">>";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cout << "invalid input " << endl;
            cin.clear();
            while(cin.get() != '\n');
        }
        if(choice == 2){
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
                sprintf(sql, "update student set password = '%s' where id = '%s';", password, userId.c_str());
                mysql_query(conn, sql);
                cout << "password has been changed"<< endl;
            } else if (update_choice == 2){
                char* address = new char[100];
                cout << "please enter new address:" << endl;
                cout << ">>";
                cin >> address;
                sprintf(sql, "update student set address = '%s' where id = '%s';", address, userId.c_str());
                mysql_query(conn, sql);
                cout << "address has been changed" << endl;
            } else {
                cout << "you entered an invalid number, jumps to sutdent menu now" << endl;
                return 1;
            }
        }
        return 1;
    }

private:
    string userId;
    char password[10];
    char courseNum[10];

    int getQuarter(int day,int month) {
        if (month>=1 && month <=3) return 2;
        else if (month>=4 && month <=6) return 3;
        else if (month>=7 && month <=8) return 4;
        else return 1;
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

    string upCase(string& strings){
        int length = strings.length();
        for (int i=0; i<length; i++) {
            if(strings[i] >= 'a' && strings[i] <= 'z') {
                strings[i] = strings[i] - 32;
            }
        }
        return strings;
    }

    void semesterTransfer(string& s) {
        if (s == "Q1" || s == "q1" || s == "1") {
            s = "1";
        } else if (s == "Q2" || s == "q2" || s == "2") {
            s = "2";
        } else if (s == "Q3" || s == "q3" || s == "3") {
            s = "3";
        } else if (s == "Q4" || s == "q4" || s == "4") {
            s = "4";
        } else if (s == "5") {
            s = "5";
        }
    }

    int checkStr(string input){
        if (input.length() != 2){
            return -1;
        } else {
            return atoi(input.substr(1,1).c_str());
        }
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
}
