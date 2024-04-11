#include <cstdio>
#include "independency.hpp"

#define IV independency::value
#define IM independency::message
#define IC independency::callback
#define IP independency::path
#define root IP()

void callback(void* ctx, IM mess)
{ std::printf("%p, %s: x %d, y %d\n",
              ctx, (~mess).c_str(), (int)mess["x"], (int)mess["y"]); }

int main(int argc, char** argv)
{ std::printf("Independency library demo\n");

  std::printf("\nValues\n");

  IV v1("Integer", 1);
  IV v2("Float", 1.0f);
  IV v3("String", "one");

  int _v1 = v1;
  float _v2 = v2;
  std::string _v3 = v3;
  std::printf("%d, %f, %s\n", _v1, _v2, _v3.c_str());

  std::printf("\nMessages\n");
  IM mess = IM ("click") << IV ("x", 100) << IV ("y", 100);
  std::printf("%s: x %d, y %d\n",
              (~mess).c_str(), (int)mess["x"], (int)mess["y"]);

  if (~mess == "click") { std::printf("mess == click\n"); }

  std::printf("\nCallbacks\n");
  IC cb(callback); cb(mess);

  std::printf("\nBuses\n");
  independency::bus bus;
  bus + callback; bus(mess); // produces one message
  bus - callback; bus(mess); // do not produce message

  std::printf("\nPath'n'storage\n");
  root / "fff" / "f" / 5;

  independency::storage storage("example");
  storage[root / "foo"] = "one";
  storage[root / "bar"] = "two";
  storage[root / "foo" / "baz"] = 3.14f;
  std::printf("inexisted: %d\n", (int)storage[root / "bax"]);
  std::printf("existed: %f\n", (float)storage[root / "foo" / "baz"]);
  std::printf("ls:\n");
  for (std::string itm : storage.ls(root))
  { std::printf("  %s\n", itm.c_str()); }

  std::printf("chk existent: %s\n",
              storage.chk(root / "foo" / "baz") ? "true" : "false");
  std::printf("chk inexistent: %s\n",
              storage.chk(root / "fff") ? "true" : "false");

  std::string sample = storage.serialize();
  std::printf("direct serialize:\n%s", sample.c_str());
  storage.parse(sample);
  std::printf("reverse:\n%s", storage.serialize().c_str());

  return 0; }
