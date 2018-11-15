# AmityCoin Build Instructions


#### Debian/Ubuntu based Linux instructions

##### Prerequisites

-Required Packages: boost 1.55 or higher, rocksdb, gcc 4.9 or higher, g++ 4.9 or higher, git, python, cmake and make.

-One liner to install dependencies: `sudo apt-get install python-dev git cmake libboost-all-dev gcc g++ build-essential -y`

This will install the required dependencies to continue.

#### Building

Run the below commands to build

- `git clone https://github.com/CalexCore/AmityCoin.git`
- `cd AmityCoin`
- `mkdir build && cd $_`
- `cmake ..`
- `make`


#### Mac

##### Prerequisites

- Install [cmake](https://cmake.org/). Please see [here](https://stackoverflow.com/questions/23849962/cmake-installer-for-mac-fails-to-create-usr-bin-symlinks) if you are having issues using `cmake` from the terminal after installing.
- Install the [boost](http://www.boost.org/) libraries. Either compile boost manually or run `brew install boost`.
- Install XCode and Developer Tools.

#### Building

- `git clone https://github.com/CalexCore/AmityCoin.git`
- `cd AmityCoin`
- `mkdir build && cd $_`
- `cmake ..` or `cmake -DBOOST_ROOT=<path_to_boost_install> ..` when building
  from a specific boost install. If you used brew to install boost, your path is most likely going to be `/usr/local/include/boost.`
- `make`

The binaries you just built will be located in the `./src` directory after completing compilation.

You can run `./src/AmityCoind` and the daemon will fire up and begin to sync with the Network. Please not that depending on how large the Blockchain is this may take some time.


#### Windows 10

#### Prerequisites

- The first step is installing [Visual Studio 2017 Community Edition](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15&page=inlineinstall)
- When installing Visual Studio, it is **required an imperitive** that you install **Desktop development with C++** and the **VC++ v140 toolchain** when selecting features during setup. If you do not do this your build will fail. The option to install the v140 toolchain is found by expanding the "Desktop development with C++" option on the right. This item will be called something like `VC++ 2015.3 v14.00 (v140) toolset for desktop`
- The next step is installing [Boost 1.59.0](https://sourceforge.net/projects/boost/files/boost-binaries/1.59.0/), make sure you download the installer for MSVC 14.
- Note** At version 1.66 of boost the naming scheme has changed, making it impossible to use(at least until the makefiles are updated).

#### Thanks
Cryptonote Developers, Bytecoin Developers, Monero Developers, Forknote Project, The Turtlecoin Project, Nerva Developers, AmityCoin
