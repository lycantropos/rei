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
#define ANCHOR_NAME "Anchor"
#define EXPRESSION_NAME "Expression"
#define OPERATION_NAME "Operation"
#define PARSE_FLAG_NAME "ParseFlag"
#define RUNE_NAME "Rune"
#define STATUS_NAME "Status"
#define STATUS_CODE_NAME "StatusCode"
#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

using Expression = re2::RE2;
using Anchor = Expression::Anchor;
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

using Status = re2::RegexpStatus;
using StatusCode = re2::RegexpStatusCode;
using StringPiece = re2::StringPiece;

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

static std::ostream& operator<<(std::ostream& stream, const StatusCode& value) {
  stream << C_STR(MODULE_NAME) "." STATUS_CODE_NAME ".";
  switch (value) {
    case StatusCode::kRegexpSuccess:
      stream << "SUCCESS";
      break;
    case StatusCode::kRegexpInternalError:
      stream << "INTERNAL_ERROR";
      break;
    case StatusCode::kRegexpBadEscape:
      stream << "BAD_ESCAPE";
      break;
    case StatusCode::kRegexpBadCharClass:
      stream << "BAD_CHAR_CLASS";
      break;
    case StatusCode::kRegexpBadCharRange:
      stream << "BAD_CHAR_RANGE";
      break;
    case StatusCode::kRegexpMissingBracket:
      stream << "MISSING_BRACKET";
      break;
    case StatusCode::kRegexpMissingParen:
      stream << "MISSING_PAREN";
      break;
    case StatusCode::kRegexpUnexpectedParen:
      stream << "UNEXPECTED_PAREN";
      break;
    case StatusCode::kRegexpTrailingBackslash:
      stream << "TRAILING_BACKSLASH";
      break;
    case StatusCode::kRegexpRepeatArgument:
      stream << "REPEAT_ARGUMENT";
      break;
    case StatusCode::kRegexpRepeatSize:
      stream << "REPEAT_SIZE";
      break;
    case StatusCode::kRegexpRepeatOp:
      stream << "REPEAT_OP";
      break;
    case StatusCode::kRegexpBadPerlOp:
      stream << "BAD_PERL_OP";
      break;
    case StatusCode::kRegexpBadUTF8:
      stream << "BAD_UTF8";
      break;
    case StatusCode::kRegexpBadNamedCapture:
      stream << "BAD_NAMED_CAPTURE";
      break;
    default:
      stream << "???";
      break;
  }
  return stream;
}

static std::ostream& operator<<(std::ostream& stream, const Status& status) {
  return stream << C_STR(MODULE_NAME) "." STATUS_NAME "(" << status.code()
                << ", '" << status.error_arg() << "')";
}

template <class Object>
std::string repr(const Object& object) {
  std::ostringstream stream;
  stream.precision(std::numeric_limits<double>::digits10 + 2);
  stream << object;
  return stream.str();
}

namespace pybind11 {
namespace detail {
template <>
struct type_caster<StringPiece> {
  PYBIND11_TYPE_CASTER(StringPiece, _("StringPiece"));

  bool load(handle src, bool) {
    PyObject* source = src.ptr();
    if (PyUnicode_Check(source) < 1) return false;
    value = StringPiece::make_shared(PyUnicode_AsUTF8(source));
    return !!(value.data());
  }

  static handle cast(StringPiece src, return_value_policy /* policy */,
                     handle /* parent */) {
    return PyUnicode_FromStringAndSize(src.data(), src.size());
  }
};
}  // namespace detail
}  // namespace pybind11

PYBIND11_MODULE(MODULE_NAME, m) {
  m.doc() = R"pbdoc(Python binding of `re2` C++ library.)pbdoc";
  m.attr("__version__") = C_STR(VERSION_INFO);

  py::enum_<Anchor>(m, ANCHOR_NAME)
      .value("NONE", Anchor::UNANCHORED)
      .value("START", Anchor::ANCHOR_START)
      .value("BOTH", Anchor::ANCHOR_BOTH);

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
      .value("MAX_OPERATION", Operation::kMaxRegexpOp);

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
      .value("ALL_PARSE_FLAGS", ParseFlag::AllParseFlags);

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

  py::enum_<StatusCode> PyStatusCode(m, STATUS_CODE_NAME);
  PyStatusCode.value("SUCCESS", StatusCode::kRegexpSuccess)
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
      .value("BAD_NAMED_CAPTURE", StatusCode::kRegexpBadNamedCapture);
  PyStatusCode.attr("__str__") =
      py::cpp_function(&Status::CodeText, py::is_method(PyStatusCode));

  py::class_<Status>(m, STATUS_NAME)
      .def(py::init([](StatusCode code, const std::string& error_arg) {
             auto result = std::make_unique<Status>();
             result->set_code(code);
             result->set_error_arg(StringPiece::make_shared(error_arg));
             return result;
           }),
           py::arg("code") = StatusCode::kRegexpSuccess,
           py::arg("error_arg") = std::string())
      .def("__repr__", repr<Status>)
      .def("__str__", &Status::Text)
      .def_property("code", &Status::code, &Status::set_code)
      .def_property("error_arg", &Status::error_arg, &Status::set_error_arg);

  py::class_<Expression>(m, EXPRESSION_NAME)
      .def(py::init<const std::string&>())
      .def_property_readonly("pattern", &Expression::pattern);
}
