#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol,
                         unsigned int day_date, 
                         unsigned int month_date, 
                         unsigned int year_date, 
                         bool buy_sell_trans, 
                         unsigned int number_shares, 
                         double trans_amount) 
{ 
  acb = 0;
  acb_per_share = 0;
  share_balance = 0; 
  cgl = 0;

  symbol = ticker_symbol; 
  day = day_date; 
  month = month_date; 
  year = year_date; 

  if ( buy_sell_trans == true) { 
    trans_type = "Buy"; 
  }else { 
    trans_type = "Sell"; 
  }

  shares = number_shares; 
  amount = trans_amount; 
  trans_id = assigned_trans_id; 
  ++assigned_trans_id; 
  p_next = nullptr; 
}

// Destructor
// TASK 1
//

Transaction::~Transaction() { 


}


// Overloaded < operator.
// TASK 2
//
bool Transaction::operator < (Transaction const & other) { 
  if ( this->year < other.year) { 
    return true; 
  } else if ( this->year == other.year && this->month < other.month) { 
    return true; 
  } else if ( this->year == other.year && this->month == other.month && this->day < other.day) { 
    return true; 
  }
  return false; 
}



// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//

// Constructor
// TASK 3
//

History::History() { 
  this->p_head = nullptr; 

}

// Destructor
// TASK 3
//
History::~History(){ 
  Transaction * p_temp{p_head}; 

  while (p_temp != nullptr) { 
    p_temp = p_temp->get_next();

    delete p_head;
    p_head = p_temp;

  }
}


// read_history(...): Read the transaction history from file.
// TASK 4 - done 
//
void History::read_history(){
  ece150::open_file(); 

  //read transactions one at a time 
  //  read specific parts:day,month,year,etc
  //  dynamically allocate the transaction objects 
  //  call insert 

  while(ece150::next_trans_entry() == true) {   
    ece150::get_trans_symbol(); 
    ece150::get_trans_day(); 
    ece150::get_trans_month(); 
    ece150::get_trans_year(); 
    ece150::get_trans_type();
    ece150::get_trans_shares(); 
    ece150::get_trans_amount(); 

    Transaction *p_new_transaction = new Transaction (ece150::get_trans_symbol(), ece150::get_trans_day(), 
                                                      ece150::get_trans_month(), ece150::get_trans_year(),
                                                      ece150::get_trans_type(), ece150::get_trans_shares(),
                                                      ece150::get_trans_amount());  

    insert(p_new_transaction); 
  }
  ece150::close_file(); 
  
}

// Second Submission
// insert(...): Insert transaction into linked list.
// TASK 5 - done
//
void History::insert(Transaction * p_new_trans) {  
  //code from David Lau's help session on Nov 17

  //change where p_head is pointing to p_new_trans
  if ( p_head == nullptr) { 
    this->p_head = p_new_trans;
  } else { 
    //p_traverse -> move through the pointers to move from one transaction to the next until we move to the last one
    Transaction * p_traverse {p_head}; 

    //get the next Transaction by calling p_traverse-get_next()
    while (p_traverse->get_next() != nullptr) { 
      p_traverse = p_traverse->get_next(); 
    }

    p_traverse->set_next(p_new_trans);
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){
  Transaction *p_front = nullptr;
  Transaction *p_head_new = this->get_p_head();  

  //got help from WEEF TA's
  
  while (p_head_new != nullptr) {
    //isolating the front node 
    Transaction *p_traverse = p_head_new->get_next(); 
    Transaction *node = p_head_new; 
 
    //inserting the node into the sorted list
    if (p_front == nullptr) {
      node->set_next(p_front);
      p_front = node; 

    } else if(node-> operator< (*p_front)) { //overloaded operator
      node->set_next(p_front); 
      p_front = node;

    } else { 
      //code from David Lau's lab session
      Transaction* p_current = p_front; 

      while(p_current->get_next() != nullptr && p_current->get_next()->operator < (*node)) { 
        p_current = p_current->get_next(); 
      }

      node->set_next(p_current->get_next()); 
      p_current->set_next(node); 

    }
    p_head_new = p_traverse;
  }
  this->p_head = p_front; 
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl() {

  //got some part of the code from WEEF TA
  double acb{0.0}; 
  double acb_per_share{0.0}; 
  double share_balance{0.0}; 
  double cgl{0.0}; 

  Transaction *p_traverse = this->get_p_head(); 
  if (p_traverse != nullptr) { 
    while (p_traverse != nullptr) { 
      //check for buy and sell
      if (p_traverse->get_trans_type()) {
        acb = acb + p_traverse->get_amount(); 
        share_balance = share_balance + p_traverse->get_shares(); 
        acb_per_share = acb/share_balance; 
        cgl = 0; 

      } else { 

        acb = acb - p_traverse->get_shares()*acb_per_share; 
        share_balance = share_balance - p_traverse->get_shares(); 
        cgl = p_traverse->get_amount() - p_traverse->get_shares()*acb_per_share; 
      }

      p_traverse->set_acb_per_share(acb_per_share); 
      p_traverse->set_share_balance(share_balance); 
      p_traverse->set_acb(acb); 
      p_traverse->set_cgl(cgl); 
      //go to the next node in the linked list
      p_traverse = p_traverse->get_next(); 
    }
  }
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl(unsigned int year) {  
  //calculate the capital gains for every transction in history
  //get the next Transaction by calling p_traverse-get_next()

  double cgl{0.0}; 
  Transaction *p_traverse = this->get_p_head();  
  if(p_traverse != nullptr) { 
    while(p_traverse != nullptr) { 
      if (p_traverse->get_year() == year) { 
        cgl+=p_traverse->get_cgl(); 
      }
      p_traverse = p_traverse ->get_next(); 
    }
  }
  return cgl; 
}

// print() Print the transaction history.
//TASK 9
//

void History::print(){ 
  //Some code from David Lau's help session
  Transaction *temp = this->get_p_head(); 
  std::cout<< "BEGIN TRANSACTION HISTORY" <<std::endl;

  if(temp != nullptr) { 
    while (temp !=nullptr) { 
    temp->print(); 
    temp = temp->get_next(); 
    }
  } 
  std::cout << "END TRANSACTION HISTORY" <<std::endl; 
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { 
  return p_head; 
}
