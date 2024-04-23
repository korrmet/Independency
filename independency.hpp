#ifndef INDEPENDENCY_HPP
#define INDEPENDENCY_HPP

#include <list>
#include <string>

namespace independency {
struct value
{ value() {}
 
  value(std::string name, int v)
  : name(name), type("int"), data(std::to_string(v)) {}
  
  value(std::string name, float v)
  : name(name), type("float"), data(std::to_string(v)) {}
  
  value(std::string name, std::string v = "")
  : name(name), type(v.empty() ? "" : "string"), data(v) {}

  operator int()
  { if (type == "int") { return std::stoi(data); } return 0; }
  
  operator float()
  { if (type == "float") { return std::stof(data); } return 0; }
  
  operator std::string()
  { if (type == "string") { return data; } return ""; }

  value& operator =(int v)
  { type = "int"; data = std::to_string(v); return *this;}

  bool operator ==(int v)
  { if (type != "int") { return false; }
    return v == std::stoi(data); }

  value& operator +=(int v)
  { if (type == "int") { data = std::to_string(std::stoi(data) + v); }
    return *this; }
  
  value& operator -=(int v)
  { if (type == "int") { data = std::to_string(std::stoi(data) - v); }
    return *this; }
  
  value& operator *=(int v)
  { if (type == "int") { data = std::to_string(std::stoi(data) * v); }
    return *this; }
  
  value& operator /=(int v)
  { if (type == "int") { data = std::to_string(std::stoi(data) / v); }
    return *this; }

  value& operator %=(int v)
  { if (type == "int") { data = std::to_string(std::stoi(data) % v); }
    return *this; }

  value& operator =(float v)
  { type = "float"; data = std::to_string(v); return *this; }

  bool operator ==(float v)
  { if (type != "float") { return false; }
    return v == std::stof(data); }

  value& operator +=(float v)
  { if (type == "float") { data = std::to_string(std::stof(data) + v); }
    return *this; }

  value& operator -=(float v)
  { if (type == "float") { data = std::to_string(std::stof(data) - v); }
    return *this; }

  value& operator *=(float v)
  { if (type == "float") { data = std::to_string(std::stof(data) * v); }
    return *this; }

  value& operator /=(float v)
  { if (type == "float") { data = std::to_string(std::stof(data) / v); }
    return *this; }

  value& operator =(std::string v)
  { type = "string"; data = v; return *this; }

  bool operator ==(std::string v)
  { if (type != "string") { return false; }
    return v == data; }

  value& operator +=(std::string v)
  { if (type == "string") { data += v; } return *this; }

  bool operator ==(value const& v)
  { return v.name == name && v.type == type && v.data == data; }

  bool operator !=(value const& v) { return !operator ==(v); }

  bool operator !() { return name == "" && type == "" && data == ""; }

  private:
  friend struct message;
  friend class storage;
  std::string name; std::string type; std::string data; };

struct message
{ message(std::string event = "") : event(event) {}

  value operator[](std::string name)
  { for (value& v : data) { if (v.name == name) { return v; } }
    return value(); }

  message& operator <<(value val) { data.push_back(val); return *this; }

  std::string operator ~() { return event; }

  bool operator ==(std::string ev) { return event == ev; }
  bool operator !=(std::string ev) { return event != ev; }

  private:
  std::string event; std::list<value> data; };

struct callback
{ callback(void (*func)(void* ctx, message mess) = nullptr,
           void* ctx = nullptr)
  : func(func), ctx(ctx) {}

  void operator()(message mess) { if (func){ func(ctx, mess); } }
  
  private:
  friend class bus;
  void (*func)(void* ctx, message mess); void* ctx; };

class bus
{ public:
  bus() {}

  bus& operator +(callback cb) { nodes.push_back(cb); return *this; }
  
  bus& operator -(callback cb)
  { nodes.remove_if([&cb](callback& i)
                    { return i.func == cb.func && i.ctx == cb.ctx; });
    return *this; }

  bus& operator +(void (*func)(void* ctx, message mess))
  { return operator +(callback(func, nullptr)); }

  bus& operator -(void (*func)(void* ctx, message mess))
  { return operator -(callback(func, nullptr)); }
  
  bus& operator ()(message mess)
  { for (callback& node : nodes) { node(mess); } return *this; }

  private:
  std::list<callback> nodes; };

struct path
{ path(std::string node = "") { if (!node.empty()) { nodes.push_back(node); } }

  path(int idx) { nodes.push_back(std::to_string(idx)); }

  path& operator /(std::string node)
  { nodes.push_back(node); return *this; }
  
  path& operator /(int idx)
  { nodes.push_back(std::to_string(idx)); return *this; }

  private:
  friend class storage;
  std::list<std::string> nodes; };

class storage
{ public:

  storage(std::string root_name = "root") { root.val.name = root_name; }
  
  value& operator [](path p)
  { item* current = &root;
    for (std::string node : p.nodes)
    { item* next = nullptr;
      for (item& child : current->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { current->childs.push_back(item(node));
                   next = &(current->childs.back()); }
      current = next; }

    return current->val; }

  value& operator ()(path p)
  { item* current = &root;
    for (std::string node : p.nodes)
    { item* next = nullptr;
      for (item& child : current->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { empty = value(); return empty; }
      current = next; }
    return current->val; }
  
  bool chk(path p)
  { item* current = &root;
    for (std::string node : p.nodes)
    { item* next = nullptr;
      for (item& child : current->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { return false; }
      current = next; }
    return true; }
  
  std::list<std::string> ls(path p)
  { item* current = &root;
    for (std::string node : p.nodes)
    { item* next = nullptr;
      for (item& child : current->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { return std::list<std::string>(); }
      current = next; }

    std::list<std::string> ret;
    for (item& child : current->childs) { ret.push_back(child.val.name); }
    return ret; }

  bool cp(path src, path dst, storage* sdst = nullptr)
  { if (!sdst) { sdst = this; }
    
    item* _src = &root;
    for (std::string node : src.nodes)
    { item* next = nullptr;
      for (item& child : _src->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { return false; }
      _src = next; }

    item* _dst = &sdst->root;
    for (std::string node : dst.nodes)
    { item* next = nullptr;
      for (item& child : _dst->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { return false; }
      _dst = next; }

    _dst->childs = _src->childs;

    return true; }

  bool mv(path src, path dst, storage* sdst = nullptr)
  { if (!sdst) { sdst = this; }
    (*sdst)[dst];
    if (!cp(src, dst, sdst)) { return false; }
    if (!del(src)) { return false; }
    return true; }
  
  bool del(path p)
  { item* current = &root;
    std::string target = p.nodes.back(); p.nodes.pop_back();
    for (std::string node : p.nodes)
    { item* next = nullptr;
      for (item& child : current->childs)
      { if (child.val.name == node) { next = &child; break; } }
      if (!next) { return false; }
      current = next; }
    
    bool found = false;
    for (item& child : current->childs)
    { if (child.val.name == target) { found = true; break; } }
    if (!found) { return false; }

    current->childs.remove_if([&target](item& i)
                              { return i.val.name == target; });

    return true; }

  std::string serialize()
  { std::string res; serialize(res, 0, root); return res; }

  storage& parse(std::string in)
  { root.childs.clear(); root.val = value(); path cp;
    bool next = false;
    bool esc = false; std::string curstr; value curval;
    for (char c : in)
    { if (esc) { curstr.push_back(c); esc = false; }
      else if (c == '\\') { esc = true; }

      else if (c == '(') { curstr.clear(); }
      else if (c == ')') { curval = value(); curval.name = curstr;
                           if (next) { next = false;
                                       cp.nodes.push_back(curstr); }
                           else      { if (cp.nodes.size())
                                       { cp.nodes.back() = curstr; } }
                           curstr.clear();
                           operator[](cp) = curval; }

      else if (c == '<') { curstr.clear(); }
      else if (c == '>') { curval.type = curstr; curstr.clear();
                           operator[](cp) = curval; }

      else if (c == '[') { curstr.clear(); }
      else if (c == ']') { curval.data = curstr; curstr.clear();
                           operator[](cp) = curval; }

      else if (c == '{') { next = true; }
      else if (c == '}') { cp.nodes.pop_back(); }

      else { curstr.push_back(c); } }
    return *this; }

  private:
  struct item { item() {}
                item(std::string name) { val.name = name; }
                item(value val) : val(val) {}
                value val; std::list<item> childs; };

  std::string escape(std::string in)
  { std::string res;
    for (char c : in)
    { if (c == '(' || c == ')' || c == '<' || c == '>' ||
          c == '[' || c == ']' || c == '{' || c == '}' ||
          c == '\\')
      { res.push_back('\\'); }
      res.push_back(c); }
    return res; }

  void serialize(std::string& str, unsigned int spacing, item& i)
  { for (unsigned int i = 0; i < spacing; i++) { str.append("    "); }
    str.append("(").append(escape(i.val.name)).append(")");
    if (!i.val.type.empty())
    { str.append("<").append(escape(i.val.type)).append(">"); }
    if (!i.val.data.empty())
    { str.append("[").append(escape(i.val.data)).append("]"); }
    if (i.childs.size())
    { str.append(" {\n");
      for(item& child : i.childs) { serialize(str, spacing + 1, child); }
      for (unsigned int i = 0; i < spacing; i++) { str.append("    "); }
      str.append("}"); }
    str.append("\n"); }

  item root;
  value empty;
};

} // independency

#endif // INDEPENDENCY_HPP
