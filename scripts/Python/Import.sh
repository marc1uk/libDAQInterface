#!/bin/bash
#set -x
#set -e

thisdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# we take 2 arguments
# first arg is the directory of the application we're adding python support to
# second arg is where to install dependencies; cppyy packages / ROOT
if [ $# -lt 1 ]; then
	echo "usage: $0 [directory of parent application] [installation directory=${thisdir}]"
	exit 1
fi

ToolAppPath=$1
# check the given directory exists and is writable
if [ ! -d ${ToolAppPath} ] || [ ! -w ${ToolAppPath} ]; then
	echo "Application directory ${ToolAppPath} is not present or is not writable!"
	exit 1
fi

INSTALLDIR="$thisdir"
if [ $# -gt 1 ]; then
	INSTALLDIR="$2"
fi

# perform installation of cppyy (or ROOT, which has cppyy internally)
echo "Calling installer script for dependencies"
#${thisdir}/Install.sh ${INSTALLDIR} ${ToolAppPath}
${thisdir}/Install_root.sh ${INSTALLDIR} ${ToolAppPath}
if [ $? -ne 0 ]; then
	echo "Install script returned an error, aborting"
	exit 1
fi

exit 0
