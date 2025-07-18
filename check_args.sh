#!/bin/bash

# Print the command name
echo "Command: $1"

# Shift to remove the command from the argument list
shift

# Print number of remaining arguments
echo "Number of arguments: $#"

# Print the actual arguments
echo "Arguments: $@"

#---------------------------------------------------------
# EXAMPLE_OF_USAGE ./check_args.sh ls -l /home
# RESULT: 
# 
# Command: ls
# Number of arguments: 2
# Arguments: -l /home
