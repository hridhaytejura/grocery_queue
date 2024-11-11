#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  delete head;//added this
}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  
  if(head == NULL)
  {
    head = customer;
  }
  Customer *p = head;
  while(p->get_next() != NULL)
  {
    p = p->get_next();
  }
  p->set_next(customer);
  customer->set_next(NULL);
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it

  if(head == NULL)
    return NULL;
  
  Customer *p = head;
  head = head->get_next();
  p->set_next(NULL);
  return p;
  
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  int count = 0;
  Customer *p = head;
  while(p != NULL)
  {
    count += p->get_numOfItems();
    p = p->get_next();
  }
  return count;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
