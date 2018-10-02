/*
Prepares an inventory report stores the result into a linked list and displays the list data.
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <stdlib.h>
using namespace std;

// start creating linked list
struct node
{
    int account_number;
    string last_name;
    string first_name;
    unsigned month;   // date created
    unsigned day;
    unsigned year;
    float balance;
    char acct_type;  
    bool acct_status;
    node *next;
};

string month[13] = {"Error", "Jan.", "Feb.", "March" "Apr.", "May", "June", "July", "Aug.", "Sep.", "Oct.", "Nov.", "Dec."};

//function prototypes
void read_data(string fileName); // for reading file inventory data
void write_data(string fileName, bool acct_status); 
char menu(); // Show menu
void insert_node(node *current);
node* find_node(int number);
void remove_node(node *_node);
void modify();
void showlist(bool acct_status);

void modify_account();
void delete_account();
void add_account();
void deposit_funds();
void withdraw_funds();
void print_reports();
//end of function prototypes

node *head;
int list_size;

int main() {
  head = NULL;
  list_size =  0;
  read_data("read.txt");
  char choice;
  int number;
  do{
    choice = menu();
    switch(toupper(choice))
    {
      case 'M':
        modify_account();
        break;
      case 'D':
        delete_account();
        break;
      case 'A':
        add_account();
        break;
      case 'R':
        deposit_funds();
        break;
      case 'W':
        withdraw_funds();
        break;
      case 'P':
        print_reports();
        break;
      case 'Q':
        cout << "Bye!!";
        break;
      default:
        cout << "Option not valid!" << endl;
        break;
    }
  } while(choice != 'Q');
  return 0;
}

int input_account() {
  int number;
  cout << "Enter the account number: " << endl;
  cin >> number;
  
  // Validate the input
  if(number < 1) {
    cout << "Input not valid!" << endl;
    return -1;
  }

  return number;
}

node *validate_account() {
  int number = input_account();
  if(number == -1) {
    return NULL;
  }

  // Find if the account number exists
  node *account = find_node(number);
  if(account == NULL) {
    cout << "Account doesnot exist!" << endl;
  }

  return account;
}

void modify_account() {
  node *account = validate_account();
  if(account == NULL) {
    return;
  }

  string status;
  cout << "Enter the new status (active or inactive): " << endl;
  cin >> status;
  if(status == "active") {
    account->acct_status = 1;
  }
  else if(status == "inactive") {
    account->acct_status = 0;
  }
  else {
    cout << "Option not valid!" << endl;
    return;
  }

  cout << "Status modified!" << endl;
}

void delete_account() {
  node *account = validate_account();
  if(account != NULL) {
    remove_node(account);
  }
}

void add_account() {
  int number = input_account();
  if(number == -1) {
    return;
  }

  // Find if the account number exists
  node *account = find_node(number);
  if(account != NULL) {
    cout << "Duplicate account: " << endl;
    return;
  }

  account = new node();
  account->account_number = number;
  // prompt for individual field
  string input;
  int number_input;

  cout << "Enter last name: " << endl;
  cin >> input;
  account->last_name = input;

  cout << "Enter first name: " << endl;
  cin >> input;
  account->first_name = input;
  
  cout << "Enter month: " << endl;
  cin >> number_input;
  account->month = number_input;

  cout << "Enter day: " << endl;
  cin >> number_input;
  account->day = number_input;

  cout << "Enter year: " << endl;
  cin >> number_input;
  account->year = number_input;

  float balance = 0;
  cout << "Enter balance: " << endl;
  cin >> balance;
  account->balance = balance;

  cout << "Enter account type (c or s): " << endl;
  cin >> input;
  account->acct_type = tolower(input[0]);

  cout << "Enter account status (active or inactive): " << endl;
  cin >> input;  
  if(input == "active") {
    account->acct_status = 1;
  }
  else if(input == "inactive") {
    account->acct_status = 0;
  }
  else {
    cout << "Option not valid!" << endl;
    return;
  }

  insert_node(account);
}

float validate_transaction(node *account) {
  if(account->acct_status == false) {
    cout << "Account inactive!" << endl;
    return 0;
  }

  // prompt for individual field
  string input;
  cout << "Enter account type: " << endl;
  cin >> input;
  if(account->acct_type != tolower(input[0])) {
    cout << "Account type mismatch!" << endl;
    return 0;
  }

  float amount = 0;
  cout << "Enter amount: " << endl;
  cin >> amount;
  if(amount <= 0) {
    cout << "Invalide input!" << endl;
  }

  return amount;
}

void deposit_funds() {
  node *account = validate_account();
  if(account == NULL) {
    return;
  }

  float amount = validate_transaction(account);
  if(amount > 0) {
    account->balance += amount;
  } 
}

void withdraw_funds() {
  node *account = validate_account();
  if(account == NULL) {
    return;
  }
  float amount = validate_transaction(account);
  if(amount > 0) {
    if(account->balance - amount < 0) {
      cout << "Balance can't be negative!" << endl;
      return;
    }
    account->balance -= amount;
  }
}

void print_reports() {
  showlist(true);
  showlist(false);
  write_data("active_report.txt", true);
  write_data("inactive_report.txt", false);
}

void write_data(string file_name, bool acct_status) {
  ofstream outfile;
  outfile.open(file_name.c_str(), ios::trunc);

  time_t timeval;
  struct tm *tp;
  time (&timeval);
  tp = gmtime(&timeval);
  int currYear = 1900+tp->tm_year;
  int currMonth = tp->tm_mon +1;
  if(head == NULL){
    outfile << "The list is empty"  << endl;
    return;
  }

  if(acct_status) {
    outfile << setw(60) << "[Active]  Account Report" << endl;
  }
  else {
    outfile << setw(60) << "[IN-Active]  Account Report" << endl;
  }

  outfile << setw(50) << "****************" << endl;
  outfile << left << setw(11) <<  "Last"   << setw(20) << "First" << setw(15) << "Date of" << setw(20) << "Account" << setw(20) << "Account" << setw(20) << "Account" << setw(20) << "Account" << endl;
  outfile << left << setw(11) <<  "Name" << setw(20) << "Name" << setw(15) << "Creation" << setw(20) << "Balance" << setw(20) << "Type" << setw(20) << "Age" << setw(20) << "Number"<< endl;
  node *current = head;
  double totalValue = 0;
  while(current != NULL)
  {
  	ostringstream date1, date2;

    //Calculating account age
    int yearDiff, monthDiff;
    yearDiff = currYear-current->year;
    monthDiff = currMonth-current->month;

    if(monthDiff>6){
      yearDiff = yearDiff+1;
    } else if(monthDiff < -6){
      yearDiff = yearDiff-1;
    }

  	date1 << (current->month > 0 && current->month < 13 ? month[current->month] : month[0]) << " " << current->day << ", " << current->year;

    if(current->acct_status == acct_status){
      outfile << setw(11) << current->last_name << setw(20) << current->first_name
         << setw(15) << date1.str() << '$' 
         << setw(20) << fixed << setprecision(2) << current->balance 
         << setw(20) << current->acct_type
         << setw(20) << yearDiff
         << setw(20) << current->account_number << endl;
      totalValue += (current->balance);
    }

    current = current->next;
  }

  outfile << "Total Value of Inventory ======== $" << totalValue << endl;
  outfile << setw(50) << left << "END OF REPORT" << endl;

  outfile.close();
  return;
}

// reads inventory data from a file this data will be store into linked list
void read_data(string fileName)
{
  ifstream infile; // files read object
  infile.open(fileName.c_str());
  char *buffer = new char [20];
  node *current;
  string newLine;
  while(getline(infile,newLine))
  {
    std::istringstream iss(newLine);
    current = new node();
    // cout << newLine << endl;
    //printf("%s\n",infile );
    iss >> current->account_number >> current->last_name >> current->first_name >> 
    current->month >> current->day >> current->year >> current->balance >> current->acct_type
    >> current->acct_status;
    current->next = NULL;
    insert_node(current);
  }

  return;
}

char menu()
{
  cout << "Enter Option:" << endl;
  string choice;
  cout << "M) Modify account {status only} " << endl;
  cout << "D) Delete Account " << endl;
  cout << "A) Add Account " << endl;
  cout << "R) Deposit funds " << endl;
  cout << "W) Withdraw Funds " << endl;
  cout << "P) Print reports " << endl;
  cout << "Q) Terminate System " << endl;
  cin >> choice;
  return toupper(choice[0]);
}

node* find_node(int number) {
  node *current = head;

  while(current != NULL){
    if(current->account_number == number) {
      return current;
    }

    current = current->next;
  }

  return NULL;
}

void remove_node(node *_node) {
  node *current = head;
  node *previous = NULL;

  while(current != NULL){
    if(current->account_number == _node->account_number) {
      if(previous == NULL) {
        head = _node->next;
      }
      else {
        previous->next = _node->next;
      }

      current = NULL;
    }
    else {
      previous = current;
      current = current->next;
    }    
  }

  delete _node;
}

void insert_node(node *newOne){
    if(head == NULL){
      list_size = 1;
      head = newOne;
      return;
    }

    node *current = head;
    node *previus = NULL;

    while(current != NULL){
      if(current->account_number > newOne->account_number){
        newOne->next = current;
        if (previus == NULL){
        	head = newOne;
        }
        else{
        	previus->next = newOne;
    	  }
		    list_size++;
        return;
      }

      previus = current;
      current = current->next;
    }

    list_size++;
    previus->next = newOne;
}

void showlist(bool acct_status)
{
  time_t timeval;
  struct tm *tp;
  time (&timeval);
  tp = gmtime(&timeval);
  int currYear = 1900+tp->tm_year;
  int currMonth = tp->tm_mon +1;
  if(head == NULL){
    cout << "The list is empty"  << endl;
    return;
  }

  if(acct_status) {
    cout << setw(60) << "[Active]  Account Report" << endl;
  }
  else {
    cout << setw(60) << "[IN-Active]  Account Report" << endl;
  }

  cout << setw(50) << "****************" << endl;
  cout << left << setw(11) <<  "Last"   << setw(20) << "First" << setw(15) << "Date of" << setw(20) << "Account" << setw(20) << "Account" << setw(20) << "Account" << setw(20) << "Account" << endl;
  cout << left << setw(11) <<  "Name" << setw(20) << "Name" << setw(15) << "Creation" << setw(20) << "Balance" << setw(20) << "Type" << setw(20) << "Age" << setw(20) << "Number"<< endl;
  node *current = head;
  double totalValue = 0;
  while(current != NULL)
  {
  	ostringstream date1, date2;

    //Calculating account age
    int yearDiff, monthDiff;
    yearDiff = currYear-current->year;
    monthDiff = currMonth-current->month;

    if(monthDiff>6){
      yearDiff = yearDiff+1;
    } else if(monthDiff < -6){
      yearDiff = yearDiff-1;
    }

  	date1 << (current->month > 0 && current->month < 13 ? month[current->month] : month[0]) << " " << current->day << ", " << current->year;
    
    if(current->acct_status == acct_status){
      cout << setw(11) << current->last_name << setw(20) << current->first_name
         << setw(15) << date1.str() << '$' 
         << setw(20) << fixed << setprecision(2) << current->balance 
         << setw(20) << current->acct_type
         << setw(20) << yearDiff
         << setw(20) << current->account_number << endl;
      totalValue += (current->balance);
    }
    current = current->next;
  }

  cout << "Total Value of Inventory ======== $" << totalValue << endl;
  cout << setw(50) << left << "END OF REPORT" << endl;

  return;
}