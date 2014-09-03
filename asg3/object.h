// $Id: object.h,v 1.4 2012-04-26 16:43:23-07 - - $

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <iomanip>
#include <iostream>
#include <list>
#include <utility>

using namespace std;

#include "numbers.h"

//
// Objects constitute a single-inheritance hierarchy, summarized
// here, with the superclass listed first, and subclasses indented
// under their immediate superclass.
//
// object
//    text
//    shape
//       ellipse
//          circle
//       polygon
//          rectangle
//             square
//          line
//

typedef list<xycoords> coordlist;

//
// Abstract base class for all shapes in this system.
//

class object {
   private:
      object (const object &); // Disable.
      object &operator= (const object &); // Disable.
   protected:
      object () {} // Only subclass may instantiate.
   public:
      virtual ~object ();
      virtual void draw (ostream &, const xycoords &,
                         const degrees &angle) = 0;
};


//
// Class for printing text.
//

class text: public object {
   protected:
      string fontname;
      points fontsize;
      string textdata;
   public:
      text (const string &fontname, const points &fontsize,
            const string &textdata);
      virtual void draw (ostream &, const xycoords &,
                         const degrees &angle);
};

//
// Shape of a geometric object.
//

class shape: public object {
   protected:
      points thick;
      shape (): thick (points()) {}
      shape (const points &thick): thick(thick) {}
};

//
// Classes for ellipse and circle.
//

class ellipse: public shape {
   protected:
      inches height;
      inches width;
   public:
      ellipse (const inches &height, const inches &width,
               const points &thick);
      virtual void draw (ostream &, const xycoords &,
                         const degrees &angle);
};

class circle: public ellipse {
   public:
      circle (const inches &diameter, const points &thick);
};


//
// Class polygon.
//

class polygon: public shape {
   protected:
      const coordlist coordinates;
   public:
      polygon (const coordlist &coords, const points &thick);
      virtual void draw (ostream &, const xycoords &,
                         const degrees &angle);
};

//
// Classes rectangle, square, and line..
//

class rectangle: public polygon {
   private:
      static coordlist make_list (
             const inches &height, const inches &width);
   public:
      rectangle (const inches &height, const inches &width,
                 const points &thick);
};

class square: public rectangle {
   public:
      square (const inches &width, const points &thick);
};

class line: public polygon {
   private:
      static coordlist make_list (const inches &length);
   public:
      line (const inches &length, const points &thick);
};

#endif


