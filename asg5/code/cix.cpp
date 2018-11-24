// $Id: cix.cpp,v 1.4 2016-05-09 16:01:56-07 - - $

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"get" , cix_command::GET },
   {"put" , cix_command::PUT },
   {"rm"  , cix_command::RM  },
};

void cix_help() {
   static const vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      log << "sent LS, server did not return LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_get(client_socket& server, string name) {
   cix_header header;
   header.command = cix_command::GET;
   auto itor = name.begin();
   for (int i = 0; i < 58; ++i)
   {
      if (itor == name.end())
      {
         header.filename[i] = '\0';
      }
      else 
      {
         header.filename[i] = name.at(i);
         ++itor;
      }
   }
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::FILE)
   {
      log << "sent GET, server did not return FILE" << endl;
      log << "server returned " << header << endl;
   }
   else 
   {
      char buffer[header.nbytes + 1];
      ofstream ofs (name);
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      ofs.write(buffer, header.nbytes);
      ofs.close();
   }
}

void cix_put(client_socket& server, string name) {
   cix_header header;
   ifstream ifs (name);
   if (ifs.good() == false)
   {
      log << name << " does not exist" << endl;
      return;
   }
   auto itor = name.begin();
   for (int i = 0; i < 58; ++i)
   {
      if (itor == name.end())
      {
         header.filename[i] = '\0';
      }
      else
      {
         header.filename[i] = name.at(i);
         ++itor;
      }
   }
   stringstream stream;
   stream << ifs.rdbuf();
   string contents = stream.str();
   header.command = cix_command::PUT;
   header.nbytes = contents.size();
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   send_packet (server, contents.c_str(), contents.size());
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   
   if (header.command != cix_command::ACK)
   {
      log << "sent PUT, server did not return ACK" << endl;
      log << "server returned " << header << endl;
   }
   else 
   {
      log << "successfully put " << header.filename 
        << " to server" << endl;
   }
}

void cix_rm (client_socket& server, string name){
   cix_header header; 
   auto itor = name.begin();
   for (int i = 0; i < 58; ++i)
   {
      if (itor == name.end())
      {
         header.filename[i] = '\0';
      }
      else
      {
         header.filename[i] = name.at(i);
         ++itor;
      }
   }
   header.command = cix_command::RM;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::ACK) {
      log << "sent RM, server did not return ACK" << endl;
      log << "server returned " << header << endl;
   }else {
      log << "successfully deleted " << header.filename 
         << " in server" << endl;
   } 
}


void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         log << "command " << line << endl;
         string comm = "";
         string filename = "";
         bool first = true;
         for (unsigned int i = 0; i < line.length(); ++i)
         {
            if (line.at(i) == ' ')
            {
               first = false;
               continue;
            }
            if (first)
            {
               comm.push_back(line.at(i));
            }
            else 
            {
               filename.push_back(line.at(i));
            }
         }
         const auto& itor = command_map.find (comm);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;

         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               break;
            case cix_command::LS:
               cix_ls (server);
               break;
            case cix_command::GET:
               cix_get (server, filename); 
               break;
            case cix_command::PUT:
               cix_put (server, filename);
               break;
            case cix_command::RM: 
               cix_rm (server, filename);
               break;
            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

