#include "workspaceController.h"

#include <stdexcept>

#include "../services/authService.h"
#include "../services/workspaceService.h"
#include "../utils/utils.h"

namespace controllers {

void WorkspaceController::handleCompress(int client, const std::string& body) {
  try {
    std::string user = utils::validateUser(body);

    services::WorkspaceService::compress(user);

    utils::sendHttpResponse(client, 200, utils::jsonMsg(true, "Compressed"));
  } catch (const std::invalid_argument& e) {
    utils::sendHttpResponse(client, 400, utils::jsonMsg(false, e.what()));
  } catch (const std::exception& e) {
    utils::sendHttpResponse(client, 500, utils::jsonMsg(false, e.what()));
  }
}

void WorkspaceController::handleExtract(int client, const std::string& body) {
  try {
    std::string user = utils::validateUser(body);
    std::string password = utils::extractJson(body, "password");

    if (!services::AuthService::verifyPassword(user, password)) {
      utils::sendHttpResponse(client, 401,
                              utils::jsonMsg(false, "Invalid password"));
      return;
    }

    services::WorkspaceService::extract(user);

    utils::sendHttpResponse(client, 200, utils::jsonMsg(true, "Extracted"));
  } catch (const std::invalid_argument& e) {
    utils::sendHttpResponse(client, 400, utils::jsonMsg(false, e.what()));
  } catch (const std::exception& e) {
    utils::sendHttpResponse(client, 500, utils::jsonMsg(false, e.what()));
  }
}

}  // namespace controllers
