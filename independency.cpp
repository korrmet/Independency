#include <cstdio>
#include "independency.hpp"

#define IV independency::value
#define IM independency::message
#define IC independency::callback
#define IP independency::path
#define root IP()

void handler(void* ctx, IM mess)
{ std::printf("%p, %s: x %d, y %d\n",
              ctx, (~mess).c_str(), (int)mess["x"], (int)mess["y"]); }

int main(int argc, char** argv)
{ std::printf("Independency library demo\n");

  std::printf("\nFile operations\n");

  independency::file foo("foo.txt");
  foo.write("Hello, world!");
  if (foo.read() == "Hello, world!")
  { std::printf("File written and readen succesfully\n"); }

  foo.del();

  std::printf("\nCLI Parser\n");

  independency::cli_parser parser(argc, argv);

  if (!parser.count("--flag")) { std::printf("specify flag\n"); }
  if (!parser.count("--int")) { std::printf("specify int\n"); }
  if (!parser.count("--float")) { std::printf("specify float\n"); }
  if (!parser.count("--string")) { std::printf("specify string\n"); }

  std::printf("flag: %d\n", (parser.count("--flag") ? 1 : 0));
  std::printf("int: %d\n", (int)parser["--int"]);
  std::printf("float: %f\n", (float)parser["--float"]);
  std::printf("string: %s\n", ((std::string)parser["--string"]).c_str());

  std::printf("\nValues\n");

  IV v1("Integer", 1);
  IV v2("Float", 1.0f);
  IV v3("String", "one");

  int _v1 = v1;
  float _v2 = v2;
  std::string _v3 = v3;
  std::printf("%d, %f, %s\n", _v1, _v2, _v3.c_str());
  std::printf("sum: %f\n", 3 + _v2);

  std::printf("\nMessages\n");
  IM mess = IM ("click") << IV ("x", 100) << IV ("y", 100);
  std::printf("%s: x %d, y %d\n",
              (~mess).c_str(), (int)mess["x"], (int)mess["y"]);

  if (mess == "click") { std::printf("mess == click\n"); }

  std::printf("\nCallbacks\n");
  IC cb(handler); cb(mess);

  std::printf("\nBuses\n");
  independency::bus bus;
  std::printf("Callback object\n");
  bus + cb; bus(mess); // produces one message, registers callback
  bus - cb; bus(mess); // do not produce message, unregisters callback
  std::printf("Callback object 2\n");
  bus + IC(handler); bus(mess); // another approach
  bus - IC(handler); bus(mess);
  std::printf("Callback function\n");
  bus + handler; bus(mess); // another approach
  bus - handler; bus(mess);

  std::printf("\nPath'n'storage\n");
  // sample path object
  IP some_path = root / "fff" / "f" / 5;

  independency::storage storage("example");
  // Square braces produces new value if not existed
  storage[root / "foo"] = "one";
  storage[root / "bar"] = "two";
  storage[root / "foo" / "baz"] = 3.14f;

  // Rounded braces returns only the existent value reference or dummy if not
  storage(root / "fff") = 10; // returns non empty value if exists

  std::printf("Default value for the integer is 0, for float is 0.0, "
              "for string is empty string\n");
  std::printf("inexisted: %d\n", (int)storage[root / "bax"]);
  std::printf("existed: %f\n", (float)storage[root / "foo" / "baz"]);
  std::printf("ls:\n");

  for (std::string itm : storage.ls(root))
  { std::printf("  %s\n", itm.c_str()); }

  std::printf("check existent explicitly: %s\n",
              storage.chk(root / "foo" / "baz") ? "true" : "false");
  std::printf("check inexistent explicityly: %s\n",
              storage.chk(root / "fff") ? "true" : "false");
  std::printf("check inexistent equal empty: %s\n",
              storage(root / "fff") == IV() ? "true" : "false");
  std::printf("check inexistent not equal empty: %s\n",
              storage(root / "fff") != IV() ? "true" : "false");
  std::printf("check inexistent NOT: %s\n",
              !storage(root / "fff") ? "true" : "false");
  std::printf("check inexistent double NOT: %s\n",
              !!storage(root / "fff") ? "true" : "false");

  storage.del(root/"bax");

  std::string sample = storage.serialize();
  std::printf("direct serialize:\n%s", sample.c_str());
  storage.parse(sample);
  std::printf("reverse:\n%s", storage.serialize().c_str());

  std::printf("\nmove:\n");
  independency::storage s2;
  storage.mv(root/"foo", root/"a"/"b"/"c", &s2);
  std::printf("%s\n%s\n", storage.serialize().c_str(), s2.serialize().c_str());

  return 0; }
