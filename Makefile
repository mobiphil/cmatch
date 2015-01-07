CC=gcc
CXX=g++

LLVM_BUILD_ROOT=/b/default/llvm
LLVM_SRC_ROOT=/g/llvm.git


CINCLUDES=\
			 -Iinclude \
			 -I$(LLVM_BUILD_ROOT)/include \
			 -I$(LLVM_BUILD_ROOT)/tools/clang/tools/c-index-test \
			 -I$(LLVM_BUILD_ROOT)/tools/clang/include \
			 -I$(LLVM_SRC_ROOT)/include \
			 -I$(LLVM_SRC_ROOT)/tools/clang/include 

CFLAGS=$(CINCLUDES)\
		 -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS \
		 -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS \
		 -g -fPIC -pedantic -Wno-long-long -Wall -W -Wno-unused-parameter \
		 -Wwrite-strings    -Wno-maybe-uninitialized -Wno-missing-field-initializers \
		 -Wno-comment -finstrument-functions


CXXINCLUDES= \
				 -Iinclude \
				 -I$(LLVM_BUILD_ROOT)/include \
				 -I$(LLVM_BUILD_ROOT)/tools/clang/libclang \
				 -I$(LLVM_BUILD_ROOT)/tools/clang/include \
				 -I$(LLVM_SRC_ROOT)/include \
				 -I$(LLVM_SRC_ROOT)/tools/clang/include  \
				 -I$(LLVM_SRC_ROOT)/tools/clang/tools/libclang 

CXXFLAGS=$(CXXINCLUDES) -c \
			-D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS \
			-D__STDC_LIMIT_MACROS  -DCLANG_ENABLE_ARCMT \
			-g -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti \
			-fPIC -ffunction-sections -fdata-sections -Wcast-qual -fno-strict-aliasing \
			-pedantic -Wno-long-long -Wall -W -Wno-unused-parameter -Wwrite-strings \
			-Wno-maybe-uninitialized -Wno-missing-field-initializers -Wno-comment -c -finstrument-functions

LIBLOC=$(LLVM_BUILD_ROOT)/Debug+Asserts/lib


src/cmatch: src/MatchFinder.o src/cmatch.o cygtrace.o murmur3.o
	g++ -Wl,--gc-sections -Wl,-R -Wl,'$(LIBLOC)' -L$(LLVM_BUILD_ROOT)/Debug+Asserts/lib -L$(LLVM_BUILD_ROOT)/Debug+Asserts/lib -Wl,--version-script=$(LLVM_SRC_ROOT)/autoconf/ExportMap.map \
	-o $@ $^ \
	-lclangDynamicASTMatchers -lclang -lclangIndex -lclangFormat -lclangRewrite \
	-lclangFrontend -lclangDriver -lclangTooling -lclangToolingCore \
	-lclangSerialization -lclangParse -lclangSema -lclangAnalysis -lclangEdit \
	-lclangAST -lclangASTMatchers -lclangLex -lclangBasic \
	-lLLVMOption -lLLVMAsmParser -lLLVMR600CodeGen -lLLVMipo -lLLVMVectorize \
	-lLLVMR600AsmParser -lLLVMR600Desc -lLLVMR600Info -lLLVMR600AsmPrinter \
	-lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser \
	-lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSystemZAsmPrinter \
	-lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonDesc \
	-lLLVMHexagonInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo \
	-lLLVMNVPTXAsmPrinter -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo \
	-lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMSP430AsmPrinter \-lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMXCoreAsmPrinter -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMMipsAsmPrinter -lLLVMAArch64Disassembler -lLLVMAArch64CodeGen -lLLVMAArch64AsmParser -lLLVMAArch64Desc -lLLVMAArch64Info -lLLVMAArch64AsmPrinter -lLLVMAArch64Utils -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMInfo -lLLVMARMAsmPrinter -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMPowerPCAsmPrinter -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMSparcAsmPrinter -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMX86Desc -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMMC -lLLVMX86Utils -lLLVMCore -lLLVMSupport \
	-lz -lpthread -ledit -ltinfo -ldl -lm \
	-lelf -liberty

cygtrace.o: /g/cygtrace.git/cygtrace.c 
	gcc -g -c $^ -o $@ -D_GNU_SOURCE

murmur3.o:	/g/cygtrace.git/murmur3.c
	gcc -g -c $^ -o $@ -D_GNU_SOURCE

clean:
	rm src/cmatch.o src/MatchFinder.o src/cmatch
