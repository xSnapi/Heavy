project "Heavy"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"
  
   targetdir "bin/%{cfg.buildcfg}"
   objdir("bin/Obj/%{cfg.buildcfg}")

   pchheader "hvpch.h"
   pchsource "core/hvpch.cpp"

   files {
      "include/**.h", 
      "include/**.hpp",
      "src/**.cpp",
      "*.lua",
      "core/hvpch.h",
      "core/hvpch.cpp",

      "extlibs/**.cpp",
      "extlibs/**.h",
   }

   includedirs {
      "core",
      "include",
      "../include",
      "../include/Optick"
   }

   defines { 
      "SFML_STATIC" 
   }

   linkoptions { 
      "/ignore:4006" 
   }

   links {
      "OptickCore.lib",
      "opengl32.lib",
      "openal32.lib",
      "freetype.lib",
      "winmm.lib",
      "gdi32.lib",
      "flac.lib",
      "vorbisenc.lib",
      "vorbisfile.lib",
      "vorbis.lib",
      "ogg.lib",
      "ws2_32.lib",
      "legacy_stdio_definitions.lib",
   }

   filter "files:extlibs/**.cpp"
      flags { "NoPCH" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      libdirs { 
         "../libs/SFML/lib/Debug",
         "../libs/SFML/extlibs",

         "../libs/Optick/lib/Debug"
      }

      links {
         "sfml-graphics-s-d.lib",
         "sfml-window-s-d.lib",
         "sfml-system-s-d.lib",
         "sfml-audio-s-d.lib",
         "sfml-network-s-d.lib"
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      libdirs { 
         "../libs/SFML/lib/Release",
         "../libs/SFML/extlibs",

         "../libs/Optick/lib/Release",
      }

      links {
         "sfml-graphics-s.lib",
         "sfml-window-s.lib",
         "sfml-system-s.lib",
         "sfml-audio-s.lib",
         "sfml-network-s.lib"
      }
