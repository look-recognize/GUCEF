#!/bin/bash
#
# Turn on tracing, we want to see what's going on
#set -x

#------------------------------------------------------------------------------

function SetGucefHome {

  # Set GUCEF_HOME if it is not set yet
  GUCEF_HOME=${GUCEF_HOME:=undefined}
  if [ "$GUCEF_HOME" = "undefined" ]; then
    echo "GUCEF_HOME environment variable not found, setting it"
    GUCEF_HOME="$GENERATEPREMAKE4_SCRIPTSTARTDIR/../.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

function FindGucefDebugProjectGenerator {

  TEST_PATH="$GUCEF_HOME/common/bin/CBNIXDBG/bin/ProjectGenerator"
  echo "Testing for executable binary @ $TEST_PATH"
  if [ -x "$TEST_PATH" ];
  then
    echo "Found Debug version of GUCEF ProjectGenerator"
    GENERATORPATH=$TEST_PATH
  fi

}

#------------------------------------------------------------------------------

function FindGucefReleaseProjectGenerator {

  TEST_PATH="$GUCEF_HOME/tools/ProjectGenerator/bin/ReleasedBins/Linux/20January2012/ProjectGenerator"
  echo "Testing for executable binary @ $TEST_PATH"
  if [ -x "$TEST_PATH" ];
  then
    echo "Found Release version of GUCEF ProjectGenerator dated 20th of January 2012"
    GENERATORPATH=$TEST_PATH
  fi

}

#------------------------------------------------------------------------------

function FindProjectGenerator {

  GENERATORPATH=${GENERATORPATH:=undefined}
  FindGucefDebugProjectGenerator
  if [ "$GENERATORPATH" = "undefined" ];
  then
    FindGucefReleaseProjectGenerator
  fi
}

#------------------------------------------------------------------------------

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
GENERATEPREMAKE4_SCRIPTSTARTDIR=${scriptPath%/*}
echo "GENERATEPREMAKE4_SCRIPTSTARTDIR = $GENERATEPREMAKE4_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

# Determine a path to a ProjectGenerator binary
GENERATORPATH=${GENERATORPATH:=undefined}
FindProjectGenerator
if [ "$GENERATORPATH" = "undefined" ]; then
  echo "Error: Unable to find a ProjectGenerator binary"
  exit
fi
echo "GENERATORPATH = $TEST_PATH"

# invoke the generator
#$GENERATORPATH *rootDir=$GUCEF_HOME* *dirsToIgnore=.svn* *projectName=GUCEF* *generators=premake4* *outputDir=$GUCEF_HOME* *premake4gen:TemplateDir=$GUCEF_HOME/projects/premake4/ProjectGenTemplates* *premake4gen:PM4OutputDir=ENVVAR:PM4OUTPUTDIR*




