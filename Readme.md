cmatch
======

1. clang libclang extension functions and datatypes for dynamic text based ast matching  
2. a command line utility to match ast   

Features:  
-> integrated with zsh completion system  
-> run methods on matched ast nodes like   

usage: src/cmatch <options>
 -a ast    --ast-file               ast files to operate on__
 -m expr   --match-expr             ast match expression__
 -r chain  --run                    run method chain (default is dump)__
 -C expr   --compl-match-expr       complete ast expression__
 -c expr   --compl-match-expr-det   complete ast expression with argument detail__
 -l expr   --list-methods           list methods that can be called for a match__

example:


Limitations:
-> you need to generate the ast file with clang
-> missing CXA
