// $Id: file_sys.cpp,v 1.5 2016-01-14 16:16:52-08 - - $

#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
   //cwd = make_shared<inode>(file_type::DIRECTORY_TYPE);
   root = make_shared<inode>(file_type::DIRECTORY_TYPE);
   cwd = root;
   auto dir = dynamic_pointer_cast<directory>(cwd->contents);
   dir->mkroot(cwd, root);
}

void directory:: mkroot(inode_ptr cwd, inode_ptr root)
{
   dirents.insert(pair<string, inode_ptr>(".", cwd));
   dirents.insert(pair<string, inode_ptr>("..", root));
}

const string& inode_state::prompt() { return prompt_; }

inode_ptr inode_state:: get_cwd()
{
   return cwd;
}

string inode_state:: get_last(const string& pathname)
{
   string str = "";
   for (auto i = pathname.begin(); i != pathname.end(); ++i)
   {
      if (*i != '/')
      {
         str.push_back(*i);
      }
      else
      {
         str = "";
      }
   } 
   return str;
}

bool check_slash(const string& pathname)
{
   for (auto i = pathname.begin(); i != pathname.end(); ++i)
   {
      if (*i == '/')
      {
         return true;
      }
   }
   return false;
}

inode_ptr inode_state:: parse_path(const string& pathname, const string& fn)
{
   string str = "";
   string str2 = "";
   inode_ptr start = nullptr;
   inode_ptr temp = cwd;
   bool flag = false;

   if (*pathname.begin() == '/')
   {
      start = root;
   }
   else 
   {
      start = cwd;
   }
    
   for (auto i = pathname.begin(); i != pathname.end(); ++i)
   {
      if (*i != '/')
      {
         str.push_back(*i);
      } 
      else  
      {
         flag = true;
         auto dir = dynamic_pointer_cast<directory>(start->contents);
         auto node = dir->find_file(str);
         if (node == nullptr)
         {
            //if (fn != "make" && fn != "mkdir")
            //{
               throw file_error (str + ": no such file or directory");
               return nullptr;
            //}
            /*else
            {
               return nullptr;
            }*/
         }
         
         else if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
         {
            if (fn != "make" && fn != "mkdir")
            {
               temp = node;
               start = node;
            }
            else
            {
               temp = node;
               cwd = node;
               start = node;
            }
         }
         else if (node->get_file_type() == file_type:: PLAIN_TYPE)
         {
            return node;
         }
         str = "";
      }
   } 
   if (flag == false)
   {
      auto dir = dynamic_pointer_cast<directory>(start->contents);
      auto node = dir->find_file(str);
      if (node == nullptr)
      {
         if (fn != "make" && fn != "mkdir")
         {
            throw file_error (str + ": no such file or directory");
            return nullptr;
         }
         else
         {
            return nullptr;
         }
      }

      else if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
      {
         if (fn != "make" && fn != "mkdir")
         {
            temp = node;
         }
         else 
         {
            temp = node;
            cwd = node;
         }
      }
      else if (node->get_file_type() == file_type:: PLAIN_TYPE)
      {
         return node;
      } 
   }
   auto dir = dynamic_pointer_cast<directory>(start->contents);
   auto node = dir->find_file(str);
   if (node == nullptr)
   {
      if (fn != "make" && fn != "mkdir")
      {
         throw file_error (str + ": no such file or directory");
         return nullptr;
      }
      else
      {
         return nullptr;
      }
   }

   else if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
   {
      if (fn != "make" && fn != "mkdir")
      {
         temp = node;
         start = node;
      }
      else
      {
         temp = node;
         cwd = node;
         start = node;
      }
   }
   else if (node->get_file_type() == file_type:: PLAIN_TYPE)
   {
      return node;
   }
   return temp;
}

void inode_state:: cat(const wordvec& words)
{
   for (auto i = words.begin() + 1; i != words.end(); ++i)
   { 
      auto dir = dynamic_pointer_cast<directory>(cwd->contents);
      auto node = dir->find_file(*i);
      if (node == nullptr)
      {
         throw file_error (*i + ": no such file exists");
      }   
      else if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
      {
         throw file_error (*i + ": is a directory");
      }
      else
      {
         auto plain = dynamic_pointer_cast<plain_file>(node->contents);
         auto vec = plain->get_data();
         for (auto j = vec.begin(); j != vec.end(); ++j)
         {
            cout << *j;
         }
         cout << endl;
      }
   }
}

void inode_state:: pwd()
{
   for (auto i = vec.begin(); i != vec.end(); ++i)
   {
      if (i == vec.begin())
      {
         cout << *i;
      }
      else if (i == vec.end() - 1)
      {
         cout << *i;
      }
      else 
      {
         cout << *i << "/";
      }
   }
   cout << endl;
}

inode_ptr directory:: find_file(const string& filename)
{
   if (dirents.find(filename) == dirents.end())
   {
      return nullptr;
   }
   return dirents.find(filename)->second;
}

string directory:: find_key(const inode_ptr ptr)
{
   for (auto i = dirents.begin(); i != dirents.end(); ++i)
   {
      if (i->second == ptr)
      {
         return i->first;
      }
   }
   return "";
}

void inode_state:: ls(const wordvec& words)
{
   if (words.size() == 1)
   {
      //cout << "/:" << endl;
      auto dir = dynamic_pointer_cast<directory>(cwd->contents);
      auto str = dir->find_key(cwd);
      if (cwd == root)
      {
         str = "/";
      }
      else 
      {
         str = vec.back();
      }
      cout << str << ":" << endl;
      dir->print_map();
   }
   else if (words[1] == "/")
   {
      cout << "/:" << endl;
      auto dir = dynamic_pointer_cast<directory>(root->contents);
      dir->print_map();
   }
   else 
   {
      for (auto i = words.begin() + 1; i != words.end(); ++i)
      {
         auto node = parse_path(*i, "ls");
         if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
         {
            auto dir = dynamic_pointer_cast<directory>(node->contents);
            auto str = get_last(*i);
            cout << str << ":" << endl;
            dir->print_map();
         }
         else if (node->get_file_type() == file_type:: PLAIN_TYPE)
         {
            cout << get_last(*i) <<":" << endl;
            cout << get_last(*i) << endl;
         }
         else 
         {
            throw file_error (get_last(*i) + ": no such file or directory exists");
         }
      }
   } 
}

void inode_state:: lsr(const wordvec& words)
{
   if (words.size() == 1)
   {
      cout << vec.back() << endl;
      auto dir = dynamic_pointer_cast<directory>(cwd->contents);
      dir->print_map();
      dir->print_dirs();
   } 
   else if (words[1] == "/")
   {
      cout << "/:" << endl;
      auto dir = dynamic_pointer_cast<directory>(root->contents);
      dir->print_map();
      dir->print_dirs();
   }
}

void directory:: print_dirs()
{
   for (auto i = dirents.begin(); i != dirents.end(); ++i)
   {
      auto node = i->second;
      auto dir = dynamic_pointer_cast<directory>(node->get_contents());
      if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
      {
         if (i->first == "." || i->first == "..")
         {}
         else
         {
            cout << i->first << ":" << endl;
            dir->print_map();
         }
      }
   }
}
void directory:: print_map()
{
   string space = "";
   string space2 = "";
   for (auto i = dirents.begin(); i != dirents.end(); ++i)
   {
      string key = i->first;
      auto node = find_file(key);
      if (node->get_inode_nr() >= 10)
      {
         space = "     ";
      }
      else 
      {
         space = "      ";
      }
      
      if (node->get_file_type() == file_type:: PLAIN_TYPE)
      {
         auto pf = dynamic_pointer_cast<plain_file>
         (node->get_contents());
         if (pf->size() >= 10)
         {
            space2 = "     ";
         }
         else 
         {
            space2 = "      ";
         }
         cout << space << node->get_inode_nr() << space2 << 
         pf->size() << "  " << key << endl;
      }
      else
      {
         auto df = dynamic_pointer_cast<directory>
         (node->get_contents());
         if (df->size() >= 10)
         {
            space2 = "     ";
         }
         else
         {
            space2 = "      ";
         }
         if (key == "." || key == "..")
         {
            cout << space << node->get_inode_nr() << space2 << 
            df->size() << "  " << key << endl;
         }
         else 
         {
            cout << space << node->get_inode_nr() << space2 <<
            df->size() << "  " << key << "/" << endl;
         }
      }
   }
}

void inode_state:: cd(const wordvec& words)
{
   if (words.size() == 1)
   {
      cwd = root;
      vec.clear();
      vec.push_back("/");
      return;
   }
   if (words[1] == ".")
   {return;} 

   if (words[1] == "..")
   {
      if (cwd == root)
      {
         vec.clear();
         vec.push_back("/");
         return;
      }
   }
   auto node = parse_path(words[1], "cd");
   if (node == nullptr)
   {
      return;
   }
   if (node->get_file_type() == file_type:: DIRECTORY_TYPE)
   {
      cwd = node;
      vec.push_back(words[1]); 
   }
   else if (node->get_file_type() == file_type:: PLAIN_TYPE)
   {
       throw file_error (get_last(words[1])  + ": is a plain file");
   }
}

void inode_state:: prompt(const wordvec& words)
{
   prompt_ = words[1] + " ";  
}
void inode_state:: make(const wordvec& words)
{
   auto store = cwd; 
   inode_ptr path = nullptr;
   path = parse_path(words[1], "make");
   string str = get_last(words[1]);
   if (path == nullptr)
   {
      auto dir = dynamic_pointer_cast<directory>(cwd->contents);
      auto node = dir->mkfile(str);
      auto plain = dynamic_pointer_cast<plain_file>(node->contents);
      plain->writefile(words);
   }
   else if (path->get_file_type() == file_type:: PLAIN_TYPE)
   {
      auto plain = dynamic_pointer_cast<plain_file>(path->contents);
      plain->writefile(words);
   }
   else 
   {
      throw file_error (str + ": is a directory");
   }
   cwd = store;
}

void inode_state:: mkdir(const wordvec& words)
{
   auto store = cwd;
   inode_ptr path = nullptr;
   path = parse_path(words[1], "mkdir");
   string str = get_last(words[1]);
   if (path == nullptr)
   {
      auto dir = dynamic_pointer_cast<directory>(cwd->contents);
      auto new_dir = dir->mkdir(str);
      auto dp = dynamic_pointer_cast<directory>(new_dir->contents);
      dp->mkroot(new_dir, cwd);
   }
   else
   {
      throw file_error(str + ": already exists");
   }
   cwd = store;
}

base_file_ptr inode:: get_contents()
{
   return contents;
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           ftype = file_type:: PLAIN_TYPE;
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           ftype = file_type:: DIRECTORY_TYPE;
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

file_type inode:: get_file_type()
{
   return ftype;
}

wordvec plain_file:: get_data()
{
   return data;
}


file_error::file_error (const string& what):
            runtime_error (what) {
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   for (auto i = data.begin(); i != data.end(); ++ i)
   {
      string str = *i;
      for (auto j = str.begin(); j != str.end(); ++j)
      {
         ++size;
      }
   }
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   string str;
   for (auto i = words.begin() + 2; i != words.end(); ++i)
   {
      str.append(*i);
      if (i != words.end() - 1)
      {
         str.append(" ");
      }
   }
   data.push_back(str);
}

void plain_file::remove (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkdir (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkfile (const string&) {
   throw file_error ("is a plain file");
}


size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   size = dirents.size();
   return size;
}

const wordvec& directory::readfile() const {
   throw file_error ("is a directory");
}

void directory::writefile (const wordvec&) {
   throw file_error ("is a directory");
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   shared_ptr<inode> node = 
   make_shared<inode>(file_type::DIRECTORY_TYPE);
   dirents.insert(pair<string, inode_ptr>(dirname, node));
   return node;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   shared_ptr<inode> node = make_shared<inode>(file_type::PLAIN_TYPE);
   dirents.insert(pair<string, inode_ptr>(filename, node));
   return node;
}

