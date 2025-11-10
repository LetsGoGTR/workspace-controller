#pragma once

#include <string>

namespace controllers {

class RobotController {
 public:
  // GET /api/robot/running
  void handleRunning(int client);

  // POST /api/robot/password
  void handleAuth(int client, const std::string& body);
};

}  // namespace controllers
