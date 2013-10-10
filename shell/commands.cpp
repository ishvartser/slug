// $Id: commands.cpp,v 1.24 2013-07-08 19:42:55-07 - - $

/*
  Igor Shvartser
  ishvarts@ucsc.edu
  CMPS109 Summer 13
 */

#include "commands.h"
#include "trace.h"
#include <string.h>

commands::commands(): map (commandmap()) {
   map["cat"    ] = fn_cat    ;
   map["cd"     ] = fn_cd     ;
   map["echo"   ] = fn_echo   ;
   map["exit"   ] = fn_exit   ;
   map["ls"     ] = fn_ls     ;
   map["lsr"    ] = fn_lsr    ;
   map["make"   ] = fn_make   ;
   map["mkdir"  ] = fn_mkdir  ;
   map["prompt" ] = fn_prompt ;
   map["pwd"    ] = fn_pwd    ;
   map["rm"     ] = fn_rm     ;
}

function commands::at (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (function)
   commandmap::const_iterator result = map.find (cmd);
   if (result == map.end()) {
      throw yshell_exn (cmd + ": no such function");
   }
   return result->second;
}


//Private inner functions:
inode* navigate_path(inode_state& state, const wordvec &words,
                     int path_index, int limitBy);
void lsr_recursive(inode* current_inode, string path);
void rmr_recursive(inode* current_inode);

//"cat pathname ..." command
//responsible for copying the contents of each file to stdout 
void fn_cat (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   //if "cat" enteted alone, print error
   if (words.size() == 1){
      cout << ": No such file or directory" << endl;
   } 
   else {
      wordvec_itor itor = words.begin() + 1;
      const wordvec_itor end = words.end();
      //print every word to stdout
      for (int i = 1; itor != end; ++itor, ++i){
         inode* target = navigate_path(state, words, i, 0);
         if (target != NULL && !target->is_dir()) {
            cout << target->readfile() << endl;
         } 
         else {
            //if not a file, print error
            cout << "cat: " << words[i] << 
            ": No such file or directory" << endl;
         }
      }
   }
}

//"cd [pathname]" command
//responsible for setting current directory
void fn_cd (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error;
   //if "cd" entered alone, set cwd to root
   if (words.size() == 1){
      state.set_cwd_to_root();
   }
   //otherwise navigate to said directory
   else if (words.size() == 2){
      inode* target = navigate_path(state, words, 1, 0);
      //if target is NULL, print error
      if (target == NULL) {
         error += "cd: " + words[1] + 
            ": No such file or directory";
      }
      //otherwise set cwd to path
      else {
         state.set_cwd(target);
      }
   }
   else {
      error += "cd:";
      wordvec_itor itor = words.begin() + 1;
      while (itor != words.end() ) error += " " + *itor++;
      error += ": No such file or directory";
   }
   if (error.size() > 0)
      throw yshell_exn(error);
}

//"echo [words...]" command
//responsible for printing words to stdout
void fn_echo (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string output("");
   //if words exist after echo, print those words out
   if (words.size() > 1) {
      wordvec_itor itor = words.begin() + 1;
      const wordvec_itor end = words.end();
      for (; itor != end; ++itor) {
         output += *itor;
         output += " ";
      }
      output.erase(output.end()-1);
   }
   cout << output << endl;
}

//"exit [status]" command
//responsible for exiting the program with a given status
void fn_exit (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   int code;
   if (words.size() > 1) {
      code = atoi(words[1].c_str());
      exit_status::set(code);
   }
   throw ysh_exit_exn();
}

//"ls [pathname...]" command
//responsible for printing a description of files/directories to stdout
void fn_ls (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   //if "ls" printed alone, list the current directory
   if (words.size() == 1 ) {
      inode* cwd = state.getcwd();
      cout << cwd->name() << ":\n" << cwd << "  " << endl;
   }
   else {
      inode* target;
      wordvec_itor itor = words.begin();
      ++itor;
      wordvec_itor end = words.end();
      for (int i = 1; itor != end; ++itor, ++i) {
         target = navigate_path(state, words, i, 0);
         if (target == NULL) {
            cerr << "ls: Invalid file or directory: " 
               << words[i] << endl;
         }
         else {
            cout << words[i] << ":" << "\n" << target << "  " << endl;
         }
      }
   }
}

//"lsr [pathname...]" command
//responsible for recursive depth-1st preorder traversal on subdirectories
void fn_lsr (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error("");
   inode* top;
   if (words.size() == 1) {
      top = state.getcwd();
      string n;
      lsr_recursive(top, n);
   }
   else {
      wordvec_itor itor = words.begin();
      ++itor;
      wordvec_itor end = words.end();
      for (int i = 1; itor!=end; ++itor, ++i) {
         top = navigate_path(state, words, i, 0);
         if ( top != NULL ){
            string n;
            lsr_recursive(top, n);
         }
         else {
            cout << "lsr: No such file or directory: "
               << words[i] << endl;
         }
      }
   }
}

//lsr recursion function
void lsr_recursive(inode* current_inode, string path){
   const string name = current_inode->name();
   path += current_inode->name();
   cout << path
   << ":\n" << current_inode << endl;
   directory* directory = current_inode->get_dirents();
   directory::const_iterator itor = directory->begin();
   ++itor; ++itor;
   directory::const_iterator end = directory->end();
   if ( path.compare("/") != 0 )
      path += "/";
   for (; itor != end; ++itor) {
      if ( itor->second->is_dir() ){
         lsr_recursive(itor->second, path);
      }
   }
}


//"make pathname [words...]" commands
//responsible for creating a file, and putting words into it
void fn_make (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error("");
   //if filename not specified, error
   if (words.size() == 1 ) {
      error += "make: Please specify a filename";
   }
   else {
      inode* target_parent = navigate_path(state, words, 1, 1);
      if (target_parent != NULL) {
         bool newfile = false;
         wordvec path = split(words[1], "/" );
         string dirname(path[path.size()-1]);
         inode* targetfile = target_parent->get_child_dir(dirname);
         //if file doesn't exist, create a new one
         if (targetfile == NULL) {
            targetfile = new inode(FILE_INODE);
            newfile = true;
         }
         if (!targetfile->is_dir()) {
            targetfile->writefile(words);
            if (newfile == true) {
               string name(words[1]);
               target_parent->add_file(dirname, targetfile);
            }
         } 
         else {
            error += "make: Directory already exists with name: " + 
               words[1];
         }
      } 
      else {
         error += "make: Invalid file or directory";
      }
   }
   if (error.size() > 0)
      throw yshell_exn(error);
}

//"mkdir pathname" command
//responsible for creating a new directory
void fn_mkdir (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error("");
   //if "mkdir" entered alone, print error
   if (words.size() == 1 || words.size() > 2) {
      error += "mkdir: Invalid directory name";
   }
   else if (words.size() == 2) {
      inode* target = navigate_path(state, words, 1, 1);
      //check if the directory already exists
      if (target != NULL) {
         wordvec path = split(words[1],"/");
         if (target->get_child_dir(path[path.size()-1]) != NULL) {
            error += "mkdir: Directory already exists";
         }
         else {
            //if directory does not exit, create one
            inode* newDir = new inode(DIR_INODE);
            target->add_dirent(path[path.size()-1], newDir);
         }
      }
      else {
         error += "mkdir: Invalid path";
      }
   }
   if (error.size() > 0)
      throw yshell_exn(error);
}

//"prompt string" command
//responsible for setting the prompt to words specified on command line
void fn_prompt (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string newprompt;
   if (words.size() > 1) {
      wordvec_itor itor = words.begin() + 1;
      const wordvec_itor end = words.end();
      for (; itor != end; ++itor) {
         newprompt += *itor;
         newprompt += " ";
      }
      newprompt.erase(newprompt.end()-1);
   }
   state.setprompt(newprompt);
}

//"pwd" command
//responsible for printing the current working directory to stdout
void fn_pwd (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string path("");
   //get current working directory
   path += state.getcwd()->name();
   inode* parent = state.getcwd()->get_parent();
   while (parent->get_inode_nr() !=
           state.getroot()->get_inode_nr()) {
      path = parent->name() + "/" + path;
      parent = parent->get_parent();
   }
   if (path.compare("/") != 0)
      path = "/" + path;
   //print
   cout << path << endl; 
}

//"rm pathname" command
//responsible for deleting the specified file or directory
void fn_rm (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error("");
   //if "rm" entered alone, print error
   if (words.size() == 1 || words.size() > 2){
      error += "rm: Invalid file or directory";
   }
   //otherwise remove target
   else if (words.size() == 2){
      inode* target = navigate_path(state, words, 1, 0);
      if (target != NULL){
         if ((target->is_dir() && target->size() <= 2) || !target->is_dir()){
            inode* target_parent = navigate_path(state, words, 1, 1);
            wordvec path = split(words[1],"/");
            if (target_parent->delete_child(path[path.size()-1]) == false){
               error += "rm: Cannot delete '.' or '..'";
            }
            else { 
               TRACE ('c', "Deleted child from parents" );
            }
         }
         else {
            error += "rm: Cannot delete a non-empty directory";
         }
      }
      else {
         error += "rm: Invalid file or directory";
      }
   }
   if (error.size() > 0)
      throw yshell_exn(error);
}

//"rmr pathname" command
//responsible for recursive removal of file or directory
void fn_rmr (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error("");
   //if "rmr" entered alone, print error
   if (words.size() == 1 || words.size() > 2) {
      error += "rmr: Invalid file or directory";
   }
   //otherwise perform recursive removal
   else if (words.size() == 2){
      inode* target_top = navigate_path(state, words, 1, 0);
      inode* parent = navigate_path(state, words, 1, 1);
      if (target_top->is_dir()) {
         rmr_recursive(target_top);
      }
      if (parent->get_inode_nr() != target_top->get_inode_nr()) {
         parent->delete_child(target_top->name());
      }
   }
   if (error.size() > 0)
      throw yshell_exn(error);
}

//rmr recursive function
void rmr_recursive(inode* current_inode){
   directory* directory = current_inode->get_dirents();
   directory::iterator itor = directory->begin();
   ++itor; ++itor;
   directory::const_iterator end = directory->end();
   for (; itor != end;) {
      if ( itor->second->is_dir() )
         rmr_recursive(itor->second);
      directory->erase(itor++);
   }
   
}

//function to help us parse through the pathname
inode* navigate_path(inode_state& state, const wordvec &words,
                     int path_index, int limitBy){
   bool start_at_root = false;
   wordvec path = split(words[path_index],"/");
   if (words[path_index].at(0)=='/') {
      start_at_root = true;
   }
   inode* top;
   if (start_at_root == true) {
      top = state.getroot();
   } 
   else {
      top = state.getcwd();
   }
   int size = path.size()-limitBy;
   for (int i = 0; i < size; ++i ) {
      if (top->is_dir()) {
         top = top->get_child_dir(path[i]);
      } 
      else {
         top = NULL;
      }
      if (top == NULL) {
         return NULL;
      }
   }
   return top;
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

