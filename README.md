# qtflow
## Depencies
### Debian
Install packages:

 sudo apt-get install qt5-qmake qtbase5-dev qtbase5-dev-tools qttools5-dev-tools qtscript-tools python-all-dev build-essential

## Clone
Clone submodules as well:

 git clone --recursive https://github.com/leviathanch/qtflow.git

Change into repo:

 cd qtflow

## Configure
Create build dir:

 mkdir build

Change into it:

 cd build

Set qt5 environment:

 export QT_SELECT=qt5

Run qmake:

 qmake ..

## Build
Compile with threads:

 make -j6 

