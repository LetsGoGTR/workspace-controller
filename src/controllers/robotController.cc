#include "robotController.h"

#include <stdexcept>

#include "../services/authService.h"
#include "../utils/utils.h"

namespace controllers {

void RobotController::handleRunning(int client) {
  utils::sendHttpResponse(client, 200, R"({"success":true,"data":false})");
}

void RobotController::handleAuth(int client, const std::string& body) {
  try {
    // Extract user
    std::string user = utils::validateUser(body);

    // Extract passwords
    std::string oldPassword = utils::extractJson(body, "oldPassword");
    std::string newPassword = utils::extractJson(body, "newPassword");

    if (oldPassword.empty() || newPassword.empty()) {
      utils::sendHttpResponse(client, 400,
                              utils::jsonMsg(false, "Missing password fields"));
      return;
    }

    // Change password
    services::AuthService authService;
    bool success = authService.changePassword(user, oldPassword, newPassword);

    if (success) {
      utils::sendHttpResponse(
          client, 200, utils::jsonMsg(true, "Password changed successfully"));
    } else {
      utils::sendHttpResponse(client, 401,
                              utils::jsonMsg(false, "Invalid old password"));
    }
  } catch (const std::invalid_argument& e) {
    utils::sendHttpResponse(client, 400, utils::jsonMsg(false, e.what()));
  } catch (const std::exception& e) {
    utils::sendHttpResponse(client, 500, utils::jsonMsg(false, e.what()));
  }
}

}  // namespace controllers
