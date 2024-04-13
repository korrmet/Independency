<img src="./logo.svg" width="300">
A small but freaking powerful set of tools for the desktop projects with
event-based architectural style.

# Overview

I guess you're already aware about numerous architecture styles and what kind
of problems they solves. Now I wanna say something about the event-based
architecture and how to ease it's usage. In this paradigm you have a lot of
separated code modules that have no dependencies between together.
Sound good but most of the code I've seen was actually yet another mess of
random dependencies. It happens just because there is no convenient tool to
apply this architectural style.

So, standard event-based architectural style eliminates part of dependencies of
name, you just create the some kind of the message dispatcher and each module
even aware about his neighbours. For a first look it's fully isolated and
depends only on the message dispatcher. But what's about data transfer and
storage? So, one of my projects goes to the data structure hell, when you have
a lot of data structs and almost only thing you do is writing adapters,
getters, converters, test it again and again, and a single change produces a
chain reaction of resolving build issues. The only thing you need is to
implement the required functionality, your plans aren't includes jumping on
your balls, right? Make a general structure for all ease the pain but not so
effectively. I introduce the alternative - global structure on a steroids.
She's dynamic and it's usage so simple as the regular array. Also it can be
serialized to any string and parsed from the string. It have a simple format.

This repo demonstrates how do the a little amount of code can solve a lot of
the problems. And also it uses just STL plus a little bit of operator
overloading magic. And, I know, everybody likes it... It's HEADER-ONLY!

There is no commens at all. Just because in this project trey're useless.
This readme and independency.cpp are enough to effectively using this lib.
Watch out the independency.cpp to learn some useful tricks.

## Value

Value is the core concept of this library. It's just a string representation of
named data. Type of the data is meaningful too. For now it supports integers,
floating-point number and string itself. You can create the value object and
use it as one of these three types because of some operator overload magic.

```
independency::value foo("foo"); // it's an empty value object
foo = 3; // now it has integer type and contains number three
int bar = foo; // bar is initialized by number 3, stored in the value object;
int baz = bar + foo; // foo casts to the integer, baz == 6
```

## Bus

Bus is the mechanism of the message propagation through your system. In common
for all of your application just declare the bus object, register callbacks
and send messages. Message is the dynamic storage of the several values.

Step 1: Declare the bus in the common for all of your application file.
```
independency::bus bus;
```

Step 2: Write some callbacks in your applictaion and register it using plus
syntax.
```
// Header's code
#include "common.hpp"

class some_class
{ public:
  some_class()
  { bus + independency::callback(handle, this);
  }
  
  private:
  static void handle(void* ctx, independency::message mess);
};
```

Step 3: Unpack the message and do the necessary stuff.
```
void some_class::handle(void* ctx, independency::message mess)
{ if (~mess == "click" && mess["x"] >= 650 && mess["y"] >= 480)
  { // you're clicked somewhere
  }
}
```

Step 4: Call it somewhere using the bus.
```
#include "common.hpp"
...
bus(independency::message("click")
    << independency::value("x", 100)
    << independency::value("y", 100));
...
```

## Storage

This is a hierarchical data structure and it can be dynamically filled in a
runtime. There is a few mechanisms that may prevent a lot of crashes.

Step 1: Declare the storage at the common of your application source file.
Additionally you may define root empty path.
```
independency::storage storage;
#define root idependency::path()
```

Step 2: Write some data using the path mechanism.
```
storage[root / "pi"] = 3.14f;
```

Step 3: Read the data using one of three mechanisms.
```
float pi = storage[root / "pi"]; // even if root/pi does not exists it would be
                                 // created
float dx = storage(root / "dx"); // if root/dx does not exists, null would be
                                 // returned, storage would be uncahnged
if (!!storage(root / "errcode")) // checks if the errcode value exists
{ // repair
}
```

# Licence
Asshole public licence:

If you like this code, just tell your buddies about it or you'll be an asshole.

Yes, it's the full text of the licence. Simple, right?
