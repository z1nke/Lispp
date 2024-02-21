#include <iostream>
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

using Object = std::string;
static std::string line;

void print(const Object *obj) { std::cout << *obj << '\n'; }

const Object *eval(const Object *ast) { return ast; }

const Object *read() {
  std::getline(std::cin, line);
  if (line.empty()) {
    std::cout << '\n';
    std::exit(0);
  }

  return &line;
}

// Read-Eval-Print-Loop.
void repl() {
  for (;;) {
    if (ISATTY(FILENO(stdin)))
      std::cout << ">> ";
    print(eval(read()));
  }
}

int main() {
  repl();
  return 0;
}
