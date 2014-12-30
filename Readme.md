cmatch
======

1. clang libclang extension functions and datatypes for dynamic text based ast matching  
2. a command line utility to match ast   

Features:  
-> integrated with zsh completion system  
-> run methods on matched ast nodes like   
-> possible integration with scripting languages

usage: src/cmatch <options>  
 -a ast    --ast-file               ast files to operate on  
 -m expr   --match-expr             ast match expression  
 -r chain  --run                    run method chain (default is dump)  
 -C expr   --compl-match-expr       complete ast expression  
 -c expr   --compl-match-expr-det   complete ast expression with argument detail  
 -l expr   --list-methods           list methods that can be called for a match  

example:     
   
cmatch \   
  -a SemaAst.ast \   
  -m  "methodDecl( \   
      returns(pointsTo(hasDeclaration(recordDecl(isDerivedFrom(anyOf( \   
                           recordDecl(hasName(\"Decl\")),  \   
                           recordDecl(hasName(\"Stmt\")),  \   
                           recordDecl(hasName(\"Type\")))))))),\   
      parameterCountIs(0), isPublic(),  \   
      hasParent(recordDecl(isDerivedFrom(anyOf( \   
                     recordDecl(hasName(\"Decl\")), \   
                     recordDecl(hasName(\"Stmt\")), \   
                     recordDecl(hasName(\"Type\")))))) "\   
  -r getParent.getNameAsString.dump   
   
   
the above will match all methodDeclarations:   
-> of subclasses of Decl, Stmt, Type that   
-> have zero parameters   
-> return subclasses of Decl, Stmt, Type   
   
and will run the method chain getParent.getNameAsString.dump. With other words will print all    
clas names that methods belong to   
   
Limitations:     
-> you need to generate the ast file with clang     
   
   
   


