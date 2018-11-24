// $Id: commands.cpp,v 1.17 2018-01-25 14:02:55-08 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1)
   {
      throw command_error ( "cat: no file specified");
   }
   else 
   {
      try
      {
         state.cat(words);
      }
      catch (file_error& error)
      {
         cout << "cat: " << error.what() << endl;
      }
   }
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() > 2)
   {
      throw command_error ( "cd: more than one operand given");
   }
   else 
   {
      try
      {
         state.cd(words);
      }
      catch (file_error& error)
      {
         cout << "cd: " << error.what() << endl;
      }
   }
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1)
   {
      exit_status::set(0);
   }
   else 
   {
      if (words.size() > 2)
      {
         exit_status::set(127);
      } 
      else
      {
         for (auto i = words[1].begin(); i != words[1].end(); ++i)
         {
            if (!(isdigit(*i)))
            {
               exit_status::set(127);
               break;
            }
            else 
            {
               int num = stoi(words[1]);
               exit_status::set(num);
            }
         }
      }
   }
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   try
   {
      state.ls(words);
   }
   catch (file_error& error)
   {
      cout << "ls: " << error.what() << endl;
   }
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   try
   {
      state.lsr(words);
   }
   catch (file_error& error)
   {
      cout << "lsr: " << error.what() << endl;
   }
}

void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1)
   {
      throw command_error ( "make: no directory specified to make");
   }
   else 
   {
      try
      {
         state.make(words);
      }
      catch (file_error& error)
      {
         cout << "make: " << error.what() << endl;
      }
   }
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1)
   {
      throw command_error ( "mkdir: no directory specified to make"); 
   }
   else
   {
      try
      {
         state.mkdir(words);
      }
      catch (file_error& error)
      {
         cout << "mkdir: " << error.what() << endl;
      }
   }
}
void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1)
   {
       throw command_error ( "prompt: no command specified to change prompt");
   }
   else 
   {
      state.prompt(words);   
   }
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   state.pwd();
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

