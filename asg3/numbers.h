// $Id: numbers.h,v 1.3 2012-04-26 12:44:05-07 - - $

#ifndef __NUMBERS_H__
#define __NUMBERS_H__

#include <iostream>
#include <utility>

using namespace std;

#include "trace.h"

const double PTS_PER_INCH = 72;

class degrees {
      friend ostream &operator<< (ostream &, const degrees &);
   private:
      double angle;
   public:
      degrees (): angle(double()) {}
      explicit degrees (double init): angle(init) {}
      operator double() const {return angle; }
};

class points {
      friend ostream &operator<< (ostream &, const points &);
   private:
      double pointvalue;
   public:
      points (): pointvalue(double()) {}
      explicit points (double init): pointvalue(init) {}
      operator double() const {return pointvalue; }
};

class inches {
      friend ostream &operator<< (ostream &, const inches &);
   private:
      double pointvalue;
   public:
      inches (): pointvalue (double() * PTS_PER_INCH) {}
      explicit inches (double init): pointvalue(init * PTS_PER_INCH) {}
      operator double() const {return pointvalue; }
};

typedef pair <inches, inches> xycoords;

ostream &operator<< (ostream &, const xycoords &);

#endif
