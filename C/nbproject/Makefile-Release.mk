#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g
CXX=g
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/5c0/ArbreAbstrait.o \
	${OBJECTDIR}/_ext/5c0/Interpreteur.o \
	${OBJECTDIR}/_ext/5c0/Lecteur.o \
	${OBJECTDIR}/_ext/5c0/Symbole.o \
	${OBJECTDIR}/_ext/5c0/SymboleValue.o \
	${OBJECTDIR}/_ext/5c0/TableSymboles.o \
	${OBJECTDIR}/_ext/5c0/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/5c0/ArbreAbstrait.o: ../ArbreAbstrait.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/ArbreAbstrait.o ../ArbreAbstrait.cpp

${OBJECTDIR}/_ext/5c0/Interpreteur.o: ../Interpreteur.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/Interpreteur.o ../Interpreteur.cpp

${OBJECTDIR}/_ext/5c0/Lecteur.o: ../Lecteur.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/Lecteur.o ../Lecteur.cpp

${OBJECTDIR}/_ext/5c0/Symbole.o: ../Symbole.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/Symbole.o ../Symbole.cpp

${OBJECTDIR}/_ext/5c0/SymboleValue.o: ../SymboleValue.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/SymboleValue.o ../SymboleValue.cpp

${OBJECTDIR}/_ext/5c0/TableSymboles.o: ../TableSymboles.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/TableSymboles.o ../TableSymboles.cpp

${OBJECTDIR}/_ext/5c0/main.o: ../main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5c0
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5c0/main.o ../main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
