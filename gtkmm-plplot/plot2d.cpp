#include "gtkmm-plplot/plot2d.h"
#include "gtkmm-plplot/exception.h"

using namespace Gtk::PLplot;

Plot2D::Plot2D(
  const Plot2DData &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title) :
  log10_x(false),
  log10_y(false),
  axis_title_x(_axis_title_x),
  axis_title_y(_axis_title_y),
  plot_title(_plot_title),
  pls(nullptr) {
  plot_data.push_back(new Plot2DData(_data));
  //connect our default signal handlers
  this->signal_select_region().connect(sigc::mem_fun(*this, &Plot2D::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &Plot2D::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &Plot2D::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &Plot2D::on_data_removed));
  plot_data[0]->signal_changed().connect([this](){_signal_changed.emit();});
  _signal_data_added.emit(plot_data.back());
}

Plot2D::Plot2D(const Plot2D &_source) :
  log10_x(_source.log10_x),
  log10_y(_source.log10_y),
  axis_title_x(_source.axis_title_x),
  axis_title_y(_source.axis_title_y),
  plot_title(_source.plot_title),
  pls(nullptr) {

  this->signal_select_region().connect(sigc::mem_fun(*this, &Plot2D::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &Plot2D::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &Plot2D::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &Plot2D::on_data_removed));

  for (auto &iter : _source.plot_data) {
    plot_data.push_back(new Plot2DData(*iter));
  }
  for (auto &iter : plot_data) {
    iter->signal_changed().connect([this](){_signal_changed.emit();});
  }
  if (!plot_data.empty())
    _signal_data_added.emit(plot_data.back());
}

Plot2D::~Plot2D() {
  for (auto &iter : plot_data) {
    delete iter;
  }
  if (pls)
    delete pls;
}

void Plot2D::on_select_region(double xmin, double xmax, double ymin, double ymax) {
  //this function does nothing
  //it is designed to be overridden by a derived class
}

void Plot2D::on_changed() {
  //this function does nothing
  //it is designed to be overridden by a derived class
}

void Plot2D::on_data_added(Plot2DData *added_data) {
  plot_data_modified();
}

void Plot2D::on_data_removed() {
  plot_data_modified();
}

void Plot2D::plot_data_modified() {
  //update ranges
  std::vector<PLFLT> min_x, min_y;
  std::vector<PLFLT> max_x, max_y;

  for (auto &iter : plot_data) {
    min_x.push_back(*std::min_element(iter->x.begin(), iter->x.end()));
    max_x.push_back(*std::max_element(iter->x.begin(), iter->x.end()));
    min_y.push_back(*std::min_element(iter->y.begin(), iter->y.end()));
    max_y.push_back(*std::max_element(iter->y.begin(), iter->y.end()));
  }
  plot_data_range_x[0] = *std::min_element(min_x.begin(), min_x.end());
  plot_data_range_x[1] = *std::max_element(max_x.begin(), max_x.end());
  plot_data_range_y[0] = *std::min_element(min_y.begin(), min_y.end());
  plot_data_range_y[1] = *std::max_element(max_y.begin(), max_y.end());

  plotted_range_x[0] = plot_data_range_x[0];
  plotted_range_x[1] = plot_data_range_x[1];
  plotted_range_y[0] = plot_data_range_y[0];
  plotted_range_y[1] = plot_data_range_y[1];
  //_signal_changed.emit();
}

void Plot2D::add_data(const Plot2DData &data) {
  plot_data.push_back(new Plot2DData(data));
  plot_data.back()->signal_changed().connect([this](){_signal_changed.emit();});

  _signal_data_added.emit(plot_data.back());
}

void Plot2D::set_axis_logarithmic_x(bool _log10) {
  //need to check that all values are positive!
  if (_log10) {
    for (auto &iter : plot_data) {
      if (std::count_if(iter->x.begin(), iter->x.end(), std::bind2nd(std::less_equal<PLFLT>(), double(0.0))) > 0) {
        throw Exception("Gtkmm::Plplot::Plot2D::set_axis_logarithmic_x -> plot X-values must be strictly positive");
      }
    }
  }
  log10_x = _log10;
  _signal_changed.emit();
}

void Plot2D::set_axis_logarithmic_y(bool _log10) {
  //need to check that all values are positive!
  if (_log10) {
    for (auto &iter : plot_data) {
      if (std::count_if(iter->y.begin(), iter->y.end(), std::bind2nd(std::less_equal<PLFLT>(), double(0.0))) > 0) {
        throw Exception("Gtkmm::Plplot::Plot2D::set_axis_logarithmic_y -> plot Y-values must be strictly positive");
      }
    }
  }
  log10_y = _log10;
  _signal_changed.emit();
}

bool Plot2D::get_axis_logarithmic_x() {
  return log10_x;
}

bool Plot2D::get_axis_logarithmic_y() {
  return log10_y;
}

void Plot2D::set_axis_title_x(Glib::ustring &title) {
  axis_title_x = title;
  _signal_changed.emit();
}

void Plot2D::set_axis_title_y(Glib::ustring &title) {
  axis_title_y = title;
  _signal_changed.emit();
}

Glib::ustring Plot2D::get_axis_title_x() {
  return axis_title_x;
}

Glib::ustring Plot2D::get_axis_title_y() {
  return axis_title_y;
}

void Plot2D::set_plot_title(Glib::ustring &title) {
  plot_title = title;
  _signal_changed.emit();
}

Glib::ustring Plot2D::get_plot_title() {
  return plot_title;
}

void Plot2D::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  plot_width = width;
  plot_height= height;
  if (pls)
    delete pls;
  pls = new plstream;

  pls->sdev("extcairo");
  pls->spage(0.0, 0.0, plot_width , plot_height, 0, 0);
  pls->init();

  //Gdk::RGBA color = get_style_context()->get_color();
  //Gdk::Cairo::set_source_rgba(cr, color);
  pls->cmd(PLESC_DEVINIT, cr->cobj());
  pls->col0(0);

  pls->env(plotted_range_x[0], plotted_range_x[1],
           plotted_range_y[0], plotted_range_y[1],
           0, 0);

  pls->lab(axis_title_x.c_str(), axis_title_y.c_str(), plot_title.c_str());

  for (auto &iter : plot_data) {
    if (iter->shown) {
      pls->col0(iter->color);
      pls->line(iter->x.size(), &iter->x[0], &iter->y[0]);
    }
  }
  convert_plplot_to_cairo_coordinates(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  convert_plplot_to_cairo_coordinates(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}


void Plot2D::convert_plplot_to_cairo_coordinates(
  double x_pl, double y_pl,
  double &x_cr, double &y_cr) {
    //inspired by http://www.mail-archive.com/plplot-devel@lists.sourceforge.net/msg02383.html
    //but the last equation was incorrect and is fixed here
    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    pls->gvpd(nxmin, nxmax, nymin, nymax);
    pls->gvpw(wxmin, wxmax, wymin, wymax);

    double xmin = plot_width * nxmin;
    double xmax = plot_width * nxmax;
    double ymin = plot_height * nymin;
    double ymax = plot_height * nymax;

    x_cr = xmin + ((xmax - xmin) * ((x_pl - wxmin) / (wxmax - wxmin)));
    y_cr = ymin + ((ymax - ymin) * ((y_pl - wymin) / (wymax - wymin)));
}

void Plot2D::set_region(double xmin, double xmax, double ymin, double ymax) {
  if (xmin == xmax && ymin == ymax) {
    //due to signal propagation, this function will actually be called twice on a double-click event,
    //the second time after the plot has already been resized to its normal geometry
    //this condition avoids the warning message...
    return;
  }
  if (xmin >= xmax || ymin >= ymax ||
      xmin < plot_data_range_x[0] ||
      xmax > plot_data_range_x[1] ||
      ymin < plot_data_range_y[0] ||
      ymax > plot_data_range_y[1]) {
    throw Exception("Gtk::PLplot::Plot2D::set_region(): Invalid arguments");
  }
  plotted_range_x[0] = xmin;
  plotted_range_x[1] = xmax;
  plotted_range_y[0] = ymin;
  plotted_range_y[1] = ymax;

  _signal_changed.emit();
}

Plot2DData *Plot2D::get_data(unsigned int index) {
  if (index < plot_data.size()) {
    return plot_data[index];
  }
  throw Exception("Gtk::PLplot::Plot2D::get_data -> Invalid index");
}
