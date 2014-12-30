#include "clang/Config/config.h"
#include "clang-c/Index.h"
#include "IndexExt.h"
#include "clang-c/CXCompilationDatabase.h"
#include "clang-c/BuildSystem.h"
#include "clang-c/Documentation.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>


int cbCompl(const char *text, const char *completion) {
   printf("%s\n", text);
   return 1;

}

int cbComplDetalied(const char *text, const char *completion) {
   printf("%s: %s\n", text, completion);
   return 1;

}

static int completeMatchExpression(const char *text, ASTMatchCompletionCallback cb) {
   if(text == 0)
      text = "";
   clang_completeASTMatchExpression(text, cb);
   return 0;
}

int matchCallback(AstNode node, void *userData) {

   const char *selectorPath = (const char *)userData;
   char selbuf[100];
   const char *selector;
   const char *pos;
   do {
      pos = strchr(selectorPath, '.');
      if(pos) {
         memcpy(selbuf, selectorPath, pos-selectorPath);
         selbuf[pos-selectorPath] = 0;
         selector = selbuf;
         selectorPath = pos + 1;
      }
      else 
         selector = selectorPath;
      node = clang_AstNode_runMethod(node, selector);
      if(node.classId == 0)
      {
         printf("%s\n", (const char*)node.status);
         return -1;
      }
   } while(pos);

   return 1;
}

int cbListMethodNames(const char *methodName) {
   printf("%s", methodName);
   return 0;
}

static int listMethods(const char *pattern)
{
   return clang_listMatcherMethods(pattern, cbListMethodNames);

}

static void describeLibclangFailure(enum CXErrorCode Err) {
   switch (Err) {
      case CXError_Success:
         fprintf(stderr, "Success\n");
         return;

      case CXError_Failure:
         fprintf(stderr, "Failure (no details available)\n");
         return;

      case CXError_Crashed:
         fprintf(stderr, "Failure: libclang crashed\n");
         return;

      case CXError_InvalidArguments:
         fprintf(stderr, "Failure: invalid arguments passed to a libclang routine\n");
         return;

      case CXError_ASTReadError:
         fprintf(stderr, "Failure: AST deserialization error occurred\n");
         return;
   }
}

static unsigned CreateTranslationUnit(CXIndex Idx, const char *file,
      CXTranslationUnit *TU) {
   enum CXErrorCode Err = clang_createTranslationUnit2(Idx, file, TU);
   if (Err != CXError_Success) {
      fprintf(stderr, "Unable to load translation unit from '%s'!\n", file);
      describeLibclangFailure(Err);
      *TU = 0;
      return 0;
   }
   return 1;
}

int specialCallback(AstNode node, void *userData) {

   AstNode result1, result2, result3;
   AstNode result = clang_AstNode_runMethod(node, "getDeclName");
   result = clang_AstNode_runMethod(result, "getAsString");
   result1 = clang_AstNode_runMethod(result, "toChar");

   result = clang_AstNode_runMethod(node, "getReturnType");
   result = clang_AstNode_runMethod(result, "getAsString");
   result2 = clang_AstNode_runMethod(result, "toChar");

   result = clang_AstNode_runMethod(node, "getParent");
   result = clang_AstNode_runMethod(result, "getNameAsString");
   result3 = clang_AstNode_runMethod(result, "toChar");


   /* TODO implement dispose clang function and call it on objects that need to be disposed */
   printf("%s::%s -> %s\n", (const char *)result3.node, (const char*)result1.node, (const char*)result2.node);

   return 1;
}

static int special(const char *file) {
#if 0
   const char *pattern = "methodDecl(parameterCountIs(0), isPublic(), hasParent(recordDecl(hasName(\"Decl\"))))";
   const char *pattern = 
      "methodDecl( \
      returns(pointsTo(hasDeclaration(recordDecl(isDerivedFrom(anyOf( \
                           recordDecl(hasName(\"Decl\")),  \
                           recordDecl(hasName(\"Stmt\")),  \
                           recordDecl(hasName(\"Type\")))))))),\
      parameterCountIs(0), isPublic(),  \
      hasParent(recordDecl(isDerivedFrom(anyOf( \
                     recordDecl(hasName(\"Decl\")), \
                     recordDecl(hasName(\"Stmt\")), \
                     recordDecl(hasName(\"Type\")))))) \
      )"; 
#endif
      const char *pattern = 
      "methodDecl( \
      parameterCountIs(0), isPublic(),  \
      hasParent(recordDecl(isDerivedFrom(anyOf( \
                     recordDecl(hasName(\"Decl\")), \
                     recordDecl(hasName(\"Stmt\")), \
                     recordDecl(hasName(\"Type\")))))) \
      )"; 

      CXIndex Idx;
   CXTranslationUnit TU;
   Idx = clang_createIndex(1 , 1);

   if (!CreateTranslationUnit(Idx, file, &TU)) {
      clang_disposeIndex(Idx);
      return 1;
   }

   clang_matchAst(TU, (void*)0, pattern, &specialCallback, (void*)0);

   return 0;
}

int usage(const char *exename) {
   printf("usage: %s <options>\n"
      " -a ast    --ast-file              ast files to operate on\n" 
      " -s src    --src-file              source file to operate on (involves parsing etc)\n" 
      " -m expr   --match-expr            ast match expression\n" 
      " -r chain  --run                   run method chain (default is dump)\n"
      " -C expr   --compl-match-expr      complete ast expression\n"
      " -c expr   --compl-match-expr-det  complete ast expression with argument detail\n" 
      " -l expr   --list-methods          list methods that can be called for a match\n" , 
      exename);

}
int main(int argc, char *const*argv) {

   static struct option long_options[] =
   {
      {"compl-match-expr",     no_argument,             0, 'C'},
      {"compl-match-expr-detailed",  no_argument,       0, 'c'},
      {"match-expr",  required_argument,              0, 'm'},
      {"ast-file",  required_argument,                      0, 'a'},
      {"run",  required_argument,              0, 'r'},
      {0, 0, 0, 0}
   };
   /* getopt_long stores the option index here. */
   int option_index = 0;
   int hasAstFile = 0;
   int hasSrcFile = 0;
   int hasPattern = 0;
   int hasSelector = 0;
   char astFile[200];
   char srcFile[200];
   char pattern[200];
   char selector[200];
   int compl = 0;
   int list = 0;
   int complDet = 0;
   int matching = 0;
   ASTMatchCompletionCallback cb ;
   int argsConsumed = 1; /* bit hackish: to detect start of -- */
   while(1) {

      int c = getopt_long (argc, argv, "C:c:m:a:r:l:s:",
            long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
         break;
      switch (c)
      {
         case 'a': 
            strcpy(astFile, optarg);
            hasAstFile = 1;
            argsConsumed +=2;
            break;
         case 's': 
            strcpy(srcFile, optarg);
            hasSrcFile = 1;
            argsConsumed +=2;
            break;
         case 'c':
            compl = 1;
            complDet = 1;
            strcpy(pattern, optarg);
            argsConsumed +=2;
            break;
         case 'C':
            compl = 1;
            strcpy(pattern, optarg);
            hasPattern = 1;
            argsConsumed +=2;
            break;
         case 'm':
            matching = 1;
            hasPattern = 1;
            strcpy(pattern, optarg);
            argsConsumed +=2;
            break;
         case 'l':
            list = 1;
            hasPattern = 1;
            strcpy(pattern, optarg);
            argsConsumed +=2;
            break;
         case 'r':
            hasSelector = 1;
            strcpy(selector, optarg);
            argsConsumed +=2;
            break;
      }
   }
   

   if(hasAstFile && hasSrcFile)
   {
      printf("either source file or ast file should be specified\n");
      usage(argv[0]);
      return -1;
   }
   if(compl && matching)
   {
      printf("completion or matching functionalities are exclusive\n");
      usage(argv[0]);
      return -1;
   }
   if(compl)
   {
      if(hasAstFile)
         return usage(argv[0]);
      cb = cbCompl;
      if(complDet)
         cb = cbComplDetalied;
      return completeMatchExpression(pattern, cb);
   }
   if(matching && hasPattern ) {
      CXIndex Idx;
      CXTranslationUnit TU;
      Idx = clang_createIndex(1 , 1);
      
      if(hasAstFile) {
         if (!CreateTranslationUnit(Idx, astFile, &TU)) {
            clang_disposeIndex(Idx);
            return 1;
         }
      }
      else if(hasSrcFile) {
         if(argsConsumed < argc && strcmp(argv[argsConsumed], "--" )  == 0) {         
            /* on my ubuntu 14 there is a bug 
            * getopt_long aparently swaps the last argument before "--" and "--" itself
            */
            argsConsumed +=2;
            clang_parseTranslationUnit2(Idx, srcFile,
                  argv + argsConsumed,
                  argc - argsConsumed, 0,0,
                  0, &TU);
         }
         else {
            clang_parseTranslationUnit2(Idx, srcFile,
                  0,
                  0, 0,0,
                  0, &TU);

         }
          
      }
      else  {
         printf("either src or ast file must be specified\n");
         usage(argv[0]);
      }

      if(!hasSelector)
         strcpy(selector, "dump");
      return clang_matchAst(TU, (void*)0, pattern, &matchCallback, (void*)selector);
   }
   if(list && hasPattern) {
      return listMethods(pattern);
   }


   usage(argv[0]);

   return -1;
}
