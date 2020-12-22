![CI](https://github.com/tschoonj/gtkmm-plplot/workflows/CI/badge.svg?branch=master&event=push)  ![Sponsor](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&link=https://github.com/sponsors/tschoonj)  [![PayPal](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.me/tomschoonjans/10)   [![Amazon Wish list](https://img.shields.io/badge/Amazon-Wishlist-green.svg)](http://amzn.eu/8ml3d0c)   [![Gitter](https://badges.gitter.im/gtkmm-plplot/community.svg)](https://gitter.im/gtkmm-plplot/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

# Gtkmm-PLplot

Gtkmm-PLplot is a simple plotting library that brings the power of PLplot to Gtkmm. It does so by relying on the Gtk::DrawingArea widget whose drawing signal uses PLplot's extcairo plotting device to generate publication quality plots on your screen...

Currently this library is under heavy development but it should already be useful for simple two-dimensional graphs. The API should not be considered to be stable at all and will be changed without warning...

Gtkmm-PLplot is licensed under the LGPLv3.

## Features

* Create plots using two std::vector, std::valarray and boost::multi_array objects.
* Zoom in by dragging a selection box.
* Support for two-dimensional plots, polar plots, contour plots with or without shaded regions, three-dimensional line plots
* Supports multiple, independently controlled plots on a single canvas.
* Extensive use of libsigc++ signals
* Highly extensible

## Requirements

* Gtkmm3
* PLplot (at least 5.10.0 but earlier versions may also work, the C++ bindings are required)
* a C++11 compiler (GCC, clang...)
* Boost (optional)

## Supported platforms

Development occurs mostly on a Mac but I will ensure it also runs properly on Windows and Linux...

## Documentation

I have generated documentation with Doxygen. Have a look at http://tschoonj.github.io/gtkmm-plplot.

## Downloads

Tarballs can be found [here](https://github.com/tschoonj/gtkmm-plplot/releases)

Homebrew users as well as Linux aficionados will find installations instructions in the [documentation](https://tschoonj.github.io/gtkmm-plplot/installation_instructions.html)


## TODO
* plsurf3d, plmesh
* ~~histograms~~
* ~~Scroll to zoom in/out~~
* plimage
* ...

Tom Schoonjans
