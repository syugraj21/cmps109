// $Id: interp.cpp,v 1.44 2013-02-18 14:41:52-08 - - $

#include <list>
#include <map>
#include <string>
#include <stdlib.h>

using namespace std;

#include "interp.h"
#include "object.h"
#include "util.h"

interpreter::interpreter(const string &filename, ostream &outfile,
                         objectmap &objmap):
   outfile(outfile), pagenr(1), objmap(objmap), infilename(filename),
   page_xoffset (inches (.25)), page_yoffset (inches (.25)) {
   if (interpmap.size() == 0) {
      interpmap["define" ] = &interpreter::do_define ;
      interpmap["draw"   ] = &interpreter::do_draw   ;
      interpmap["newpage"] = &interpreter::do_newpage;
   }
   if (factorymap.size() == 0) {
      factorymap["text"     ] = &interpreter::make_text     ;
      factorymap["ellipse"  ] = &interpreter::make_ellipse  ;
      factorymap["circle"   ] = &interpreter::make_circle   ;
      factorymap["polygon"  ] = &interpreter::make_polygon  ;
      factorymap["rectangle"] = &interpreter::make_rectangle;
      factorymap["square"   ] = &interpreter::make_square   ;
      factorymap["line"     ] = &interpreter::make_line     ;
   }
   prolog ();
   startpage ();
}

interpreter::~interpreter () {
   endpage ();
   epilog ();
   objectmap::iterator it = objmap.begin();
   objectmap::const_iterator end = objmap.end();
   while(it != end){
      delete (*it).second;
      ++it;
   }
}

map <string, interpreter::interpreterfn> interpreter::interpmap;
map <string, interpreter::factoryfn> interpreter::factorymap;

string shift (list<string> &words) {
   if (words.size() == 0) throw runtime_error ("syntax error");
   string front = words.front();
   words.pop_front();
   return front;
}

void interpreter::interpret (parameters &params) {
   TRACE ('i', params);
   string command = shift (params);
   interpreterfn function = interpmap[command];
   if (function == NULL) throw runtime_error ("syntax error");
   (this->*function) (params);
}


void interpreter::do_define (parameters &params) {
   TRACE ('i', params);
   string name = shift (params);
   objmap[name] = make_object (params);
}

void interpreter::do_draw (parameters &params) {
   TRACE ('i', params);
   string name = shift (params);
   object *thing = objmap[name];
   string temp;
   if (thing == NULL) throw runtime_error (name + ": no such object");
   degrees angle = degrees (0);
   if (params.size() == 3) {
      temp = params.back();
      if (is_a_num(temp)){
         angle = degrees (from_string<double> (params.back()));
         params.pop_back();
      } else { 
         throw runtime_error("syntax error: invalid parameters");
      }
   }
   if (params.size() != 2) throw runtime_error ("syntax error");
   temp = params.back();
   name = params.front();
   if (!is_a_num(temp)){
      throw runtime_error("syntax error: invalid parameters");
   }
   if (!is_a_num(name)){
      throw runtime_error("syntax error: invalid parameters");
   }
   xycoords coords (inches (from_string<double> (params.front())),
                    inches (from_string<double> (params.back())));
   thing->draw (outfile, coords, angle);
}

void interpreter::do_newpage (parameters &params) {
   if (params.size() != 0) throw runtime_error ("syntax error");
   endpage ();
   ++pagenr;
   startpage ();
}


void interpreter::prolog () {
   outfile << "%!PS-Adobe-3.0" << endl;
   outfile << "%%Creator: " << sys_info::get_execname () << endl;
   outfile << "%%CreationDate: " << datestring() << endl;
   outfile << "%%PageOrder: Ascend" << endl;
   outfile << "%%Orientation: Portrait" << endl;
   outfile << "%%SourceFile: " << infilename << endl;
   outfile << "%%EndComments" << endl;
}

void interpreter::startpage () {
   outfile << endl;
   outfile << "%%Page: " << pagenr << " " << pagenr << endl;
   outfile << page_xoffset << " " << page_yoffset
           << " translate" << endl;
   outfile << "/Courier findfont 10 scalefont setfont" << endl;
   outfile << "0 0 moveto (" << infilename << ":"
           << pagenr << ") show" << endl;

}

void interpreter::endpage () {
   outfile << endl;
   outfile << "showpage" << endl;
   outfile << "grestoreall" << endl;
}

void interpreter::epilog () {
   outfile << endl;
   outfile << "%%Trailer" << endl;
   outfile << "%%Pages: " << pagenr << endl;
   outfile << "%%EOF" << endl;

}


object *interpreter::make_object (parameters &command) {
   TRACE ('f', command);
   string type = shift (command);
   factoryfn func = factorymap[type];
   if (func == NULL) throw runtime_error (type + ": no such object");
   return (this->*func) (command);
}

object *interpreter::make_text (parameters &command) {
   TRACE ('f', command);
   if (command.size() < 2){
      throw runtime_error("syntax error: invalid parameters for text");
   }

   string temp = shift(command);
   double font_size;
   string font = "";
   string text_data = "";
   //size_t found;
   
   //check to see if the first string is comprised of digits
   //found = temp.find_first_not_of("0123456789.");

   if (is_a_num(temp)){
      font_size = from_string<double>(temp); 
      font = shift(command);
   } else {
      font_size = 12;
      font = temp;
   }
   /*
   parameters::const_iterator begin = command.begin();
   parameters::const_iterator end = command.end();
   */
   while (!command.empty()){
      text_data += shift(command);
   }
   string::iterator itor = text_data.begin();
   string::const_iterator end = text_data.end();

   while (itor != end){
      if ( *itor == '(' || *itor == ')' || *itor == '\\'){  
         text_data.insert(itor,'\\');
          ++itor;
      }
      ++itor;
   }
   return new text (font, points(font_size),text_data);
}

object *interpreter::make_ellipse (parameters &command) {
   TRACE ('f', command);
   if (command.size() < 2 || command.size() > 3){
      throw runtime_error("invalid number parameters for ellipse");
   }
   double height;
   double width;
   double thick = 2;
   string temp;

   temp = shift(command);
   if (is_a_num(temp)){
      height = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid parameters");
   }
   
   temp = shift(command);
   if (is_a_num(temp)){
      width = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid paramters");
   }
      
   if (!command.empty()){
      temp = shift(command);
      if (is_a_num(temp)){
         thick = from_string<double>(temp);
      } else {
         throw runtime_error("syntax error:invalid paramters");
      }
   }
   return new ellipse (inches(height), inches(width), points(thick));
}

object *interpreter::make_circle (parameters &command) {
   TRACE ('f', command);
   if (command.size() < 1 || command.size() > 2){
      throw runtime_error("invalid number of parameters");
   } 
   double dia;
   double thick = 2;
   string temp;
   temp = shift(command);
   if (is_a_num(temp)){
      dia = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid parameters");
   }
   if (!command.empty()){
      temp = shift(command);
      if (is_a_num(temp)){
         thick = from_string<double>(temp);
      } else {
         throw runtime_error("syntax error: invalid parameters");
      }
   }
   return new circle (inches(dia), points(thick));
}

object *interpreter::make_polygon (parameters &command) {
   TRACE ('f', command);
   if (command.size() <= 1){
       throw runtime_error("invalid number of parameters");
   }
   double thick = 2;
   coordlist coord;
   string temp;
   //get the thickness
   if ((command.size() % 2) != 0){
      temp = command.back();
      if(is_a_num(temp)){
         thick = from_string<double>(temp);
      } else {
         throw runtime_error("syntax error: invalid parameters");
      }
      command.pop_back();
   }

   while (!command.empty()){
      string x = shift(command);
      string y = shift(command);
      if (is_a_num(x) && is_a_num(y)){
         xycoords xy_pair = xycoords(inches(from_string<double>(x)),
         inches(from_string<double>(y)));
         coord.push_back(xy_pair); 
      } else {
         throw runtime_error("syntax error: invalid parameters");
      }
      
   }
   return new polygon (coord, points(thick));
}

object *interpreter::make_rectangle (parameters &command) {
   TRACE ('f', command);
   if (command.size() < 2 || command.size() > 3){
      throw runtime_error("invalid number of parameters");
   }
   double height;
   double width;
   double thick = 2;
   string temp;
   
   temp = shift(command);
   if(is_a_num(temp)){
      height = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid parameters");
   }

   temp = shift(command);
   if(is_a_num(temp)){
      width = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid parameters");
   }
   if(!command.empty()){
      temp = shift(command);
      if(is_a_num(temp)){
         thick = from_string<double>(temp);
      } else {
         throw runtime_error("syntax error: invalid parameters");
      }
   }
   return new rectangle (inches(height), inches(width), points(thick));
}

object *interpreter::make_square (parameters &command) {
   TRACE ('f', command);
   if (command.size() < 1 || command.size() > 2){
      throw runtime_error("invalid number of parameters");
   }
   double width;
   double thick = 2;
   string temp;

   temp = shift(command);
   if (is_a_num(temp)){
      width = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid parameters");
   }
   if (!command.empty()){
      temp = shift(command);
      if (is_a_num(temp)){
         thick = from_string<double>(temp);
      } else {
         throw runtime_error("syntax error: invalid parameters");
      }
   }
   return new square (inches(width), points(thick));
}

object *interpreter::make_line (parameters &command) {
   TRACE ('f', command);
   if (command.size() < 1 || command.size() > 2){
      throw runtime_error("invalid number of parameters");
   }
   double length;
   double thick = 2;
   string temp;
   
   temp = shift(command);
   if (is_a_num(temp)){
      length = from_string<double>(temp);
   } else {
      throw runtime_error("syntax error: invalid parameters");
   }
   if (!command.empty()){
      temp = shift(command);
      if(is_a_num(temp)){
         thick = from_string<double>(temp);
      } else {
         throw runtime_error("syntax error: invalid parameters");
      }
   }
   return new line (inches(length), points(thick));
}

bool interpreter::is_a_num(const string &input){
   double temp;
   try {
     temp = from_string<double>(input);
     return true;
   } catch (domain_error) {
     return false;
   }
   /*string::const_iterator itor = input.begin();
   string::const_iterator end = input.end();
   while (itor != end && isdigit(*itor)){
      ++itor;
   }
   return itor == end && !input.empty();*/
}


