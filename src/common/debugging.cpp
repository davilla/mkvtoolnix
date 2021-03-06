/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   debugging functions

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#include "common/common_pch.h"

#include "common/strings/editing.h"

static std::map<std::string, std::string> s_debugging_options;

bool
debugging_requested(const char *option,
                    std::string *arg) {
  std::vector<std::string> options = split(option, "|");

  for (auto &current_option : options) {
    std::map<std::string, std::string>::iterator option_ptr = s_debugging_options.find(current_option);

    if (s_debugging_options.end() != option_ptr) {
      if (arg)
        *arg = option_ptr->second;
      return true;
    }
  }

  return false;
}

bool
debugging_requested(const std::string &option,
                    std::string *arg) {
  return debugging_requested(option.c_str(), arg);
}

void
request_debugging(const std::string &options) {
  std::vector<std::string> all_options = split(options);

  for (auto &one_option : all_options) {
    std::vector<std::string> parts = split(one_option, "=", 2);
    if (parts[0] == "!")
      s_debugging_options.clear();
    else
      s_debugging_options[parts[0]] = 1 == parts.size() ? std::string("") : parts[1];
  }
}

void
init_debugging() {
  std::vector<std::string> env_vars = { "MKVTOOLNIX_DEBUG", "MTX_DEBUG", balg::to_upper_copy(get_program_name()) + "_DEBUG" };

  for (auto &name : env_vars) {
    auto value = getenv(name.c_str());
    if (value)
      request_debugging(value);
  }
}
