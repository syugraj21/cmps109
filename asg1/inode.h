// $Id: inode.h,v 1.5 2013-01-02 19:13:46-08 - - $

#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#include "trace.h"
#include "util.h"

//
// inode_t -
//    An inode is either a directory or a plain file.
//

enum inode_t {DIR_INODE, FILE_INODE};

//
// directory -
//    A directory is a list of paired strings (filenames) and inodes.
//    An inode in a directory may be a directory or a file.
//

class inode;
typedef map<string, inode *> directory;


//
// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.
//

class inode_state {
   friend class inode;
   friend ostream &operator<< (ostream &out, const inode_state &);
   private:
      inode_state (const inode_state &); // disable copy ctor
      inode_state &operator= (const inode_state &); // disable op=
      inode *root;
      inode *cwd;
      string prompt;
      wordvec pwd;
   public:
      inode_state(inode &theRoot, inode &theCwd,const string 
      &thePrompt);
      void set_prompt(const string &the_prompt);
      string return_prompt();
      void init_state(inode &theRoot);
      void print_ls();
      void path_ls(const string &pathname);
      inode *get_cwd();
      void cat_file(const wordvec &words);
      void make_file(const wordvec &words);
      void make_dir(const wordvec &words);
      void print_pwd();
      void cd_root();
      void cd_set_cwd(inode_state &state,const wordvec &words);
      void remove_inode(const wordvec &words); 
};

ostream &operator<< (ostream &out, const inode_state &);


//
// class inode -
//
// inode ctor -
//    Create a new inode of the given type, using a union.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
//    Throws an yshell_exn for a directory.
// writefile -
//    Replaces the contents of a file with new contents.
//    Throws an yshell_exn for a directory.
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an yshell_exn if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.
//    

class inode {
   friend class inode_state;
   private:
      int inode_nr;
      inode_t type;
      union {
         directory *dirents;
         wordvec *data;
      } contents;
      static int next_inode_nr;
   public:
      inode (inode_t init_type);
      inode (const inode &source);
      inode &operator= (const inode &from);
      int get_inode_nr();
      int size();
      const wordvec &readfile() const;
      void writefile (const wordvec &newdata);
      void remove (const string &filename);
      inode &mkdir (const string &dirname);
      inode &mkfile (const string &filename);
      void inode_print_ls();
      void print_file();
      
};

#endif


