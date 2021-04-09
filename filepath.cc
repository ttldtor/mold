#include "mold.h"

std::string get_current_dir() {
  std::string buf(8192, '\0');
  getcwd(buf.data(), buf.size());
  buf.resize(buf.find_first_of('\0'));
  return buf;
}

std::string path_dirname(std::string_view path) {
  i64 pos = path.find_last_of('/');
  if (pos == path.npos)
    return ".";
  return std::string(path.substr(0, pos));
}

std::string path_filename(std::string_view path) {
  if (path.empty())
    return ".";

  while (path.ends_with('/'))
    path = path.substr(0, path.size() - 2);

  if (path.empty())
    return "/";

  i64 pos = path.find_last_of('/');
  if (pos == path.npos)
    return std::string(path);
  return std::string(path.substr(pos + 1));
}

std::string path_basename(std::string_view path) {
  std::string name = path_filename(path);

  i64 pos = name.find_last_of('.');
  if (pos == name.npos)
    return name;
  return name.substr(0, pos);
}

std::string path_to_absolute(std::string_view path) {
  if (path.starts_with('/'))
    return std::string(path);
  return get_current_dir() + "/" + std::string(path);
}

std::string path_clean(std::string_view path) {
  std::vector<std::string_view> components;
  bool is_rooted = path.starts_with('/');

  while (!path.empty()) {
    size_t pos = path.find('/');
    std::string_view elem;

    if (pos == path.npos) {
      elem = path;
      path = "";
    } else {
      elem = path.substr(0, pos);
      path = path.substr(pos + 1);
    }

    if (elem.empty() || elem == ".")
      continue;

    if (elem == "..") {
      if (components.empty()) {
        if (is_rooted)
          continue;
        components.push_back("..");
        continue;
      }

      if (components.back() == "..")
        components.push_back("..");
      else
        components.pop_back();
      continue;
    }

    components.push_back(elem);
  }

  std::string ret;
  if (is_rooted)
    ret = "/";

  for (i64 i = 0, end = components.size(); i < end; i++) {
    ret += components[i];
    if (i != end - 1)
      ret += "/";
  }
  return ret;
}
