#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

//void updateSystem();
void updateSingleSystem(string method);
void updateMultipleSystem(string method);

void printStats();

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  
  cout << "Finished at time " << expTimeElapsed << endl;
  cout << "Statistics:" << endl;
  printStats();


  //deleting singleQueue and its customers
  // Customer *cust = singleQueue->get_head();
  // while(cust != NULL)
  // {
  //   Customer *c1 = cust->get_next();
  //   delete cust;
  //   cust = c1;
  // }
  delete singleQueue;

  //deleting doneList and its customers
  // cust= doneList->get_head();
  // while(cust != NULL)
  // {
  //   Customer *c1 = cust->get_next();
  //   delete cust;
  //   cust = c1;
  // }
  delete doneList;

  //deleting registerList and its registers
  // Register *reg = registerList->get_head();
  // while(reg != nullptr)
  // {
  //   Register *r1 = reg->get_next();
  //   delete reg;
  //   reg = r1;
  // }
  delete registerList;


  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  expTimeElapsed += timeElapsed;
  Customer *cust = new Customer(expTimeElapsed, items);
  
  if(mode == "single")
  {
    updateSingleSystem("addCustomer");
    Register *reg = registerList->get_free_register();
    if(reg != nullptr)
    {
      reg->get_queue_list()->enqueue(cust);
      cust->set_arrivalTime(expTimeElapsed);
      cout << "Queued a customer with free register " << reg->get_ID() << endl;
    }
    else
    {
      singleQueue->enqueue(cust);
      cout << "No free registers" << endl;
    }
    
  }

  //multiple
  else if(mode == "multiple")
  {
    updateMultipleSystem("addCustomer");
    cout << "Queued a customer with quickest register " << registerList->get_min_items_register()->get_ID() << endl;
    registerList->get_min_items_register()->get_queue_list()->enqueue(cust);
  }
  // cust->set_next(nullptr);
  // delete cust;  
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register

  Register *reg = registerList->get_head();

  while(reg != nullptr)
  {
    if(reg->get_ID() == ID)
    {
      cout << "Error: register " << ID << " is already open" << endl;
      return;
    }
    reg = reg->get_next();
  }
  expTimeElapsed += timeElapsed;
  Register *newreg = new Register(ID, secPerItem, setupTime, expTimeElapsed);
  registerList->enqueue(newreg);
  if(mode == "multiple")
    cout << "Opened register " << ID << endl;

  if(mode == "single")
  {
    updateSingleSystem("openRegister");
    cout << "Opened register " << ID << endl;
    if(singleQueue->get_head() != NULL)
    {
      newreg->get_queue_list()->enqueue(singleQueue->dequeue());
      cout << "Queued a customer with free register " << ID << endl;
    }
  }
  
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments" << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 

  expTimeElapsed += timeElapsed;
  if(mode == "single")
    updateSingleSystem("closeRegister");
  else if(mode == "multiple")
    updateMultipleSystem("closeRegister");
  else return;

  Register *dqd = registerList->dequeue(ID);
  if(dqd == nullptr)
  {
    cout << "Error: register " << ID << " is not open" << endl;
  }
  else
  {
    cout << "Closed register " << ID << endl;
    delete dqd;
  }
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}


void updateSingleSystem(string method)
{
  // Register* reg = registerList->get_head();
  // while(reg != NULL)
  // {
  //   if(reg->calculateDepartTime() <= expTimeElapsed)
  //   {
  //     reg->departCustomer(doneList);
  //   }
  //   reg = reg->get_next();
  // }

  while(registerList->calculateMinDepartTimeRegister(expTimeElapsed) != nullptr)
  {
    if(registerList->calculateMinDepartTimeRegister(expTimeElapsed)->calculateDepartTime() > expTimeElapsed)
    {
      break;
    }
    else{
      Register *reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
      cout << "Departed a customer at register ID " << reg->get_ID() << " at " << reg->calculateDepartTime() << endl; //expTimeElapsed << endl; //- registerList->calculateMinDepartTimeRegister(expTimeElapsed)->calculateDepartTime() << endl;
      reg->departCustomer(doneList);
      if(singleQueue != NULL)
      {
        Customer *cust = singleQueue->dequeue();
        if(cust != NULL)
        {
          reg->get_queue_list()->enqueue(cust);
          cout << "Queued a customer with free register " << reg->get_ID() << endl;
        }
          
      }
    }
  }

  if(method == "addCustomer")
  {
    cout << "A customer entered" << endl;
  }


  
}

void updateMultipleSystem(string method)
{
  while(registerList->calculateMinDepartTimeRegister(expTimeElapsed) != nullptr)
  {
    if(registerList->calculateMinDepartTimeRegister(expTimeElapsed)->calculateDepartTime() > expTimeElapsed)
    {
      break;
    }
    else{
      cout << "Departed a customer at register ID " << registerList->calculateMinDepartTimeRegister(expTimeElapsed)->get_ID() << " at " << registerList->calculateMinDepartTimeRegister(expTimeElapsed)->calculateDepartTime() << endl; //- registerList->calculateMinDepartTimeRegister(expTimeElapsed)->calculateDepartTime() << endl;
      registerList->calculateMinDepartTimeRegister(expTimeElapsed)->departCustomer(doneList);
    }
  }

  if(method == "addCustomer")
  {
    cout << "A customer entered" << endl;
  }
}

void printStats()
{
  double maxWaitTime = 0;
  double totalWaitTime = 0;
  int numOfCustomers = 0;

  Customer *p = doneList->get_head();

  while(p != NULL)
  {
    totalWaitTime += p->get_departureTime() - p->get_arrivalTime();
    if(p->get_departureTime() - p->get_arrivalTime() > maxWaitTime)
      maxWaitTime = p->get_departureTime() - p->get_arrivalTime();
    numOfCustomers++;
    p = p->get_next();
  }

  double avgWaitTime = 0.0;
  if(numOfCustomers != 0)  avgWaitTime = totalWaitTime / (double)numOfCustomers;
  
  cout << "Maximum wait time: " << maxWaitTime << endl;
  cout << fixed;
  //cout << setprecision(4);
  if(double(avgWaitTime - (int)avgWaitTime) == 0.0)
    cout << "Average wait time: " << (int)avgWaitTime << endl;
  else
    cout << "Average wait time: " << setprecision(4) << avgWaitTime << endl;
  
  p = doneList->get_head();
  double sumOfSqDiff = 0;
  while(p != NULL)
  {
    sumOfSqDiff += pow((p->get_departureTime() - p->get_arrivalTime()) - avgWaitTime, 2);
    p = p->get_next();
  }
  double std_dev = sqrt(sumOfSqDiff / numOfCustomers);
  if(double(std_dev - (int)std_dev) == 0.0)
    cout << "Standard Deviation of wait time: " << (int)std_dev << endl;
  else
    cout << "Standard Deviation of wait time: " << setprecision(4) << std_dev << endl;
}
