// $Id: util.cpp,v 1.2 2013-02-12 15:05:06-08 - - $

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

using namespace std;

#include "util.h"

int sys_info::exit_status = EXIT_SUCCESS;
string *sys_info::execname = NULL; // Must be initialized from main().

void sys_info_error (const string &condition) {
   throw logic_error ("main() has " + condition
               + " called sys_info::set_execname()");
}

void sys_info::set_execname (const string &argv0) {
   if (execname != NULL) sys_info_error ("already");
   int slashpos = argv0.find_last_of ('/') + 1;
   execname = new string (argv0.substr (slashpos));
   cout << boolalpha;
   cerr << boolalpha;
   TRACE ('u', "execname = " << execname);
}

const string &sys_info::get_execname () {
   if (execname == NULL) sys_info_error ("not yet");
   return *execname;
}

void sys_info::set_exit_status (int status) {
   if (execname == NULL) sys_info_error ("not yet");
   exit_status = status;
}

int sys_info::get_exit_status () {
   if (execname == NULL) sys_info_error ("not yet");
   return exit_status;
}

const string datestring () {
   time_t clock = time (NULL);
   struct tm *tm_ptr = localtime (&clock);
   char timebuf[256];
   strftime (timebuf, sizeof timebuf,
             "%a %b %e %H:%M:%S %Z %Y", tm_ptr);
   return timebuf;
}


list<string> split (const string &line, const string &delimiters) {
   list<string> words;
   size_t end = 0;
   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output list<string>.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   TRACE ('u', words);
   return words;
}

ostream &complain() {
   sys_info::set_exit_status (EXIT_FAILURE);
   cerr << sys_info::get_execname () << ": ";
   return cerr;
}

void syscall_error (const string &object) {
   complain() << object << ": " << strerror (errno) << endl;
}

template <typename item_t>
ostream &operator<< (ostream &out, const list<item_t> &vec) {
   typename list<item_t>::const_iterator itor = vec.begin();
   typename list<item_t>::const_iterator end = vec.end();
   // If the list is empty, do nothing.
   if (itor != end) {
      // Print out the first element without a space.
      out << *itor++;
      // Print out the rest of the elements each preceded by a space.
      while (itor != end) out << " " << *itor++;
   }
   return out;
}

template <typename type_t>
string to_string (const type_t &that) {
   ostringstream stream;
   stream << that;
   return stream.str ();
}

template <typename type_t>
type_t from_string (const string &that) {
   stringstream stream;
   stream << that;
   type_t result;
   if ( !(stream >> result   // Can we read type from string?
       && stream >> std::ws  // Flush trailing white space.
       && stream.eof ())     // Must now be at end of stream.
   ) {
      throw domain_error (string (typeid (type_t).name ())
            + " from_string (" + that + ")");
   }
   return result;
}

#include "templates.tcc"


