METHOD(BlockDecl, getCompoundBody, Stmt)
METHOD(CapturedDecl, getContextParam, Decl)
METHOD(ClassScopeFunctionSpecializationDecl, getSpecialization, Decl)
METHOD(ClassTemplateDecl, getCanonicalDecl, Decl)
//METHOD(ClassTemplateDecl, getInstantiatedFromMemberTemplate, Decl)
METHOD(ClassTemplateDecl, getMostRecentDecl, Decl)
METHOD(ClassTemplateDecl, getPreviousDecl, Decl)
METHOD(ClassTemplateDecl, getTemplatedDecl, Decl)
//METHOD(ClassTemplatePartialSpecializationDecl, getInstantiatedFromMember, Decl)
//METHOD(ClassTemplatePartialSpecializationDecl, getMostRecentDecl, Decl)
//METHOD(ClassTemplateSpecializationDecl, getMostRecentDecl, Decl)
METHOD(ClassTemplateSpecializationDecl, getSpecializedTemplate, Decl)
METHOD(CXXConstructorDecl, getCanonicalDecl, Decl)
METHOD(CXXConstructorDecl, getInheritedConstructor, Decl)
METHOD(CXXConstructorDecl, getTargetConstructor, Decl)
METHOD(CXXDestructorDecl, getOperatorDelete, Decl)
METHOD(CXXMethodDecl, getCanonicalDecl, Decl)
METHOD(CXXMethodDecl, getMostRecentDecl, Decl)
METHOD(CXXMethodDecl, getParent, Decl)
METHOD(CXXRecordDecl, getCanonicalDecl, Decl)
METHOD(CXXRecordDecl, getDefinition, Decl)
METHOD(CXXRecordDecl, getDescribedClassTemplate, Decl)
METHOD(CXXRecordDecl, getDestructor, Decl)
METHOD(CXXRecordDecl, getInstantiatedFromMemberClass, Decl)
METHOD(CXXRecordDecl, getLambdaCallOperator, Decl)
METHOD(CXXRecordDecl, getLambdaStaticInvoker, Decl)
METHOD(CXXRecordDecl, getMostRecentDecl, Decl)
METHOD(CXXRecordDecl, getPreviousDecl, Decl)
METHOD(CXXRecordDecl, getTemplateInstantiationPattern, Decl)
METHOD(CXXRecordDecl, isLocalClass, Decl)
METHOD(Decl, dumpColor, void)
METHOD(Decl, dump, void)
METHOD(Decl, getDeclKindName, char)
METHOD(EnumConstantDecl, getCanonicalDecl, Decl)
METHOD(EnumConstantDecl, getInitExpr, Stmt)
METHOD(EnumDecl, getCanonicalDecl, Decl)
METHOD(EnumDecl, getDefinition, Decl)
METHOD(EnumDecl, getInstantiatedFromMemberEnum, Decl)
METHOD(EnumDecl, getMostRecentDecl, Decl)
METHOD(EnumDecl, getPreviousDecl, Decl)
METHOD(FieldDecl, getBitWidth, Stmt)
METHOD(FieldDecl, getCanonicalDecl, Decl)
//METHOD(FieldDecl, getCapturedVLAType, VariableArrayType)
METHOD(FieldDecl, getInClassInitializer, Stmt)
METHOD(FieldDecl, getParent, Decl)
//METHOD(FileScopeAsmDecl, getAsmString, StringLiteral)
METHOD(FriendDecl, getFriendDecl, Decl)
METHOD(FriendTemplateDecl, getFriendDecl, Decl)
METHOD(FunctionDecl, getBody, Stmt)
METHOD(FunctionDecl, getCanonicalDecl, Decl)
METHOD(FunctionDecl, getClassScopeSpecializationPattern, Decl)
METHOD(FunctionDecl, getCorrespondingUnsizedGlobalDeallocationFunction, Decl)
METHOD(FunctionDecl, getDeclName, DeclarationName)
METHOD(FunctionDecl, getDescribedFunctionTemplate, Decl)
METHOD(FunctionDecl, getInstantiatedFromMemberFunction, Decl)
METHOD(FunctionDecl, getPrimaryTemplate, Decl)
METHOD(FunctionDecl, getReturnType, QualType)
METHOD(FunctionDecl, getTemplateInstantiationPattern, Decl)
METHOD(FunctionTemplateDecl, getCanonicalDecl, Decl)
//METHOD(FunctionTemplateDecl, getInstantiatedFromMemberTemplate, Decl)
METHOD(FunctionTemplateDecl, getPreviousDecl, Decl)
METHOD(FunctionTemplateDecl, getTemplatedDecl, Decl)
METHOD(IndirectFieldDecl, getAnonField, Decl)
METHOD(IndirectFieldDecl, getVarDecl, Decl)
METHOD(LabelDecl, getStmt, Stmt)
METHOD(NamedDecl, getMostRecentDecl, Decl)
METHOD(NamedDecl, getNameAsString, string)
METHOD(NamedDecl, getUnderlyingDecl, Decl)
METHOD(NamespaceAliasDecl, getAliasedNamespace, Decl)
METHOD(NamespaceAliasDecl, getCanonicalDecl, Decl)
METHOD(NamespaceAliasDecl, getNamespace, Decl)
METHOD(NamespaceDecl, getAnonymousNamespace, Decl)
METHOD(NamespaceDecl, getCanonicalDecl, Decl)
METHOD(NamespaceDecl, getOriginalNamespace, Decl)
METHOD(NonTypeTemplateParmDecl, getDefaultArgument, Stmt)
//METHOD(ObjCCategoryDecl, getClassInterface, Decl)
//METHOD(ObjCCategoryDecl, getImplementation, Decl)
//METHOD(ObjCCategoryDecl, getNextClassCategory, Decl)
//METHOD(ObjCCategoryDecl, getNextClassCategoryRaw, Decl)
//METHOD(ObjCCategoryImplDecl, getCategoryDecl, Decl)
//METHOD(ObjCCompatibleAliasDecl, getClassInterface, Decl)
//METHOD(ObjCImplDecl, getClassInterface, Decl)
//METHOD(ObjCImplementationDecl, getSuperClass, Decl)
//METHOD(ObjCInterfaceDecl, all_declared_ivar_begin, Decl)
//METHOD(ObjCInterfaceDecl, getCanonicalDecl, Decl)
//METHOD(ObjCInterfaceDecl, getCategoryListRaw, Decl)
//METHOD(ObjCInterfaceDecl, getDefinition, Decl)
//METHOD(ObjCInterfaceDecl, getImplementation, Decl)
//METHOD(ObjCInterfaceDecl, getSuperClass, Decl)
//METHOD(ObjCInterfaceDecl, isObjCRequiresPropertyDefs, Decl)
//METHOD(ObjCIvarDecl, getContainingInterface, Decl)
//METHOD(ObjCIvarDecl, getNextIvar, Decl)
//METHOD(ObjCMethodDecl, getCanonicalDecl, Decl)
//METHOD(ObjCMethodDecl, getClassInterface, Decl)
//METHOD(ObjCMethodDecl, getCmdDecl, Decl)
//METHOD(ObjCMethodDecl, getCompoundBody, Stmt)
//METHOD(ObjCMethodDecl, getSelfDecl, Decl)
//METHOD(ObjCPropertyDecl, getGetterMethodDecl, Decl)
//METHOD(ObjCPropertyDecl, getPropertyIvarDecl, Decl)
//METHOD(ObjCPropertyDecl, getSetterMethodDecl, Decl)
//METHOD(ObjCPropertyImplDecl, getGetterCXXConstructor, Stmt)
//METHOD(ObjCPropertyImplDecl, getPropertyDecl, Decl)
//METHOD(ObjCPropertyImplDecl, getPropertyIvarDecl, Decl)
//METHOD(ObjCPropertyImplDecl, getSetterCXXAssignment, Stmt)
//METHOD(ObjCProtocolDecl, getCanonicalDecl, Decl)
//METHOD(ObjCProtocolDecl, getDefinition, Decl)
//METHOD(ParmVarDecl, getDefaultArg, Stmt)
//METHOD(ParmVarDecl, getUninstantiatedDefaultArg, Stmt)
METHOD(RecordDecl, findFirstNamedDataMember, Decl)
METHOD(RecordDecl, getDefinition, Decl)
METHOD(RecordDecl, getMostRecentDecl, Decl)
METHOD(RecordDecl, getPreviousDecl, Decl)
METHOD(RecordDecl, hasVolatileMember, bool)
METHOD(RedeclarableTemplateDecl, getCanonicalDecl, Decl)
METHOD(RedeclarableTemplateDecl, getInstantiatedFromMemberTemplate, Decl)
METHOD(StaticAssertDecl, getAssertExpr, Stmt)
//METHOD(StaticAssertDecl, getMessage, StringLiteral)
METHOD(TagDecl, getCanonicalDecl, Decl)
METHOD(TagDecl, getDeclaratorForAnonDecl, Decl)
METHOD(TagDecl, getDefinition, Decl)
METHOD(TagDecl, getTypedefNameForAnonDecl, Decl)
METHOD(TemplateDecl, getTemplatedDecl, Decl)
METHOD(TranslationUnitDecl, getAnonymousNamespace, Decl)
METHOD(TypeAliasDecl, getDescribedAliasTemplate, Decl)
METHOD(TypeAliasTemplateDecl, getCanonicalDecl, Decl)
//METHOD(TypeAliasTemplateDecl, getInstantiatedFromMemberTemplate, Decl)
METHOD(TypeAliasTemplateDecl, getPreviousDecl, Decl)
METHOD(TypeAliasTemplateDecl, getTemplatedDecl, Decl)
METHOD(TypedefNameDecl, getCanonicalDecl, Decl)
METHOD(UnresolvedUsingTypenameDecl, getCanonicalDecl, Decl)
METHOD(UnresolvedUsingValueDecl, getCanonicalDecl, Decl)
METHOD(UsingDecl, getCanonicalDecl, Decl)
METHOD(UsingDirectiveDecl, getNominatedNamespaceAsWritten, Decl)
METHOD(UsingDirectiveDecl, getNominatedNamespace, Decl)
METHOD(UsingShadowDecl, getCanonicalDecl, Decl)
METHOD(UsingShadowDecl, getNextUsingShadowDecl, Decl)
METHOD(UsingShadowDecl, getTargetDecl, Decl)
METHOD(UsingShadowDecl, getUsingDecl, Decl)
METHOD(VarDecl, getActingDefinition, Decl)
METHOD(VarDecl, getAnyInitializer, Stmt)
METHOD(VarDecl, getCanonicalDecl, Decl)
METHOD(VarDecl, getDefinition, Decl)
METHOD(VarDecl, getDescribedVarTemplate, Decl)
METHOD(VarDecl, getInit, Stmt)
METHOD(VarDecl, getInstantiatedFromStaticDataMember, Decl)
//METHOD(VarDecl, getOutOfLineDefinition, Decl)
METHOD(VarTemplateDecl, getCanonicalDecl, Decl)
//METHOD(VarTemplateDecl, getDefinition, Decl)
//METHOD(VarTemplateDecl, getInstantiatedFromMemberTemplate, Decl)
METHOD(VarTemplateDecl, getPreviousDecl, Decl)
METHOD(VarTemplateDecl, getTemplatedDecl, Decl)
//METHOD(VarTemplatePartialSpecializationDecl, getInstantiatedFromMember, Decl)
//METHOD(VarTemplatePartialSpecializationDecl, getMostRecentDecl, Decl)
//METHOD(VarTemplateSpecializationDecl, getMostRecentDecl, Decl)
//METHOD(VarTemplateSpecializationDecl, getSpecializedTemplate, Decl)
