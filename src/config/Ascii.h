// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information

#pragma once

const std::string windowsAsciiArt =
   "\n══════════════════════════════════PRESENTING════════════════════════════════\n"
     "    ###    ##     ## #### ######## ##    ##  ######   #######  #### ##    ##\n"
     "   ## ##   ###   ###  ##     ##     ##  ##  ##    ## ##     ##  ##  ###   ##\n"
     "  ##   ##  #### ####  ##     ##      ####   ##       ##     ##  ##  ####  ##\n"
     " ##     ## ## ### ##  ##     ##       ##    ##       ##     ##  ##  ## ## ##\n"
     " ######### ##     ##  ##     ##       ##    ##       ##     ##  ##  ##  ####\n"
     " ##     ## ##     ##  ##     ##       ##    ##    ## ##     ##  ##  ##   ###\n"
     " ##     ## ##     ## ####    ##       ##     ######   #######  #### ##    ##\n"
     " ══════════════════════════════CN═SOFT═SHELL═V3═════════════════════════════\n";


const std::string nonWindowsAsciiArt = 

  "\n═══════════════════════════════════PRESENTING═══════════════════════════════════\n"
    "██╗   █████╗ ███╗   ███╗██╗████████╗██╗   ██╗ ██████╗ ██████╗ ██╗███╗   ██╗  ██╗\n"
    "╚██╗ ██╔══██╗████╗ ████║██║╚══██╔══╝╚██╗ ██╔╝██╔════╝██╔═══██╗██║████╗  ██║ ██╔╝\n"
    " ╚██╗███████║██╔████╔██║██║   ██║    ╚████╔╝ ██║     ██║   ██║██║██╔██╗ ██║██╔╝ \n"
    " ██╔╝██╔══██║██║╚██╔╝██║██║   ██║     ╚██╔╝  ██║     ██║   ██║██║██║╚██╗██║╚██╗ \n"
    "██╔╝ ██║  ██║██║ ╚═╝ ██║██║   ██║      ██║   ╚██████╗╚██████╔╝██║██║ ╚████║ ╚██╗\n"
    "╚═╝  ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝   ╚═╝      ╚═╝     ╚════╝  ╚════╝ ╚═╝╚═╝   ╚══╝  ╚═╝\n"
    "════════════════════════════════CN═SOFT═SHELL═V3════════════════════════════════\n";


/* Windows has some characters it won't display in a terminal. If your ascii
   art works fine on Windows and Linux terminals, just replace 'asciiArt' with
   the art itself, and remove these two #ifdefs and above ascii arts */
#ifdef _WIN32
const std::string asciiArt = windowsAsciiArt;
#else
const std::string asciiArt = nonWindowsAsciiArt;
#endif
