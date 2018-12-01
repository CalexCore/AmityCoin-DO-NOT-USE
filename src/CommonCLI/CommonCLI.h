#pragma once

#include <string>

namespace CommonCLI {
  /*!
 * \brief header returns an appropiate header to display including a message telling you you are
 * on a testing version, if so
 * \return a header to display at startup
 */
std::string header();

/*!
 * \brief checks wheter this version was built from non master branch source code
 * \return true if this version is not built from master branch, otherwise false
 */
bool isDevVersion();

/*!
 * \brief verifyDevelopmentExecution checks whether the current version is build from master or if
 * not whether the user provided the --dev-mode flag.
 *
 * If this is a development version and the --dev-mode flag is not provided it will print an
 * error message and exit the program.
 */
void verifyDevExecution(int &argc, char** argv);
}
