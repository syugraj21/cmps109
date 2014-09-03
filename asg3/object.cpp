// $Id: object.cpp,v 1.15 2013-02-18 15:48:12-08 - - $

#include <typeinfo>

using namespace std;

#include "object.h"
#include "util.h"

#define WHOAMI "[" << typeid(*this).name() << "@" \
               << reinterpret_cast<const void *const> (this) << "]"

#define CTRACE(ARGS) TRACE ('c', WHOAMI << " " << ARGS)

#define DTRACE(ARGS) TRACE ('d', WHOAMI << " coords=" << coords \
                     << " angle=" << angle << endl << ARGS);

object::~object () {
   CTRACE ("delete");
}

text::text (const string &font, const points &size, const string &data):
      fontname(font), fontsize(size), textdata(data) {
   CTRACE ("font=" << fontname << " size=" << fontsize
           << " \"" << textdata << "\"")
}

ellipse::ellipse (const inches &initheight, const inches &initwidth,
                  const points &initthick):
      shape(initthick), height(initheight), width(initwidth) {
   CTRACE ("height=" << height << " width=" << width
           << " thick=" << thick);
}

circle::circle (const inches &diameter, const points &thick):
      ellipse (diameter, diameter, thick) {
}

polygon::polygon (const coordlist &coords, const points &initthick):
      shape(initthick), coordinates(coords) {
   CTRACE ( "thick=" << thick << " coords=" << endl
            << coordinates);
}

rectangle::rectangle (const inches &height, const inches &width,
                      const points &initthick):
      polygon (make_list (height, width), initthick) {
}

square::square (const inches &width, const points &thick):
      rectangle (width, width, thick) {
}

line::line (const inches &length, const points &initthick):
      polygon (make_list (length), initthick) {
      
}


void text::draw (ostream &out, const xycoords &coords,
                 const degrees &angle) {
   DTRACE ("font=" << fontname << " size=" << fontsize
           << " \"" << textdata << "\"");
   double font_size = double(fontsize);
   double x = double(coords.first);
   double y = double(coords.second);
   double ang = double(angle);
   
   out << "gsave\n   " << "/" << fontname <<" findfont\n   "
       << font_size << " scalefont setfont\n   "
       << x << " " << y << " translate\n   " << ang
       << " rotate\n   " << "0 0 moveto\n   "
       << "(" << textdata << ")\n   " <<"show\n"
       << "grestore" << endl;
}

void ellipse::draw (ostream &out, const xycoords &coords,
                    const degrees &angle) {
   DTRACE ("height=" << height << " width=" << width
           << " thick=" << thick);
   double hght = double(height);
   double wdth = double(width);
   double thickness = double(thick);
   double ang = double(angle);
   double x = double(coords.first);
   double y = double(coords.second);
   double xscale, yscale, radius;

   if (hght < wdth){
     xscale = 1;
     yscale = hght/wdth;
     radius = wdth/2;
   } else {
     xscale = wdth/hght;
     yscale = 1;
     radius = hght/2;
   } 
   out << "gsave\n" <<"   newpath\n"
       <<"   " <<"/save matrix currentmatrix def\n"
       <<"   " << x << " " << y << " translate\n"
       <<"   " << ang << " rotate\n" 
       <<"   " << xscale << " " << yscale << " scale\n" 
       <<"   " << "0 0 " << radius <<"  0 360 arc\n"
       <<"   " <<"save setmatrix\n" 
       <<"   " << thickness << " setlinewidth\n"
       <<"   stroke\n" << "grestore" << endl;
}

void polygon::draw (ostream &out, const xycoords &coords,
                    const degrees &angle) {
   DTRACE ( "thick=" << thick << " coords=" << endl
            << coordinates); 
   double ang = double(angle);
   double thickness = double(thick);
   double x = double(coords.first);
   double y = double(coords.second);

   out << "gsave\n   " << " newpath\n   "
       << x << " " << y << " translate\n   " << ang << " rotate\n   "
       << "0 0 moveto\n   ";
   
   coordlist::const_iterator it = coordinates.begin();
   coordlist::const_iterator end = coordinates.end();
   for (; it != end; ++it){
      double cx = double(it->first);
      double cy = double(it->second);
      out << cx << "  " << cy << " rlineto\n   "; 
   }

   out << "closepath\n   " << thickness << " setlinewidth\n   "
       << "stroke\n" << "grestore" << endl;
}

coordlist rectangle::make_list (const inches &height,
                                const inches &width) {
   coordlist coordlist;
   double hght = double(height);
   //double wdth = double(width); 
   inches temp = inches(-hght/PTS_PER_INCH);
   coordlist.push_back(xycoords(height,inches(0)));
   coordlist.push_back(xycoords(inches(0),width));
   coordlist.push_back(xycoords(temp, inches(0)));
   return coordlist;
}

coordlist line::make_list (const inches &length) {
   coordlist coordlist;
   coordlist.push_back(xycoords(inches(0), length));
   return coordlist;
}


