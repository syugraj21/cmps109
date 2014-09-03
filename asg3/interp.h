// $Id: interp.h,v 1.2 2012-04-25 20:33:59-07 - - $

#ifndef __INTERP_H__
#define __INTERP_H__

#include <iostream>
#include <map>

using namespace std;

#include "object.h"
#include "trace.h"

typedef map <string, object *> objectmap;

class interpreter {
   private:
      interpreter (); // DISABLE
      interpreter (const interpreter &); // DISABLE
      interpreter &operator= (const interpreter &); // DISABLE

      // Data fields.
      typedef list<string> parameters;
      typedef void (interpreter::*interpreterfn) (parameters &);
      typedef object *(interpreter::*factoryfn) (parameters &);
      static map <string, interpreterfn> interpmap;
      static map <string, factoryfn> factorymap;
      ostream &outfile;
      int pagenr;
      objectmap objmap;
      string infilename;
      double page_xoffset;
      double page_yoffset;

      // Service functions.
      void do_define (parameters &);
      void do_draw (parameters &);
      void do_newpage (parameters &);
      void prolog ();
      void startpage ();
      void endpage ();
      void epilog ();

      // Factory functions.
      object *make_object (parameters &);
      object *make_text (parameters &);
      object *make_ellipse (parameters &);
      object *make_circle (parameters &);
      object *make_polygon (parameters &);
      object *make_rectangle (parameters &);
      object *make_square (parameters &);
      object *make_line (parameters &);

      //private functions
      bool is_a_num(const string &input);

   public:
      interpreter (const string &, ostream &, objectmap &);
      ~interpreter ();
      void interpret (parameters &);
};

#endif


