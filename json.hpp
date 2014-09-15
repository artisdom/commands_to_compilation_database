#ifndef json_hpp_
#define json_hpp_

#include <iostream>
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <regex>

#include <vector>
#include <string>

#include <cassert>

namespace json
{

   struct compilation_database_entry
   {
      boost::filesystem::path directory;
      std::string command;
      boost::filesystem::path filename;
   };

   typedef std::vector<compilation_database_entry> compilation_database_type;

   std::string
   escape (const std::string & s)
   {
      std::string es;
      es.reserve (2 * s.size ());

      for (const auto & e : s)
      {
         if (e == '"')
         {
            es.push_back ('\\');
            es.push_back ('"');
         }
         else
         {
            es.push_back (e);
         }
      }

      return es;
   }

   compilation_database_type
   load (std::istream & is)
   {
      compilation_database_type compilation_database;

      std::regex command_field_regex ("^\"command\": \"(.+)\",$");
      std::regex directory_field_regex ("^\"directory\": \"(.+)\",$");
      std::regex file_field_regex ("^\"file\": \"(.+)\"$");

      std::string line;
      std::smatch m;

      std::string command;
      std::string directory;
      std::string filename;

      while (is)
      {
         std::getline (is,line);
         boost::trim (line);

         if ((line == "[") || (line == "]"))
         {
            continue;
         }

         std::getline (is,line); // {
         std::getline (is,line); //    "command":
         std::regex_match (line,m,command_field_regex);
         assert (m.ready ());
         if (m.ready ())
         {
            command = m [1];
         }
         std::getline (is,line); //    "directory":
         std::regex_match (line,m,directory_field_regex);
         assert (m.ready ());
         if (m.ready ())
         {
            directory = m [1];
         }
         std::getline (is,line); //    "file":
         std::regex_match (line,m,file_field_regex);
         assert (m.ready ());
         if (m.ready ())
         {
            filename = m [1];
         }
         std::getline (is,line); // },?

         compilation_database_entry entry =
         {
            boost::filesystem::path (directory),
            command,
            boost::filesystem::path (filename)
         };

         compilation_database.push_back (entry);
      }

      return compilation_database;
   }

   void
   dump (const compilation_database_type & compilation_database,
         std::ostream & os)
   {
      os << "[\n";
      for (auto i = std::begin (compilation_database);
           i != std::end (compilation_database);
           ++i)
      {
         // sorted by keys
         os <<
            "  {" << "\n" <<
            "    \"command\": \"" << escape (i->command) << "\", " << "\n" <<
            "    \"directory\": \"" << escape (i->directory.string ()) << "\", " << "\n" <<
            "    \"file\": \"" << escape (i->filename.string ()) << "\"\n"
            "  }" << ((i != (std::end (compilation_database) - 1)) ? ", " : "") << "\n";
      }
      os << "]";
   }

}

#endif
