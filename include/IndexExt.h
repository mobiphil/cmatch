#ifndef INDEX_EXT_H
#define INDEX_EXT_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief function type used as callback for a match expression completion
 *
 * 
 */
typedef int (*ASTMatchCompletionCallback)(const char *typedText, const char *matcherDecl);


/**
 * \brief complete an ast match expression
 *
 * the completion will be passed to the callback provided
 */
CINDEX_LINKAGE
int clang_completeASTMatchExpression(const char *text, ASTMatchCompletionCallback callback);



/**
 * \brief name value map used for matching
 */
typedef void *NamedValueMap;

/**
 * \brief variant value used in the name value map
 */
typedef void *VarVal; 

/**
 * \brief create a named value map used for AST match
 */
CINDEX_LINKAGE
NamedValueMap clang_createNamedValueMap();

/**
 * \brief free the given named value map
 */
CINDEX_LINKAGE
void clang_disposeNamedValueMap(NamedValueMap NamedValueMap);

/**
 * \brief adds a name value pair to the map
 *
 * the result can be further added to the dictionary or used for a match find
 */
CINDEX_LINKAGE
void clang_NamedValueMap_addNamedValue(NamedValueMap NamedValueMap, const char *name, VarVal Value);


/**
 * \brief ADT for representing a matched node
 *
 * such nodes are further used to run methods on the nodes
 */
typedef struct { 
   unsigned classId;
   const void *node;
   const char *status;
} AstAny;

/**
 * \brief equivalent of clang::ASTUnit class 
 */
typedef void* AstUnit ;

typedef int (*AstMatchCallback)(const AstAny node, void *userData);
/**
 * \brief matches the expr and named value map against ast
 */
CINDEX_LINKAGE
int clang_matchAst(CXTranslationUnit astUnit, NamedValueMap namedValueMap, 
      const char *expr, AstMatchCallback, void *userData );


/**
 * \brief run the method with name method
 *
 * FIXME ... implement here the method chain in form of 
 * getParent.getNameAsString.dump
 */
CINDEX_LINKAGE
AstAny clang_AstAny_runMethod(AstAny node, const char *method);

/**
 * \brief function type used as callback for method list
 *
 */
typedef int (*MatcherMethodsCallback)(const char *className, const char *methodName);


/**
 * \brief list the methods available for a match expression
 *
 * the methods will be listed by calling the callback for each one
 *
 */
CINDEX_LINKAGE
int clang_listMatcherMethods(const char *expr, MatcherMethodsCallback);

#ifdef __cplusplus
}
#endif

#endif
