#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <re2/re2.h>
#include <re2/regexp.h>
#include <util/utf.h>

#include <numeric>
#include <sstream>
#include <string>

namespace py = pybind11;

#define MODULE_NAME _rei
#define C_STR_HELPER(a) #a
#define C_STR(a) C_STR_HELPER(a)
#define EXPRESSION_NAME "Expression"
#define PARSE_FLAG_NAME "ParseFlag"
#define RUNE_NAME "Rune"
#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

using Expression = re2::RE2;
using ParseFlag = re2::Regexp::ParseFlags;
class Rune {
 public:
  Rune(const py::bytes& components) {
    re2::chartorune(&_raw, std::string(components).c_str());
  }

  bool operator==(const Rune& other) const { return _raw == other._raw; }

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

template <class Object>
std::string repr(const Object& object) {
  std::ostringstream stream;
  stream.precision(std::numeric_limits<double>::digits10 + 2);
  stream << object;
  return stream.str();
}

template <class Iterable>
static std::string join(const Iterable& elements,
                        const std::string& separator) {
  const auto begin = std::begin(elements);
  const auto end = std::end(elements);
  if (begin == end) return std::string();
  return std::accumulate(
      std::next(begin), end, std::string(*begin),
      [&separator](const std::string& result,
                   const typename Iterable::value_type& value) {
        return result + separator + std::string(value);
      });
};

static std::ostream& operator<<(std::ostream& stream, const py::bytes& bytes) {
  std::vector<py::str> components;
  for (auto byte : py::iter(bytes)) components.push_back(py::str(byte));
  return stream << "bytes([" << join(components, ", ") << "])";
}

static std::ostream& operator<<(std::ostream& stream, const Rune& rune) {
  return stream << C_STR(MODULE_NAME) "." RUNE_NAME "(" << rune.components()
                << ")";
}

PYBIND11_MODULE(MODULE_NAME, m) {
  m.doc() = R"pbdoc(Python binding of `re2` C++ library.)pbdoc";
  m.attr("__version__") = C_STR(VERSION_INFO);

  py::enum_<ParseFlag>(m, PARSE_FLAG_NAME)
      .value("NO_PARSE_FLAGS", ParseFlag::NoParseFlags)
      .value("FOLD_CASE", ParseFlag::FoldCase)
      .value("LITERAL", ParseFlag::Literal)
      .value("CLASS_NL", ParseFlag::ClassNL)
      .value("DOT_NL", ParseFlag::DotNL)
      .value("MATCH_NL", ParseFlag::MatchNL)
      .value("ONE_LINE", ParseFlag::OneLine)
      .value("LATIN1", ParseFlag::Latin1)
      .value("NON_GREEDY", ParseFlag::NonGreedy)
      .value("PERL_CLASSES", ParseFlag::PerlClasses)
      .value("PERL_B", ParseFlag::PerlB)
      .value("PERL_X", ParseFlag::PerlX)
      .value("UNICODE_GROUPS", ParseFlag::UnicodeGroups)
      .value("NEVER_NL", ParseFlag::NeverNL)
      .value("NEVER_CAPTURE", ParseFlag::NeverCapture)
      .value("LIKE_PERL", ParseFlag::LikePerl)
      .value("WAS_DOLLAR", ParseFlag::WasDollar)
      .value("ALL_PARSEFLAGS", ParseFlag::AllParseFlags)
      .export_values();

  py::class_<Rune>(m, RUNE_NAME)
      .def(py::init<const py::bytes&>(), py::arg("components"))
      .def(py::self == py::self)
      .def("__bool__", &Rune::operator bool)
      .def("__iter__",
           [](const Rune& self) { return py::iter(self.components()); })
      .def("__len__", &Rune::size)
      .def("__repr__", repr<Rune>)
      .def("__str__", &Rune::operator py::str)
      .def_property_readonly("components", &Rune::components);

  py::class_<Expression>(m, EXPRESSION_NAME)
      .def(py::init([](const std::string& pattern) {
        return std::make_unique<Expression>(re2::StringPiece{pattern});
      }))
      .def_property_readonly("pattern", &Expression::pattern);
}
