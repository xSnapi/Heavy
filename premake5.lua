-- premake5.lua
workspace "Heavy Engine"
   startproject "Sandbox"
   architecture "x64"
   configurations {
      "Debug", 
      "Release" 
   }

   filter { "platforms:Win64" }
      system "Windows"
      
include "Heavy"
include "Sandbox"