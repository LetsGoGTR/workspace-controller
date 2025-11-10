#pragma once

#include <string>

namespace services {

class WorkspaceService {
 public:
  static void compress(const std::string& user);
  static void extract(const std::string& user);
};

}  // namespace services
