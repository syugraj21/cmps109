// $Id: inode.cpp,v 1.255 2013-01-21 16:05:56-08 - - $

#include <cassert>
#include <iostream>

using namespace std;

#include "trace.h"
#include "inode.h"

int inode::next_inode_nr = 1;

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case DIR_INODE:
           contents.dirents = new directory();
           break;
      case FILE_INODE:
           contents.data = new wordvec();
           break;
   }
   TRACE ('i', "inode " << inode_nr << ", type = " << type);
}

//
// copy ctor -
//    Make a copy of a given inode.  This should not be used in
//    your program if you can avoid it, since it is expensive.
//    Here, we can leverage operator=.
//
inode::inode (const inode &that) {
   *this = that;
}

//
// operator= -
//    Assignment operator.  Copy an inode.  Make a copy of a
//    given inode.  This should not be used in your program if
//    you can avoid it, since it is expensive.
//
inode &inode::operator= (const inode &that) {
   if (this != &that) {
      inode_nr = that.inode_nr;
      type = that.type;
      contents = that.contents;
   }
   TRACE ('i', "inode " << inode_nr << ", type = " << type);
   return *this;
}

int inode::get_inode_nr() {
   TRACE ('i', "inode = " << inode_nr);
   return inode_nr;
}

int inode::size() {
   int size = 0;
   int hold;
   TRACE ('i', "size = " << size);
   switch(type) {
      case DIR_INODE:
         size = contents.dirents->size();
         break;
      case FILE_INODE:
         if(contents.data->size() == 0){
            size = 0;
         } else {
            hold = contents.data->size();
            for(int i = 0; i<hold; i++){
               size += contents.data->at(i).size();
            }
            size = size + hold - 1;
         }
         break;
   }
   return size;
}

const wordvec &inode::readfile() const {
   TRACE ('i', *contents.data);
   assert (type == FILE_INODE);
   return *contents.data;
}

/*changes the contents of the file*/
void inode::writefile (const wordvec &words) {
   TRACE ('i', words);
   assert (type == FILE_INODE);
   while( !contents.data->empty()){
     contents.data->pop_back();
   }
   for(unsigned i = 2; i<words.size(); ++i){
     contents.data->push_back(words[i]);
   }
}

//removes the files or empty directory from the tree
void inode::remove (const string &filename) {
   TRACE ('i', filename);
   assert (type == DIR_INODE);
   auto it = contents.dirents->find(filename);
   contents.dirents->erase(it);
}
/*
  inode_print_ls: handles the actual printing of the 
                  inode
*/
void inode::inode_print_ls(){
   auto it = contents.dirents->begin();
   for(; it != contents.dirents->end(); ++it){
      cout<<"     "<<it->second->get_inode_nr()<<"        "
      <<it->second->size();
      if( it->second->type == FILE_INODE){
         cout <<" "<< it->first <<endl;
      }else {
         cout<<" "<<it->first <<"/" <<endl;
      }
   }
}

// prints the contents of the file
void inode::print_file(){
   for(unsigned i=0; i< contents.data->size(); ++i){
      cout<< contents.data->at(i) <<" ";
   }
   cout<<endl;
}


inode_state::inode_state(inode &theRoot, inode &theCwd, const string
  &thePrompt): root (&theRoot), cwd (&theCwd), prompt (thePrompt) {
   TRACE ('i', "root = " << (void*) root << ", cwd = " << (void*) cwd
          << ", prompt = " << prompt);
}

// initializes the root directory 
void inode_state::init_state(inode &theRoot){
   (*root->contents.dirents)["."] = &theRoot;
   (*root->contents.dirents)[".."] = &theRoot;
   pwd.push_back("/");
}

/*
  cat_file: does error checking and prints out he 
           specified file 
*/
void inode_state::cat_file(const wordvec &words){
   for(unsigned i =1; i<words.size(); ++i){
      auto it = cwd->contents.dirents->find(words[i]);
      if(it == cwd->contents.dirents->end()){
         throw yshell_exn("no file "+words[i]+" exist");
      } else if ((*it).second->type == FILE_INODE){
         cout << words[i]<<": "<< endl;
         it->second->print_file();      
      } else {
         throw yshell_exn( words[i]+": is directory");
      }
   }
}

/*
  prints the current working directory
*/
void inode_state::print_ls(){
   cout << pwd <<":"<< endl;
   cwd->inode_print_ls();
}

/*
  path_ls: does error checking, and prints out specified path
*/
void inode_state::path_ls(const string &pathname){
   auto it = cwd->contents.dirents->find(pathname);
   cout << pwd << pathname <<":" << endl;
   if( it == cwd->contents.dirents->end()){
      throw yshell_exn("no such file or directory: "+ pathname);
   } else {
      if((*it).second->type == FILE_INODE){
         cout <<  it->first <<endl;
      } else{
         (*it).second->inode_print_ls();
      }
   }
}
// sets the prompt to the new prompt
void inode_state::set_prompt(const string &the_prompt){
   prompt = the_prompt;
}

//returns the prompt
string inode_state::return_prompt(){
   return prompt;
}
//returns a pointer to the current working directory 
inode* inode_state::get_cwd(){
   return cwd;
}
/*
  make_file: does error checking and creates a new file inode
*/
void inode_state::make_file(const wordvec &words){
   auto it = cwd->contents.dirents->find(words[1]);
   if( it == cwd->contents.dirents->end()){
      inode *file = new inode(FILE_INODE);
      for(unsigned i = 2; i < words.size(); ++i){
         file->contents.data->push_back(words[i]);
      }
      (*cwd->contents.dirents)[words[1]] = file;
   } else{
      it->second->writefile(words);
   }
}

/*
  make_dir: does error checking and creates a new directory inode
*/
void inode_state::make_dir(const wordvec &words){
   auto it = cwd->contents.dirents->find(words[1]);
   if( it != cwd->contents.dirents->end()){
      throw yshell_exn("file or directory: "+ words[1]
      +": already exists");
   } else { //initializes inode
      inode *dir = new inode(DIR_INODE);
      (*cwd->contents.dirents)[words[1]]=dir;
      (*dir->contents.dirents)["."] = dir;
      (*dir->contents.dirents)[".."] = cwd;
   }
}

/*
prints the current working directories 
*/
void inode_state::print_pwd(){
   for(unsigned i =0; i< pwd.size(); i++){
      cout << pwd[i];
   }
   cout << endl;
}
/*
 cd_root: cd to the root directory 
*/
void inode_state::cd_root(){
   cwd = root;
   while(!pwd.empty()){
      pwd.pop_back();
   }
   pwd.push_back("/");
}

/*
  cd_set_cwd: checks for error and changes directory recursively 
*/
void inode_state::cd_set_cwd(inode_state &state,const wordvec &words){
   if(words.size() != 0){
      wordvec new_word;
      auto it = state.cwd->contents.dirents->find(words[0]);
      if(it == state.cwd->contents.dirents->end()){
         throw yshell_exn("no directory named:" + words[0]); 
      } else if( it->second->type == FILE_INODE){
         throw yshell_exn(words[0] +": not a directory");
      } else if (words[0] == ".."){ //cd to previous/parent directory  
         if( state.root == state.cwd){
            state.cwd = it->second;
            for(unsigned i =1; i<words.size(); ++i){
               new_word.push_back(words[i]);
            }
            state.cd_set_cwd(state,new_word);
         } else {
            state.cwd = it->second;
            state.pwd.pop_back();
            for(unsigned i =1; i<words.size(); ++i){
               new_word.push_back(words[i]);
            }
            state.cd_set_cwd(state,new_word);
         }
      } else if(words[0]=="."){ // cd to current inode
         for(unsigned i=1; i<words.size(); i++){
            new_word.push_back(words[i]);
         } 
         state.cd_set_cwd(state,new_word);
      } else { // cd to any other directory
         state.pwd.push_back(words[0]+"/");
         state.cwd = it->second;
         for(unsigned i = 1; i<words.size(); ++i){
            new_word.push_back(words[i]);
         }
         state.cd_set_cwd(state,new_word);
      }
   }
}

/*
 remove_inode: does error checking, and removes the inode
               from tree
*/
void inode_state::remove_inode(const wordvec &words){
    auto it = cwd->contents.dirents->find(words[1]);
    if( it == cwd->contents.dirents->end()){
       throw yshell_exn("no file or directory: "+words[1]);
    } else if( it->second->type == DIR_INODE){
       if(it->second->size() > 2){
          throw yshell_exn("cannot remove nonempty directory");
       } else {
          cwd->remove(words[1]);
       }
    } else {
       cwd->remove(words[1]);
    }
}

ostream &operator<< (ostream &out, const inode_state &state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}


