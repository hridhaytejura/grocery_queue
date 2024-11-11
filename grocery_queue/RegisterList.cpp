#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
  delete head;
}

Register* RegisterList::get_head() {  
  return head;
}

int RegisterList::get_size() { 
  // return number of registers 
  return size;
}


Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  Register *p = head;
  Register *ret = nullptr;
  int compare = 2147483647;
  

  while(p != NULL && p->get_queue_list() != NULL)
  {
    Customer *cust = p->get_queue_list()->get_head();
    int items = 0;
    while(cust != NULL)
    {
      items += cust->get_numOfItems();
      cust = cust->get_next();
    }
    if(items < compare)
    {
      compare = items;
      ret = p;
    }
    p = p->get_next();
  }
  return ret;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr

  Register *p = head;
  while(p != NULL)
  {
    if(p->get_queue_list()->get_head() == NULL)
    {
      return p;
    }
    else{
      p = p->get_next();
    }
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 

  if(head == NULL)
  {
    head = newRegister;
    head->set_next(NULL);
    ++size;
    return;
  }

  Register *p = head;

  while(p->get_next() != NULL)
  {
    p = p->get_next();
  }

  p->set_next(newRegister);
  ++size;
  
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise

  Register *p = head;

  while(p != NULL)
  {
    if(p->get_ID() == ID)
      return true;
    p = p->get_next();
  }
  return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found

  if(head == NULL){
    return nullptr;
  }

  if(!foundRegister(ID))
  {
    return nullptr;
  }    
  

  if(head->get_ID() == ID)
  {
    Register*p = head;
    head = head->get_next();
    p->set_next(nullptr);
    --size;
    return p;
  }

  Register* p = head;
  while(p != NULL && p->get_next()!= NULL)
  {
    if(p->get_next()->get_ID() ==  ID)
    {
      p->set_next(p->get_next()->get_next());
      p->get_next()->set_next(NULL);
      --size;
      return p->get_next();
    }
  }
  return nullptr;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr

  Register* p = head;
  Register* ret = nullptr;
  double time = - 999999999999999;

  while(p != nullptr)
  {
    Customer* cust = p->get_queue_list()->get_head();
    if(cust == NULL)
    {
      p = p->get_next();
      continue;
    }
    if(expTimeElapsed - p->calculateDepartTime() > time)
    {
      time = expTimeElapsed - p->calculateDepartTime();
      ret = p;
    } 
  
    p = p->get_next();
  }
  return ret;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
