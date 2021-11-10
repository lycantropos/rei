#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <re2/prog.h>
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
#define CANNED_OPTION_NAME "CannedOption"
#define ENCODING_NAME "Encoding"
#define EXPRESSION_NAME "Expression"
#define INSTRUCTION_NAME "Instruction"
#define OPERATION_NAME "Operation"
#define OPTIONS_NAME "Options"
#define PARSE_FLAG_NAME "ParseFlag"
#define PARSE_STATE_NAME "ParseState"
#define PROGRAM_NAME "Program"
#define REGEXP_NAME "Regexp"
#define RUNE_NAME "Rune"
#define STATUS_NAME "Status"
#define STATUS_CODE_NAME "StatusCode"
#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

using Anchor = re2::RE2::Anchor;
using CannedOption = re2::RE2::CannedOptions;
using Encoding = re2::RE2::Options::Encoding;
using Expression = re2::RE2;
using Instruction = re2::Prog::Inst;
using Operation = re2::RegexpOp;
using Options = re2::RE2::Options;
using ParseFlag = re2::Regexp::ParseFlags;
using ParseState = re2::Regexp::ParseState;
using Program = re2::Prog;
using Regexp = re2::Regexp;

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

static std::ostream& operator<<(std::ostream& stream, const Encoding& value) {
  stream << C_STR(MODULE_NAME) "." ENCODING_NAME ".";
  switch (value) {
    case Encoding::EncodingLatin1:
      stream << "LATIN_1";
      break;
    case Encoding::EncodingUTF8:
      stream << "UTF_8";
      break;
    default:
      stream << "???";
      break;
  }
  return stream;
}

static std::ostream& operator<<(std::ostream& stream, const ParseFlag& value) {
  stream << C_STR(MODULE_NAME) "." STATUS_CODE_NAME ".";
  switch (value) {
    case ParseFlag::NoParseFlags:
      stream << "NO_PARSE_FLAGS";
      break;
    case ParseFlag::FoldCase:
      stream << "FOLD_CASE";
      break;
    case ParseFlag::Literal:
      stream << "LITERAL";
      break;
    case ParseFlag::ClassNL:
      stream << "CLASS_NL";
      break;
    case ParseFlag::DotNL:
      stream << "DOT_NL";
      break;
    case ParseFlag::MatchNL:
      stream << "MATCH_NL";
      break;
    case ParseFlag::OneLine:
      stream << "ONE_LINE";
      break;
    case ParseFlag::Latin1:
      stream << "LATIN1";
      break;
    case ParseFlag::NonGreedy:
      stream << "NON_GREEDY";
      break;
    case ParseFlag::PerlClasses:
      stream << "PERL_CLASSES";
      break;
    case ParseFlag::PerlB:
      stream << "PERL_B";
      break;
    case ParseFlag::PerlX:
      stream << "PERL_X";
      break;
    case ParseFlag::UnicodeGroups:
      stream << "UNICODE_GROUPS";
      break;
    case ParseFlag::NeverNL:
      stream << "NEVER_NL";
      break;
    case ParseFlag::NeverCapture:
      stream << "NEVER_CAPTURE";
      break;
    case ParseFlag::LikePerl:
      stream << "LIKE_PERL";
      break;
    case ParseFlag::WasDollar:
      stream << "WAS_DOLLAR";
      break;
    case ParseFlag::AllParseFlags:
      stream << "ALL_PARSE_FLAGS";
      break;
    default:
      stream << "???";
      break;
  }
  return stream;
}

static std::ostream& operator<<(std::ostream& stream, const ParseState& state) {
  return stream << C_STR(MODULE_NAME) "." STATUS_NAME "(" << state.flags()
                << ", '" << state.whole_regexp() << "')";
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

static void write_bool(std::ostream& stream, bool value) {
  stream << (value ? "True" : "False");
}

static std::ostream& operator<<(std::ostream& stream, const Options& options) {
  stream << C_STR(MODULE_NAME) "." OPTIONS_NAME "(" << options.encoding()
         << ", ";
  write_bool(stream, options.posix_syntax());
  stream << ", ";
  write_bool(stream, options.longest_match());
  stream << ", ";
  write_bool(stream, options.log_errors());
  stream << ", ";
  write_bool(stream, options.max_mem());
  stream << ", ";
  write_bool(stream, options.literal());
  stream << ", ";
  write_bool(stream, options.never_nl());
  stream << ", ";
  write_bool(stream, options.dot_nl());
  stream << ", ";
  write_bool(stream, options.never_capture());
  stream << ", ";
  write_bool(stream, options.case_sensitive());
  stream << ", ";
  write_bool(stream, options.perl_classes());
  stream << ", ";
  write_bool(stream, options.word_boundary());
  stream << ", ";
  write_bool(stream, options.one_line());
  return stream << ")";
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
    Py_ssize_t size;
    const char* data = PyUnicode_AsUTF8AndSize(source, &size);
    if (data == nullptr) return false;
    value = StringPiece::make_shared(data,
                                     static_cast<StringPiece::size_type>(size));
    return true;
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

  py::enum_<CannedOption>(m, CANNED_OPTION_NAME)
      .value("DEFAULT", CannedOption::DefaultOptions)
      .value("LATIN_1", CannedOption::Latin1)
      .value("POSIX", CannedOption::POSIX)
      .value("QUIET", CannedOption::Quiet);

  py::enum_<Anchor>(m, ANCHOR_NAME)
      .value("NONE", Anchor::UNANCHORED)
      .value("START", Anchor::ANCHOR_START)
      .value("BOTH", Anchor::ANCHOR_BOTH);

  py::enum_<Encoding>(m, ENCODING_NAME)
      .value("UTF_8", Encoding::EncodingUTF8)
      .value("LATIN_1", Encoding::EncodingLatin1);

  py::class_<Instruction>(m, INSTRUCTION_NAME).def(py::init());

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

  py::class_<Options>(m, OPTIONS_NAME)
      .def(py::init([](Encoding encoding, bool posix_syntax, bool longest_match,
                       bool log_errors, std::int64_t max_memory, bool literal,
                       bool never_nl, bool dot_nl, bool never_capture,
                       bool case_sensitive, bool perl_classes,
                       bool word_boundary, bool one_line) {
             Options result;
             result.set_encoding(encoding);
             result.set_posix_syntax(posix_syntax);
             result.set_longest_match(longest_match);
             result.set_log_errors(log_errors);
             result.set_max_mem(max_memory);
             result.set_literal(literal);
             result.set_never_nl(never_nl);
             result.set_dot_nl(dot_nl);
             result.set_never_capture(never_capture);
             result.set_case_sensitive(case_sensitive);
             result.set_perl_classes(perl_classes);
             result.set_word_boundary(word_boundary);
             result.set_one_line(one_line);
             return result;
           }),
           py::arg("encoding") = Encoding::EncodingUTF8,
           py::arg("posix_syntax") = false, py::arg("longest_match") = false,
           py::arg("log_errors") = true,
           py::arg("max_memory") = Options::kDefaultMaxMem,
           py::arg("literal") = false, py::arg("never_nl") = false,
           py::arg("dot_nl") = false, py::arg("never_capture") = false,
           py::arg("case_sensitive") = true, py::arg("perl_classes") = false,
           py::arg("word_boundary") = false, py::arg("one_line") = false)
      .def_static("from_canned_option",
                  [](CannedOption option) { return Options(option); })
      .def("__repr__", repr<Options>)
      .def_property("encoding", &Options::encoding, &Options::set_encoding)
      .def_property("posix_syntax", &Options::posix_syntax,
                    &Options::set_posix_syntax)
      .def_property("longest_match", &Options::longest_match,
                    &Options::set_longest_match)
      .def_property("log_errors", &Options::log_errors,
                    &Options::set_log_errors)
      .def_property("max_memory", &Options::max_mem, &Options::set_max_mem)
      .def_property("literal", &Options::literal, &Options::set_literal)
      .def_property("never_nl", &Options::never_nl, &Options::set_never_nl)
      .def_property("dot_nl", &Options::dot_nl, &Options::set_dot_nl)
      .def_property("never_capture", &Options::never_capture,
                    &Options::set_never_capture)
      .def_property("case_sensitive", &Options::case_sensitive,
                    &Options::set_case_sensitive)
      .def_property("perl_classes", &Options::perl_classes,
                    &Options::set_perl_classes)
      .def_property("word_boundary", &Options::word_boundary,
                    &Options::set_word_boundary)
      .def_property("one_line", &Options::one_line, &Options::set_one_line);

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

  struct ParseStateDeleter {
    void operator()(ParseState* self) const noexcept { delete self->status(); }
  };

  py::class_<ParseState, std::unique_ptr<ParseState, ParseStateDeleter>>(
      m, PARSE_STATE_NAME)
      .def(py::init([](const StringPiece& pattern, ParseFlag flag) {
        return std::unique_ptr<ParseState, ParseStateDeleter>(
            new ParseState(flag, pattern, new Status()));
      }))
      .def("__repr__", repr<ParseState>)
      .def_property_readonly("flag", &ParseState::flags)
      .def_property_readonly("pattern", &ParseState::whole_regexp)
      .def_property_readonly("status", &ParseState::status);

  py::class_<Program>(m, PROGRAM_NAME).def(py::init());

  struct RegexpDeleter {
    void operator()(Regexp* self) const noexcept { self->Decref(); }
  };

  py::class_<Regexp, std::unique_ptr<Regexp, RegexpDeleter>>(m, REGEXP_NAME)
      .def(py::init([](const StringPiece& pattern, ParseFlag flag) {
             Status* status = new Status();
             Regexp* result = Regexp::Parse(pattern, flag, status);
             if (status->code() != StatusCode::kRegexpSuccess) {
               std::invalid_argument error(status->Text());
               delete status;
               throw error;
             }
             delete status;
             return std::unique_ptr<Regexp, RegexpDeleter>(result);
           }),
           py::arg("pattern"), py::arg("flag") = ParseFlag::NoParseFlags)
      .def("__str__", &Regexp::ToString);

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
      .def(py::init([](StatusCode code, const StringPiece& error_arg) {
             auto result = std::make_unique<Status>();
             result->set_code(code);
             result->set_error_arg(error_arg);
             return result;
           }),
           py::arg("code") = StatusCode::kRegexpSuccess,
           py::arg("error_arg") = std::string())
      .def("__repr__", repr<Status>)
      .def("__str__", &Status::Text)
      .def_property("code", &Status::code, &Status::set_code)
      .def_property("error_arg", &Status::error_arg, &Status::set_error_arg);

  py::class_<Expression>(m, EXPRESSION_NAME)
      .def(py::init<const std::string&, const Options&>(), py::arg("pattern"),
           py::arg("options") = Options(CannedOption::DefaultOptions))
      .def_property_readonly("pattern", &Expression::pattern);
}
