// $Id: main.cpp,v 1.1 2013-02-12 14:51:41-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

void scan_options (int argc, char **argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}
//removes white space
void trim(string &str, string &delim){
   size_t start = str.find_first_not_of(delim);
   size_t end = str.find_last_not_of(delim);
   if (start == string::npos || end == string::npos){
      str = "";
   } else {
      str = str.substr(start,end-start+1);
   }
}

void parsefile(const string &infilename, istream &infile){
   listmap <string,string> lexio_list;
   for (int linenr = 1;; ++linenr){
      try {
         string line;
	 getline (infile,line);
	 if (infile.eof()) break;
	 if (line.size() == 0) {
	    cout << infilename <<": "<< linenr <<": " << ""<<endl;
	    continue;
	 }
	 for(;;){
            int last = line.size() - 1;
	    if (line[last] != '\\') break;
	    line[last] = ' ';
	    string contin;
	    getline (infile, contin);
	    if (infile.eof()) break;
	    line = line + contin;
	 }

         trim (line," \t");
	 cout << infilename <<": "<<linenr <<": " <<line<<endl;
	 if (line.size() == 0 || line.at(0) == '#'){
            continue;
	 }
	 if (line == "="){
            listmap<string,string>::iterator itor = lexio_list.begin();
	    listmap<string,string>::iterator end = lexio_list.end();
	    while (itor != end){
               cout << itor->first << " = "<< itor->second<<endl;
	       ++itor;
	    }
	    continue;
	 }
	 if (line == "/="){
            listmap<string,string>iterator itor = lexio_list.rbegin();
	    listmap<string,string>iterator end = lexio_list.rend();
	    while (itor != end){
               cout << itor->first << " = " << itor->second << endl;
	       --itor;
	    }
	    continue;
	 }
	 list<string> words = split(line,"=");

      } catch (runtime_error error){
         complain() << infilename << ":" << linenr <<": "
	            << error.what() << endl;
      }
   }
}

int main (int argc, char **argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   if (optind == argc){
      parsefile("-",cin);
   } else { 
      for (int argi = optind; argi < argc; ++argi){
         const string input = argv[argi];
	 ifstream infile (input.c_str());
	 if (infile.fail()){
            syscall_error(infile);
	    continue;
	 }
	 parsefile(input,infile);
	 infile.close();
      }

   }
   /*
   listmap <string, string> test;
   for (int argi = 0; argi < argc; ++argi) {
      xpair <string, string> pair (argv[argi], to_string <int> (argi));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   listmap <string, string>::iterator itor = test.begin();
   listmap <string, string>::iterator end = test.end();
   for (; itor != end; ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   cout << "EXIT_SUCCESS" << endl;*/
   return sys_info::get_exit_status();;
}


