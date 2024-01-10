#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<regex>

#include "sha1.hpp"

using namespace std;


struct Time
{
	int hours, minutes;
};

struct Course 
{
	int courseID, courseCapacity;
	string courseName, instructorName;
	Time timeSchedule;
	Course* Next;
	Course* Previous;
};

struct UniversityCourseList 
{
	Course* head, * tail;
};

struct EnrolledCourse
{
	Course* courseReference;
	EnrolledCourse* next;
	EnrolledCourse* previous;
};

struct EnrolledCourseList
{
	EnrolledCourse* head, * tail;
};

struct student
{
	int studentID=0;
	string firstName, password, lastName, emailAddress, att;
	student* next;
	EnrolledCourseList enrolledCourses;
};

struct EnrolledStudent
{
	student* studentReference;
	EnrolledStudent* next;
};

bool Password_is_valid(string);
bool Id_is_found (int);
bool Email_is_valid(string);
bool course_is_full(UniversityCourseList, int);
bool already_enrolled(int, int);
bool Available_places(int);
bool day_is_valid(int, int);

string Hashed(string);
string File_name(int);
string course_name_by_courseID(int, UniversityCourseList);
string Attendence_file_name(string, int, int);

student Info_by_ID(int);

void login();
void signup();
void login_as_student(int);
void login_as_admin(int);
void add_courses();
void Enroll(int);
void view_courses(int);
void Drop(int);
void Display_Course(Course*);
void Display_Student(student*);
void swap_courses(int);

void Enroll_in_course(int,int,UniversityCourseList);
void Drop_course(int, int,UniversityCourseList,int);
void Fill_From_File(UniversityCourseList&);
void Fill_From_File2(student*&);
void Edit_Capacity_in_file(UniversityCourseList, int,int);
void Fill_From_coursefile(student*&, string);

int assign_course_code();
int assignID();

void add_grades(int);
void check_courses(int);
void take_attendence(int);
void Check_attendance(int);
void Check_grades(int);

int main(void)
{
	int restart = 1;
	while (restart == 1)
	{
		cout << "SIS UA : \n choose an option : \n 1- login \n 2- sign up \n ---------------------------------------------------------------- \n \nOption : ";
		char c;
		do
		{
			cin >> c;
			if (c != '1' && c != '2')
				cout << endl << "enter a valid value" << endl << "Option : ";
		} while (c != '1' && c != '2');

		cin.ignore();

		switch (c)
		{
		case '2':
		{signup();

		cout << "continue to login [y/n]: ";
		cin >> c;
		c = tolower(c);
		if (c == 'y')
			c = '1';
		else
			break;
		}
		case '1':
			login();
			break;

		default:
			exit(1);//should never arrive here
			break;
		}
		cout << "Restart Program (1-yes/0-No): ";
		cin >> restart;
		for (int i = 0; i < 120; i++)
			cout << endl;
	}

	return 0;
}

string Hashed(string s)
{
	SHA1 checksum;
	checksum.update(s);
	const string hash = checksum.final();
	return hash;
}

int assignID()
{
	ifstream f("ID.txt");
	int current_ID = 0;
	f >> current_ID;
	f.close();
	fstream F("ID.txt" , ios::in | ios::out);
	if (current_ID>1)	
{
	F << (current_ID + 1);
	return current_ID;
}
else
F<<202301001;
return 202301001;
}

bool Password_is_valid(string password)
{
	int lower = 0, upper = 0, symbol = 0, number = 0, Error = 0;

	if (password.length() < 8)
	{
		cout << endl << "ERROR 101 : Password should be at least 8 characters long" << endl;//101
		Error++;
		cout << endl << "--------------------------------------------------------" << endl;
	}

	else
	{
		for (int i = 0; i < password.length(); i++)
		{
			if (password[i] >= 65 && password[i] <= 90)
				upper++;
			else if (password[i] >= 97 && password[i] <= 122)
				lower++;
			else if (password[i] >= 48 && password[i] <= 57)
				number++;
			else if ((password[i] >= 33 && password[i] <= 47) || (password[i] >= 58 && password[i] <= 64) || (password[i] >= 91 && password[i] <= 96) || (password[i] >= 123 && password[i] <= 126))
				symbol++;
		}
		if (upper == 0)
		{
			cout << endl << "ERROR 102 :Password must Include uppercase letters" << endl;//101
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}
		if (lower == 0)
		{
			cout << endl << "ERROR 103 :Password must Include lowercase letters" << endl;//103
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}
		if (number == 0)
		{
			cout << endl << "ERROR 104 :Password must Include Numbers" << endl;//104
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}
		if (symbol == 0)
		{
			cout << endl << "ERROR 105 :Password must Include Symbols and special characters" << endl;//105
			Error++;
			cout << endl << "--------------------------------------------------------" << endl;
		}

	}

	if (Error == 0)
		return true;
return false;
}

bool Id_is_found(int id)
{
	ifstream f("ID.txt");
	int current_ID = 0;
	f >> current_ID;
	f.close();
	f.ignore();
	if (id >= current_ID || id < 202301001)
	{
		cout << "\nId not found \n";
		return false;
	}
	return true;
}

bool Email_is_valid(string Email_address)
{
	const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return regex_match(Email_address, pattern);

}


void signup()
{
	fstream f("student.csv", ios::out | ios::app);
	student new_student;
	string pass;
	cout << "Firstname : ";
	getline(cin, new_student.firstName);


	for (int i = 0; i < new_student.firstName.length(); i++)
		new_student.firstName[i] = tolower(new_student.firstName[i]);

	cout << "lastname : ";
	getline(cin, new_student.lastName);
	do
	{
		cout << "email_address : ";
		getline(cin, new_student.emailAddress);
		if (!Email_is_valid(new_student.emailAddress))
			cout << endl << "email isn't valid !!!" << endl;
	} while (!Email_is_valid(new_student.emailAddress));

	do
	{
		cout << "Password : ";
			getline(cin, new_student.password);
	} while (!Password_is_valid(new_student.password));
	string password1 = Hashed(new_student.password);
	new_student.studentID = assignID();

		cout << "Acount type :\n1-Admin\n2-student\nOption: ";
		int choice = 0;
		cin >> choice;
		char att;
		if (choice==1)
		{
			cout << "Enter Password Any wrong pass will sign you as student : ";
			cin.ignore();
			cin >> pass;
			if (pass == "Admin")
			{
				att = 'a';
				cout << endl << "you're an admin" << endl;
			}
			else
				att = 's';
		}
		else
			att = 's';

		f << new_student.studentID << ',' << password1 << ',' << new_student.firstName << ',' << new_student.lastName << ',' << new_student.emailAddress <<','<<att<< '\n';
		f.close();
		cout << "your ID is : " << new_student.studentID << endl;
}


void login()
{
	student logingin;
	int ID = 0;
	do
	{
		cout << "ID : ";
		cin >> logingin.studentID;
	} while (!Id_is_found(logingin.studentID));
	cin.ignore();
	ID = logingin.studentID;
	
	logingin = Info_by_ID(logingin.studentID);

	cout << "welcome : " << logingin.firstName << '\t' << logingin.lastName << '\n';

	string password;
	int j = 0, k = 3;
	do
	{
		
		cout << "Enter your password : ";
		getline(cin, password);
		password = Hashed(password);


		for (int i = 0; i < logingin.password.length(); i++)
		{
			if (logingin.password[i] ==password[i])
				j++;
			else
			{
				j = 0;
				break;
			}
		}

		if (j == logingin.password.length())
		{
			cout << endl << "sign in successful" << "\n Welcome back  " << logingin.firstName << endl;
			break;
			
		}
		else
			cout << endl << "Wrong Pass \n you have " << --k << " More tries \n" << endl;
		
	} while (k != 0);

	cout << "\n ID is : " << ID;


	if (k!=0)
	{
		if (logingin.att == "a")
			login_as_admin(ID);
		
		else login_as_student(ID);
	}
} 

student Info_by_ID(int ID)
{
	student logingin;
	int loginId;
	ifstream f("student.csv");
	while (f)
	{
		f >> loginId;
		f.ignore();	
			getline(f, logingin.password, ',');
			getline(f, logingin.firstName, ',');
			getline(f, logingin.lastName, ',');
			getline(f, logingin.emailAddress, ',');
			getline(f, logingin.att, '\n');
			if (loginId == ID)
				break;
	}
	return logingin;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void login_as_student(int id)
{
	student s;
	s = Info_by_ID(id);
	char restart = 'y';
	while (restart == 'y')
	{
		cout << "\n Welcome "<<s.firstName<<" !!!!\n\n\n-Would you like to :\n\n 1-Enroll in new courses \n 2-check your courses \n 3-Drop courses\n 4-swap \n 5-check grades \n 6-check attendance\n option : ";
		char c;
		do
		{
			cin >> c;
			if (c != '1' && c != '4' && c != '3' && c != '2' && c != '5' && c != '6')
				cout << "\nenter a valid value\n";
		} while (c != '1' && c != '4' && c != '3' && c != '2' && c != '5' && c != '6');

		switch (c)
		{
		case '1':
		{
			Enroll(id);
			break;
		}
		case '2':
		{
			view_courses(id);
			break;
		}
		case '3':
		{
			Drop(id);
			break;
		}
		case '4':
		{
			swap_courses(id);
			break;
		}
		case '5':
		{
			Check_grades(id);
			break;
		}
		case '6':
		{
			Check_attendance(id);
			break;
		}
		default:
			exit(1);
			break;
		}
		
		cout << "continue? : [y/n]";
		cin >> restart;
		restart = tolower(restart);
		if (restart == 'y')
			cin.ignore(INT_MAX,'\n');
	}
}

void login_as_admin(int id)
{

	char restart = 'y';
	while (restart == 'y')
	{
		cout << "\n Welcome admin !!!!\n\n\n-Would you like to :\n\n 1-add grades \n 2-check your courses \n 3-take attendence \n option : ";
		int c=0;
		do
		{
			cin >> c;
			if (c<1 || c>3)
				cout << "\nenter a valid value\n";
		} while (c < 1 || c>3);

		switch (c)
		{
		case 1:
		{
			add_grades(id);
			break;
		}
		case 2:
		{
			check_courses(id);
			break;
		}
		case 3:
		{
			take_attendence(id);
			break;
		}
		
		default:
			exit(1);
			break;
		}

		cout << "continue? : [y/n]";
		cin >> restart;
		restart = tolower(restart);
		if (restart == 'y')
			cin.ignore(INT_MAX, '\n');
	}
}

void add_courses()
{
	Course course;
	fstream F("Courses.csv", ios::in | ios::out | ios::app);
	cout << "\n how many cours : ";
	int x = 0;
	do { cin >> x; if (x <= 0) cout << "\n enter a valid option \n"; } while (x <= 0);
	for (int i = 0; i < x; i++)
	{
		course.courseID = assign_course_code();
		cout << "course name ; ";
		getline(cin, course.courseName);
		cout << "course Instructor : ";
		getline(cin, course.instructorName);
		cout << "Starting Time: \nHour (24 hrs format) : ";
		do
		{
			cin >> course.timeSchedule.hours;
			cout << "minutes : ";
			cin >> course.timeSchedule.minutes;
			if (course.timeSchedule.hours > 20 || course.timeSchedule.hours < 8 || course.timeSchedule.minutes < 0 || course.timeSchedule.minutes>59)
				cout << "\nenter a valid value \n";
		} while (course.timeSchedule.hours > 20 || course.timeSchedule.hours < 8 || course.timeSchedule.minutes < 0 || course.timeSchedule.minutes>59);
		cout << "Capacity : ";
		cin >> course.courseCapacity;
		cin.ignore();
		F << course.courseID << ',' << course.courseName << ',' << course.instructorName << ',' << course.timeSchedule.hours << ',' << course.timeSchedule.minutes <<','<<course.courseCapacity << '\n';
	}
	F.close();
}
string File_name(int id)
{
	string File;
	File = to_string(id);
	File += ".csv";
	return File;
}
void Enroll(int Id)
{
	cout << "\nEnroll : \n";
	student s1;
	s1 = Info_by_ID(Id);
	
	fstream enroll_file(File_name(Id), ios::out | ios::app | ios::in);
	
	ifstream C("Courses.csv");
	cout << "avilable courses are : \n" << endl;
	UniversityCourseList list;
	Fill_From_File(list);

	Display_Course(list.head);

	cout << endl << "--------------------------------------------" << endl;

	cout << endl << "Enter the course ID you would like to enroll in  : ";
	int op=0;
	do
	{
		cin >> op;
		if (op > 8 || op < 0)
			cout << "\tenter a valid value : \n";
		if (course_is_full(list, op))
			cout<<"course is full no place available : ";
	} while (op > 8 || op < 0 || course_is_full(list, op));

	Enroll_in_course(op, Id, list);

	cout << "\n Succesfully enrolled \n";
	
}

void view_courses(int id)
{
	int counter=0;
	cout << "view_courses : \n" << endl << "------------------------------------" << endl;
	UniversityCourseList list;
	Fill_From_File(list);
	Display_Course(list.head);
	cout << endl<<"---------------------------------------------"<<endl;
	UniversityCourseList cur;
	cur = list;
	int num=0,i=0,n=1,num2=0;
	while (cur.head != NULL)
	{
		cout << endl << n++;
		cout << "\nPeople who are enrolled in " << cur.head->courseName << " are : \n ";
		string coursenamefile = cur.head->courseName;
		//cout << coursenamefile;
		coursenamefile += ".csv";
		ifstream f(coursenamefile);
		string s1, s2;
		if (cur.head->courseCapacity == 28)
			cout << "NO ONE !!! \n";
		else
		{
			while (f)
			{
				
				f >> num;
				f.ignore();
				getline(f, s1, ',');
				getline(f, s2, '\n');
				student s = Info_by_ID(num);
				i++;
				
				cout << endl << i << "-" << s.firstName << "\t" << s.lastName << endl;


				if (id==num)
				{
					counter += 1;
				}

				if (i == (28 - cur.head->courseCapacity))
					break;
			}

		}
		cur.head = cur.head->Next;
		i = 0;
		cout << endl << "------------------------------------------------------" << endl;
		if (n == 9) 
			break;
	}
	
	ifstream F (File_name(id));
	Course* head, * curr=NULL;
	head = curr;
	int j = 0;
	curr = new Course;
	
	while (F)
	{
		F >> curr->courseID;
		F.ignore();
		getline(F, curr->courseName, '\n');
		j++;
		if (j == 1)
			head = curr;
		if (counter == j)
		{
			curr->Next = NULL;
			break;
		}
		else
		{
			curr->Next = new Course;
			curr = curr->Next;
		}
	}
	
	curr = head;
	j = 0;
	cout << "\n your enrolled in "<<counter<<" courses which is / are : \n";
	if (counter==0)
		cout << "NONE \n\n\n";
	else
	for (int i = 1; i <= 8; i++)
	{
		curr = head;
		while (curr != NULL)
		{
			if (curr->courseID == i)
				cout << endl << curr->courseID <<"\t" << curr->courseName <<endl;
			j++;
			if (j == counter)
				break;
			curr = curr->Next;
		}
	}
	curr = head;
	cout << endl << endl << "----------------------------------------------------------" << endl << endl;
}

void Drop(int id)
{
	cout << "\nDrop Course : ";
	student s1 = Info_by_ID(id);
	int counter = 0;

	UniversityCourseList list;
	Fill_From_File(list);

	cout << endl << "---------------------------------------------" << endl;
	UniversityCourseList cur;
	cur = list;
	int num = 0, i = 1, n = 1;
	while (cur.head != NULL)
	{
		n++;
		string coursenamefile = cur.head->courseName;
		coursenamefile += ".csv";
		ifstream f(coursenamefile);
		if (cur.head->courseCapacity != 28)
		{
			while (f)
			{
				f >> num;
				f.ignore();
				i++;

				if (num == id)
					counter++;

				if (i = (28 - cur.head->courseCapacity))
					break;
			}

		}
		cur.head = cur.head->Next;
		i = 1;
		if (n == 9)
			break;
	}

	ifstream F(File_name(id));
	Course* head, * curr = NULL;
	head = NULL;
	int j = 0;
	curr = new Course;

	while (F)
	{
		F >> curr->courseID;
		F.ignore();
		getline(F, curr->courseName, '\n');
		j++;
		if (j == 1)
			head = curr;
		if (counter == j)
		{
			curr->Next = NULL;
			break;
		}
		else
		{
			curr->Next = new Course;
			curr = curr->Next;
		}
	}

	curr = head;
	j = 0;
	cout << "\n your enrolled in " << counter << " courses which  are : \n\n\n";
	for (int i = 1; i <= 8; i++)
	{
		while (curr != NULL)
		{
			if (curr->courseID == i)
				cout << endl << curr->courseID << "\t" << curr->courseName << endl;
			j++;
			if (j == counter)
				break;
			curr = curr->Next;
		}
		curr = head;
	}

	int choice = -1;

	do
	{
		cout << "which course would you like to drop : ";
		cin >> choice;
		if (!already_enrolled(choice, id))
			cout << "\n\nYou are not enrolled in this course\n\n";
		if (choice > 8 || choice < 1)
			cout << "\n\n you entered an invalid option \n\n";
	} while (!already_enrolled(choice, id) || choice > 8 || choice < 1);

	Drop_course(choice, id,list,counter);
}
void swap_courses(int id)
{
	cout << "\n\nIn order to swap course you should drop a course then enroll into another : \n\n";
	Drop(id);
	Enroll(id);
	cout << "\n\nswap successfull\n\n";
}

int assign_course_code()
{
	
		ifstream f("Course_code.txt");
		int current_code = 0;
		f >> current_code;
		f.close();
		fstream F("Course_code.txt", ios::in | ios::out);
		F << (current_code + 1);
		return current_code;
}


void Fill_From_File(UniversityCourseList& l)
{
	ifstream f("Courses.csv");
	Course* tmp=new Course;
	l.head = tmp;
	l.tail =tmp;

	tmp->courseCapacity = 0;
	tmp->courseID = 0;
	tmp->courseName = "0";
	tmp->instructorName = "0";
	tmp->timeSchedule.hours = 0;
	tmp->timeSchedule.minutes = 0;
	tmp->Next = NULL;
	tmp->Previous = NULL;
	int a = 0;
	while (f)
	{

		f >> a;

		tmp->courseID = a;
		f.ignore();
		getline(f, tmp->courseName, ',');
		getline(f, tmp->instructorName, ',');
		f >> tmp->timeSchedule.hours;
		f.ignore();
		f >> tmp->timeSchedule.minutes;
		f.ignore();
		f >> tmp->courseCapacity;
		f.ignore();
		tmp->Next = new Course;
		tmp->Next->Previous = tmp;
		tmp = tmp->Next;
		
		if (a == 8)
		{
			tmp->Next = NULL;
			l.tail = tmp;
			break;
		}
	}
}

void Display_Course(Course* c)
{
	while (c != NULL)
	{
		cout <<"course ID : " <<c->courseID << "\ncourse name : " << c->courseName << "\nInstructor : " << c->instructorName << "\n TIME\t from : " << c->timeSchedule.hours << ':' << c->timeSchedule.minutes << "\nAvailable capacity : " << c->courseCapacity << " free of 28 \n"<<endl;
		if (c->courseID == 8)
			break;
		else
			c = c->Next;
	}
}

void Fill_From_File2(student*& head)
{
	ifstream f("student.csv");
	ifstream F("ID.txt");
	int last_ID=0;
	F >> last_ID;
	F.close();
	student* s;
	
	cout << endl;
	student logingin;
	int loginId;
	s = new student;
	head = s;
	while (f)
	{
		
		f >> loginId;
		f.ignore();
		getline(f, s->password, ',');
		getline(f, s->firstName, ',');
		getline(f, s->lastName, ',');
		getline(f, s->emailAddress, ',');
		getline(f, s->att, '\n');
		
		s->studentID = loginId;
		if (loginId == (last_ID - 1))
		{
			s->next = NULL;
			break;
		}
		else
		{
			s->next = new student;
			s = s->next;
		}
	}
	//Display_Student(head);
}

void Display_Student(student*s)
{
	while (s != NULL)
	{
		cout <<"ID:"<< s->studentID << "\n Name " << s->firstName<<"\t" << s->lastName << "\nemail : " << s->emailAddress << '\n';
		s = s->next;
	}
}

bool course_is_full(UniversityCourseList l , int a)
{
	int cap=0;
	while (l.head != NULL)
	{
		cap = l.head->courseCapacity;
		if (cap == 28)
			return false;
		l.head = l.head->Next;
		if (l.head->courseID == a)
			break;
	}
	if (cap == 0)
		return true;
	return false;
}

void Enroll_in_course(int code, int id,UniversityCourseList l)
{
	UniversityCourseList cur;
	cur = l;
	
	string coursename;
	fstream student_file(File_name(id), ios::in | ios::out | ios::app);

	if (cur.head!=NULL)
	{
		while (cur.head != NULL)
		{
			if (cur.head->courseID == code)
				break;

			cur.head = cur.head->Next;
		}
	}

	coursename = cur.head->courseName;
	
	if (already_enrolled(code, id))
	{
		cout << "already enrolled in this course : ";
		return;
	}
	else if (!Available_places(l.head->courseCapacity))
	{
		cout << "no places availbale" << endl;
		return;
	}
	int a = cur.head->courseCapacity;
	a -= 1;
	coursename += ".csv";
	fstream course_file(coursename, ios::in | ios::out | ios::app);
	student enrolling = Info_by_ID(id);
	course_file << id << ','<<enrolling.firstName<<','<<enrolling.lastName<<'\n';
	course_file.close();
	student_file << code << ',' << cur.head->courseName << '\n';
	student_file.close();
	
	
	Edit_Capacity_in_file(l, a,code);

}

bool already_enrolled(int code,int id)
{
	ifstream f(File_name(id));
	int s = 0;
	string str;
	while (f)
	{
		f >> s;
		f.ignore();
		getline(f, str, '\n');
		if (s == code)
			return true;
	}
	return false;
}

bool Available_places(int a)
{
	if (a == 0)
		return false;
	return true;
}

void Edit_Capacity_in_file(UniversityCourseList l ,int a,int op)
{
	fstream f("Courses.csv", ios::in | ios::out);
	while (l.head != NULL)
	{
		f << l.head->courseID << ',' << l.head->courseName << ',' << l.head->instructorName << ',' << l.head->timeSchedule.hours << ',' << l.head->timeSchedule.minutes << ',' ;
		if (l.head->courseID == op)
			f << a << '\n';
		else
			f << l.head->courseCapacity << '\n';

		if (l.head->courseID == 8)
		{
			f.close();
			break;
		}
		l.head = l.head->Next;
	}
}

void Drop_course(int choice, int id,UniversityCourseList list,int course_number)
{
	fstream student_file(File_name(id), ios::in | ios::out);
	string coursename = course_name_by_courseID(choice, list);
	coursename += ".csv";
	fstream course_file(coursename, ios::in | ios::out );
	int a = 0;
	UniversityCourseList c = list;
	
	while (c.head != NULL)
	{
		a = c.head->courseCapacity;
		if (c.head->courseID == choice)
			break;
		c.head = c.head->Next;
	}
	a+=1;
	
	//cout << a<<c.head->courseName;
	Course* head = new Course,* std;
	std=head;
	int j = 0,student_file_lines=0,course_file_lines=0;
	while (student_file)
	{
		j++;
		student_file >> std->courseID;
		student_file.ignore();
		getline(student_file, std->courseName, '\n');
		//cout << std->courseID<<std->courseName << endl;
		if (j == course_number)
		{
			std->Next = NULL;
			break;
		}
		else
		{
			std->Next = new Course;
			std = std->Next;
		}
	}
	student_file_lines = j;
	j = 0;
	
	student* head2 = new student, * crs ;
	crs = head2;

	while (course_file)
	{
		j++;
		course_file >> crs->studentID;
		course_file.ignore();
		getline(course_file, crs->firstName, ',');
		getline(course_file, crs->lastName, '\n');
		cout << crs->studentID << ',' << crs->firstName << ',' << crs->lastName << '\n';
		if (j == 28-(a - 1))
		{
			crs->next = NULL;
			break;
		}
		else
		{
			crs->next = new student;
			crs = crs->next;
		}
	}

	course_file_lines = j;
	crs = head2;
	j = 0;
	cout << course_file_lines;
	course_file.close();
	student_file.close();
	ofstream course_file2(coursename);
	ofstream student_file2(File_name(id));

	while (crs != NULL)
	{
		j++;
	
		if (crs->studentID = id)
		{
			crs = crs->next;
		}
		else
		{
			course_file2 << crs->studentID << ',' << crs->firstName << ',' << crs->lastName << '\n';
			cout << crs->studentID << ',' << crs->firstName << ',' << crs->lastName << '\n';
			crs = crs->next;
		}
		if (course_file_lines - 1 == j)
			break;
	}
	std = head;
	j = 0;
	while (std != NULL)
	{
		j++;

		if (std->courseID == choice)
		{
			std = std->Next;
		}
		else
		{
			student_file2 << std->courseID << ',' << std->courseName << '\n';
			cout << std->courseID << ',' << std->courseName << '\n';
			std = std->Next;
		}
		if (student_file_lines - 1 == j)
			break;
		
	}
	student_file2.close();
	course_file2.close();
	Edit_Capacity_in_file(list, a, choice);
	cout << "\n succesfuly dropped \n";
}
string course_name_by_courseID(int a, UniversityCourseList l)
{
	UniversityCourseList c = l;
	string str="web";
	int num = 0;
	while (c.head != NULL)
	{
		num = c.head->courseID;
		str = c.head->courseName;
		if (num == a)
			return str;
		c.head = c.head->Next;
	}
	return str;
}

void add_grades(int id)
{
	
	student instructor;
	instructor = Info_by_ID(id);
	UniversityCourseList list, l2;
	Fill_From_File(list);
	Course* C = list.head;
	int i = 0;
	string courses;
	while (C != NULL)
	{
		if (C->instructorName == instructor.firstName)
		{
			i++;
			courses = C->courseName;
			break;
		}
		C = C->Next;
	}
	if (i == 0)
	{
		cout << "you don't teach any course \n";
	}
	else
	{
		cout << "\nadd_grades for " << courses << endl;
		string coursename_file = courses + "grades.csv";
		ofstream f(coursename_file, ios::app);
		student* s = NULL;
		Fill_From_coursefile(s, courses);
		student* cur = s;
		int grade = -1;
		while (cur != NULL)
		{
			if (cur->studentID == 0)
				break;
			do {
				cout << "enter " << cur->firstName << ' ' << cur->lastName << "'s grade (/20) : ";
				cin >> grade;
				if (grade < 0 || grade>20)
					cout << "\n enter a valid grade !!!";
			} while (grade < 0 || grade>20);
			f << cur->studentID << ',' << grade * 5 << "\n";
			cur = cur->next;
		}
	}


}
void check_courses(int id)
{
	cout << "\nThe course(s) you teach are/is :  \n";
	student instructor;
	instructor = Info_by_ID(id);
	UniversityCourseList list,l2;
	Fill_From_File(list);
	Course* C = list.head;
	int i = 0;
	while (C != NULL)
	{
		if (C->instructorName==instructor.firstName)
		{
			i++;
			cout << C->courseName << endl;
		}
		C = C->Next;
	}
	if (i == 0)
		cout << "none";
}
void take_attendence(int id)
{
	cout << "\n take attendance \n enter today's date\n month (1 to 12) : ";
	int day=0, month=0;
	do
	{
		do
		{
			cin >> month;
			if (month > 12 || month < 1)
				cout << "\n invalid month \n\n month (1 to 12) :  ";
		} while (month > 12 || month < 1);
		cout << "\nday : ";
		cin >> day;

		if (!day_is_valid(day, month))
			cout << "\nday not valid \n";
	} while (!day_is_valid(day, month));

	student instructor;
	instructor = Info_by_ID(id);
	UniversityCourseList list, l2;
	Fill_From_File(list);
	Course* C = list.head;
	int i = 0;
	string courses;
	while (C != NULL)
	{
		if (C->instructorName == instructor.firstName)
		{
			i++;
			courses= C->courseName;
			break;
		}
		C = C->Next;
	}
	if (i == 0)
	{
		cout << "you don't teach any course \n";
	}
	else
	{
		cout << "\n attandance for " << courses << endl;
		ofstream F(Attendence_file_name(courses, day, month), ios::app);
		student* s=NULL;
		Fill_From_coursefile(s, courses);
		student* cur = s;
		int presence = 2;
		cout << "enter 0 for absent and 1 for present : \n";
		while (cur != NULL)
		{
			if (cur->studentID == 0)
				break;
			cout << "- " << cur->studentID << "\t" << cur->firstName << "\t" << cur->lastName << ": ";
			do
			{
				cin >> presence;
				if (presence != 1 && presence != 0)
					cout << "enter a valid value : ";
			} while (presence != 1 && presence != 0);
			F << cur->studentID << ',' << presence << '\n';
			cur = cur->next;
		}
	}
}

string Attendence_file_name(string course_name, int day, int month)
{
	string file = course_name + '.' + to_string(day) + '.' + to_string(month) + ".attandence.csv";
	return file;
}
bool day_is_valid(int day, int month)
{
	if (day > 31)
		return false;
	if (day < 1)
		return false;
	if (month == 2 && day > 29)
		return false;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return false;

	return true;
}

void Fill_From_coursefile(student*& s1, string course_name)
{
	ifstream f(course_name + ".csv");
	student* s = s1,*head=NULL;
	s = new student;
	head = s;
	student* p = NULL;
	while (f)
	{
		f >> s->studentID;
		f.ignore();
		getline(f, s->firstName, ',');
		getline(f, s->lastName, '\n');
		s->next = new student;
		p = s;
		s = s->next;
	}
	if (s->studentID == 0)
	{
		delete s;
		s = p;
	}
	s->next = NULL;
	f.close();
	s1 = head;
}


void Check_attendance(int id)
{
	view_courses(id);
	int month = 0, day = 0,courseids=0;
	
	cout << "enter course id to check for attendence : \n";
	do
	{
		cout << "options : ";
		cin >> courseids;
		if (!already_enrolled(courseids, id))
			cout << "\n enter a valid value \n: ";
	} while (!already_enrolled(courseids, id));
	
	UniversityCourseList l;
	l.head = NULL;
	l.tail = NULL;
	Fill_From_File(l);
	Course* s = l.head;
	string coursesname;
	while (s != NULL)
	{
		if (s->courseID == courseids)
		{
			coursesname = s->courseName;
			break;
		}
		s = s->Next;
	}
	cout << "enter the date of absence : \n \n month (1 to 12) : ";
	do
	{
		do
		{
			cin >> month;
			if (month > 12 || month < 1)
				cout << "\n invalid month \n\n month (1 to 12) :  ";
		} while (month > 12 || month < 1);
		cout << "\nday : ";
		cin >> day;

		if (!day_is_valid(day, month))
			cout << "\nday not valid \n";
	} while (!day_is_valid(day, month));
	ifstream f (Attendence_file_name(coursesname,day,month));
	if (!f.good())
	{
		cout << endl << "Attendance doesn't exists : \n";
	}
	else
	{
		int ids, cas;
		while (f)
		{
			f >> ids;
			f.ignore();
			f >> cas;
			f.ignore();
			cout << ids << cas;
			if (ids == courseids)
				break;
		}
		if (cas == 1)
			cout << "\nyou were present \n";
		else if (cas == 0)
			cout << "\nyou were absent \n";
		else
			cout << "error!!";
	}
}
void Check_grades(int id)
{
	int courseids = 0;
	view_courses(id);
	cout << "enter course id to check your grade : \n";
	do
	{
		cout << "options : ";
		cin >> courseids;
		if (!already_enrolled(courseids, id))
			cout << "\n enter a valid value \n: ";
	} while (!already_enrolled(courseids, id));


	UniversityCourseList l;
	l.head = NULL;
	l.tail = NULL;
	Fill_From_File(l);
	Course* s = l.head;
	string coursesname;
	while (s != NULL)
	{
		if (s->courseID == courseids)
		{
			coursesname = s->courseName;
			break;
		}
		s = s->Next;
	}
	string coursename_file = coursesname + "grades.csv";
	ifstream f(coursename_file);
	if (!f.good())
	{
		cout << endl << "grades not available yet \n";
	}
	else
	{
		int ids = 0, grade = 0;
		while (f)
		{
			f >> ids;
			f.ignore();
			f >> grade;
			f.ignore();
			cout << ids << grade;
			if (ids == id)
				break;
		}
		cout << "\n your grade is : " << grade * 5 << '%' << endl;
	}
}
