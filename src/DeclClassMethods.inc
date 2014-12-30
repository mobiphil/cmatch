


METHOD(Decl, getDeclKindName, char)
METHOD(Decl, dump, void)
METHOD(Decl, dumpColor, void)

METHOD(NamedDecl, getNameAsString, string)

METHOD(RecordDecl, getPreviousDecl, Decl)
METHOD(RecordDecl, getMostRecentDecl, Decl)
METHOD(RecordDecl, hasVolatileMember, bool)

METHOD(FunctionDecl, getBody, Stmt)
METHOD(FunctionDecl, getDeclName, DeclarationName)
METHOD(FunctionDecl, getReturnType, QualType)


METHOD(CXXMethodDecl, getParent, Decl)
