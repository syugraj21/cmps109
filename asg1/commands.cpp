
#include "commands.h"
#include "trace.h"

commands::commands(): map (commandmap()) {
   map["cat"    ] = fn_cat    ;
   map["cd"     ] = fn_cd     ;
   map["echo"   ] = fn_echo   ;
   map["exit"   ] = fn_exit   ;
   map["ls"     ] = fn_ls     ;
   map["make"   ] = fn_make   ;
   map["mkdir"  ] = fn_mkdir  ;
   map["prompt" ] = fn_prompt ;
   map["pwd"    ] = fn_pwd    ;
   map["rm"     ] = fn_rm     ;
   map["#"      ] = fn_comment;
}

function commands::operator[] (const string& cmd) {
   return map[cmd];
}


/*
get_prompt:returns the string/char for the prompt
           from vector of string
*/
string get_prompt(const wordvec &words){
   string new_prompt;
   for(unsigned i =1; i<words.size(); i++){
      if( i == words.size()-1){
         new_prompt = new_prompt + words[i];
      }
      else{
         new_prompt = new_prompt + words[i] + " ";
      }
   }
   return new_prompt;
}

void fn_cat (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string line;
   if(words.size() == 1){
      throw yshell_exn("no files specified");
   }
   else{
      state.cat_file(words);
   }
}

void fn_cd (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string hold_dir;
   wordvec move_dir;
   if (words.size() == 1){
      state.cd_root();
   } else{
      for(unsigned i=1; i< words.size(); ++i){
         hold_dir += words.at(i);
      }
      move_dir = split(hold_dir,"/");
      state.cd_set_cwd(state,move_dir);
   }
}

void fn_echo (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   for(unsigned i =1; i< words.size(); i++){
      cout << words[i] << " ";
   }
   cout << endl;
}

void fn_exit (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   throw ysh_exit_exn ();
}

void fn_ls (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   if(words.size() == 1){
      state.print_ls();
   } else {
      for(unsigned i =1; i< words.size(); ++i){
         state.path_ls(words[i]);
      }      
   }
}


void fn_make (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   state.make_file(words);
}

void fn_mkdir (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   state.make_dir(words);
}

void fn_prompt (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string newPrompt = get_prompt(words);
   state.set_prompt(newPrompt);
}

void fn_pwd (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   state.print_pwd();
}

void fn_rm (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   if(words.size() == 1){
      throw yshell_exn("no pathname specified");
   } else if(words[1] == "/"){
      throw yshell_exn("cannot rm " + words[1]); 
   } else{
      state.remove_inode(words);
   }
}

/*
 handles comments entered by the user
*/
void fn_comment(inode_state &state, const wordvec &words){
   TRACE('c', state);
   TRACE('c', words); 
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}


