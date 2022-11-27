#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <iostream>

std::string getText() {
   return "Hello world";
}

EMSCRIPTEN_BINDINGS(my_module) {
   emscripten::function("getText", &getText);
}