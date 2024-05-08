#include <cstdarg>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#if _WIN32
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

[[noreturn]] static void errorf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  const bool hasColor = ISATTY(FILENO(stderr));
  if (hasColor)
    fprintf(stderr, "\033[0;1;31m");
  fprintf(stderr, "error: ");
  if (hasColor)
    fprintf(stderr, "\033[0m");
  vfprintf(stderr, fmt, ap);
  fputc('\n', stderr);
  va_end(ap);
  std::exit(1);
}

class Environment;
class Object;

class Environment {
public:
  // We use raw pointer + allocator to manage memory.
  // And later try to implement GC.
  using Map = std::map<std::string, Object *>;

  Environment() = default;

  /// \returns a pointer to the object associated with \p key if it exists,
  /// otherwise nullptr.
  Object *lookup(const std::string &key) const;
  Object &operator[](const std::string &key);

  void print(std::ostream &os);
  void dump();

private:
  static void printDefs(std::ostream &os, const Map &defs);

private:
  Map defs;
  Environment *parent = nullptr;
};

struct Unit {};
struct Symbol {
  std::string name;
};
struct Cell {
  Object *car;
  Object *cdr;
};

struct BuiltinFunc {
  std::string name;
  Object *params;
  Object *env;
};

struct Lambda {
  Object *params;
  Object *body;
  Environment *env;
};

class Object {
public:
  using Variant = std::variant<Unit, bool, int64_t, double, std::string, Symbol,
                               Cell, BuiltinFunc, Lambda>;

  Object(Unit) : data(Unit{}) {}
  Object(bool b) : data(b) {}

  const Variant &getData() const { return data; }

private:
  Variant data;
};

static Object nilobj{Unit{}};
static Object *nil = &nilobj;

static std::ostream &operator<<(std::ostream &os, const Object &obj);

static std::ostream &operator<<(std::ostream &os, Unit) {
  os << "()";
  return os;
}

static std::ostream &operator<<(std::ostream &os, const Symbol &sym) {
  os << "symbol: " << sym.name;
  return os;
}

static std::ostream &operator<<(std::ostream &os, const Cell &cell) {
  os << '(' << (*cell.car) << ", " << (*cell.cdr) << ')';
  return os;
}

static std::ostream &operator<<(std::ostream &os, const BuiltinFunc &builtin) {
  os << "<builtin>: " << builtin.name;
  return os;
}

static std::ostream &operator<<(std::ostream &os, const Lambda &) {
  os << "<lambda>";
  return os;
}

static std::ostream &operator<<(std::ostream &os, const Object &obj) {
  std::visit([&os](auto &&arg) { os << arg; }, obj.getData());
  return os;
}

static void print(const Object *obj) { std::cout << *obj << '\n'; }

static const Object *eval(const Object *ast, Environment *) { return ast; }

struct Token {
  enum Kind {
    TK_EOF,
    TK_LPAREN,
    TK_RPAREN,
    // TODO: Other tokens.
  } kind;
};

static bool lex(Token &tok) {
  for (;;) {
    int ch = getchar();
    switch (ch) {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
      continue;
    case EOF:
      tok.kind = Token::TK_EOF;
      return true;
    case '(':
      tok.kind = Token::TK_LPAREN;
      return true;
    case ')':
      tok.kind = Token::TK_RPAREN;
      return true;
    // TODO: Other tokens.
    default:
      errorf("unknown character '%c'", static_cast<char>(ch));
    }
  }
}

static Object *parseList();

static Object *parse(const Token &tok) {
  switch (tok.kind) {
  case Token::TK_EOF:
    return nullptr;
  case Token::TK_LPAREN:
    return parseList();
  default:
    errorf("unknown token");
  }
}

static Object *parse() {
  Token tok;
  return lex(tok) ? parse(tok) : nullptr;
}

static Object *parseList() {
  Object *list = nil;
  while (true) {
    Token tok;
    if (!lex(tok))
      return nullptr;

    if (tok.kind == Token::TK_RPAREN)
      return list;

    // TODO: Add obj to list.
    Object *obj = parse(tok);
    (void)obj;
  }

  return list;
}

static const Object *read() { return parse(); }

// Read-Eval-Print-Loop.
void repl(Environment *env) {
  for (;;) {
    if (ISATTY(FILENO(stdin)))
      std::cout << ">> ";
    print(eval(read(), env));
  }
}

int main() {
  Environment globalEnv;
  repl(&globalEnv);
  return 0;
}

Object &Environment::operator[](const std::string &key) { return *defs[key]; }

Object *Environment::lookup(const std::string &key) const {
  const Environment *env = this;
  while (env) {
    auto iter = env->defs.find(key);
    if (iter != env->defs.end())
      return iter->second;
    env = env->parent;
  }

  return nullptr;
}

void Environment::print(std::ostream &os) {
  printDefs(os, defs);
  Environment *env = parent;
  while (env) {
    os << "---\n";
    printDefs(os, env->defs);
    env = env->parent;
  }
}

void Environment::printDefs(std::ostream &os, const Map &defs) {
  for (const auto &[name, obj] : defs) {
    os << name << ": " << *obj << '\n';
  }
}

void Environment::dump() { print(std::cerr); }