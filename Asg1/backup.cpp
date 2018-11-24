// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that): ubig_value() {
   //DEBUGF ('~', this << " -> " << ubig_value)
   
}

ubigint::ubigint (const string& that): ubig_value(0) {
   DEBUGF ('~', "that = \"" << that << "\"");
   std :: vector<udigit_t>::reverse_iterator rit = ubig_value.rbegin();
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
      *rit = digit - '0';
      ++rit; 
   }
}

ubigint ubigint::operator+ (const ubigint& that) const {
   bool remainder = false;
   int sum = 0;
   int rem = 0;
   auto vec = ubigint();
   std :: vector<udigit_t>:: const_reverse_iterator rit1 = ubig_value.crbegin(); 
   std :: vector<udigit_t>:: const_reverse_iterator rit2 = that.ubig_value.crbegin();
   
   while (rit1 != ubig_value.crend() || rit2 != that.ubig_value.crend())
   {
       if (remainder == false)
       {
           sum = *rit1 + *rit2;
           if (sum >= 10)
       {
           remainder = true;
           rem = sum % 10;
               vec.ubig_value.push_back(rem);
       }
       else 
       {
          remainder = false;
          vec.ubig_value.push_back(sum);
       }
       }
       else 
       {
          sum = *rit1 + *rit2 + 1;
          if (sum >= 10)
      {
         remainder = true;
             rem = sum % 10;
             vec.ubig_value.push_back(rem);
      }
       }
       ++rit1;
       ++rit2;
   }

   if (rit1 != ubig_value.crend() && rit2 == that.ubig_value.crend()) //if this vector is greater than that vector
   {
   } 
   //if size of vector1 is greater equal than size of vector2 take the size of vector1
   //else take size of vector2
   //for i = 0; i < size of biggest vector; ++i)
      //if there are still values in the smaller vector
         //if remainder is false
           //sum = *rit1 + *rit2
           //if sum >= 10 
              //have a bool called remainder be set to true 
              //rem = sum % 10
              //vec.push_back(rem)
           //else
              //remainder = false
              //vec.push_back(sum)

         //else (if remainder is true)
            //sum = *rit1 + *rit2 + 1
            //if sum>= 10
               //remainder = true
               //rem = sum % 10
               //vec.push_back(rem)
            //else
                //remainder = false
                //vec.push_back(sum)
         //++rit1
         //++rit2

      //else (if there are no more values in the smaller vector)
         //sum = *bigger vector + 0  
         //vec.push_back(sum)
         //++bigger vector (iterator)


   return vec;
}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   auto vec = ubigint();
   
   //if size of vector1 is greater equal than size of vector2 take the size of vector1
   //else take size of vector2
   //for i = 0; i < size of biggest vector; ++i)
      // 
   return vec;
}

ubigint ubigint::operator* (const ubigint& that) const {
   return ubigint (uvalue * that.uvalue);
}

void ubigint::multiply_by_2() {
   uvalue *= 2;
}

void ubigint::divide_by_2() {
   uvalue /= 2;
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
   // Note: divisor is modified so pass by value (copy).
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
   return uvalue == that.uvalue;
}

bool ubigint::operator< (const ubigint& that) const {
   return uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   return out << "ubigint(" << that.uvalue << ")";
}

