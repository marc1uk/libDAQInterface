#!/bin/bash

Dependencies=`pwd`/Dependencies

export LD_LIBRARY_PATH=`pwd`/lib:${Dependencies}/zeromq-4.0.7/lib:${Dependencies}/boost_1_66_0/install/lib:${Dependencies}/ToolDAQFramework/lib:${Dependencies}/ToolFrameworkCore/lib:${Dependencies}/libpqxx-6.4.5/install/lib:$LD_LIBRARY_PATH
#. ${Dependencies}/root_v6.26.04/bin/thisroot.sh
