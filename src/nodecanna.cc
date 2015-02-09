// nodecanna.cc
#include <cstring>
#include "nodecanna.h"

using namespace v8;

static CannaSession* session;

NodeCanna::NodeCanna() {}
NodeCanna::~NodeCanna() {}

void NodeCanna::Init(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("NodeCanna"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("Open"),
                                FunctionTemplate::New(Open)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("Convert"),
                                FunctionTemplate::New(Convert)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("Close"),
                                FunctionTemplate::New(Close)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("NodeCanna"), constructor);
}

Handle<Value> NodeCanna::New(const Arguments& args) {
  HandleScope scope;
  NodeCanna* obj = new NodeCanna();
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> NodeCanna::Open(const Arguments& args) {
  HandleScope scope;
  session = new CannaSession();
  return args.This();
}

Handle<Value> NodeCanna::Convert(const Arguments& args) {
  HandleScope scope;
  char retbuf[CANNA_BUFSIZE];

  memset(retbuf, 0, CANNA_BUFSIZE);
  v8::String::Utf8Value str(args[0]->ToString());

  char* c_arg = *str;
  session->convert(c_arg);
  snprintf(retbuf, CANNA_BUFSIZE, "{\"kana\":\"%s\",\"kanj\":\"%s\"}",
           session->kana, session->kanj);

  return scope.Close(String::New(retbuf));
}

Handle<Value> NodeCanna::Close(const Arguments& args) {
  HandleScope scope;
  delete session;
  return args.This();
}
