add_definitions(-DSTATICLIB)

set(
    AMITY_INCLUDE_DIRS

    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/external/rocksdb/include
    ${PROJECT_SOURCE_DIR}/external/linenoise
    ${PROJECT_SOURCE_DIR}/external/json
    ${PROJECT_SOURCE_DIR}/external/cxxopts
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${AMITY_VERSION_INCLUDE_DIR}
)


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
foreach(source_file ${Common} ${Crypto} ${CryptoNoteCore} ${CryptoNoteProtocol} ${AmityCoind} ${JsonRpcServer} ${Http} ${Logging} ${miner} ${Mnemonics} ${NodeRpcProxy} ${P2p} ${Rpc} ${Serialization} ${System} ${Transfers} ${Wallet} ${WalletLegacy} ${zedwallet})
  if(AMITY_CXX_FLAGS)
    set_source_files_properties(${source_file} PROPERTIES COMPILE_FLAGS "${AMITY_CXX_FLAGS} /W3 /GS-")
  endif() # AMITY_CXX_FLAGS
  source_group("" FILES ${source_file})
endforeach()

add_library(BlockchainExplorer STATIC ${BlockchainExplorer})
add_library(Common STATIC ${Common})
add_library(Crypto STATIC ${Crypto})
add_library(CryptoNoteCore STATIC ${CryptoNoteCore})
add_library(Http STATIC ${Http})
add_library(Logging STATIC ${Logging})
add_library(Mnemonics STATIC ${Mnemonics})
add_library(NodeRpcProxy STATIC ${NodeRpcProxy})
add_library(Rpc STATIC ${Rpc})
add_library(P2P STATIC ${CryptoNoteProtocol} ${P2p})
add_library(Serialization STATIC ${Serialization})
add_library(System STATIC ${System})
add_library(Transfers STATIC ${Transfers})
add_library(Wallet STATIC ${Wallet} ${WalletLegacy})
add_library(JsonRpcServer STATIC ${JsonRpcServer})
add_library(WalletService STATIC ${service})

set(
    AMITY_LIBRARIES

    BlockchainExplorer
    Common
    Crypto
    CryptoNoteCore
    Http
    Logging
    Mnemonics
    NodeRpcProxy
    Rpc
    P2P
    Serialization
    System
    Transfers
    Wallet
    JsonRpcServer
    WalletService
)

foreach(amityLib ${AMITY_LIBRARIES})
    target_include_directories(${amityLib} PUBLIC ${AMITY_INCLUDE_DIRS})
endforeach()

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
endif()

if(AMITY_BUILD_EXECUTABLES)
  file(GLOB_RECURSE CommonCLI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/CommonCLI/*")
  add_library(CommonCLI STATIC ${CommonCLI_SOURCES})
  target_include_directories(CommonCLI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/CommonCLI" ${AMITY_INCLUDE_DIRS})
  add_dependencies(CommonCLI version)

  add_executable(AmityCoind ${AmityCoind} ${DAEMON_SOURCES_OS})
  add_executable(zedwallet ${zedwallet} ${ZED_WALLET_SOURCES_OS})
  add_executable(service ${service} ${PG_SOURCES_OS})
  add_executable(miner ${miner} ${MINER_SOURCES_OS})

  if(MSVC)
    target_link_libraries(AmityCoind Rpcrt4)
    target_link_libraries(service Rpcrt4)
  endif()
endif() # AMITY_BUILD_EXECUTABLES

if(MSVC)
  target_link_libraries(System ws2_32)
endif ()

# Add the dependencies we need
target_link_libraries(
    CryptoNoteCore

    PUBLIC
        Common
        Logging
        Crypto
        P2P
        Rpc
        Http
        Serialization
        System
        ${Boost_LIBRARIES}
)
target_link_libraries(P2P PUBLIC CryptoNoteCore upnpc-static)
target_link_libraries(Rpc PUBLIC CryptoNoteCore)
target_link_libraries(Wallet NodeRpcProxy Transfers Rpc P2P upnpc-static Http Serialization CryptoNoteCore System Logging Common Crypto ${Boost_LIBRARIES})
target_link_libraries(WalletService PUBLIC Mnemonics CommonCLI)

if(AMITY_BUILD_EXECUTABLES)
  if(MSVC)
    target_link_libraries(AmityCoind P2P Rpc Serialization System Http Logging CryptoNoteCore Crypto Common CommonCLI rocksdb ${Boost_LIBRARIES} )
  else()
    target_link_libraries(AmityCoind P2P Rpc Serialization System Http Logging CryptoNoteCore Crypto Common CommonCLI rocksdblib ${Boost_LIBRARIES} )
  endif()

  target_link_libraries(zedwallet CommonCLI Mnemonics Wallet NodeRpcProxy Transfers Rpc Http CryptoNoteCore System Logging Common CommonCLI ${Boost_LIBRARIES})
  target_link_libraries(service CommonCLI WalletService JsonRpcServer Wallet NodeRpcProxy Transfers CryptoNoteCore Crypto Rpc Http System Logging Common CommonCLI ${Boost_LIBRARIES})
  target_link_libraries(miner CommonCLI CryptoNoteCore Rpc Serialization System Http Logging Common CommonCLI Crypto ${Boost_LIBRARIES})
endif() # AMITY_BUILD_EXECUTABLES

add_dependencies(WalletService version)
add_dependencies(Rpc version)
add_dependencies(P2P version)

if(AMITY_BUILD_EXECUTABLES)
  # Add dependencies means we have to build the latter before we build the former
  # In this case it's because we need to have the current version name rather
  # than a cached one
  add_dependencies(AmityCoind version)
  add_dependencies(zedwallet version)
  add_dependencies(service version)
  add_dependencies(miner version)

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
