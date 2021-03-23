#include <pybind11/pybind11.h>
#include <re2/re2.h>
#include <util/utf.h>

#include <string>

namespace py = pybind11;

#define MODULE_NAME _rei
#define C_STR_HELPER(a) #a
#define C_STR(a) C_STR_HELPER(a)
#define EXPRESSION_NAME "Expression"
#define RUNE_NAME "Rune"
#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

using Expression = re2::RE2;
class Rune {
 public:
  Rune(const py::bytes& components) {
    re2::chartorune(&_raw, std::string(components).c_str());
  }

  operator bool() const { return _raw != re2::Runeerror; }

  operator py::str() const {
    char* c_string = new char[re2::UTFmax]();
    int size = re2::runetochar(c_string, &_raw);
    py::str result(c_string, size);
    delete[] c_string;
    return result;
  }

  int size() const {
    char* c_string = new char[re2::UTFmax]();
    int result = re2::runetochar(c_string, &_raw);
    delete[] c_string;
    return result;
  }

  py::bytes components() const {
    char* c_string = new char[re2::UTFmax]();
    int size = re2::runetochar(c_string, &_raw);
    py::bytes result(c_string, size);
    delete[] c_string;
    return result;
  }

 private:
  re2::Rune _raw;
};

PYBIND11_MODULE(MODULE_NAME, m) {
  m.doc() = R"pbdoc(Python binding of `re2` C++ library.)pbdoc";
  m.attr("__version__") = C_STR(VERSION_INFO);

  py::class_<Rune>(m, RUNE_NAME)
      .def(py::init<const py::bytes&>(), py::arg("components"))
      .def("__bool__", &Rune::operator bool)
      .def("__iter__", [](const Rune& self) {
        return py::iter(self.components());
      })
      .def("__len__", &Rune::size)
      .def("__str__", &Rune::operator py::str);

  py::class_<Expression>(m, EXPRESSION_NAME)
      .def(py::init([](const std::string& pattern) {
        return std::make_unique<Expression>(re2::StringPiece{pattern});
      }))
      .def_property_readonly("pattern", &Expression::pattern);
}
