// $Id: inode.cpp,v 1.35 2013-07-08 20:06:04-07 - - $

/*
  Igor Shvartser
  ishvarts@ucsc.edu
  CMPS109 Summer 13
 */

#include <cassert>
#include <iostream>
#include <iomanip>

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
           contents.dirents->insert( make_pair(".", this));
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

inode::~inode(){
   if ( type == DIR_INODE ){
      delete contents.dirents;
   }else
      delete contents.data;
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

void inode::add_dirent(string& name, inode* addition) {
   contents.dirents->insert( make_pair(name, addition));
   addition->set_parent(this);
}

directory* inode::get_dirents() {
   if ( !is_dir() ) return NULL;
   return contents.dirents;
}


int inode::get_inode_nr() const {
   TRACE ('i', "inode = " << inode_nr);
   return inode_nr;
}

inode* inode_state::getcwd() {
   return cwd;
}

void inode::set_parent(inode* parent) {
   contents.dirents->insert( make_pair("..", parent) );
}

inode* inode::get_parent() {
   if ( type == FILE_INODE )
      throw yshell_exn("Cannot call get_parent() on FILE_INODE");
   directory::iterator p = contents.dirents->begin();
   ++p;
   return p->second;
}

void inode_state::setprompt(const string &newprompt){
   prompt = newprompt;
}

string inode_state::getprompt(){
   return prompt;
}

inode* inode::get_child_dir(const string &dirname){
   inode* target = NULL;
   if ( type == FILE_INODE ) return target;
   directory::const_iterator itor = contents.dirents->begin();
   directory::const_iterator end = contents.dirents->end();
   for (; itor != end; ++itor) {
      if ( dirname.compare(itor->first) == 0)
         return itor->second;
   }
   return NULL;
   
}

bool inode::delete_child(const string& child_name){
   if ( type == FILE_INODE )
      throw yshell_exn("Oops! Cannot call delete_child() on FILE_INODE");
   if ( child_name.compare(".") == 0 ||
         child_name.compare("..") == 0 )
      return false;
   directory::iterator itor = contents.dirents->begin();
   directory::const_iterator end = contents.dirents->end();
   for (; itor != end; ++itor) {
      if ( itor->first.compare(child_name) == 0 ){
         contents.dirents->erase(itor++);
         return true;
      }
      else ++itor;
   }
   return false;
}

inode* inode_state::getroot(){
   return root;
}

void inode_state::set_cwd(inode* node){
   assert(node->is_dir());
   cwd = node;
}

void inode_state::set_cwd_to_root(){
   cwd = root;
}

bool inode::is_dir(){
   if (type == DIR_INODE) {
      return true;
   }
   return false;
}

int inode::size() const {
   int size = 0;
   switch ( type ){
      case DIR_INODE:
         size = contents.dirents->size();
         break;
      case FILE_INODE:
         wordvec_itor i = contents.data->begin();
         wordvec_itor end = contents.data->end();
         while( i != end ) {
             size += i++->size();
         }
         size += contents.data->size();
         break;
   }
   TRACE ('i', "size = " << size);
   return size;
}

const string inode::name() {
   inode* parent = get_child_dir("..");
   if ( parent == this ) { 
      return "/";
   } 
   else {
      directory::const_iterator itor = 
         parent->contents.dirents->begin();
      directory::const_iterator end = 
         parent->contents.dirents->end();
      for (; itor != end; ++itor) {
         if ( itor->second == this ) {
            return string(itor->first);
         }
      }
   }
   return "";
}

const wordvec &inode::readfile() const {
   TRACE ('i', *contents.data);
   assert (type == FILE_INODE);
   return *contents.data;
}

void inode_state::create_new_file_system(){
   root = new inode(DIR_INODE);
   root->set_parent(root);
   set_cwd_to_root();
}

void inode::writefile (const wordvec &words) {
   TRACE ('i', words);
   assert (type == FILE_INODE);
   contents.data->clear();
   wordvec_itor itor = words.begin()+2;
   while (itor != words.end()) {
      contents.data->push_back(*itor++);
   }
}

void inode::add_file(string& name, inode* newfile){
   contents.dirents->insert(make_pair(name, newfile));
}

void inode::remove (const string &filename) {
   TRACE ('i', filename);
   assert (type == DIR_INODE);
}

inode_state::inode_state(): root (NULL), cwd (NULL), prompt ("%") {
   TRACE ('i', "root = " << (void*) root << ", cwd = " << (void*) cwd
          << ", prompt = " << prompt);
}

inode_state::~inode_state(){
   TRACE ('i', "root = " << (void*) root << ", cwd = " << (void*) cwd
          << ", prompt = " << prompt);
   
   delete root;
}

ostream &operator<< (ostream &out, inode* node) {
   directory::const_iterator itor = node->contents.dirents->begin();
   directory::const_iterator end = node->contents.dirents->end();
   int i = 0;
   for (; itor != end;) {
      out << setw(6) << itor->second->get_inode_nr() << setw(6)
         << itor->second->size() << "  " << itor->first;
      if ( i < 2 ){
         ++i;
      } else if ( itor->second->is_dir() ) 
         out << "/";
      ++itor;
      if ( itor != end ) out << "\n";
   }
   return out;
}

ostream &operator<< (ostream &out, const inode_state &state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

