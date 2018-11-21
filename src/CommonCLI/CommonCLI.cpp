#include "CommonCLI.h"

#include <sstream>
#include <iostream>

#include "version.h"
#include "config/Ascii.h"
#include "config/CryptoNoteConfig.h"

// clang-format off
const std::string DevelpmentVersionHeader = R"(
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ATTENTION   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!                                                                                       !
! You are running a development version! The program may contain bugs or is not         !
! compatible with the mainnet. In case you accidentally ran into this version and do    !
! not want to use it to for testing purposes you should visit our github page           !
!     https://github.com/CalexCore/AmityCoin/releases                                   !
! . Or if you want to build amity yourself make sure you are building from the master   !
! branch.                                                                               !
!                                                                                       !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ATTENTION   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
)";
// clang-format on

std::string CommonCLI::header()
{
  std::stringstream programHeader;
  programHeader << std::endl
    << asciiArt << std::endl
    << " " << CryptoNote::CRYPTONOTE_NAME << " v" << PROJECT_VERSION_LONG << std::endl
    << " This software is distributed under the General Public License v3.0"
    << std::endl << std::endl
    << " " << PROJECT_COPYRIGHT
    << std::endl << std::endl
    << " Additional Copyright(s) may apply, please see the included LICENSE file for more information." << std::endl
    << " If you did not receive a copy of the LICENSE, please visit:" << std::endl
    << " " << CryptoNote::LICENSE_URL
    << std::endl << std::endl;

  if(isDevVersion())
  {
    programHeader << DevelpmentVersionHeader << std::endl;
  }

  return programHeader.str();
}

bool CommonCLI::isDevVersion()
{
  return
#ifdef AMITY_DEV_VERSION
      true
#else
      false
#endif
   ;
}

void CommonCLI::verifyDevExecution(int& argc, char **argv)
{
  const std::string devModeFlag{ "--dev-mode" };
  for(int i = 1; i < argc; ++i) {
      if(std::string{argv[i]} == devModeFlag) {
        for(int k = 0; i + k < argc - 1; ++k) {
          argv[i + k] = argv[i + k + 1];
        }
        argc -= 1;
        return;
      }
  }

  if(isDevVersion()) {
    std::cout << header() << "\n You are using a development version and did not provide the --dev-mode flag. Exiting..." << std::endl;
    exit(-1);
  }
}
