-------------------------------------------BANK MANAGEMENT APP----------------------------------

In this project we create a new note and do some operations like edit and delete the note.


Requirements:
------------

1. Code Blocks
2. XAMPP
3. Dev c++(Install in Program Files (x86))

4. Mysql.zip which will be provided


STEPS:
------

STEP 1: Install all of the above 4 applications and files.

STEP 2: Extract the files in Mysql.zip open mysqlheaders->mysqlheaders->copy all
	Goto C:\Program Files (x86)\Dev-Cpp\MinGW64\include and paste

STEP 3: Goto libmysql.a Copy file and paste in C:\Program Files (x86)\Dev-Cpp\MinGW64\lib

STEP 4: Open Code Blocks and open Project named Notes of type project. Then in the managment bar(present at the left of the window)
	open main.cpp under Notes->Sources->main.cpp.

STEP 5: Before Execution of the file goto projects->build options->linker settings and add libmysql.a 
	from devcpp folder and next goto search directories and add include folder
	(C:\Program Files (x86)\Dev-Cpp\MinGW64\include) and Press OK.
	
	NOTE:- search directories is accessed same as linker settings(projects->build options->search directories).

STEP 6: While running the file if you get an error add (Mysql->libmysql.dll->libmysql.dll) in to the project folder.

STEP 7: Now Build and Run The File.


XAMPP:
------
In XAMPP Create a new user account with details as follows:
username: user
password: pass

      AND 

Create Database named 'bankcs'

CREATE TABLE users (
    account_no INTEGER NOT NULL AUTO_INCREMENT,
    name       VARCHAR (50),
    dob        VARCHAR (50),
    phoneno    CHAR (10),
    address    TEXT,
    district   VARCHAR (50),
    state     VARCHAR (50),
    gender    VARCHAR (50),
    balance    INTEGER,
    email VARCHAR(128),
    password VARCHAR(128),
    PRIMARY KEY CLUSTERED (account_no)
) ENGINE = InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE transfer (
    sno INTEGER NOT NULL AUTO_INCREMENT,
    date       VARCHAR (100),
    account_no INTEGER,
    balance    INTEGER,
    transamt INTEGER,
    to_acc_no INTEGER,
    PRIMARY KEY CLUSTERED (sno)
) ENGINE = InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE deposit (
    sno INTEGER NOT NULL AUTO_INCREMENT,
    date       VARCHAR (100),
    account_no INTEGER,
    balance    INTEGER,
    transamt INTEGER,
    PRIMARY KEY CLUSTERED (sno)
) ENGINE = InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE withdraw (
    sno INTEGER NOT NULL AUTO_INCREMENT,
    date      VARCHAR (100),
    account_no INTEGER,
    balance    INTEGER,
    transamt INTEGER,
    PRIMARY KEY CLUSTERED (sno)
) ENGINE = InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE mpin (
num INTEGER NOT NULL AUTO_INCREMENT,
pin INTEGER,
account_no INTEGER,
PRIMARY KEY CLUSTERED (num)
)ENGINE = InnoDB DEFAULT CHARSET=utf8;


If you cannot access the database through your code in xampp edit previleges where you can see all user accounts
check the box of the user you write in your code.(Goto check previliges to view this).


To Run Project in CodeBlocks

Press ctrl + O and open notes of type project.

NOTE:- Prepare the Backend before executing the project.



1.This project has covered almost all errors user inputs like entering wrong details for login and transaction purposes
2.This project also validates the user date of birth and checks if it is a leap year or not.

Step1: Enter Details to create an account. on creating an account number is generated
Step2: To do various operations login

All the function uses have been mentioned beside function calls
If you forget a pin enter your email address at 9th operation.

NOTE: After Entering details check the database if the input details reflected correctly.
      If 1st number or character missed in database remove cin.ignore() which is present before input of data.