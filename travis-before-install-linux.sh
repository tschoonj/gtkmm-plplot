sudo add-apt-repository ppa:kalakris/cmake -y || exit 1

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y || exit 1

sudo apt-get update || exit 1

sudo apt-get install -y libplplot-dev libplplot-c++11 libgtkmm-3.0-dev libgtkmm-3.0-doc libffi-dev mm-common autoconf automake libtool doxygen pkg-config cmake libpng12-dev gettext libfreetype6-dev libfontconfig1-dev libjpeg-dev libtiff4-dev intltool libxtst-dev libboost-all-dev || exit 1



if [ $CXX == "g++" ] ; then
	sudo apt-get install -y gcc-4.9 g++-4.9 || exit 1
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 50 || exit 1
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 50 || exit 1
elif [ $CXX == "clang++" ] ; then
	sudo apt-get install -y clang || exit 1
fi
