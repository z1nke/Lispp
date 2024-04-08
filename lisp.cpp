#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>

#if _WIN32
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

class Environment;

using Object = std::string;
static std::string line;

class Environment {
public:
  // We use smart pointers to manage memory and later try to implement GC.
  using Map = std::map<std::string, std::shared_ptr<Object>>;

  Environment() = default;

  std::optional<Object> lookup(const std::string &key) const;
  Object &operator[](const std::string &key);

  void print(std::ostream &os);
  void dump();

private:
  void printDefs(std::ostream &os, const Map &defs);

private:
  Map defs;
  Environment *parent = nullptr;
};

void print(const Object *obj) { std::cout << *obj << '\n'; }

const Object *eval(const Object *ast, Environment *) { return ast; }

const Object *read() {
  std::getline(std::cin, line);
  if (line.empty()) {
    std::cout << '\n';
    std::exit(0);
  }

  return &line;
}

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

std::optional<Object> Environment::lookup(const std::string &key) const {
  const Environment *env = this;
  while (env) {
    auto iter = env->defs.find(key);
    if (iter != env->defs.end())
      return *iter->second;
    env = env->parent;
  }

  return std::nullopt;
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