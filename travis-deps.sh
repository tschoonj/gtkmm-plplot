function install_dep() {
	FILENAME="${1##*/}"
	EXTENSION="${1##*.}"
	TAR_OPTION=
	case $EXTENSION in
	  gz)
	  	TAR_OPTION="xfz"
		;;
	  xz)
	  	TAR_OPTION="xfJ"
		;;
	  bz2)
	  	TAR_OPTION="xfj"
		;;
	esac
	echo "Installing $FILENAME"
	FILENAME_NOEXT="${FILENAME%.*.*}"
	wget $1 || exit 1
	tar $TAR_OPTION $FILENAME || exit 1
	cd $FILENAME_NOEXT
	./configure || exit 1
	make || exit 1
	sudo make install || exit 1
	cd ..
}


deps="
https://download.gnome.org/sources/glib/2.44/glib-2.44.1.tar.xz
https://download.gnome.org/sources/atk/2.16/atk-2.16.0.tar.xz
http://cairographics.org/releases/pixman-0.32.6.tar.gz
http://cairographics.org/releases/cairo-1.14.2.tar.xz
http://www.freedesktop.org/software/harfbuzz/release/harfbuzz-0.9.41.tar.bz2
https://download.gnome.org/sources/pango/1.36/pango-1.36.8.tar.xz
https://download.gnome.org/sources/gdk-pixbuf/2.30/gdk-pixbuf-2.30.8.tar.xz
https://download.gnome.org/sources/gtk+/3.14/gtk+-3.14.15.tar.xz
https://download.gnome.org/sources/libsigc++/2.4/libsigc++-2.4.1.tar.xz
http://cairographics.org/releases/cairomm-1.11.2.tar.gz
https://download.gnome.org/sources/glibmm/2.44/glibmm-2.44.0.tar.xz
https://download.gnome.org/sources/pangomm/2.36/pangomm-2.36.0.tar.xz
https://download.gnome.org/sources/atkmm/2.22/atkmm-2.22.7.tar.xz
https://download.gnome.org/sources/gtkmm/3.14/gtkmm-3.14.0.tar.xz
"

for dep in $deps ; do
  install_dep $dep
done


#plplot last
wget https://downloads.sourceforge.net/project/plplot/plplot/5.11.0%20Source/plplot-5.11.0.tar.gz || exit 1
tar xfz plplot-5.11.0.tar.gz || exit 1
cd plplot-5.11.0
mkdir our_build || exit 1
cd our_build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. || exit 1
make || exit 1
sudo make install || exit 1
cd ../..
