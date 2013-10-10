// $Id: inode.h,v 1.2 2013-06-18 17:50:59-07 - - $

/*
  Igor Shvartser
  ishvarts@ucsc.edu
  CMPS109 Summer 13
 */

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
      inode_state (const inode_state &) = delete; // copy ctor
      inode_state &operator= (const inode_state &) = delete; // op=
      inode *root;
      inode *cwd;
      string prompt;
   public:
      inode_state();
      ~inode_state();
      inode* getcwd();
      inode* getpwd();
      inode* getroot();
      void set_cwd_to_root();
      void set_cwd(inode* node);
      void create_new_file_system();
      string getprompt();
      void setprompt(const string &newprompt);
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
   friend ostream &operator<< (ostream &out, inode*);
   private:
      int inode_nr;
      inode_t type;
      union {
         directory *dirents;
         wordvec *data;
      } contents;
      static int next_inode_nr;
   public:
      ~inode();
      inode (inode_t init_type);
      inode (const inode &source);
      inode &operator= (const inode &from);
      int get_inode_nr() const;
      int size() const;
      const wordvec &readfile() const;
      void writefile (const wordvec &newdata);
      void remove (const string &filename);
      void add_dirent(string& name, inode* addition);
      directory* get_dirents();
      bool is_dir();
      inode &mkdir (const string &dirname);
      inode &mkfile (const string &filename);
      inode* get_child_dir(const string& childname);
      bool delete_child(const string& child_name);
      void set_parent(inode* parent);
      inode* get_parent();
      void add_file(string&name, inode* newfile);
      const string name();
      
};

ostream &operator<< (ostream &out, inode*);

#endif

