#include "clang-c/Index.h"
#include "IndexExt.h"
#include "CXString.h"
#include "CXCursor.h"
#include "CXTranslationUnit.h"

#include "clang/ASTMatchers/Dynamic/VariantValue.h"
#include "clang/ASTMatchers/Dynamic/Registry.h"
#include "clang/ASTMatchers/Dynamic/Parser.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/ASTUnit.h"
#include "llvm/ADT/StringMap.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers::dynamic;
using namespace clang::ast_matchers;


#define Decl_do(type, object, method) \
   const Decl *res = object->method(); \
   result.classId = AstAnyMethodRunner::singleton()->classNameToId(res->getDeclKindName(), "Decl" ); \
   result.node = res;

#define Stmt_do(type, object, method) \
   const Stmt *res = object->method(); \
   result.classId = AstAnyMethodRunner::singleton()->classNameToId(res->getStmtClassName() ); \
   result.node = res;

#define Type_do(type, object, method) \
   const Type *res = object->method(); \
   result.classId = AstAnyMethodRunner::singleton()->classNameToId(res->getTypeClassName(), "Type" ); \
   result.node = obj;

#define char_do(type, object, method) \
   result.classId = cls_char; \
   result.node =  object->method();

#define void_do(type, object, method ) \
   result.classId = cls_void; \
   result.node = 0; \
   object->method();

#define pod_do(type, object, method) \
   result.classId = cls_##type;\
   type *ret = new type; \
   *ret = object->method(); \
   result.node = ret;

#define DeclarationName_do(type, object, method)   pod_do(type, object, method)
#define QualType_do(type, object, method)          pod_do(type, object, method)
#define bool_do(type, object, method)              pod_do(type, object, method)
#define string_do(type, object, method)              pod_do(type, object, method)
#define int_do(type, object, method)              pod_do(type, object, method)


#define void_poddo(type, object, method)  \
   result.classId = cls_void; \
   result.node = 0; \
   object->method();

#define Decl_poddo(type, object, method) \
   const Decl *res = object->method(); \
   result.classId = AstAnyMethodRunner::singleton()->classNameToId(res->getDeclKindName(), "Decl" ); \
   result.node = res;

#define Stmt_poddo(type, object, method) \
   const Stmt *res = object->method(); \
   result.classId = AstAnyMethodRunner::singleton()->classNameToId(res->getStmtClassName() ); \
   result.node = res;

#define Type_poddo(type, object, method) \
   const Type *res = object->method(); \
   result.classId = AstAnyMethodRunner::singleton()->classNameToId(res->getTypeClassName(), "Type" ); \
   result.node = obj;

#define DeclarationName_poddo(type, object, method)   pod_do(type, object, method)
#define QualType_poddo(type, object, method)          pod_do(type, object, method)
#define bool_poddo(type, object, method)              pod_do(type, object, method)
#define string_poddo(type, object, method)              pod_do(type, object, method)
#define unsigned_poddo(type, object, method)              pod_do(type, object, method)
#define int_poddo(type, object, method)              pod_do(type, object, method)

class AstAnyMethodRunner 
{
   private:
      enum MethodFunctionId  {
#define METHOD(kind, member, rettype) kind##_##member,
#include "DeclClassMethods.inc"
#include "StmtClassMethods.inc"
#include "TypeClassMethods.inc"
#include "PodMethods.inc"
#undef METHOD
};
#undef METHOD

public:
   enum ClassId {
      cls_none,
      cls_void,
      cls_char,
      cls_bool,
      cls_unsigned,
      cls_int,
      cls_string,
      cls_DeclarationName,
      cls_QualType,
      cls_Decl,
      cls_DeclDecl,
      //cls_NamedDeclDecl,
#define DECL(DERIVED, BASE) cls_##DERIVED##Decl,
#include "clang/AST/DeclNodes.inc"
      cls_Stmt,
#define STMT(DERIVED, BASE) cls_##DERIVED,
#include "clang/AST/StmtNodes.inc"
      cls_Type,
#define TYPE(DERIVED, BASE) cls_##DERIVED##Type,
#include "clang/AST/TypeNodes.def"
      cls_last
   };
   
   ClassId baseClasses[cls_last];
   typedef int MethodId;

   private:
   static AstAnyMethodRunner *singleton_ ;

   private:
   void initMethodsMaps();
   AstAnyMethodRunner() ;
   typedef int MethodNameId;
   map<string, MethodNameId> methodNameMap;
   vector<string> methodNameVector;

   /* ClassId + MethodNameId -> MethodId */
   map<MethodId, MethodFunctionId> methodMap;
   map<string, ClassId> classNameMap;

   public:
   AstAny runMethod(AstAny node, const char *methodName);
   static AstAnyMethodRunner *singleton();
   ClassId classNameToId(const char *name, const char *suffix = 0) {
      string n = name;
      if(suffix) n += suffix;

      ClassId ret = cls_none;
      map<string, ClassId>::iterator it = classNameMap.find(n) ; 
      if( it != classNameMap.end() )
         ret = it->second;
#ifdef DDDEBUG
      cout << __FUNCTION__ << ": " << name << ": " << (suffix ? suffix : "(none)" )<< ": " << ret << endl;
#endif 
      return ret;
   }
   
   int listMatcherMethods(const char *expr, MatcherMethodsCallback cb);

   void addToAllHierarchy(MethodNameId methodNameId, int classId, MethodFunctionId methodFunctionId) {
      /* we need to add the current method function to present class and all it's subclasses (inheritance) */ 
      methodMap[classId * 1000 + methodNameId ] = methodFunctionId;
      //printf("adding method function %d to class id: %d, method: %d \n", methodFunctionId, classId, methodNameId);

      for( int id = cls_none; id < cls_last; id++)
      {
         if (baseClasses[id] == classId)
            addToAllHierarchy(methodNameId, id, methodFunctionId) ;
      }

   }
};

int AstAnyMethodRunner::listMatcherMethods(const char *expr, MatcherMethodsCallback cb)
{
   //cout << "inside clang_matchAst" << endl;
   /* TODO ... threaded version? though the match is fast, printing is slow*/
  llvm::StringMap<ast_matchers::dynamic::VariantValue> *namedValues =
         static_cast<llvm::StringMap<ast_matchers::dynamic::VariantValue> *> (0);
   /* TODO ... is named value here needed, can be avoided? */
   Diagnostics diag ;
   Optional<DynTypedMatcher> Matcher = Parser::parseMatcherExpression(
         StringRef(expr), nullptr, namedValues, &diag);

    if (!Matcher) {
       /* TODO .. add diag */
       std::string ErrStr;
       llvm::raw_string_ostream OS(ErrStr);
       diag.printToStreamFull(OS);
       cout << __FUNCTION__ << ": Matcher not found: " << OS.str() << endl;
       return -1;
    }
    const char *className = Matcher->getRestrictKind().asStringRef().data();
    //asked clang-dev mailist to add method, without it, this will not work
    //const char *className = Matcher->getSupportedKind().asStringRef().data();
    unsigned cid = AstAnyMethodRunner::singleton()->classNameToId(className);
   
    //cout << "cid: " << cid << ", for: " << Matcher->getRestrictKind().asStringRef().data() << endl;
    /* find method name from methodNameMap through methodMap */
    for(int mid = cid*1000, to = (cid+1)*1000; 
          mid < to; mid++) {
       
       /*map<AstAnyMethodRunner::MethodId, 
          AstAnyMethodRunner::MethodFunctionId>::iterator it = methodMap.find( mid ); */
       auto it = methodMap.find(mid);
       if(it!=methodMap.end()) {
          /* TODO find the base class really implementing the method*/
          /* now find the method name id as the offset */
          cb(className, methodNameVector[mid - cid *1000].c_str());

       }
    }
    return 0;
}

void AstAnyMethodRunner::initMethodsMaps()
{
   /* record base class vector */
#ifdef DDDEBUG
#define DECL(DERIVED, BASE) baseClasses[cls_##DERIVED##Decl]  = cls_##BASE; \
   cout << "adding base: " << #DERIVED << ":" #BASE << ":" << cls_##DERIVED##Decl << ":" << cls_##BASE << endl;
#include "clang/AST/DeclNodes.inc"
#undef DECL
#define STMT(DERIVED, BASE) baseClasses[cls_##DERIVED]  = cls_##BASE; \
   cout << "adding base: " << #DERIVED << ":" #BASE << ":" << cls_##DERIVED << ":" << cls_##BASE << endl;
#include "clang/AST/StmtNodes.inc"
#undef STMT
#define TYPE(DERIVED, BASE) baseClasses[cls_##DERIVED##Type]  = cls_##BASE; \
   cout << "adding base: " << #DERIVED << ":" #BASE << ":" << cls_##DERIVED##Type << ":" << cls_##BASE << endl;
#include "clang/AST/TypeNodes.def"
#undef TYPE
#else
#define DECL(DERIVED, BASE) baseClasses[cls_##DERIVED##Decl]  = cls_##BASE; 
#include "clang/AST/DeclNodes.inc"
#undef DECL
#define STMT(DERIVED, BASE) baseClasses[cls_##DERIVED]  = cls_##BASE; 
#include "clang/AST/StmtNodes.inc"
#undef STMT
#define TYPE(DERIVED, BASE) baseClasses[cls_##DERIVED##Type]  = cls_##BASE; 
#include "clang/AST/TypeNodes.def"
#undef TYPE
#endif

   baseClasses[cls_none]  = cls_none;
   baseClasses[cls_void]  = cls_none;
   baseClasses[cls_char]  = cls_none;
   baseClasses[cls_bool]  = cls_none;
   baseClasses[cls_DeclarationName]  = cls_none;
   baseClasses[cls_QualType]  = cls_none;
   baseClasses[cls_Decl]  = cls_none;
   baseClasses[cls_Stmt]  = cls_none;

   /* create class name to id map */
   classNameMap["DeclDecl"] = cls_Decl;
#define DECL(DERIVED, BASE) classNameMap[#DERIVED "Decl"]  = cls_##DERIVED##Decl;
#include "clang/AST/DeclNodes.inc"
   classNameMap["StmtStmt"] = cls_Stmt;
#define STMT(DERIVED, BASE)  classNameMap[#DERIVED] = cls_##DERIVED;
#include "clang/AST/StmtNodes.inc"
   classNameMap["TypeType"] = cls_Type;
#define TYPE(DERIVED, BASE)  classNameMap[#DERIVED "Type"] = cls_##DERIVED##Type;
#include "clang/AST/TypeNodes.def"

   /* create method name map */
   int i = 0;
   map<string, int>::iterator it ;
#define METHOD(kind, member, rettype) \
   it = methodNameMap.find(#member) ;\
   if(it == methodNameMap.end()) {\
      methodNameVector.push_back(#member); \
      methodNameMap[#member] = i++; \
   }

#include "DeclClassMethods.inc"
#include "StmtClassMethods.inc"
#include "TypeClassMethods.inc"
#include "PodMethods.inc"



#undef METHOD

   /* we generate unique keys with class type and method id */
   int mid = 0;
#define METHOD(kind, mname, rettype) \
   mid = methodNameMap[#mname] ;\
   addToAllHierarchy(mid, cls_##kind, kind##_##mname);
#include "DeclClassMethods.inc"
#undef METHOD
#define METHOD(kind, mname, rettype) \
   mid = methodNameMap[#mname] ;\
   addToAllHierarchy(mid, cls_##kind, kind##_##mname);
#include "StmtClassMethods.inc"
#undef METHOD
#define METHOD(kind, mname, rettype)  \
   mid = methodNameMap[#mname] ;\
   addToAllHierarchy(mid, cls_##kind, kind##_##mname);
#include "TypeClassMethods.inc"
#undef METHOD
#define METHOD(kind, mname, rettype)  \
   mid = methodNameMap[#mname] ;\
   methodMap[cls_##kind * 1000 + mid ] = kind##_##mname;
#include "PodMethods.inc"
#undef METHOD

#ifdef DDDEBUG
   cout << "methodNameMap: " << endl;
   for( auto it : methodNameMap ) {
      cout << "method: " <<  it.first << ": " << it.second << endl;
   }
   cout << "methodmap: " << endl;
   for( auto it : methodMap ) {
      cout << "methodId: " << it.first << ": " << it.second << endl;
   }
   cout << "classmap: " << endl;
   for( auto it : classNameMap ) {
      cout << "class: " << it.first << ": " << it.second << endl;
   }
   cout << "base classes: " << endl;
   for( int id = cls_none; id < cls_last; id++)
   {
      cout << "baseclass: id: " << id << ", base: " << baseClasses[id] << endl; 
   }
#endif

#ifdef DDDEBUG
#define METHOD(kind, member, rettype) cout << "meth: " << kind##_##member << ":" << #kind << ":" << #member << endl;
#include "DeclClassMethods.inc"
#include "StmtClassMethods.inc"
#include "TypeClassMethods.inc"
#include "PodMethods.inc"
#undef METHOD
#endif

   for( int id = cls_none; id < cls_last; id++)
   {
      //checkBaseClassConsistency(id);
   }

}
AstAnyMethodRunner::AstAnyMethodRunner(){
   initMethodsMaps();
}

AstAnyMethodRunner *AstAnyMethodRunner::singleton_  = nullptr;

AstAnyMethodRunner *AstAnyMethodRunner::singleton(){
   if (AstAnyMethodRunner::singleton_ == 0)
      singleton_ = new AstAnyMethodRunner();
   return singleton_;
}

#include "clang/AST/ASTFwd.h"

AstAny AstAnyMethodRunner::runMethod(AstAny node, const char *methodName){

   //cout << __FUNCTION__ << ": " << methodName << endl;
   AstAny result;
   result.status = "success";
   result.node = 0;
   result.classId = cls_none;
   /* treat special case print */
   if(node.classId == cls_char || node.classId == cls_bool 
         || node.classId == cls_string) {
      if(node.classId == cls_string && strcmp(methodName, "toChar") == 0) {
         result.classId = cls_char; /* todo .. make distinction between char * that needs to be deleted and const one's */
         const string *s = (const string *)node.node;
         char *res = (char *)malloc(s->size()+1);
         memcpy(res, s->c_str(), s->size());
         *(res+s->size()) = 0;
         result.node = res;
         return result;
      }

      if(strcmp(methodName, "dump") != 0) {
         result.status = "char * type supports only dump method";
         return result;
      }
      else {
         result.classId = cls_void;
         switch(node.classId) {
            case cls_char:
               printf("%s", (const char*)node.node);
               break;
            case cls_string:
               printf("%s", ((const string*)node.node)->c_str());
               break;
            case cls_bool:
               printf("%s", node.node ? "true" : "false");
               break;
            case cls_DeclarationName:
               {
               /* very uggly... but should work.. we are at the C/C++ boundaries at the end */
               const DeclarationName *d  = (const DeclarationName *)node.node;
               d->dump();
               }
               break;
            case cls_QualType:
               {
               /* very uggly... but should work.. we are at the C/C++ boundaries at the end */
               const QualType *q  = (const QualType *)node.node;
               q->dump();
               }
               break;

         }
         return result;
      }
   }
   map<string, int>::iterator mnit = methodNameMap.find(methodName);
   if(mnit == methodNameMap.end()) {
      result.status = "method name not found";
      result.classId = cls_none;
      return result;
   }
   int mnid = mnit->second;
   map<MethodId, MethodFunctionId>::iterator it = 
      methodMap.find(node.classId * 1000 + mnid);
   if (it == methodMap.end()) {
      cout << "method name not found: " << methodName << endl;
      result.status = "method name not found for given object";
      result.node = 0;
      //printf("function not found for class id: %d, method: %d \n", node.classId, mnid);
      return result;
   }
   switch(it->second) {
#define METHOD(kind, method, rettype) \
            case kind##_##method: {\
               const kind *obj = static_cast<const kind *>(node.node); \
               if(obj!=0) { \
                     rettype##_do(rettype, obj, method); \
               } \
               else { \
                  result.classId = cls_none; \
                  result.status = "trying to call method on null pointer "; \
                  result.node = 0; \
               }} break;
#include "DeclClassMethods.inc"
#include "StmtClassMethods.inc"
#include "TypeClassMethods.inc"
#undef METHOD
#define METHOD(kind, method, rettype) \
            case kind##_##method: {\
               const kind *obj = static_cast<const kind *>(node.node); \
               rettype##_poddo(rettype, obj, method);}  \
               break ;
#include "PodMethods.inc"
            default: 
               cout << "method not found: " << methodName << endl;
               break;
   }
   
   return result;
}

extern "C" {

AstAny clang_AstAny_runMethod(AstAny node, const char *methodName)
{
   return AstAnyMethodRunner::singleton()->runMethod(node, methodName);
}



CINDEX_LINKAGE
NamedValueMap clang_createNamedValueMap()
{
  return new llvm::StringMap<ast_matchers::dynamic::VariantValue> ;
}

CINDEX_LINKAGE
void clang_disposeNamedValueMap(NamedValueMap NamedValueMap)
{
  llvm::StringMap<ast_matchers::dynamic::VariantValue> *namedValues =
         static_cast<llvm::StringMap<ast_matchers::dynamic::VariantValue> *> (NamedValueMap);

  if(!namedValues)
     return;
         
  delete namedValues;
}

CINDEX_LINKAGE
void clang_NamedValueMap_addNamedValue(NamedValueMap NamedValueMap, const char *name, VarVal Value)
{
  llvm::StringMap<ast_matchers::dynamic::VariantValue> *namedValues =
         static_cast<llvm::StringMap<ast_matchers::dynamic::VariantValue> *> (NamedValueMap);
  ast_matchers::dynamic::VariantValue *value = static_cast<ast_matchers::dynamic::VariantValue *>(Value);

  if(!namedValues) return;
  if(!name) return;
  if(!value) return;
  
  (*namedValues)[name] = *value;
}
#include "clang/AST/ASTTypeTraits.h"


enum ClassClass {
   CC_none,
   CC_void,
   CC_CXXCtorInitializer,
   CC_TemplateArgument,
   CC_NestedNameSpecifier,
   CC_NestedNameSpecifierLoc,
   CC_QualType,
   CC_TypeLoc,
   CC_Decl,
   CC_Stmt,
   CC_Type,
   CC_constChar
};

struct CollectBoundNodes : MatchFinder::MatchCallback {
   CXTranslationUnit tu ;
   AstMatchCallback callback;
   void *userData;
   CollectBoundNodes(AstMatchCallback cb, CXTranslationUnit tu, void *userData) :
      tu(tu), callback(cb), userData(userData) {}
   void run(const MatchFinder::MatchResult &Result) {

      std::string ErrStr;
      llvm::raw_string_ostream os(ErrStr);
      for (BoundNodes::IDToNodeMap::const_iterator BI = Result.Nodes.getMap().begin(),
            BE = Result.Nodes.getMap().end();
            BI != BE; ++BI) {
         /* probably this sausage could be simplified by calling 
          * astnodekind.asstringref() */
         string className ;
         if(not ast_type_traits::ASTNodeKind::getMostDerivedCommonAncestor(
                  BI->second.getNodeKind(), 
                 ast_type_traits::ASTNodeKind::getFromNodeKind<Decl>() ).isNone()) {
            const Decl *d = BI->second.get<Decl>();
            className = d->getDeclKindName() ;
            className += "Decl";
         }
         else if(not ast_type_traits::ASTNodeKind::getMostDerivedCommonAncestor(
                  BI->second.getNodeKind(), 
                  ast_type_traits::ASTNodeKind::getFromNodeKind<Stmt>() ).isNone()) {
            const Stmt *s = BI->second.get<Stmt>();
            className = s->getStmtClassName() ;
         } 
         else if(not ast_type_traits::ASTNodeKind::getMostDerivedCommonAncestor(
                  BI->second.getNodeKind(), 
                  ast_type_traits::ASTNodeKind::getFromNodeKind<Type>() ).isNone()) {
            const Type *t = BI->second.get<Type>();
            className = t->getTypeClassName() ;
         } 
         unsigned cid = AstAnyMethodRunner::singleton()->classNameToId(className.c_str());
         if( cid == 0) {
            const AstAny node = {0, 0, "class not found in internal map" };
            callback(node, userData);
            return;
         }
            
         const AstAny node = {cid, BI->second.getMemoizationData(), "success" };
         
         callback(node, userData);
      }
   }

};


CINDEX_LINKAGE
int clang_matchAst(CXTranslationUnit translationUnit, NamedValueMap namedValueMap, 
      const char *expr, AstMatchCallback callback, void *userData)
{
   //cout << "inside clang_matchAst" << endl;
   /* TODO ... threaded version? though the match is fast, printing is slow*/
  llvm::StringMap<ast_matchers::dynamic::VariantValue> *namedValues =
         static_cast<llvm::StringMap<ast_matchers::dynamic::VariantValue> *> (namedValueMap);
  /* TODO ... add report if input is invalid*/
   Diagnostics diag ;

   Optional<DynTypedMatcher> Matcher = Parser::parseMatcherExpression(
         StringRef(expr), nullptr, namedValues, &diag);

    if (!Matcher) {
       /* TODO .. add diag */
       std::string ErrStr;
       llvm::raw_string_ostream OS(ErrStr);
       diag.printToStreamFull(OS);
       cout << __FUNCTION__ << ": Matcher not found: " << OS.str() << endl;
       return -1;
    }

   MatchFinder Finder;
   ASTUnit *AST = cxtu::getASTUnit(translationUnit);
   if(!AST)
      return -1;
   std::vector<BoundNodes> bindings;

   CollectBoundNodes Collect(callback, translationUnit, userData);

   DynTypedMatcher MaybeBoundMatcher = *Matcher;
   llvm::Optional<DynTypedMatcher> M = Matcher->tryBind("root");
   if (M)
      MaybeBoundMatcher = *M;
   

   if (!Finder.addDynamicMatcher(MaybeBoundMatcher, &Collect)) {
      /* TODO ... find a propper way to repport */

      return -1;
   }
   Finder.matchAST(AST->getASTContext());
   
   return 0;
}



CINDEX_LINKAGE
int clang_completeASTMatchExpression(const char *code,
      ASTMatchCompletionCallback callback) {

   llvm::StringMap<ast_matchers::dynamic::VariantValue> namedValues;
   std::vector<MatcherCompletion> comps = Parser::completeExpression(
         StringRef(code), strlen(code), nullptr, &namedValues);
   
   for (std::vector<MatcherCompletion>::iterator I = comps.begin(),
         E = comps.end();
         I != E; ++I) {
       
      callback( (code + I->TypedText).c_str(), (code + I->MatcherDecl).c_str()  );
   }

   return 0;
}


CINDEX_LINKAGE
int clang_listMatcherMethods(const char *expr, MatcherMethodsCallback cb)
{
   return AstAnyMethodRunner::singleton()->listMatcherMethods(expr, cb);

}


}// extern "C" 
