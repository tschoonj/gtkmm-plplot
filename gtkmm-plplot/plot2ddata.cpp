#include <gtkmm-plplot/plot2ddata.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>

using namespace Gtk::PLplot;

Plot2DData::Plot2DData(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  x(_x), y(_y), color(_color), line_style(_line_style),
  line_width(_line_width), shown(true) {

   //ensure both arrays have the same size
  if (x.size() != y.size()) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data arrays x and y must have the same size!");
  }
  //ensure there are at least two elements in the arrays
  if (x.size() < 2) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data arrays x and y must have at least two elements");
  }
  //ensure the X-values are not all the same
  if (std::count(x.begin(), x.end(), x[0]) == x.size()) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data array x must have at least two different elements");
  }
  this->signal_changed().connect(sigc::mem_fun(*this,
             &Plot2DData::on_changed));
}

Plot2DData::Plot2DData(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Plot2DData(std::vector<PLFLT>(std::begin(_x), std::end(_x)),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

Plot2DData::Plot2DData(
  const std::vector<PLFLT> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Plot2DData(indgen(_y.size()), _y, _color, _line_style, _line_width) {}

Plot2DData::Plot2DData(
  const std::valarray<PLFLT> &_y,
  Gdk::RGBA _color, LineStyle _line_style,
  double _line_width) :
  Plot2DData(std::vector<PLFLT>(indgen(_y.size())),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

Plot2DData::Plot2DData(const Plot2DData &_data) :
  Plot2DData(_data.x, _data.y, _data.color, _data.line_style, _data.line_width) {
    shown = _data.shown;
  }

void Plot2DData::set_color(Gdk::RGBA _color) {
  color = _color;
  _signal_changed.emit();
}

Gdk::RGBA Plot2DData::get_color() {
  return color;
}

void Plot2DData::set_line_style(LineStyle _line_style) {
  line_style = _line_style;
  _signal_changed.emit();
}

LineStyle Plot2DData::get_line_style() {
  return line_style;
}

void Plot2DData::set_line_width(double _line_width) {
  line_width = _line_width;
  _signal_changed.emit();
}

double Plot2DData::get_line_width() {
  return line_width;
}

void Plot2DData::on_changed() {
  //do nothing
}

void Plot2DData::show() {
  shown = true;
  _signal_changed.emit();
}

void Plot2DData::hide() {
  shown = false;
  _signal_changed.emit();
}

bool Plot2DData::is_showing() const {
  return shown;
}

void Plot2DData::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, bool log10_x, bool log10_y) {
  if (!shown)
    return;

  //get the color from col0a and store it
  PLINT red_u_old, green_u_old, blue_u_old;
  PLFLT alpha_old;
  pls->gcol0a(5, red_u_old, green_u_old, blue_u_old, alpha_old);
  pls->scol0a(5, color.get_red_u()/256, color.get_green_u()/256, color.get_blue_u()/256, color.get_alpha());
  pls->col0(5);
  pls->lsty(line_style);
  pls->width(line_width);

  //now let's see if we are dealing with logarithmic axes
  PLFLT *x_pl = &x[0], *y_pl = &y[0];
  std::vector<PLFLT> x_vc, y_vc;

  if (log10_x) {
    std::valarray<PLFLT> x_va(x_pl, x.size());
    x_va = log10(x_va);
    x_vc.assign(std::begin(x_va), std::end(x_va));
    x_pl = &x_vc[0];
  }

  if (log10_y) {
    std::valarray<PLFLT> y_va(y_pl, y.size());
    y_va = log10(y_va);
    y_vc.assign(std::begin(y_va), std::end(y_va));
    y_pl = &y_vc[0];
  }
  pls->line(x.size(), x_pl, y_pl);

  //restore color
  pls->scol0a(5, red_u_old, green_u_old, blue_u_old, alpha_old);
}
