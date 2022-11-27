#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
#include <iostream>

emscripten::val globalIOContext = emscripten::val::null();

namespace {
   void log(const std::string &message) {
      if (!globalIOContext.isNull() && globalIOContext.hasOwnProperty("log")) {
         globalIOContext.call<void>("log", "LOG: " + message);
      } else {
         std::cout << "stream-log: " << message << std::endl;
      }
   }

   void setIOContext(emscripten::val ioContext) {
      globalIOContext = ioContext;
      log("Hello World!");
   } 

   std::string getText() {
      return "Hello World!";
   }
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function<void, emscripten::val>("setIOContext", &setIOContext);
   emscripten::function("getText", &getText);
}