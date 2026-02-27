/**
 * @file process.hpp
 * @brief Cross-platform process spawning (replaces std::system).
 */

#pragma once

#include <filesystem>
#include <string>
#include <vector>


#ifdef _WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace dev {

/// Spawn an executable, wait for it to finish, and return its exit code.
///
/// @param executable  Full path to the child executable.
/// @param argc        Original argc from main().
/// @param argv        Original argv from main().
/// @param arg_offset  Index of the first argument to forward (default: 2,
///                    which skips argv[0]="dev" and argv[1]="<command>").
/// @return            Child exit code, or -1 on spawn failure.
inline int spawn(const std::filesystem::path &executable, int argc,
                 char *argv[], int arg_offset = 2) {

  std::string exe_str = executable.string();

  // Build the child argv: { exe, forwarded args..., nullptr }
  std::vector<const char *> child_argv;
  child_argv.reserve(static_cast<size_t>(argc - arg_offset) + 2);
  child_argv.push_back(exe_str.c_str());
  for (int i = arg_offset; i < argc; ++i) {
    child_argv.push_back(argv[i]);
  }
  child_argv.push_back(nullptr);

#ifdef _WIN32
  // _spawnv blocks until child exits and returns its exit code directly.
  auto rc = _spawnv(_P_WAIT, exe_str.c_str(),
                    const_cast<char *const *>(child_argv.data()));
  return (rc == -1) ? -1 : static_cast<int>(rc);
#else
  pid_t pid = fork();
  if (pid == 0) {
    // Child — replace with plugin executable.
    execv(exe_str.c_str(), const_cast<char **>(child_argv.data()));
    _exit(126); // execv only returns on failure
  }
  if (pid < 0) {
    return -1; // fork failed
  }
  // Parent — wait for child.
  int status = 0;
  waitpid(pid, &status, 0);
  return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
#endif
}

} // namespace dev
