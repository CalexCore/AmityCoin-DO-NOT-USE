add_definitions(-DSTATICLIB)

include_directories(${CMAKE_SOURCE_DIR}/external/rocksdb/include)
include_directories(${CMAKE_SOURCE_DIR}/external/linenoise)

file(GLOB_RECURSE BlockchainExplorer BlockchainExplorer/*)
file(GLOB_RECURSE Common Common/*)
file(GLOB_RECURSE Crypto crypto/*)
file(GLOB_RECURSE CryptoNoteCore CryptoNoteCore/* CryptoNoteConfig.h)
file(GLOB_RECURSE CryptoNoteProtocol CryptoNoteProtocol/*)
file(GLOB_RECURSE AmityCoind Daemon/*)
file(GLOB_RECURSE Http HTTP/*)
file(GLOB_RECURSE Logging Logging/*)
file(GLOB_RECURSE miner Miner/*)
file(GLOB_RECURSE Mnemonics Mnemonics/*)
file(GLOB_RECURSE NodeRpcProxy NodeRpcProxy/*)
file(GLOB_RECURSE P2p P2p/*)
file(GLOB_RECURSE Rpc Rpc/*)
file(GLOB_RECURSE Serialization Serialization/*)
file(GLOB_RECURSE Transfers Transfers/*)
file(GLOB_RECURSE Wallet Wallet/*)
file(GLOB_RECURSE WalletLegacy WalletLegacy/*)
file(GLOB_RECURSE JsonRpcServer JsonRpcServer/*)
file(GLOB_RECURSE service WalletService/*)
file(GLOB_RECURSE zedwallet zedwallet/*)
file(GLOB_RECURSE CryptoTest CryptoTest/*)

if(MSVC)
file(GLOB_RECURSE System System/* Platform/Windows/System/*)
elseif(APPLE)
file(GLOB_RECURSE System System/* Platform/OSX/System/* Platform/Posix/System/*)
else()
file(GLOB_RECURSE System System/* Platform/Linux/System/* Platform/Posix/System/*)
endif()

# This appears to be an IDE thing, to group files together.
# https://cmake.org/cmake/help/v3.0/command/source_group.html
# Probably not what you need to be looking at if something isn't building
source_group("" FILES $${Common} ${Crypto} ${CryptoNoteCore} ${CryptoNoteProtocol} ${AmityCoind} ${JsonRpcServer} ${Http} ${Logging} ${miner} ${Mnemonics} ${NodeRpcProxy} ${P2p} ${Rpc} ${Serialization} ${System} ${Transfers} ${Wallet} ${WalletLegacy} ${zedwallet} ${CryptoTest})

add_library(BlockchainExplorer ${BlockchainExplorer})
add_library(Common ${Common})
add_library(Crypto ${Crypto})
add_library(CryptoNoteCore ${CryptoNoteCore})
add_library(Http ${Http})
add_library(Logging ${Logging})
add_library(Mnemonics ${Mnemonics})
add_library(NodeRpcProxy ${NodeRpcProxy})
add_library(Rpc ${Rpc})
add_library(P2P ${CryptoNoteProtocol} ${P2p})
add_library(Serialization ${Serialization})
add_library(System ${System})
add_library(Transfers ${Transfers})
add_library(Wallet ${Wallet} ${WalletLegacy})
add_library(JsonRpcServer ${JsonRpcServer})
add_library(WalletService ${service})

if(MSVC)
  set(DAEMON_SOURCES_OS
    BinaryInfo/daemon.rc
  )
  set(ZED_WALLET_SOURCES_OS
    BinaryInfo/zedwallet.rc
  )
  set(MINER_SOURCES_OS
    BinaryInfo/miner.rc
  )
  set(PG_SOURCES_OS
    BinaryInfo/service.rc
  )
  set(CT_SOURCES_OS
    BinaryInfo/cryptotest.rc
  )
endif()

if(AMITY_BUILD_EXECUTABLES)
  add_executable(AmityCoind ${AmityCoind} ${DAEMON_SOURCES_OS})
  add_executable(zedwallet ${zedwallet} ${ZED_WALLET_SOURCES_OS})
  add_executable(service ${service} ${PG_SOURCES_OS})
  add_executable(miner ${miner} ${MINER_SOURCES_OS})
endif() # AMITY_BUILD_EXECUTABLES

if(AMITY_BUILD_CRYPTOTEST)
  add_executable(cryptotest ${CryptoTest} ${CT_SOURCES_OS})
endif() # AMITY_BUILD_CRYPTOTEST

if(MSVC)
  target_link_libraries(System ws2_32)
  target_link_libraries(AmityCoind Rpcrt4)
  target_link_libraries(service Rpcrt4)
endif ()

# Add the dependencies we need
target_link_libraries(CryptoNoteCore Common Logging Crypto P2P Rpc Http Serialization System ${Boost_LIBRARIES})
target_link_libraries(CryptoNoteCore Common Crypto Logging Serialization)
target_link_libraries(P2P CryptoNoteCore Logging ${Boost_LIBRARIES} upnpc-static)
target_link_libraries(Rpc CryptoNoteCore Logging P2P)
target_link_libraries(WalletService Mnemonics)

if(AMITY_BUILD_EXECUTABLES)
  if(MSVC)
    target_link_libraries(AmityCoind P2P Rpc Serialization System Http Logging CryptoNoteCore Crypto Common rocksdb ${Boost_LIBRARIES} )
  else()
    target_link_libraries(AmityCoind P2P Rpc Serialization System Http Logging CryptoNoteCore Crypto Common rocksdblib ${Boost_LIBRARIES} )
  endif()

  target_link_libraries(zedwallet Mnemonics Wallet NodeRpcProxy Transfers Rpc Http CryptoNoteCore System Logging Common ${Boost_LIBRARIES})
  target_link_libraries(service WalletService JsonRpcServer Wallet NodeRpcProxy Transfers CryptoNoteCore Crypto Rpc Http System Logging Common ${Boost_LIBRARIES})
  target_link_libraries(Wallet NodeRpcProxy Transfers Rpc P2P upnpc-static Http Serialization CryptoNoteCore System Logging Common Crypto ${Boost_LIBRARIES})
  target_link_libraries(miner CryptoNoteCore Rpc Serialization System Http Logging Common Crypto ${Boost_LIBRARIES})
endif() # AMITY_BUILD_EXECUTABLES

if(AMITY_BUILD_CRYPTOTEST)
  target_link_libraries(cryptotest Crypto Common)
  add_test(cryptotest cryptotest)
endif() # AMITY_BUILD_CRYPTOTEST

if(AMITY_BUILD_EXECUTABLES)
  # Add dependencies means we have to build the latter before we build the former
  # In this case it's because we need to have the current version name rather
  # than a cached one
  add_dependencies(WalletService version)
  add_dependencies(Rpc version)
  add_dependencies(AmityCoind version)
  add_dependencies(zedwallet version)
  add_dependencies(service version)
  add_dependencies(P2P version)

  # Finally build the binaries
  set_property(TARGET AmityCoind PROPERTY OUTPUT_NAME "AmityCoind")
  set_property(TARGET zedwallet PROPERTY OUTPUT_NAME "zedwallet")
  set_property(TARGET service PROPERTY OUTPUT_NAME "amity-service")
  set_property(TARGET miner PROPERTY OUTPUT_NAME "miner")

  # Additional make targets
  add_custom_target(pool DEPENDS AmityCoind service)
  add_custom_target(solominer DEPENDS AmityCoind zedwallet miner)
  add_custom_target(cli DEPENDS AmityCoind zedwallet)

  # Install targets
  install(
    TARGETS AmityCoind zedwallet miner service
    RUNTIME DESTINATION bin
  )
endif() # AMITY_BUILD_EXECUTABLES

if(AMITY_BUILD_CRYPTOTEST)
  add_dependencies(cryptotest version)
  set_property(TARGET cryptotest PROPERTY OUTPUT_NAME "cryptotest")
endif() # AMITY_BUILD_CRYPTOTEST
