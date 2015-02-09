// mod_node_canna.cc
#include <node.h>
#include "nodecanna.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  NodeCanna::Init(exports);
}

NODE_MODULE(node_canna, InitAll)
