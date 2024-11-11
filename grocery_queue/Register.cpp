
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime; 
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() { delete queue; delete next; }//added delete queue

QueueList* Register::get_queue_list() { return queue; }

Register* Register::get_next() { return next; }

int Register::get_ID() { return ID; }

double Register::get_secPerItem() { return secPerItem; }

double Register::get_overheadPerCustomer() { return overheadPerCustomer; }

double Register::get_availableTime() { return availableTime; }

void Register::set_next(Register* nextRegister) { 
  next = nextRegister;
}


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue
  
  if(queue == NULL || queue->get_head() == nullptr)
    return -1;
  Customer *p = queue->get_head();

  return (get_availableTime()>p->get_arrivalTime()?get_availableTime():p->get_arrivalTime()) + p->get_numOfItems()*get_secPerItem() + get_overheadPerCustomer();//p->get_arrivalTime()
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, SET LAST DEQUEUE TIME, add to doneList,
  // update availableTime of the register

  //sort this

  // if(queue->get_head() ==  NULL)
  //   return;

  double departTime = calculateDepartTime();

  Customer *p = queue->dequeue();
  if(p != NULL)
  {
    doneList->enqueue(p);
    p->set_departureTime(departTime);
  }

  set_availableTime(departTime);  
  //set_availableTime(p->get_departureTime());


  // Customer *p1 = doneList->get_head();

  // if(p1 == NULL)
  //   doneList->enqueue()

  // while(p1->get_next() != nullptr)
  // {
  //   p1 = p1->get_next();
  // }
  

}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
