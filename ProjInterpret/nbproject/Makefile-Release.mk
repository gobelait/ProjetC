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
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/cf974b4a/ArbreAbstrait.o \
	${OBJECTDIR}/_ext/cf974b4a/Interpreteur.o \
	${OBJECTDIR}/_ext/cf974b4a/Lecteur.o \
	${OBJECTDIR}/_ext/cf974b4a/Symbole.o \
	${OBJECTDIR}/_ext/cf974b4a/SymboleValue.o \
	${OBJECTDIR}/_ext/cf974b4a/TableSymboles.o \
	${OBJECTDIR}/_ext/cf974b4a/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projinterpret

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projinterpret: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projinterpret ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/cf974b4a/ArbreAbstrait.o: ../source/ArbreAbstrait.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/ArbreAbstrait.o ../source/ArbreAbstrait.cpp

${OBJECTDIR}/_ext/cf974b4a/Interpreteur.o: ../source/Interpreteur.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/Interpreteur.o ../source/Interpreteur.cpp

${OBJECTDIR}/_ext/cf974b4a/Lecteur.o: ../source/Lecteur.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/Lecteur.o ../source/Lecteur.cpp

${OBJECTDIR}/_ext/cf974b4a/Symbole.o: ../source/Symbole.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/Symbole.o ../source/Symbole.cpp

${OBJECTDIR}/_ext/cf974b4a/SymboleValue.o: ../source/SymboleValue.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/SymboleValue.o ../source/SymboleValue.cpp

${OBJECTDIR}/_ext/cf974b4a/TableSymboles.o: ../source/TableSymboles.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/TableSymboles.o ../source/TableSymboles.cpp

${OBJECTDIR}/_ext/cf974b4a/main.o: ../source/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cf974b4a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cf974b4a/main.o ../source/main.cpp

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
