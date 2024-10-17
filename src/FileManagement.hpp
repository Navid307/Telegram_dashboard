#include <boost/filesystem.hpp>
#include <iostream>

std::vector<std::string> get_list_of_files(std::string path) {
  std::vector<std::string> list;
  boost::filesystem::path p(path);
  for (auto i = boost::filesystem::directory_iterator(p);
       i != boost::filesystem::directory_iterator(); i++) {
    list.push_back(i->path().filename().string());
  }
  return list;
}

std::string get_current_dir(void) {
  return boost::filesystem::current_path().string();
}
