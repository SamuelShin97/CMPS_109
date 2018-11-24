// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <vector> 
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that): ubig_value() {
   //DEBUGF ('~', this << " -> " << ubig_value)
   that = (that); 
}

ubigint::ubigint (const string& that) {
   DEBUGF ('~', "that = \"" << that << "\"");
   //ubigvalue_t::reverse_iterator rit = ubig_value.rbegin();
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
      ubig_value.insert(ubig_value.begin(), digit - '0');
   }
}

ubigint ubigint::operator+ (const ubigint& that) const {
   bool remainder = false;
   int sum = 0;
   int rem = 0;
   auto vec = ubigint();
   vector<udigit_t>:: const_iterator rit1 = ubig_value.begin(); 
   vector<udigit_t>:: const_iterator rit2 = that.ubig_value.begin();
   
   while (rit1 != ubig_value.end() && rit2 != that.ubig_value.end())
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
            (vec.ubig_value).push_back(sum);
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
         else 
         {
            remainder = false;
            vec.ubig_value.push_back(sum);
         }
      }
      ++rit1;
      ++rit2;
   }

   if (rit1 != ubig_value.cend() && rit2 == that.ubig_value.cend())
   //if this vector is greater than that vector
   {
      while (rit1 != ubig_value.cend())
      {
         if (remainder == true)
         {
            sum = *rit1 + 1;
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
            sum = *rit1 + 0;
            vec.ubig_value.push_back(sum);
         }
         ++rit1;
      }
      if (remainder == true)
      {
         vec.ubig_value.push_back(1);
      }
   } 
   else if (rit1 == ubig_value.cend() && rit2 != that.ubig_value.cend())
   //if that vector is larger than this vector
   {
      while (rit2 != that.ubig_value.cend())
      {
         if (remainder == true)
         {
            sum = *rit2 + 1;
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
            sum = *rit2 + 0;
            vec.ubig_value.push_back(sum);
         }
         ++rit2;
      } 
      if (remainder == true)
      {
         vec.ubig_value.push_back(1);
      }
   }
   else //if the vectors are the same size
   {
      if (remainder == true)
      {
         vec.ubig_value.push_back(1);
      }
   } 
   
   return vec;
}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   auto vec = ubigint();
   bool remainder = false;
   int sum = 0;
   vector<udigit_t>:: const_iterator rit1 = ubig_value.begin();
   vector<udigit_t>:: const_iterator rit2 = that.ubig_value.begin();

   while (rit1 != ubig_value.end() && rit2 != that.ubig_value.end())
   {
      if (remainder == false)
      {
         sum = *rit1 - *rit2;
         if (sum < 0)
         {
            remainder = true;
            sum = *rit1 - *rit2 + 10;
            vec.ubig_value.push_back(sum);
         }
         else
         {
            remainder = false;
            (vec.ubig_value).push_back(sum);
         }
      }
      else
      { 
         sum = *rit1 - *rit2 - 1;
         if (sum < 0)
         {
            remainder = true;
            sum = *rit1 - *rit2 + 10;
            vec.ubig_value.push_back(sum);
         }
         else
         {
            remainder = false;
            vec.ubig_value.push_back(sum);
         }
      }
      ++rit1;
      ++rit2;
   }

   if (rit1 != ubig_value.cend() && rit2 == that.ubig_value.cend())
    //if this vector is greater than that vector
   {
      while (rit1 != ubig_value.cend())
      {
         if (remainder == true)
         {
            sum = *rit1 - 1;
            if (sum < 0)
            {
               remainder = true;
               sum = *rit1 - *rit2 + 10;
               vec.ubig_value.push_back(sum);
            }
            else 
            {
               vec.ubig_value.push_back(sum);
               remainder = false;
            }
         }
         else
         {
            sum = *rit1 - 0;
            vec.ubig_value.push_back(sum);
         }
         ++rit1;
      }
   }
   else if (rit1 == ubig_value.cend() && rit2 != that.ubig_value.cend())
    //if that vector is larger than this vector
   {
      while (rit2 != that.ubig_value.cend())
      {
         if (remainder == true)
         {
            sum = *rit2 - 1;
            if (sum < 0)
            {
               remainder = true;
               sum = *rit1 - *rit2 + 10;
               vec.ubig_value.push_back(sum);
            }
            else
            {
               vec.ubig_value.push_back(sum);
               remainder = false;
            }
         }
         else
         {
            sum = *rit2 - 0;
            vec.ubig_value.push_back(sum);
         }
         ++rit2;
      }
   }
   else //if the vectors are the same size
   {
      /*if (remainder == true)
      {
         vec.ubig_value.push_back(1);
      }*/
   }
   return vec.trim();
}

ubigint ubigint::operator* (const ubigint& that) const {
   //return ubigint (uvalue * that.uvalue);
   int size1 = ubig_value.size();
   int size2 = that.ubig_value.size();
   int carry = 0;
   int digit = 0;
   auto vec = ubigint();
   
   for (int a = 0; a < size1 + size2; ++a)
   {
       vec.ubig_value.push_back(0);
   }
   
   for (int i = 0; i < size1; ++i)
   {
      carry = 0;
      for (int j = 0; j < size2; ++j)
      {
         digit = vec.ubig_value[i + j];
         digit += (ubig_value[i] * that.ubig_value[j]);
         digit += carry;
         vec.ubig_value[i + j] = digit % 10;
         carry = digit / 10;
      }
      vec.ubig_value[i + size2] = carry;
   }
   return vec.trim();
}

void ubigint::multiply_by_2() {
   //uvalue *= 2;
   auto vec = ubigint();
   for (unsigned int i = 0; i < ubig_value.size(); ++i)
   {
      vec.ubig_value.insert(vec.ubig_value.begin(), 2);
   }
   *this = *this * (vec);
}

void ubigint::divide_by_2() {
   int div = 0;
   int mod = 0;

   for (unsigned int i = 0; i < ubig_value.size(); ++i)
   {
      if (i == ubig_value.size() - 1)
      {
         ubig_value[i] = ubig_value[i] / 2;
      }
      else 
      {
         mod = ubig_value[i + 1] % 2;
         div = ubig_value[i] / 2;
         if (mod == 0)
         {
            ubig_value[i] = div;
         }
         else 
         {
            ubig_value[i] = div + 5;
         }
      }
   }
 
   (*this).trim();
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
   // Note: divisor is modified so pass by value (copy).
   ubigint zero {"0"};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {"1"};
   ubigint quotient {"0"};
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
   //return udivide (*this, that).quotient;
   auto quo = udivide(*this, that);
   return quo.quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   //return udivide (*this, that).remainder;
   auto rem = udivide(*this, that);
   return rem.remainder;
}

bool ubigint::operator== (const ubigint& that) const {
   //return uvalue == that.uvalue;
   if (ubig_value.size() != that.ubig_value.size())
   {
      return false;
   }
   else 
   {
      for (unsigned int i = 0; i < ubig_value.size(); ++i)
      {
         if (ubig_value[i] != that.ubig_value[i])
         {
            return false;
         }
      }
      return true;
   }
   return false;
}

bool ubigint::operator< (const ubigint& that) const {
   //return uvalue < that.uvalue;
   if (ubig_value.size() < that.ubig_value.size())
   {
      return true;
   }
   else if (ubig_value.size() > that.ubig_value.size())
   {
      return false;
   }
   else
   {
      ubigvalue_t::const_reverse_iterator rit1 = ubig_value.rbegin();
      ubigvalue_t::const_reverse_iterator r =(that.ubig_value).rbegin();
      for (unsigned int i = 0; i < ubig_value.size(); ++i)
      {
         if (*rit1 < *r)
         {
            return true;
         }
         ++rit1;
         ++r;
      }
      return false;
   }
   return false; 
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   //return out << "ubigint(" << that.ubig_value << ")";
   vector<unsigned char>::const_reverse_iterator rit = 
   (that.ubig_value).rbegin();
   while (rit != (that.ubig_value).rend())
   {
      out << static_cast<int>(*rit);
      ++rit;
   }
   return out;
}

ubigint ubigint::trim()
{
   while (ubig_value.size() > 0 && ubig_value.back() == 0)
   {
      ubig_value.pop_back();
   } 
   return *this;
}

