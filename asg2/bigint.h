// $Id: bigint.h,v 1.2 2012-04-10 21:02:20-07 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <exception>
#include <iostream>
#include <utility>

using namespace std;

#include "trace.h"

class bigint;
typedef pair <bigint, bigint> bigpair;

//
// Operators with a left side of int.
//
bigint operator+ (int left, const bigint &that);
bigint operator- (int left, const bigint &that);
bigint operator* (int left, const bigint &that);
bigint operator/ (int left, const bigint &that);
bigint operator% (int left, const bigint &that);
bool operator== (int left, const bigint &that);
bool operator!= (int left, const bigint &that);
bool operator<  (int left, const bigint &that);
bool operator<= (int left, const bigint &that);
bool operator>  (int left, const bigint &that);
bool operator>= (int left, const bigint &that);


//
// Define class bigint
//
class bigint {
      friend ostream &operator<< (ostream &, const bigint &);
   private:
      int small_value;
      typedef unsigned char digit_t;
      typedef vector <digit_t> bigvalue_t;
      bool negative;
      bigvalue_t *big_value;
      void insert_bigvalue(const string &s);
      bigpair div_rem (const bigint &that) const;
      void set_neg_value(const bool &that);
      void do_bigadd(const bigvalue_t *this_val,const 
          bigvalue_t *that_val, bigvalue_t *new_val) const;
      void do_bigsub(const bigvalue_t *big_val,const
          bigvalue_t *small_val, bigvalue_t *new_val) const;
      int compare (const bigint &that) const;
      int compare_unequal_size(const bigint &that) const;
      int compare_equal_size(const bigint &that) const;
      int abscompare (const bigint &that) const;
      void no_leading_zero(bigint &that) const; 
      bigint mul_by_2 ();
      
   public:
      //
      // Override implicit members.
      //
      bigint ();
      bigint (const bigint &that);
      bigint &operator= (const bigint &that);
      ~bigint ();
      //
      // Extra ctors to make bigints.
      //
      bigint (const int that);
      bigint (const string &that);
      //
      // Basic add/sub operators.
      //
      bigint operator+ (const bigint &that) const;
      bigint operator- (const bigint &that) const;
      bigint operator- () const;
      int smallint () const;
      //
      // Extended operators implemented with add/sub.
      //
      bigint operator* (const bigint &that) const;
      bigint operator/ (const bigint &that) const;
      bigint operator% (const bigint &that) const;
      bigint pow (const bigint &that) const;
      //
      // Comparison operators.
      //
      bool operator== (const bigint &that) const;
      bool operator!= (const bigint &that) const;
      bool operator<  (const bigint &that) const;
      bool operator<= (const bigint &that) const;
      bool operator>  (const bigint &that) const;
      bool operator>= (const bigint &that) const;
};

#endif


