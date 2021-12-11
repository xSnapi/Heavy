project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"

   targetdir "bin/%{cfg.buildcfg}"
   objdir("bin/Obj/%{cfg.buildcfg}")

   files {
      "**.h", 
      "**.hpp", 
      "**.cpp",
      "*.lua",
      "*.dll"
   }

   includedirs {
      "../Heavy/include",
      "../include",
   }

   links { 
      "Heavy" 
   }
   
   defines { 
      "SFML_STATIC" 
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
