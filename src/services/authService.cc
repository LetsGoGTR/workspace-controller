#include "authService.h"

#include <crypt.h>
#include <unistd.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <stdexcept>

#include "../utils/config.h"

namespace fs = std::filesystem;

namespace services {

std::string AuthService::getPasswordFilePath(const std::string& user) {
  return Config::PATH_HOME_BASE + user + "/tmp/password";
}

std::string AuthService::hashPassword(const std::string& password) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 61);

  const char* chars =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::string salt = "$6$";
  for (int i = 0; i < 16; i++) {
    salt += chars[dis(gen)];
  }
  salt += "$";

  // Use crypt to hash password
  char* hash = crypt(password.c_str(), salt.c_str());
  if (!hash) {
    throw std::runtime_error("Failed to hash password");
  }

  return std::string(hash);
}

std::string AuthService::readPasswordHash(const std::string& user) {
  std::string path = getPasswordFilePath(user);

  if (!fs::exists(path)) {
    return "";
  }

  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to read password file");
  }

  std::string hash;
  std::getline(file, hash);
  return hash;
}

void AuthService::writePasswordHash(const std::string& user,
                                    const std::string& hash) {
  std::string path = getPasswordFilePath(user);
  std::string dir = Config::PATH_HOME_BASE + user + "/tmp";

  // Create directory if it doesn't exist
  if (!fs::exists(dir)) {
    fs::create_directories(dir);
  }

  std::ofstream file(path, std::ios::trunc);
  if (!file) {
    throw std::runtime_error("Failed to write password file");
  }

  file << hash;
  file.close();

  // Set file permissions to 600 (owner read/write only)
  fs::permissions(path, fs::perms::owner_read | fs::perms::owner_write,
                  fs::perm_options::replace);
}

std::string AuthService::initPassword(const std::string& user) {
  // Set initial password to "0000"
  std::string initialPassword = "0000";
  std::string hash = hashPassword(initialPassword);
  writePasswordHash(user, hash);

  return hash;
}

bool AuthService::verifyPassword(const std::string& user,
                                 const std::string& password) {
  std::string storedHash = readPasswordHash(user);

  // Initialize password if it doesn't exist
  if (storedHash.empty()) {
    storedHash = initPassword(user);
  }

  // Extract salt from stored hash (everything up to last $)
  size_t lastDollar = storedHash.rfind('$');
  if (lastDollar == std::string::npos) {
    return false;
  }
  std::string salt = storedHash.substr(0, lastDollar + 1);

  // Hash provided password with same salt
  char* hash = crypt(password.c_str(), salt.c_str());
  if (!hash) {
    return false;
  }

  if (storedHash != std::string(hash)) {
    return false;
  }

  return true;
}

bool AuthService::changePassword(const std::string& user,
                                 const std::string& oldPassword,
                                 const std::string& newPassword) {
  if (!verifyPassword(user, oldPassword)) {
    return false;
  }

  // Hash and store new password
  std::string newHash = hashPassword(newPassword);
  writePasswordHash(user, newHash);

  return true;
}

}  // namespace services
