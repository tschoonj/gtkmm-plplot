sudo add-apt-repository ppa:kalakris/cmake -y

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y

sudo apt-get update

sudo apt-get install -y libffi-dev mm-common autoconf automake libtool doxygen pkg-config cmake libpng12-dev gettext libfreetype6-dev libfontconfig1-dev libjpeg-dev libtiff4-dev intltool libxtst-dev



if [ $CXX == "g++" ] ; then
	sudo apt-get install -y g++4.9
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 50
elif [ $CXX == "clang++"] ; then
	sudo apt-get install -y clang
fi
