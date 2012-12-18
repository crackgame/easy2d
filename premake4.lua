local action = _ACTION or ""

solution "easy2d"
    location ( "Build/" .. action )
    configurations { "Debug", "Release" }
    kind "WindowedApp"
    language "C++"
    defines "WIN32"
    includedirs
        {
            "$(DXSDK_DIR)Include",
            "3Party/glew-1.9.0/include",
        }
    libdirs
        {
            "$(DXSDK_DIR)Lib/x86",
            "3Party/glew-1.9.0/lib",
        }
    configuration "vs*"
	linkoptions { "/ENTRY:WinMain" }
        defines { "_CRT_SECURE_NO_WARNINGS", "WIN32_LEAN_AND_MEAN"}   
    configuration "Debug"
        targetdir ( "Build/" .. action .. "/bin/Debug" )
        defines {"_DEBUG"}
        flags { "Symbols" }
   configuration "Release"
        targetdir ( "Build/" .. action .. "/bin/Release" )
        defines { "NDEBUG" }
        flags { "Optimize" }

    project "easy2d"
        files {"easy2d/*.cpp", "easy2d/*.h"}
        defines {"_WINDOWS"}
            
