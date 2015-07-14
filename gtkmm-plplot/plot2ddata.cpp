#include "gtkmm-plplot/plot2ddata.h"
#include "gtkmm-plplot/exception.h"
#include "gtkmm-plplot/utils.h"

using namespace Gtk::PLplot;

Plot2DData::Plot2DData(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  Color _color) :
  x(_x), y(_y), color(_color) {
  if (x.size() != y.size()) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data arrays x and y must have the same size!");
  }
  this->signal_changed().connect(sigc::mem_fun(*this,
             &Plot2DData::on_changed));
}

Plot2DData::Plot2DData(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  Color _color) :
  Plot2DData(std::vector<PLFLT>(std::begin(_x), std::end(_x)),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _color) {}

Plot2DData::Plot2DData(
  const std::vector<PLFLT> &_y,
  Color _color) :
  Plot2DData(indgen(_y.size()), _y, _color) {}

Plot2DData::Plot2DData(
  const std::valarray<PLFLT> &_y,
  Color _color) :
  Plot2DData(std::vector<PLFLT>(indgen(_y.size())),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _color) {}

Plot2DData::Plot2DData(const Plot2DData &_data) :
  Plot2DData(_data.x, _data.y, _data.color) {}

void Plot2DData::set_color(Color _color) {
    color = _color;
    _signal_changed.emit();
}

Color Plot2DData::get_color() {
  return color;
}

void Plot2DData::on_changed() {
  //do nothing
}
