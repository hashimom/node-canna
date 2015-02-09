// nodecanna.h
#ifndef NODECANNA_H
#define NODECANNA_H

#include <node.h>
#include <canna/jrkanji.h>

#define CANNA_BUFSIZE (1024)

class NodeCanna : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> target);

 private:
  NodeCanna();
  ~NodeCanna();
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Open(const v8::Arguments& args);
  static v8::Handle<v8::Value> Convert(const v8::Arguments& args);
  static v8::Handle<v8::Value> Close(const v8::Arguments& args);
};

class CannaSession {
 public:
  CannaSession();
  ~CannaSession();
  int convert(char *inromaji);
  char kana[CANNA_BUFSIZE];
  char kanj[CANNA_BUFSIZE];

 private:
  jrKanjiStatus ks;
};

#endif
