#ifndef LISPP_LISP_H
#define LISPP_LISP_H

#include <iostream>
#include <map>
#include <string>
#include <variant>

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
  std::string toString() const;

private:
  Variant data;
};

const Object *read(const char *input = nullptr);
const Object *eval(const Object *ast, Environment *);
void repl();

#endif // LISPP_LISP_H