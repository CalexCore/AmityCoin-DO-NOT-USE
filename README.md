# AmityCoin

###### We want a decentralized future, we want a decentralized currency, we want a decentralized life.  We want to be different.  We are Amity

#### About AmityCoin

We at Calex believe in project AmityCoin; so much so that we launch with no Premine. AmityCoin aims to change the status quo; to be accepted anywhere good and services are rendered. We believe with this ethos the value of AmityCoin will speak volumes about itself.  Please, join us in shaping the future! We are a unique, CryptoNote privacy coin that aims for fair mining and distribution for decentralisation. We use a unique algorithm called CryptoNight Soft Shell created by our good friends over at TurtleCoin. Soft Shell is a novel adaptation to the Cryptonote hashing algorithm. By dynamically adjusting the scratchpad and memory utilization, we present a CryptoNote coin more ASIC resistant than those which preceed it. At the present time there are is no compatible pool software or GPU-enabled miners, ensuring a more natural growth and network decentralization. During our testing phase, even a moderately powered computing device stood a 'fair' chance of solving a block enabling a truly fair mining experience.

### Coin Information

| Coin | Info |
|:-----:|:-----:|
| Name | AmityCoin |
| Ticker | AMIT |
| Forked | TurtleCoin - TRTL |
| Algorithm | CN Soft Shell v3 |
| Difficulty | LWMA-3 |
| Block Time | 120 seconds |
| Block Reward | ~186 |
| Mature Rate | 360 blocks |
| Decimals | 4 |
| P2P | 21018 |
| RPC | 31018 |
| Max Supply | 100,000,000,000 (100B) |
| Premine | **0** |
| Add' Prefix | amit… |

#### Our journey so far

- [x] **No Premine**
- [x] Using a unique hashing algorithm
- [x] LWMA 3 difficulty algorithm
- [x] Made our mark on ForkMaps
- [x] Made our mark on the TurtleCoin server
- [x] Building a strong team
- [x] Discord tipping bot
- [x] Block explorer

#### What we’re currently doing

- Expanding the team & community
- Working on upgrading the codebase to contribute back to TurtleCoin
- GUI

#### What we have planned

- Upon mainnet, we’re going to launch the Community Buyback System (CBS) where we buy back AMIT from our users.
- Introducing a tipbot to get the flow of coins circulating between users hoping it helps introduce more people to get involved
- Creating a web wallet so you can easily login to wallet via mobile browsers
- Setting up a merchandise shop that accepts AMITs. This helps with the early economy of the coin proving it would be a reliable source of income for merchants
- *and still more yet to be announced*

#### Execution of the CBS

For the Community Buyback System to be successful we will still need to depend on the generosity and dedication of our users.  The system will fail if it is not supported.  To make raising the required amount of AMIT feasible we will be asking that users willing to participate in the system exchange 1 AMIT for 1 TRTL. Because TurtleCoin is where AmityCoin came from we think this is fitting. Users who are interested in taking part in the CBS can contact any of the AmityCoin team members.  We feel strongly that this method of funding future development is both fair to the users as well as the team.  You as the user should not be asked to fund a project in "Hope" of a return. This way early supporters will get at least something other than a thank you for their patronage.  We believe in our project. Will you join us?

#### Screenshots

<h1 align="center"><img title="Daemon" src="https://github.com/Neo-Amity/docs/blob/master/images/daemon.png" ><img/></h1>

<h1 align="center"><img title="Zedwallet" src="https://github.com/Neo-Amity/docs/blob/master/images/wallet.png" ><img/></h1>

***

#### Quick Links

Discord - https://discord.gg/k2jYvBT

Twitter - https://twitter.com/AmityCore

Website - https://www.getamitycoin.org

#### Donations

AMIT: amitWnmgfgYG4XerZGPLNFd5AM87rUkb3X2q4FcELpPsB4DXtT8YE3mETzAjrYLdDH39pJoCxSUHPU2yqZeY1RsJ1h5DgikVAz

TRTL: TRTLuwoBbZb9SyHyEjrbEiYpFQpgTviwj6bPAcd8jbnpBwY2bdqqjw7UDwPSja2afeg5urGXkVp5SAX8NCCLia6DhVoTmsWPg2U 

#### Build Instructions

#### Linux

##### Prerequisites

- You will need the following packages: boost (1.55 or higher), rocksdb, cmake, git, gcc (4.9 or higher), g++ (4.9 or higher), make, and python. Most of these should already be installed on your system.
- For example on Ubuntu: `sudo apt-get install -y build-essential python-dev gcc g++ git cmake libboost-all-dev`

##### Building

- `git clone https://github.com/CalexCore/AmityCoin.git`
- `cd AmityCoin`
- `mkdir build && cd $_`
- `cmake ..`
- `make`

#### Apple

##### Prerequisites

- Install [cmake](https://cmake.org/). See [here](https://stackoverflow.com/questions/23849962/cmake-installer-for-mac-fails-to-create-usr-bin-symlinks) if you are unable call `cmake` from the terminal after installing.
- Install the [boost](http://www.boost.org/) libraries. Either compile boost manually or run `brew install boost`.
- Install XCode and Developer Tools.

##### Building

- `git clone https://github.com/CalexCore/AmityCoin.git`
- `cd AmityCoin`
- `mkdir build && cd $_`
- `cmake ..` or `cmake -DBOOST_ROOT=<path_to_boost_install> ..` when building
  from a specific boost install. If you used brew to install boost, your path is most likely `/usr/local/include/boost.`
- `make`

The binaries will be in `./src` after compilation is complete.

Run `./src/AmityCoind` to connect to the network and let it sync (it may take a while).

#### Windows 10

##### Prerequisites
- Install [Visual Studio 2017 Community Edition](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15&page=inlineinstall)
- When installing Visual Studio, it is **required** that you install **Desktop development with C++** and the **VC++ v140 toolchain** when selecting features. The option to install the v140 toolchain can be found by expanding the "Desktop development with C++" node on the right. You will need this for the project to build correctly. This item will be called something like `VC++ 2015.3 v14.00 (v140) toolset for desktop`
- Install [Boost 1.59.0](https://sourceforge.net/projects/boost/files/boost-binaries/1.59.0/), ensuring you download the installer for MSVC 14.

##### Building

- From the start menu, open 'x64 Native Tools Command Prompt for vs2017'.
- `cd <your_AmityCoin_directory>`
- `mkdir build`
- `cd build`
- Set the PATH variable for cmake: ie. `set PATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin";%PATH%`
- `cmake -G "Visual Studio 14 Win64" .. -DBOOST_ROOT=C:/local/boost_1_59_0` (Or your boost installed dir.)
- `MSBuild Celestial.sln /p:Configuration=Release /m`
- If all went well, it will complete successfully, and you will find all your binaries in the '..\build\src\Release' directory.
- Additionally, a `.sln` file will have been created in the `build` directory. If you wish to open the project in Visual Studio with this, you can.


#### Thanks
Cryptonote Developers, Bytecoin Developers, Monero Developers, Forknote Project, TurtleCoin Community, Celestial Cash

### Copy and paste into edited files

Please put this at the top of files you use retrieved from this repository when you submit pull requests or fork. You can read more in the LICENSE file.


```
// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2014-2018, The Monero Project
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Celestial Developers
// 
// Please see the included LICENSE file for more info

