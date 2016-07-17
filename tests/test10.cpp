/*
Copyright (C) 2016 Tom Schoonjans

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* This examples demonstrates how to use Gtkmm-PLplot from Gtk */

#include "gtkmm-plplot.h"
#include <gtkmm/main.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

static void activate (GtkApplication* app, gpointer user_data) {
  Glib::ustring x_title = "X-axis";
  Glib::ustring y_title = "Y-axis";
  Glib::ustring plot_title = "";
  Gdk::RGBA color = Gdk::RGBA("red");
  std::valarray<double> x_va(1000), y_va(1000);
  for (unsigned int i = 0 ; i < 1000 ; i++) {
    x_va[i] = 8*M_PI*i/999;
  }
  y_va = 2*cos(x_va)-1;

  // Gtk::manage even seems to work when the canvas is added to a GTK+ container,
  // instead of Gtkmm::Container!!!
  // This means we will not have to manually free canvas ourselves, which is
  // really cool!
  Gtk::PLplot::Canvas *canvas = Gtk::manage(new Gtk::PLplot::Canvas(
    *Gtk::manage(
      new Gtk::PLplot::Plot2D(
        *Gtk::manage(
          new Gtk::PLplot::PlotData2D(
            x_va,
            y_va,
            color
          )
        ),
        x_title,
        y_title,
        plot_title
      )
    )
  ));

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title (GTK_WINDOW (window), "Gtkmm-PLplot test10");
  gtk_window_set_default_size (GTK_WINDOW (window), 720, 580);
  // print the type name
  std::cout << "Type:" << g_type_name(G_OBJECT_TYPE(canvas->gobj())) << std::endl;
  // this next line is where the magic happens
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(canvas->gobj()));
  gtk_widget_show_all (window);
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  // this next line is necessary to ensure that the gtkmm internals are initialized
  Gtk::Main::init_gtkmm_internals ();
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
