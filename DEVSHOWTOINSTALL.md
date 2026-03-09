In order to get this running on your machine do the following 

1. be running in a WSL instance
2. install dependencies 
3. have a QT version newer than 6.2

HOW TO INSTALL DEPENDENcIES

copy and paste the following into your terminal


sudo apt update
sudo apt install -y \
  cmake ninja-build g++ \
  qt6-base-dev qt6-declarative-dev \
  qml6-module-qtquick \
  qml6-module-qtqml \
  qml6-module-qtquick-controls \
  qml6-module-qtquick-templates \
  qml6-module-qtquick-layouts \
  qml6-module-qtqml-workerscript


--FROM THE ROOT DIREcTORY run the following.

rm -rf build
cd build
cmake ..
make


After running the following you may still have errors. they are probably because you are still missing packages

