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
#define OPERATION_NAME "Operation"
#define PARSE_FLAG_NAME "ParseFlag"
#define RUNE_NAME "Rune"
#define STATUS_CODE_NAME "StatusCode"
#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

using Expression = re2::RE2;
using Operation = re2::RegexpOp;
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

using StatusCode = re2::RegexpStatusCode;

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

  py::enum_<Operation>(m, OPERATION_NAME)
      .value("NO_MATCH", Operation::kRegexpNoMatch)
      .value("EMPTY_MATCH", Operation::kRegexpEmptyMatch)
      .value("LITERAL", Operation::kRegexpLiteral)
      .value("LITERAL_STRING", Operation::kRegexpLiteralString)
      .value("CONCAT", Operation::kRegexpConcat)
      .value("ALTERNATE", Operation::kRegexpAlternate)
      .value("STAR", Operation::kRegexpStar)
      .value("PLUS", Operation::kRegexpPlus)
      .value("QUEST", Operation::kRegexpQuest)
      .value("REPEAT", Operation::kRegexpRepeat)
      .value("CAPTURE", Operation::kRegexpCapture)
      .value("ANY_CHAR", Operation::kRegexpAnyChar)
      .value("ANY_BYTE", Operation::kRegexpAnyByte)
      .value("BEGIN_LINE", Operation::kRegexpBeginLine)
      .value("END_LINE", Operation::kRegexpEndLine)
      .value("WORD_BOUNDARY", Operation::kRegexpWordBoundary)
      .value("NO_WORD_BOUNDARY", Operation::kRegexpNoWordBoundary)
      .value("BEGIN_TEXT", Operation::kRegexpBeginText)
      .value("END_TEXT", Operation::kRegexpEndText)
      .value("CHAR_CLASS", Operation::kRegexpCharClass)
      .value("HAVE_MATCH", Operation::kRegexpHaveMatch)
      .value("MAX_OPERATION", Operation::kMaxRegexpOp)
      .export_values();

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
      .value("ALL_PARSE_FLAGS", ParseFlag::AllParseFlags)
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

  py::enum_<StatusCode>(m, STATUS_CODE_NAME)
      .value("SUCCESS", StatusCode::kRegexpSuccess)
      .value("INTERNAL_ERROR", StatusCode::kRegexpInternalError)
      .value("BAD_ESCAPE", StatusCode::kRegexpBadEscape)
      .value("BAD_CHAR_CLASS", StatusCode::kRegexpBadCharClass)
      .value("BAD_CHAR_RANGE", StatusCode::kRegexpBadCharRange)
      .value("MISSING_BRACKET", StatusCode::kRegexpMissingBracket)
      .value("MISSING_PAREN", StatusCode::kRegexpMissingParen)
      .value("UNEXPECTED_PAREN", StatusCode::kRegexpUnexpectedParen)
      .value("TRAILING_BACKSLASH", StatusCode::kRegexpTrailingBackslash)
      .value("REPEAT_ARGUMENT", StatusCode::kRegexpRepeatArgument)
      .value("REPEAT_SIZE", StatusCode::kRegexpRepeatSize)
      .value("REPEAT_OP", StatusCode::kRegexpRepeatOp)
      .value("BAD_PERL_OP", StatusCode::kRegexpBadPerlOp)
      .value("BAD_UTF8", StatusCode::kRegexpBadUTF8)
      .value("BAD_NAMED_CAPTURE", StatusCode::kRegexpBadNamedCapture)
      .export_values();

  py::class_<Expression>(m, EXPRESSION_NAME)
      .def(py::init([](const std::string& pattern) {
        return std::make_unique<Expression>(re2::StringPiece{pattern});
      }))
      .def_property_readonly("pattern", &Expression::pattern);
}
