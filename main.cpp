#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <bits/stdc++.h>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <algorithm>


using namespace std;

//Checks if it is leap year or not
int leapyear(int y,int d)
{
    if(((y%400==0) || ((y%100 != 0) && (y%4 == 0))) && (d>0 && d<=29))//To Check Leap Year
       return 1;
    else if(d>0 && d<=28)
        return 2;
    else
        return 0;
}

//Validates Date of birth
int validdob(int d, int m, int y)
{
    if((m>=1 && m<8) && (m%2 == 1) && (d>0 && d<=31) && (m!=2) && (y>=0 && y<=2021))//Condition for days = 31 between months 1 and 8
       return 1;
    else if((m>=1 && m<8) && (m%2 == 0) && (d>0 && d<=30) && (m!=2) && (y>=0 && y<=2021))//Condition for days = 30 between months 1 and 8
        return 1;
    else if((m>=8 && m<=12) && (m%2 == 0) && (d>0 && d<=30) && (y>=0 && y<=2021))//Condition for days = 30 between months 8 and 12
        return 1;
    else if((m>=8 && m<=12) && (m%2 == 1) && (d>0 && d<=31) && (y>=0 && y<=2021))//Condition for days = 31 between months 8 and 12
        return 1;
    else if(m==2)//Condition for February month
    {
        int l = leapyear(y,d);//Validates For a LeapYear
        if((l==1 || l==2) && (y>=0 && y<=2021)) return 1;
        else
        return 0;
    }
    else
    {
        return 0;
    }

}


//Function to validate password
int validpass(string &pass,int n)
{
    int c=0,c1=0,c2=0,c3=0;
    string special = "!#$%&()\"*+,\'-./:;<=>?@[]\\^_`{|}~";// \' represents ' in string and \" represents "
    for(int i=0;i<n;i++)
    {
        size_t at = special.find(pass[i]);//Finds '@' Symbol and if not found returns -1
        if (at != string::npos)//string::npos means -1
            c2++;
        if(islower(pass[i]))
            c++;
        else if(isdigit(pass[i]))
            c1++;
        else if(isupper(pass[i]))
            c3++;
    }

    if(c>0 && c1>0 && c2>0 && c3>0 && n>=8)
        return 1;
    else
    {
        if(c == 0) cout<<"Password must contain at least 1 lowercase character"<<endl;
        if(c1 == 0) cout<<"Password must contain at least 1 digit(0 to 9)"<<endl;
        if(c2 == 0) cout<<"Password must contain at least 1 special character"<<endl;
        if(c3 == 0) cout<<"Password must contain at least 1 uppercase character"<<endl;
        if(n < 8) cout<<"Password must contain at least 8 characters"<<endl;
        return 0;
    }
    return 0;
}


//Account Number Generator
//This Function searches database with newly created name to know the Account number as Account number is declared as an auto increment in back-end.
int account_generator(MYSQL* conn, string email)
{
  stringstream ss;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int qstate;
  ss << "SELECT account_no,email FROM users";
  string query = ss.str();
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if(qstate == 0){
    res = mysql_store_result(conn);//storing the result in res
    while((row = mysql_fetch_row(res))){ // fetching each row in res and printing the output.
        if(row[1] == email)
        {
            char *ch = row[0];
            return atoi(ch);
        }
    }
  }
  return 0;
}


//Checks whether account number is present or not in the database
int account_validator(MYSQL* conn, int acc)
{
    stringstream ss;
    int qstate = 0;
    int flag = 0;
    MYSQL_RES* res;
    MYSQL_ROW row;
    int acc_num;

    ss<<"SELECT account_no FROM users";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    if(qstate == 0)
    {
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)))
        {
         char *ch = row[0];
         acc_num = atoi(ch);
         if(acc_num == acc){flag = 1;}
        }
    }
    else
    {
        cout<<"Query error: "<<mysql_error(conn)<<endl;
    }
    if(flag == 1) return 1;//Account Exists
    else return 0;//Account Does not exists

}



//Function to create an account
void createaccount(MYSQL* conn)
{
    stringstream ss;
    string namedb,address,district;
    string state,dob;
    string email,pass;
    char gender;
    char phonenum[20];
    int qstate = 0,balance;
    int dd,mm,yyyy;//for date of birth(dob)

    cout<<"\nEnter Your Name: ";
    cin.ignore();//used to ignore newline
    getline(cin,namedb);

    int valid_dob = 0;
    while(valid_dob == 0)
    {
    cout<<"\nDate Of Birth "<<endl;
    cout<<"Enter Date of month(1 - 31): ";cin>>dd;
    cout<<"Enter Month: ";cin>>mm;
    cout<<"Enter Year: ";cin>>yyyy;
    valid_dob = validdob(dd,mm,yyyy);

    if(valid_dob == 1)
      {
        stringstream d,m,y;
        string d1,m1,y1;
        d<<dd;d>>d1;
        m<<mm;m>>m1;
        y<<yyyy;y>>y1;
        dob = d1 + "/" + m1 + "/" + y1;
      }
    else
    {
        cout<<"\nINVALID INPUT\n"<<endl;
    }

    }


    int phonelength = 0;
    while(phonelength == 0)
    {
        cout<<"\nEnter Mobile Number: "<<endl;
        cin.ignore();
        cin>>phonenum;
        phonelength = strlen(phonenum);
        if(phonelength != 10)
        {
          cout<<"Invalid Number"<<endl;
          phonelength = 0;
        }

    }

    cout<<"\nEnter Your Address(Type '.' in new line to terminate):  ";
    cin.ignore();
    string text;
    while(text != "."){
        getline(cin, text);
        address += text;
    }//Takes a paragraph as input with '.' as terminating condition in the new line


    cout<<"\nEnter District:  ";
    getline(cin, district);//takes a sentence as input


    cout<<"\nEnter State:  ";
    getline(cin, state);//takes a sentence as input


    int flag = 0;
    while(flag == 0)
    {
      cout<<"\nEnter 'M' for male and 'F' for Female: "<<endl;
      cin>>gender;

      if(((gender == 'M') || (gender == 'm')) || ((gender == 'F') || (gender == 'f')))
        flag = 1;
      else
      {
         cout<<"Invalid Input"<<endl;
         flag = 0;
      }

    }

    cout<<"\nEnter Amount To Deposit: "<<endl;
    cin>>balance;
    if(balance < 0) balance *= -1;//If Balance Entered is negative by the user it changes to positive

    int flag_email = 0;
    cout<<"\nEnter Email Address: "<<endl;
    cin.ignore();
    while(flag_email == 0)
    {
      getline(cin,email);//used to ignore newline
      size_t at = email.find('@');//Finds '@' Symbol and if not found returns -1
      size_t dot = email.find('.', at + 1);//Checks '.' present after '@' symbol
      if ((at == string::npos) || (dot == string::npos))//string::npos means -1
      {
        cout << "Missing @ symbol or email should end with '.com'\n";
        cout<<"Enter email address again: ";
        flag_email = 0;
      }
      else
        flag_email = 1;

    }
    transform(email.begin(), email.end(), email.begin(), ::tolower);//Function to transform the string to lowercase whatever the case input is.
    //transform function is an in-built function that is declared in <algorithm> library

    cout<<"Password must contain at least 8 characters with at least 1 number, 1 uppercase and 1 special character";
    int flag_pass = 0;
    while(flag_pass == 0)
    {
        cout<<"\nEnter Password:  ";
        getline(cin, pass);
        int len_pass = pass.size();//size of password
        flag_pass = validpass(pass,len_pass);
        if(flag_pass == 0)
        {
            cout<<"Invalid Input"<<endl;
        }
    }

    ss << "INSERT INTO users (name, dob, phoneno, address, district, state, gender, balance, email, password) VALUES ('" << namedb << "', '" << dob << "', '" << phonenum <<"', '" << address <<"', '" << district <<"', '"<< state <<"', '" << gender <<"', "<< balance <<", '"<< email <<"', '"<< pass <<"')";

    string query = ss.str();
    const char* q = query.c_str();//c_str() used to convert the string into a character array
    qstate = mysql_query(conn, q);//It executes the query and returns 0 if successful else returns 1
    if(qstate == 0){
        cout<<"Account Created...."<<endl;
        cout<<"Your Account Number is: "<<account_generator(conn,email)<<endl;
    }
    else{
        cout<<"Account Not Created Error Occurred...."<<endl;
        cout<<mysql_error(conn)<<endl;
    }


}




int loginaccount(MYSQL* conn, string emaildb, string passdb)
{
  stringstream ss;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int qstate = 0;
  int flag = 0;
  ss << "SELECT account_no,email,password FROM users";
  string query = ss.str();
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if(qstate == 0){
    res = mysql_store_result(conn);//storing the result in res
    while((row = mysql_fetch_row(res))){ // fetching each row in res and printing the output.
        if((strcmp(row[1], emaildb.c_str()) == 0) && (strcmp(row[2], passdb.c_str()) == 0))
        {
            flag = 1;
            cout<<"Login Successful..."<<endl;
            char *ch = row[0];
            return atoi(ch);
        }
    }
    if(flag == 0)
    {
        cout<<"Invalid User name/password...."<<endl;
        return 0;
    }
  }
  else
  {
      cout<<"Error Occurred: "<<mysql_error(conn)<<endl;
      return 0;
  }
  return 0;
}


//Function to display details of the user
void display_details(MYSQL* conn,int acc)
{
  stringstream ss;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int qstate;
  ss << "SELECT account_no, name, dob, phoneno, address, district, state, gender, balance, email FROM users";
  string query = ss.str();
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if(qstate == 0){
    res = mysql_store_result(conn);//storing the result in res
    while((row = mysql_fetch_row(res))){ // fetching each row in res and printing the output.
        char *ch = row[0];
        if(atoi(ch) == acc)
        {
            cout<<"\t\t\t\t-----------------DETAILS ARE AS FOLLOWS------------------"<<endl;
            cout<<"WELCOME "<< row[1] << endl <<"ACCOUNT NUMBER: "<<atoi(ch)<<endl<< "DOB: " << row[2] << endl << "CONTACT NUMBER: " << row[3] << endl << "ADDRESS: " << row[4] << endl;
            cout<< "DISTRICT: " << row[5] << endl << "STATE: " << row[6] << endl << "GENDER: " << row[7] << endl << "BALANCE: " << row[8] << endl << "EMAIL: " << row[9] << endl;
        }
    }

  }
  else
  {
      cout<<"Error Occurred  "<<endl;
      cout<<"Error is: "<<mysql_error(conn)<<endl;;
  }
}


//Function to check if pin is present or not in database
int pin_generate(MYSQL* conn, int acc, int pin)
{
    stringstream ss;
    int qstate = 0;
    MYSQL_RES* res;
    MYSQL_ROW row;
    int p,p1,flag = 0;

    ss<<"SELECT pin,account_no FROM mpin";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    int flag1 = 0;
    if(qstate == 0)
    {
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)))
        {
        char *ch = row[0];
        p = atoi(ch);
        char *ch1 = row[1];
        p1 = atoi(ch1);
        if(p1 == acc)
        {
          flag1 = 1;
          if(p == pin){flag = 1;return 1;}//pin matched
          else {flag = 1;return 2;};//pin not matches
        }
        }
        if(flag1 == 0) return 0;//pin not exists for account number in case of function call in create pin function
        if(flag == 0) return 3;//pin not exists for account number in case of function call in transfer function
    }
    else
    {
        cout<<"P error"<<endl;//pin not created
        return 0;
    }

    return 0;
}


//Function to create a new pin
int create_pin(MYSQL* conn,int acc)
{
    stringstream ss;
    int qstate = 0;
    int pin,flag = 0;
    int p;

    while(flag == 0)
    {
        cout<<"Enter a 4 digit pin: ";
        cin>>pin;
        if((pin >= 1000) && (pin <= 9999))
        {
            flag = 1;
            continue;
        }
        else
        cout<<"\nEnter Valid Input"<<endl;

    }
    p = pin_generate(conn,acc,pin);//checks is pin already exists if p != 0 means pin already created for this account number
    if(p == 0)
    {
    ss<<"INSERT INTO mpin (pin, account_no) VALUES ("<< pin <<", "<< acc <<")";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    if(qstate == 0 && flag == 1)//pin created successfully
    {
        return 1;
    }
    else
    {
        cout<<"Not created: "<<mysql_error(conn)<<endl;
        return 0;
    }
    }
    else cout<<"Pin already exists for the account: "<<acc<<endl;
    return 0;
}


//Function to check if the amount entered in transfer or withdraw valid or not if not it means insufficient balance
int bal_validate(MYSQL* conn, int acc, int transfer)
{
    stringstream ss;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate = 0;

    ss<<"select balance from users where account_no = "<< acc <<"";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    if(qstate == 0)
    {
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        const char* ch = row[0];
        int bal = atoi(ch);
        if(bal >= transfer)
            return 1;
        else
            return 2;
    }
    else
    {
        cout<<"Balance query error: "<<mysql_error(conn)<<endl;
        return 0;
    }

}


//Function to transfer money
void transfer_money(MYSQL* conn, int acc)
{
  stringstream ss,ss1,ss2,ss3;
  MYSQL_ROW row;
  MYSQL_RES* res;
  int recipient_amt = 0,sender_amt = 0,transfer_amt,account_num;
  int qstate = 0,qstate1 = 0,qstate2 = 0,qstate3 = 0;
  int p = 0;

  cout<<"Enter Account Number of the recipient: ";
  cin>>account_num;
  int acc_val = account_validator(conn,account_num);
  if(acc == account_num)
    {
        cout<<"please do not enter your account number";
        return;
    }
  if(acc_val == 0)
    {
        cout<<"Recipient account does not exist\nExiting...."<<endl;
        return;
    }

  cout<<"Enter Amount to transfer: ";
  cin>>transfer_amt;
  if(transfer_amt < 0)
    {
        cout<<"Enter Valid Input";
        return;
    }//if amount entered is negative

  if(transfer_amt > 10000000)
    {
        cout<<"Max Transaction limit per day is 1crore rupees. "<<endl;
        return;
    }


  int bal_valid = bal_validate(conn,acc,transfer_amt);
  if(bal_valid == 2)
  {
      cout<<"Insufficient Balance......"<<endl;
      return;
  }


  sender_amt = sender_amt - transfer_amt;
  recipient_amt = recipient_amt + transfer_amt;


  cout<<"enter pin: "<<endl;
  cin>>p;

  int spin = pin_generate(conn,acc,p);
  if(spin == 2){cout<<"Wrong PIN number\nExiting...."<<endl;return;}
  else if((spin == 3) || (spin == 0)){cout<<"PIN Not Exists for this account number. Press '6' to create your pin...."<<endl;return;}


  ss<<"SELECT account_no,balance FROM users";
  string query = ss.str();
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);

  if(spin == 1 && acc_val != 0)// && validpin != 0)
 {
  if(qstate == 0){
    res = mysql_store_result(conn);
    while((row = mysql_fetch_row(res)))
    {
        char *ch = row[0];
        int ac = atoi(ch);
        char *ch1 = row[1];
        int bala = atoi(ch1);
        if(acc == ac)//For Sender Balance Update
        {
            if(bala > transfer_amt)
            {
              sender_amt = bala - transfer_amt;
              ss1 << "UPDATE users SET balance = "<< sender_amt <<" WHERE account_no = "<< acc <<"";
              string query = ss1.str();
              const char* q1 = query.c_str();
              qstate1 = mysql_query(conn, q1);
              if(qstate1 == 0)
              {
                  cout<<"Amount Of Rupees "<<transfer_amt<<" Transfered Successfully to Account Number: "<<account_num<<endl;
              }
              else
              {
                  cout<<"Transfer Error: "<<mysql_error(conn);
              }
            }
            else
            {
                cout<<"Insufficient Balance......"<<endl;
            }
        }
        char *ch2 = row[0];
        int ac1 = atoi(ch2);
        char *ch3 = row[1];
        int bala1 = atoi(ch3);
        if(account_num == ac1)//For Recipient Balance Update
        {
              recipient_amt = bala1 + transfer_amt;
              ss2 << "UPDATE users SET balance = "<< recipient_amt <<" WHERE account_no = "<< account_num <<"";
              string query2 = ss2.str();
              const char* q2 = query2.c_str();
              qstate2 = mysql_query(conn, q2);
              if(qstate2 == 0)
              {
                  cout<<"Amount Of Rupees "<<transfer_amt<<" Transfered Successfully From Account Number: "<<acc<<endl;
              }
              else
              {
                  cout<<"Transfer Error at recipient: "<<mysql_error(conn);
              }
        }
      }
      time_t now = time(0);
      //convert now to string form
      char* dt = ctime(&now);
      string d = dt;
      ss3<<"INSERT INTO transfer(date, account_no, balance, transamt, to_acc_no) VALUES ('"<< d <<"', "<< acc <<", "<< sender_amt <<", "<< transfer_amt <<", "<< account_num <<")";
      string query3 = ss3.str();
      const char* q3 = query3.c_str();
      qstate3 = mysql_query(conn, q3);
      if(qstate3 == 0)
      {
          cout<<"\nTransaction Successful without any discrepancies....."<<endl;
          cout<<"Current balance in your account is: "<<sender_amt<<endl;
      }
      else cout<<"Transfer Error is: "<<mysql_error(conn)<<endl;

    }
    else cout<<"query Error is: "<<mysql_error(conn)<<endl;
  }
  else cout<<"Pin not created"<<endl;
}



void deposit(MYSQL* conn, int acc_no)
{
    int qstate = 0,qstate1 = 0,qstate2 = 0;
    stringstream ss,ss1,ss2;
    int amount;
    MYSQL_ROW row;
    MYSQL_RES *res;
    int p = 0;

    cout<<"Enter Amount to deposit in your account: ";
    cin>>amount;
    if(amount < 0)
        {
            cout<<"Enter Valid Input"<<endl;
            return;
        }//When input is negative

    if(amount > 10000000)
        {
            cout<<"Max deposit limit per day is 1crore rupees. "<<endl;
            return;
        }

    cout<<"enter pin: "<<endl;
    cin>>p;

    int spin = pin_generate(conn,acc_no,p);
    if(spin == 2)
        {
            cout<<"Wrong PIN number\nExiting...."<<endl;
            return;
        }
    else if((spin == 3) || (spin == 0))
        {
            cout<<"PIN Not Exists for this account number. Press '6' to create your pin...."<<endl;
            return;
        }


    if(spin == 1)
    {
        ss1<<"select balance FROM users WHERE account_no = "<< acc_no <<"";
        string query1 = ss1.str();
        const char *q1 = query1.c_str();
        qstate1 = mysql_query(conn,q1);
        if(qstate1 == 0)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            char *ch = row[0];
            int bal = atoi(ch);
            bal = bal + amount;

            time_t now = time(0);
            //convert now to string form
            char* dt = ctime(&now);
            string d = dt;

            ss2<<"UPDATE users SET balance = "<< bal <<" WHERE account_no = "<< acc_no <<"";
            string query2 = ss2.str();
            const char *q2 = query2.c_str();
            qstate2 = mysql_query(conn,q2);
            if(qstate2 == 0)
            {
                cout<<"Balance Updated..."<<endl;
            }
            else cout<<"Balance Not Updated Error is : "<<mysql_error(conn)<<endl;

            ss<<"INSERT INTO deposit (date, account_no, balance, transamt) VALUES ('"<< d <<"', "<< acc_no <<", "<< bal <<","<< amount <<")";
            string query = ss.str();
            const char *q = query.c_str();
            qstate = mysql_query(conn,q);
            if(qstate == 0)
            {
                cout<<"Amount of rupees "<<amount<<" has been deposited into account "<<acc_no<<" Successfully"<<endl;
                cout<<"Current balance in your account is: "<<bal<<endl;
            }
            else cout<<"Deposition error occurred: "<<mysql_error(conn);

        }
        else{cout<<"Account Does not exist... error is: "<<mysql_error(conn)<<endl;return;}
    }
    else cout<<"Pin not created"<<endl;
}



void withdraw(MYSQL* conn, int acc_no)
{
    int qstate = 0,qstate1 = 0,qstate2 = 0;
    stringstream ss,ss1,ss2;
    int amount;
    MYSQL_ROW row;
    MYSQL_RES *res;
    int p = 0;

    cout<<"Enter Amount to withdraw from your account: ";
    cin>>amount;
    if(amount < 0)
        {
            cout<<"Enter Valid Input"<<endl;
            return;
        }//When input is negative
    if(amount > 10000000)//When input is larger than int i.e if input is long int
        {
            cout<<"Max withdraw limit per day is 1 crore rupees. "<<endl;
            return;
        }


    int bal_valid = bal_validate(conn,acc_no,amount);
    if(bal_valid == 2)
    {
      cout<<"Not enough balance Exiting......"<<endl;
      return;
    }


    cout<<"enter pin: "<<endl;
    cin>>p;

    int spin = pin_generate(conn,acc_no,p);
    if(spin == 2)
        {
            cout<<"Wrong PIN number\nExiting...."<<endl;
            return;
        }
    else if((spin == 3) || (spin == 0))
        {
            cout<<"PIN Not Exists for this account number. Press '6' to create your pin...."<<endl;
            return;
        }


    if(spin == 1)//Pin exists for this account number
    {
        ss1<<"select balance FROM users WHERE account_no = "<< acc_no <<"";
        string query1 = ss1.str();
        const char *q1 = query1.c_str();
        qstate1 = mysql_query(conn,q1);
        if(qstate1 == 0)
        {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            char *ch = row[0];
            int bal = atoi(ch);
            bal = bal - amount;

            time_t now = time(0);
            //convert now to string form
            char* dt = ctime(&now);
            string d = dt;

            ss2<<"UPDATE users SET balance = "<< bal <<" WHERE account_no = "<< acc_no <<"";
            string query2 = ss2.str();
            const char *q2 = query2.c_str();
            qstate2 = mysql_query(conn,q2);
            if(qstate2 == 0)
            {
                cout<<"Balance Updated..."<<endl;
            }
            else cout<<"Balance Not Updated Error is : "<<mysql_error(conn)<<endl;

            ss<<"INSERT INTO withdraw (date, account_no, balance, transamt) VALUES ('"<< d <<"', "<< acc_no <<", "<< bal <<","<< amount <<")";
            string query = ss.str();
            const char *q = query.c_str();
            qstate = mysql_query(conn,q);
            if(qstate == 0)
            {
                cout<<"Amount of rupees "<<amount<<" has been debited from account "<<acc_no<<" Successfully"<<endl;
                cout<<"Current balance in your account is: "<<bal<<endl;
            }
            else cout<<"Withdraw error occurred: "<<mysql_error(conn);

        }
        else{cout<<"Account Does not exist... error is: "<<mysql_error(conn)<<endl;return;}
    }
    else cout<<"Pin not created"<<endl;
}



void show_transaction(MYSQL* conn, int acc_no)
{
    stringstream ss,ss1,ss2;
    MYSQL_ROW row,row1,row2;
    MYSQL_RES *res,*res1,*res2;
    int qstate = 0,qstate1 = 0,qstate2 = 0;


    //For Transfer
    ss<<"SELECT sno, date, account_no, transamt, to_acc_no FROM transfer WHERE account_no = "<< acc_no <<"";
    string query = ss.str();
    const char *q = query.c_str();
    qstate = mysql_query(conn,q);
    if(qstate == 0)
    {
        cout<<"\t\t\t\t-----------------TRANSACTION DETAILS------------------"<<endl;
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res)))
        {
        char *ch = row[0];//for s.no that is Transaction Number
        char *ch1 = row[2];//for account number
        char *ch2 = row[3];//for transaction amount
        char *ch3 = row[4];//for recipient account number

        cout<<"\nTransaction Number: "<< atoi(ch) << endl <<"Transaction Date and Time: " << row[1] <<"From Account Number: " << atoi(ch1) << endl;
        cout<<"To Account Number: "<< atoi(ch3) << endl <<"Transaction Amount: "<< atoi(ch2) << endl;
        }
    }
    else cout<<"Transaction details Error: "<<mysql_error(conn)<<endl;


    //For Deposit
    ss1<<"SELECT sno, date, account_no, transamt FROM deposit WHERE account_no = "<< acc_no <<"";
    string query1 = ss1.str();
    const char *q1 = query1.c_str();
    qstate1 = mysql_query(conn,q1);
    if(qstate1 == 0)
    {
        cout<<"\t\t\t\t-------------------DEPOSIT DETAILS---------------------"<<endl;
        res1 = mysql_store_result(conn);
        while((row1 = mysql_fetch_row(res1)))
        {
        char *ch = row1[0];//for s.no that is Transaction Number
        char *ch1 = row1[2];//for account number
        char *ch2 = row1[3];//for transaction amount

        cout<<"\nTransaction Number: "<< atoi(ch) << endl <<"Transaction Date and Time: " << row1[1] << "Account Number: " << atoi(ch1) << endl;
        cout<<"Transaction Amount: "<< atoi(ch2) << endl;
        }
    }
    else cout<<"Deposit details Error: "<<mysql_error(conn)<<endl;


    //For withdraw
    ss2<<"SELECT sno, date, account_no, transamt FROM withdraw WHERE account_no = "<< acc_no <<"";
    string query2 = ss2.str();
    const char *q2 = query2.c_str();
    qstate2 = mysql_query(conn,q2);
    if(qstate2 == 0)
    {
        cout<<"\t\t\t\t-------------------WITHDRAW DETAILS--------------------"<<endl;
        res2 = mysql_store_result(conn);
        while((row2 = mysql_fetch_row(res2)))
        {
        char *ch = row2[0];//for s.no that is Transaction Number
        char *ch1 = row2[2];//for account number
        char *ch2 = row2[3];//for transaction amount

        cout<<"\nTransaction Number: "<< atoi(ch) << endl <<"Transaction Date and Time: " << row2[1] << "Account Number: " << atoi(ch1) << endl;
        cout<<"Transaction Amount: "<< atoi(ch2) << endl;
        }
    }
    else cout<<"Deposit details Error: "<<mysql_error(conn)<<endl;

}


void show_pin(MYSQL* conn,int acc_no)
{
    stringstream ss;
    MYSQL_ROW row;
    MYSQL_RES *res;
    int qstate = 0;
    string email;

    cout<<"Enter your email address: ";
    cin.ignore();
    getline(cin,email);
    transform(email.begin(), email.end(), email.begin(), ::tolower);//Function to transform the string to lowercase whatever the case input is.
    cout<<"Account Number is: "<<acc_no<<endl;

    ss<<"SELECT pin FROM mpin, users WHERE (mpin.account_no = "<< acc_no <<" AND users.account_no = "<< acc_no <<") AND users.email = '" << email << "'";
    string query = ss.str();
    const char *q = query.c_str();
    qstate = mysql_query(conn,q);
    int flag = 0;
    if(qstate == 0)
    {
          res = mysql_store_result(conn);
          row = mysql_fetch_row(res);
          if(row != NULL)//condition to check whether email address is correct or not. if not correct then res will be null.
          {
            char *ch = row[0];
            cout<<"Your pin number is: "<<atoi(ch)<<endl;
            flag = 1;
          }

          if(flag == 0){cout<<"Invalid email address"<<endl;}
    }
    else
    {
        cout<<"Pin does not exist or wrong email address...."<<mysql_error(conn)<<endl;
    }
    return;
}


int main()
{
    MYSQL* conn;
    conn = mysql_init(0);//Intializating the connection
    conn = mysql_real_connect(conn, "192.168.43.230", "user", "pass", "bankcs", 3306, NULL, 0);
    int acc_no = 0,pin = 0;//Variable to check if login successful or not. Initially taking user did not login(Taking account_number as a return value in below cases).
    string email,pass;//To input email and password in login function
    //mysql_real_connect(pointer, local ipv4 address, user name, password, database name, port, socket to NULL, 0)
    system("Color E4");//E is background color yellow and 4 is text color red
    cout<<"\t\t\t=================================================================="<<endl;
    cout<<"\t\t\t                        WELCOME TO BANK                           "<<endl;
    cout<<"\t\t\t=================================================================="<<endl;

    if(conn){
        char ch;
        cout<<"Database Connected Successfully. . ."<<endl;
        do
        {
            cout<<"Enter\n\n\t 1 - Create Account\n\t 2 - Login\n\t 3 - Transfer\n\t 4 - Deposit\n\t 5 - Withdraw\n\t 6 - create pin\n\t 7 - Display Details\n\t 8 - show Transaction Details\n\t 9 - forgot pin?\n\t 0 - exit"<<endl;
            cin>>ch;
            system("cls");
            switch(ch)
            {
            case '1':
                createaccount(conn);
                break;
            case '2':
                 cout<<"Enter email address: ";
                 cin.ignore();
                 getline(cin,email);
                 transform(email.begin(), email.end(), email.begin(), ::tolower);//Function to transform the string to lowercase whatever the case input is.

                 cout<<"Enter password: ";
                 getline(cin,pass);

                 acc_no = loginaccount(conn,email,pass);//returns account number
                 if(acc_no != 0) display_details(conn,acc_no);
                 else cout<<"Please Login..."<<endl;
                 break;
            case '3':
                if(acc_no!=0) transfer_money(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '4':
                if(acc_no!=0) deposit(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '5':
                if(acc_no!=0) withdraw(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '6':
                if(acc_no!=0){
                  pin = create_pin(conn,acc_no);
                  if(!pin) cout<<"Pin Not created"<<endl;
                  else if(pin == 1) cout<<"Pin Created Successfully"<<endl;
                  else continue;
                }
                else cout<<"Please Login..."<<endl;
                break;
            case '7':
                if(acc_no!=0) display_details(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '8':
                if(acc_no!=0) show_transaction(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '9':
                if(acc_no!=0) show_pin(conn,acc_no);
                else cout<<"Please Login..."<<endl;
                break;
            case '0':
                cout<<"Thank You!!!!!"<<endl;
                break;
            default:
                cout<<"Please Enter a valid input"<<endl;
                break;
            }
        }while(ch != '0');
    }
    else
    {
        cout<<"Database Not Connected"<<endl;
    }
    return 0;
}
