# Gtkmm-PLplot

Gtkmm-PLplot is a (currently very) simple plotting library that brings the power of PLplot to Gtkmm. It does so by relying on the Gtk::DrawingArea widget whose drawing signal uses PLplot's extcairo plotting device to generate publication quality plots on your screen...

Currently this library is under development but it should already be useful for simple two-dimensional graphs. 
It allows for zooming in by dragging a rectangle with the mouse button pressed in, while zooming out is performed using a mouse double-click.

What is needed to compile Gtkmm-PLplot:

	* Gtkmm (minimum 3.0.0)
	* PLplot (minimum version unknown, but the C++ bindings are required)
	* a C++11 compiler (GCC, clang...)

Development occurs mostly on a Mac but I will ensure it also runs properly on Windows and Linux...


Tom Schoonjans
