#pragma once

#include <string>

namespace services {

class AuthService {
 public:
  static bool changePassword(const std::string& user,
                             const std::string& oldPassword,
                             const std::string& newPassword);

  static bool verifyPassword(const std::string& user,
                             const std::string& password);

 private:
  // Get password file path for user
  static std::string getPasswordFilePath(const std::string& user);

  // Hash password using bcrypt
  static std::string hashPassword(const std::string& password);

  // Read stored password hash from file
  static std::string readPasswordHash(const std::string& user);

  // Initialize password file for user if it doesn't exist
  static std::string initPassword(const std::string& user);

  // Write password hash to file
  static void writePasswordHash(const std::string& user,
                                const std::string& hash);
};

}  // namespace services
