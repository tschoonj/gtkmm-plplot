# Gtkmm-PLplot

Gtkmm-PLplot is a (currently very) simple plotting library that brings the power of PLplot to Gtkmm. It does so by relying on the Gtk::DrawingArea widget whose drawing signal uses PLplot's extcairo plotting device to generate publication quality plots on your screen...

Currently this library is under heavy development but it should already be useful for simple two-dimensional graphs. The API should not be considered to be stable at all and will be changed without warning...

## Features

* Create plots using two std::vector or std::valarray instances
* Zoom in by dragging a selection box
* C++ classes Canvas, Plot2D and Plot2DData, designed to be overridden
* Extensive use of libsigc++ signals

## Requirements

* Gtkmm (minimum 3.0.0)
* PLplot (minimum version unknown, but the C++ bindings are required)
* a C++11 compiler (GCC, clang...)

## Supported platforms

Development occurs mostly on a Mac but I will ensure it also runs properly on Windows and Linux...

## Documentation

I have generated documentation with Doxygen. Have a look at http://tschoonj.github.io/gtkmm-plplot.


Tom Schoonjans
