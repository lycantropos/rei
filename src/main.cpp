#include <pybind11/pybind11.h>
#include <re2/re2.h>

#include <string>

namespace py = pybind11;

#define MODULE_NAME _rei
#define C_STR_HELPER(a) #a
#define C_STR(a) C_STR_HELPER(a)
#define EXPRESSION_NAME "Expression"
#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

using Expression = re2::RE2;

PYBIND11_MODULE(MODULE_NAME, m) {
  m.doc() = R"pbdoc(Python binding of `re2` C++ library.)pbdoc";
  m.attr("__version__") = C_STR(VERSION_INFO);

  py::class_<Expression>(m, EXPRESSION_NAME)
      .def(py::init([](const std::string& pattern) {
        return std::make_unique<Expression>(re2::StringPiece{pattern});
      }))
      .def_property_readonly("pattern", &Expression::pattern);
}
