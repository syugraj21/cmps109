// $Id: bigint.cpp,v 1.157 2013-02-04 15:09:21-08 - - $

#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
#include <sstream>

using namespace std;

#include "bigint.h"
#include "trace.h"

#define CDTOR_TRACE TRACE ('~', this << " -> " << small_value)

bigint::bigint (){
   negative = false;
   big_value = new bigvalue_t();
   CDTOR_TRACE;
}

bigint::bigint (const bigint &that) {
   if(this == &that){
      *this = that;
   } else {
      negative = that.negative;
      big_value = new bigvalue_t(*that.big_value);
   }
   CDTOR_TRACE;
}

bigint &bigint::operator= (const bigint &that) {
   if (this == &that) return *this;
   this->negative = that.negative;
   if (this->big_value != NULL){
         delete big_value;
   }
   this->big_value = new bigvalue_t(*that.big_value);
   return *this;
}

bigint::~bigint() {
   if ( big_value != NULL){
      delete big_value;
   }
   CDTOR_TRACE;
}

bigint::bigint (int that){
   string s;
   negative = false;
   big_value = new bigvalue_t();
   stringstream input;
   input >> that;
   s = input.str();
   insert_bigvalue(s);
   CDTOR_TRACE;
}

bigint::bigint (const string &that) {
   negative = false;
   big_value = new bigvalue_t();
   insert_bigvalue(that);
/*
   string::const_iterator itor = that.begin();
   string::const_iterator end = that.end();
   bool isnegative = false;
   if (*itor == '_') {isnegative = true; ++itor; }
   int newval = 0;
   for (; itor != end; ++itor) newval = newval * 10 + *itor - '0';
   small_value = isnegative ? - newval : + newval;*/
   CDTOR_TRACE;
}

void bigint::insert_bigvalue(const string &that){
   int inc_one = 0;
   string::const_reverse_iterator itor = that.rbegin();
   string::const_reverse_iterator end = that.rend();
   if ( *(end-1) == '_'){
      negative = true;
      inc_one = 1;
   }
   for(; itor != end-inc_one; ++itor){
      if (!isdigit(*itor)) {
         continue;
      }
      big_value->push_back( *itor - '0');
   }
}

bigint bigint::operator+ (const bigint &that) const {
   //return this->small_value + that.small_value;
   bigint new_value = bigint();
   if (this->negative == that.negative){
      do_bigadd(this->big_value,that.big_value,new_value.big_value);
      new_value.set_neg_value(this->negative); 
   } else { 
      if ( abscompare(that) >0){
         do_bigsub(this->big_value,that.big_value,new_value.big_value);
         new_value.set_neg_value(this->negative);
      } else if ( abscompare(that) <0){
         do_bigsub(that.big_value,this->big_value,new_value.big_value);
         new_value.set_neg_value(that.negative);
      }
   }
   if (new_value.big_value->size() == 0){
      new_value.set_neg_value(false);
   }
   no_leading_zero(new_value);
   return new_value;
}

bigint bigint::operator- (const bigint &that) const {
   bigint new_value = bigint();
   if(this->negative == that.negative){
      if (abscompare(that) == 1 ){
         do_bigsub(this->big_value,that.big_value,new_value.big_value);
         new_value.set_neg_value(this->negative);
      } else if (abscompare(that) == -1 ){
         do_bigsub(that.big_value,this->big_value,new_value.big_value);
         new_value.set_neg_value(that.negative);
      }
   } else {
      do_bigadd(this->big_value,that.big_value,new_value.big_value);
      new_value.set_neg_value(this->negative);
   }
   if(new_value.big_value->size() == 0){
       new_value.set_neg_value(false);
   }
   no_leading_zero(new_value);
   return new_value;
}

void bigint::do_bigadd(const bigvalue_t *this_val, 
        const bigvalue_t *that_val, bigvalue_t *new_val) const{
    bigvalue_t::const_iterator this_it = this_val->begin();
    bigvalue_t::const_iterator this_end = this_val->end();
    bigvalue_t::const_iterator that_it = that_val->begin();
    bigvalue_t::const_iterator that_end = that_val->end();
    int carry_digit = 0;
    int first_value;
    int second_value;
    bool first_num_bool = false;
    bool second_num_bool = false;
    if (this_val->size() == 0){
             first_num_bool = true;
    } 
    if (that_val->size() == 0){
         second_num_bool = true;
    }
    while (true){
         if (first_num_bool){
             first_value = 0;
         } else {
             first_value = *this_it;
         }

         if (second_num_bool){
              second_value = 0;
         } else {
              second_value = *that_it;
         }
         int sum = first_value + second_value + carry_digit;
         carry_digit = 0;
         if (sum >= 10){
             sum = sum-10;
             carry_digit = 1;
         }
         new_val->push_back(sum);
         ++this_it;
         ++that_it;
         if (this_it == this_end){
             first_num_bool = true; 
         }
         if (that_it == that_end){
             second_num_bool = true;
         }

         if (first_num_bool && second_num_bool){
             if (carry_digit > 0){
                 new_val->push_back(carry_digit);
             }
             break;
         } 
    } 
    /*while (this_it != this_end && that_it != that_end){
         if (this_it != this_end){
            first_value = *this_it;
            cout<< "first "  << int(*this_it) << endl;
         } else {
            first_value = 0;
         }
         if (that_it != that_end){
            second_value = *that_it;
            cout<< "second " << int(*that_it) << endl;
         } else {
            second_value = 0;
         }
         int sum = first_value + second_value + carry_digit;
         carry_digit = 0;
         if (sum >= 10){
            carry_digit = 1;
            sum = sum - 10;
         }
         cout << "sum "  << sum << endl;
         new_val->push_back(sum);
         if ( this_it != this_end){
            ++this_it;
         }
         if(that_it != that_end){
            ++that_it;
         }
         if (this_it == this_end && that_it == that_end){
            if (carry_digit > 0){
               new_val->push_back(carry_digit);
               break; 
            }
         }
    }*/
}

void bigint::do_bigsub(const bigvalue_t *big_val,
      const bigvalue_t *small_val, bigvalue_t *new_val) const{
   bigvalue_t::const_iterator big_it = big_val->begin();
   bigvalue_t::const_iterator big_end = big_val->end();
   bigvalue_t::const_iterator small_it = small_val->begin();
   bigvalue_t::const_iterator small_end = small_val->end();
   int carry_digit = 0;
   int first_value;
   int second_value;
   bool first_num_bool = false ;
   bool second_num_bool = false;
   if (big_val->size() == 0){
      first_num_bool = true;
   } 
   if (small_val->size() == 0){
      second_num_bool = true;
   }
   while(true){
      if (first_num_bool){
         first_value = 0;
      } else {
         first_value = *big_it; //- carry_digit;
         //cout << int(*big_it) << endl;
      }
      if (second_num_bool){
         second_value = 0;
      } else {
         second_value = *small_it;
      }
     
     /* if (first_value < second_value){
         first_value += 10;
         carry_digit = 1;
      } else {
         carry_digit = 0; 
      }*/
      int diff = first_value - second_value - carry_digit;
      carry_digit = 0;
      if( diff < 0 && !first_num_bool){
          carry_digit = 1;
          diff = diff + 10;
      }
      
      new_val->push_back(diff);
      ++big_it;
      ++small_it;
      if (big_it == big_end && !first_num_bool){
         first_num_bool = true; 
      }
      if (small_it == small_end && !second_num_bool){
         second_num_bool = true;
      }
      if (first_num_bool && second_num_bool){
         break;
      }
   }
}

bigint bigint::operator- () const {
   bigint temp = *this;
   temp.set_neg_value(!this->negative);
   return temp;
}

void bigint::set_neg_value(const bool &that){
   negative = that;
}

int bigint::compare (const bigint &that) const {
   if (this == &that){
      return 0;
   } else if (this->big_value->size() != that.big_value->size()){/*
      if (this->negative == false && that.negative == false){
          if (this->size() > that.size()){
             return +1;
          } else {
             return -1;
          }
      } else if (this->negative == true && that.negative == true){
          if(this->size() > that.size()){
             return -1;
          } else {
             return +1;
          }
      } else {
          if(this->negative == false){
             return +1;
          } else {
             return -1;
          }
      }*/
      return compare_unequal_size(that);
   } else {
      return compare_equal_size(that);
   }
   /*
   return this->small_value < that.small_value ? -1
        : this->small_value > that.small_value ? +1 : 0;*/
}

int bigint::compare_unequal_size(const bigint &that) const{ 
   if (this->negative == false && that.negative == false){
       if (this->big_value->size() > that.big_value->size()){
           return +1;
       } else {
           return -1;
       }
   } else if (this->negative == true && that.negative == true){
       if(this->big_value->size() > that.big_value->size()){
           return -1;
       } else {
           return +1;
           }
   } else {
       if(this->negative == false){
           return +1;
       } else {
           return -1;
       }
   }
}

int bigint::compare_equal_size(const bigint &that) const{
   bigvalue_t::const_reverse_iterator this_it =
        this->big_value->rbegin();
   bigvalue_t::const_reverse_iterator this_end =
        this->big_value->rend();
   bigvalue_t::const_reverse_iterator that_it =
        that.big_value->rbegin();
   bigvalue_t::const_reverse_iterator that_end = 
        that.big_value->rend();
   if (this->negative == false && that.negative == true){
      return +1;
   } else if (this->negative == true && that.negative == false){
      return -1;
   } else if (this->negative == false && that.negative == false){
      while (this_it != this_end && that_it != that_end){
         if (*this_it > *that_it){
            return +1;
         } else if (*this_it < *that_it){ 
            return -1;
         }
         ++this_it;
         ++that_it;
      }  
   } else {
      while (this_it != this_end && that_it != that_end){
         if (*this_it < *that_it){
            return +1;
         } else if (*this_it > *that_it){
            return -1; 
         }
      }
   }
   return 0;
}

int bigint::abscompare (const bigint &that) const {
   /*return abs (this->small_value) < abs (that.small_value) ? -1
        : abs (this->small_value) > abs (that.small_value) ? +1 : 0;*/
   bigvalue_t::const_reverse_iterator this_it =
        this->big_value->rbegin();
   bigvalue_t::const_reverse_iterator this_end =
        this->big_value->rend();
   bigvalue_t::const_reverse_iterator that_it =
        that.big_value->rbegin();
   bigvalue_t::const_reverse_iterator that_end =
        that.big_value->rend();
   if ( big_value->size() > that.big_value->size()){
       return 1;
   } else if (big_value->size() < that.big_value->size()){
       return -1;
   } else {    
    while (this_it != this_end && that_it != that_end){
        if (*this_it > *that_it){
           return +1;
        } else if (*this_it < *that_it) {
           return -1;
        }
        ++this_it;
        ++that_it;
    }
   }
   return 0;
}

int bigint::smallint () const {
   if (*this < numeric_limits<int>::min()
    || *this > numeric_limits<int>::max())
               throw range_error ("smallint: out of range");
   bigvalue_t::const_iterator itor = big_value->begin();
   bigvalue_t::const_iterator end = big_value->end();
   int small_val = 0;
   int mul_of_10 = 1;
   while (itor != end){
      small_val = small_val + (mul_of_10 * (*itor));
      mul_of_10 *= 10;
      ++itor;
   }
   if (this->negative ==true){
      small_val = -small_val;
   }
   return small_val;
}

bigint bigint::mul_by_2 () {
   bigint new_val = bigint();
   do_bigadd(big_value,big_value,new_val.big_value);
   return new_val;
}

static bigpair popstack (stack <bigpair> &egyptstack) {
   bigpair result = egyptstack.top ();
   egyptstack.pop();
   return result;
}


//
// Ancient Egyptian multiplication algorithm.
//
bigint bigint::operator* (const bigint &that) const {
   bigint top = that;
   bigint count =  bigint("1");
   bigint this_val = *this;
   bigint new_val;
   //cout<<"before loop"<<endl;
   //cout<<count<<endl;
   //cout<<top<<endl;
   //cout<<*this<<endl;
   TRACE ('*', *this << " * " << that);
   stack <bigpair> egyptstack;
   while (count <= *this ){
      egyptstack.push(bigpair(count,top));
      top = top.mul_by_2();
      count = count.mul_by_2();
      if (count > *this){
         break;
      }
   }
   while (!egyptstack.empty()){
      bigpair egypt_pair = popstack(egyptstack);
      //cout <<"egypt_pair: " <<egypt_pair.first<<endl;
      if (egypt_pair.first <= this_val){
         this_val = this_val - egypt_pair.first;
         new_val = new_val + egypt_pair.second;
        // cout << this_val<< endl;
      }
      //cout<<"this_val: "<< this_val << endl;
      //cout<< "new_val: "<<new_val<<endl;
   }
   if (this->negative != that.negative){
      new_val.set_neg_value(true);
   }
   return new_val;
}

//
// Ancient Egyptian division algorithm.
//
bigpair bigint::div_rem (const bigint &that) const {
   if (that == 0) throw range_error ("divide by 0");
   bigint count= bigint("1");
   bigint top = that;
   TRACE ('/', *this << " /% " << that);
   stack <bigpair> egyptstack;
   bigint quotient = bigint("0");
   bigint remainder = *this;
   while (top <= *this){
      egyptstack.push(bigpair(count,top));
      count = count.mul_by_2();
      top = top.mul_by_2();
   } 
   while (!egyptstack.empty()){
      bigpair egypt_pair = popstack(egyptstack);
      if (egypt_pair.second <= remainder){
         remainder = remainder - egypt_pair.second;
         quotient = quotient + egypt_pair.first;
      }
   }
   return bigpair (quotient, remainder);
}

bigint bigint::operator/ (const bigint &that) const {
   return div_rem (that).first;
}

bigint bigint::operator% (const bigint &that) const {
   return div_rem (that).second;
}


#define TRACE_POW TRACE ('^', "result: " << result \
                  << ", base: " << base << ", expt: " << expt);
bigint bigint::pow (const bigint &that) const {
   bigint base = *this;
   if (that > 999) throw range_error ("exp too big");
   int expt = that.smallint();
   bigint result = 1;
   TRACE_POW;
   if (expt < 0) {
      base = 1 / base;
      expt = - expt;
   }
   while (expt > 0) {
      TRACE_POW;
      if (expt & 1) { //odd
         result = result * base;
         --expt;
      }else { //even
         base = base * base;
         expt /= 2;
      }
   }
   TRACE_POW;
   return result;
}

//
// Macros can make repetitive code easier.
//

#define COMPARE(OPER) \
   bool bigint::operator OPER (const bigint &that) const { \
      return compare (that) OPER 0; \
   }
COMPARE (==)
COMPARE (!=)
COMPARE (< )
COMPARE (<=)
COMPARE (> )
COMPARE (>=)

#define INT_LEFT(RESULT,OPER) \
   RESULT operator OPER (int left, const bigint &that) { \
      return bigint (left) OPER that; \
   }
INT_LEFT (bigint, +)
INT_LEFT (bigint, -)
INT_LEFT (bigint, *)
INT_LEFT (bigint, /)
INT_LEFT (bigint, %)
INT_LEFT (bool, ==)
INT_LEFT (bool, !=)
INT_LEFT (bool, < )
INT_LEFT (bool, <=)
INT_LEFT (bool, > )
INT_LEFT (bool, >=)

ostream &operator<< (ostream &out, const bigint &that) {
   bigint::bigvalue_t::const_reverse_iterator itor =
         that.big_value->rbegin();
   bigint::bigvalue_t::const_reverse_iterator end =
         that.big_value->rend();
   while (itor != end){
      out << int(*itor); 
      ++itor;
   }
   //out << that.small_value;
   return out;
}

void bigint::no_leading_zero(bigint &that) const{
   bigint::bigvalue_t::const_reverse_iterator itor =
          that.big_value->rbegin();
   bigint::bigvalue_t::const_reverse_iterator end = 
          that.big_value->rend();
   while (itor != end && *itor ==  0){
      that.big_value->pop_back();
      ++itor;     
   } 
}



