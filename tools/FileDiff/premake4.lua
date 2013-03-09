--------------------------------------------------------------------
-- This file was automatically generated by ProjectGenerator
-- which is tooling part the build system designed for GUCEF
--     (Galaxy Unlimited Framework)
-- For the latest info, see http://www.VanvelzenSoftware.com/
--
-- The contents of this file are placed in the public domain. Feel
-- free to make use of it in any way you like.
--------------------------------------------------------------------
--

-- Configuration for module: FileDiff

project( "FileDiff" )
location( os.getenv( "PM4OUTPUTDIR" ) )
configuration( { "WIN32" } )
kind( "WindowedApp" )
configuration( { "NOT WIN32" } )
  kind( "ConsoleApp" )
links( { "gucefCORE" "gucefMT" } )
links( { "gucefCORE" "gucefMT" } )


configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/FileDiff_main.cpp"
 } )


configuration( {} )
includedirs( { "../../common/include", "../../gucefCORE/include", "../../gucefMT/include" } )

configuration( { "ANDROID" } )
includedirs( { "../../gucefCORE/include/android" } )

configuration( { "LINUX" } )
includedirs( { "../../gucefCORE/include/linux" } )

configuration( { "WIN32" } )
includedirs( { "../../gucefCORE/include/mswin" } )

configuration( { "WIN64" } )
includedirs( { "../../gucefCORE/include/mswin" } )
