#include "gtkmm-plplot/canvas.h"
#include "gtkmm-plplot/exception.h"

#include <valarray>
#include <cmath>
#include <gdkmm/general.h>

using namespace Gtk::PLplot;

Canvas::Canvas(const Plot2D &plot, Gdk::RGBA _background_color) :
  Canvas(_background_color) {

  add_plot(plot);
}

Canvas::Canvas(Gdk::RGBA _background_color) :
  start_event{-1.0, -1.0},
  start_cairo{-1.0, -1.0},
  end_event{-1.0, -1.0},
  end_cairo{-1.0, -1.0},
  selecting(false),
  selected_plot(0),
  background_color(_background_color) {

  add_events(Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  signal_changed().connect(sigc::mem_fun(*this, &Canvas::on_changed));
}

void Canvas::add_plot(const Plot2D &plot) {
  plots.push_back(new Plot2D(plot));
  plots.back()->signal_select_region().connect(sigc::mem_fun(*plots.back(),
             &Plot2D::set_region));
  plots.back()->signal_changed().connect([this](){_signal_changed.emit();});
  _signal_changed.emit();
}

Canvas::~Canvas() {
  for (std::vector<Plot2D*>::iterator iter = plots.begin() ;
    iter != plots.end() ;
    ++iter) {
      delete *iter;
  }
}

void Canvas::on_changed() {
  //this catches all signal_changed emissions recursively from the Plot2D and Plot2DData classes
  //so this is the method that ensures things get redrawn when one of the parameters is changed.
  this->queue_draw();
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  //start by drawing the background
  cr->rectangle(0.0, 0.0, width, height);
  Gdk::Cairo::set_source_rgba(cr, background_color);
  cr->fill();

  for (std::vector<Plot2D*>::iterator iter = plots.begin() ;
    iter != plots.end() ;
    ++iter) {
      (*iter)->draw_plot(cr, width, height);
  }
  if (selecting &&
    start_cairo[0] >= 0.0 &&
    start_cairo[1] >= 0.0 &&
    end_cairo[0] >= 0.0 &&
    end_cairo[1] >= 0.0) {

    cr->set_line_width(2);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(MIN(start_cairo[0], end_cairo[0]),
                  MIN(start_cairo[1], end_cairo[1]),
                  fabs(end_cairo[0] - start_cairo[0]),
                  fabs(end_cairo[1] - start_cairo[1]));
    cr->stroke();
  }
  return TRUE;
}

bool Canvas::on_button_press_event(GdkEventButton *event) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  start_event[0] = event->x;
  start_event[1] = event->y;
  start_cairo[0] = event->x;
  start_cairo[1] = height - 1.0 * event->y;
  end_event[0] = -1.0;
  end_event[1] = -1.0;
  end_cairo[0] = -1.0;
  end_cairo[1] = -1.0;

  //check if the starting coordinates are valid
  for (unsigned int plot = 0 ; plot < plots.size() ; plot++) {
    if (start_cairo[0] >= plots[plot]->cairo_range_x[0] &&
      start_cairo[0] <= plots[plot]->cairo_range_x[1] &&
      start_cairo[1] >= plots[plot]->cairo_range_y[0] &&
      start_cairo[1] <= plots[plot]->cairo_range_y[1]) {
      if (event->type == GDK_2BUTTON_PRESS) {
        std::valarray<double> plot_data_range_x = {plots[plot]->plot_data_range_x[0], plots[plot]->plot_data_range_x[1]};
        std::valarray<double> plot_data_range_y = {plots[plot]->plot_data_range_y[0], plots[plot]->plot_data_range_y[1]};
        if (plots[plot]->log10_x)
          plot_data_range_x = std::pow(10.0, plot_data_range_x);
        if (plots[plot]->log10_y)
          plot_data_range_y = std::pow(10.0, plot_data_range_y);
        plots[plot]->set_region(
          plot_data_range_x[0],
          plot_data_range_x[1],
          plot_data_range_y[0],
          plot_data_range_y[1]
        );
        return false;
      }
      selecting = true;
      selected_plot = plot;
      _signal_changed.emit();
      return false;
    }
  }

  selecting = false;

  return true;
}

bool Canvas::on_button_release_event(GdkEventButton *event) {
  if (!selecting)
    return true;

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  end_event[0] = event->x;
  end_event[1] = event->y;
  end_cairo[0] = event->x;
  end_cairo[1] = height - 1.0 * event->y;

  //make sure we stay within the plot while selecting
  if (end_cairo[0] > start_cairo[0]) {
    //this 1E-10 subtraction is necessary to ensure calc_world works properly
    //when dragging a box that touches the right axis.
    end_cairo[0] = MIN(end_cairo[0], plots[selected_plot]->cairo_range_x[1] - 1E-10);
  }
  else if (end_cairo[0] < start_cairo[0]) {
    end_cairo[0] = MAX(end_cairo[0], plots[selected_plot]->cairo_range_x[0]);
  }

  if (end_cairo[1] > start_cairo[1]) {
    end_cairo[1] = MIN(end_cairo[1], plots[selected_plot]->cairo_range_y[1] - 1E-10);
  }
  else if (end_cairo[1] < start_cairo[1]) {
    end_cairo[1] = MAX(end_cairo[1], plots[selected_plot]->cairo_range_y[0]);
  }

  selecting = false;

  //emit signal!
  //prepare plplot coordinates
  //inspired by https://www.mail-archive.com/plplot-devel@lists.sourceforge.net/msg03079.html
  double start_cairo_norm[2] = {start_cairo[0]/width, start_cairo[1]/height};
  double end_cairo_norm[2] = {end_cairo[0]/width, end_cairo[1]/height};

  double start_plplot[2];
  double end_plplot[2];
  int index;

  //get the plot coordinates corresponding to the cairo coordinates
  plots[selected_plot]->pls->calc_world(start_cairo_norm[0], start_cairo_norm[1],
                  start_plplot[0], start_plplot[1], index);
  plots[selected_plot]->pls->calc_world(end_cairo_norm[0], end_cairo_norm[1],
                  end_plplot[0], end_plplot[1], index);

  double start_plplot_def[2];
  double end_plplot_def[2];

  //ensure that the coordinates are within the extremes based on the x and y vectors
  //in case of the full view, due to precision errors, the extremes calculated based on calc_world
  //are actually slightly outside of these data extremes, meaning that it's not possible to drag the selection
  //along the plot grid
  start_plplot_def[0] = MAX(MIN(start_plplot[0], end_plplot[0]), plots[selected_plot]->plot_data_range_x[0]);
  start_plplot_def[1] = MAX(MIN(start_plplot[1], end_plplot[1]), plots[selected_plot]->plot_data_range_y[0]);
  end_plplot_def[0] = MIN(MAX(start_plplot[0], end_plplot[0]), plots[selected_plot]->plot_data_range_x[1]);
  end_plplot_def[1] = MIN(MAX(start_plplot[1], end_plplot[1]), plots[selected_plot]->plot_data_range_y[1]);

  _signal_changed.emit();

  if (plots[selected_plot]->log10_x) {
    start_plplot_def[0] = pow(10.0, start_plplot_def[0]);
    end_plplot_def[0] = pow(10.0, end_plplot_def[0]);
  }

  if (plots[selected_plot]->log10_y) {
    start_plplot_def[1] = pow(10.0, start_plplot_def[1]);
    end_plplot_def[1] = pow(10.0, end_plplot_def[1]);
  }


  plots[selected_plot]->_signal_select_region.emit(start_plplot_def[0], end_plplot_def[0], start_plplot_def[1], end_plplot_def[1]);

  return true;
}

bool Canvas::on_motion_notify_event (GdkEventMotion *event) {
  if (!selecting)
    return true;

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  end_event[0] = event->x;
  end_event[1] = event->y;
  end_cairo[0] = event->x;
  end_cairo[1] = height - 1.0 * event->y;

  //make sure we stay within the plot while selecting
  if (end_cairo[0] > start_cairo[0]) {
    end_cairo[0] = MIN(end_cairo[0], plots[selected_plot]->cairo_range_x[1]);
  }
  else if (end_cairo[0] < start_cairo[0]) {
    end_cairo[0] = MAX(end_cairo[0], plots[selected_plot]->cairo_range_x[0]);
  }

  if (end_cairo[1] > start_cairo[1]) {
    end_cairo[1] = MIN(end_cairo[1], plots[selected_plot]->cairo_range_y[1]);
  }
  else if (end_cairo[1] < start_cairo[1]) {
    end_cairo[1] = MAX(end_cairo[1], plots[selected_plot]->cairo_range_y[0]);
  }

  _signal_changed.emit();

  return true;
}

Plot2D *Canvas::get_plot(unsigned int index) {
  if (index < plots.size()) {
    return plots[index];
  }
  throw Exception("Gtk::PLplot::Canvas::get_plot -> Invalid index");
}

Gdk::RGBA Canvas::get_background_color() {
  return background_color;
}
void Canvas::set_background_color(Gdk::RGBA _background_color) {
  background_color = _background_color;
  _signal_changed.emit();
}
