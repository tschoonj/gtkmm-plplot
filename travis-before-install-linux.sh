sudo add-apt-repository ppa:kalakris/cmake -y || exit 1

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y || exit 1

sudo apt-get update || exit 1

sudo apt-get install -y libffi-dev mm-common autoconf automake libtool doxygen pkg-config cmake libpng12-dev gettext libfreetype6-dev libfontconfig1-dev libjpeg-dev libtiff4-dev intltool libxtst-dev || exit 1



if [ $CXX == "g++" ] ; then
	sudo apt-get install -y g++-4.9 || exit 1
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 50 || exit 1
elif [ $CXX == "clang++" ] ; then
	sudo apt-get install -y clang || exit 1
fi
