#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

emscripten::val globalIOContext = emscripten::val::null();

namespace
{
   void log(const std::string &message)
   {
      if (!globalIOContext.isNull() && globalIOContext.hasOwnProperty("log"))
      {
         globalIOContext.call<void>("log", "LOG: " + message);
      }
      else
      {
         std::cout << "stream-log: " << message << std::endl;
      }
   }

   void setIOContext(emscripten::val ioContext)
   {
      globalIOContext = ioContext;
      log("Hello World!");
   }

   void createShape(std::string id, float size)
   {
      std::cout << "LOG: " << "createShape" << " " << id << " " << size << std::endl;

      const float s_8_9 = static_cast<float>(std::sqrt(8.0 / 9.0));
      const float s_2_9 = static_cast<float>(std::sqrt(2.0 / 9.0));
      const float s_2_3 = static_cast<float>(std::sqrt(2.0 / 3.0));
      const float s_1_3 = static_cast<float>(1.0 / 3.0);
      const auto v = std::vector<float>{0, 0, 1, s_8_9, 0, -s_1_3, -s_2_9, s_2_3, -s_1_3, -s_2_9, -s_2_3, -s_1_3};
      const auto t = std::vector<float>{0, 0, 0, 1, 1, 1, 1, 0};
      const auto ni = std::vector<uint32_t>{3, 1, 2, 0};
      const auto e = std::vector<uint32_t>{0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 3, 2};

      float l = size / 2.0f;
      std::vector<float> vertices;
      std::vector<float> normals;
      std::vector<float> uvs;
      std::vector<uint32_t> indices(12);
      std::iota(indices.begin(), indices.end(), 0); 
      for (uint32_t si = 0; si < 4; ++si)
      {
         for (uint32_t pi = 0; pi < 3; ++pi)
         {
            uint32_t i = si * 3 + pi;
            vertices.insert(vertices.end(), {l * v[e[i] * 3], l * v[e[i] * 3 + 1], l * v[e[i] * 3 + 2]});
            normals.insert(normals.end(), {-v[ni[si] * 3], -v[ni[si] * 3 + 1], -v[ni[si] * 3 + 2]});
            uvs.insert(uvs.end(), {t[e[i] * 2], t[e[i] * 2 + 1]});
         }
      }

      if (!globalIOContext.isNull() && globalIOContext.hasOwnProperty("newShape"))
      {
         globalIOContext.call<void>(
            "newShape",
            id,
            emscripten::val(emscripten::typed_memory_view(vertices.size(), vertices.data())),
            emscripten::val(emscripten::typed_memory_view(indices.size(), indices.data())),
            emscripten::val(emscripten::typed_memory_view(normals.size(), normals.data())),
            emscripten::val(emscripten::typed_memory_view(uvs.size(), uvs.data()))
         );
      }
   }

   std::string getVersion()
   {
      return "1.0.0";
   }
}

EMSCRIPTEN_BINDINGS(my_module)
{
   emscripten::function<void, emscripten::val>("setIOContext", &setIOContext);
   emscripten::function<void, std::string, float>("createShape", &createShape);
   emscripten::function<std::string>("getVersion", &getVersion);
}