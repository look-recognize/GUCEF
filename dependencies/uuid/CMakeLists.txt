#-------------------------------------------------------------------
# This file was automatically generated by ProjectGenerator
# which is tooling part the build system designed for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------
#

# Configuration for module: uuid

set( HEADER_FILES 
  config.h
  uuidd.h
  uuidP.h
  uuid_types.h
)

set( SOURCE_FILES 
  clear.c
  compare.c
  copy.c
  gen_uuid.c
  gen_uuid_nt.c
  isnull.c
  pack.c
  parse.c
  tst_uuid.c
  unpack.c
  unparse.c
  uuid_time.c
)

# Make sure the PLATFORM_HEADER_FILES variable is always defined
 set( PLATFORM_HEADER_FILES "" )

# Make sure the PLATFORM_SOURCE_FILES variable is always defined
 set( PLATFORM_SOURCE_FILES "" )

# For ease of use make a variable that has all files for this module
set( ALL_FILES ${HEADER_FILES} ${SOURCE_FILES} ${PLATFORM_HEADER_FILES} ${PLATFORM_SOURCE_FILES} )

set( MODULE_NAME "uuid" )
add_library( ${MODULE_NAME} STATIC ${ALL_FILES} )




#******** START OF AUTOGENERATED TEMPLATED ADDITION CONTENT ********




if (OSX)
GUCEF_config_lib( ${MODULE_NAME} )
else()
GUCEF_config_lib( ${MODULE_NAME} )
endif()



#******** END OF AUTOGENERATED TEMPLATED ADDITION CONTENT ********


