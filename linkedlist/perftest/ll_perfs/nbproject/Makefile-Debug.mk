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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/29dd86f/linkedlist.o \
	${OBJECTDIR}/ll_perftests.o


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
LDLIBSOPTIONS=`pkg-config --libs linkedlist` -lpthread  `pkg-config --libs uepwide`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ll_perfs

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ll_perfs: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ll_perfs ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/29dd86f/linkedlist.o: ../../linkedlist.c
	${MKDIR} -p ${OBJECTDIR}/_ext/29dd86f
	${RM} "$@.d"
	$(COMPILE.c) -g `pkg-config --cflags linkedlist` `pkg-config --cflags uepwide`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/29dd86f/linkedlist.o ../../linkedlist.c

${OBJECTDIR}/ll_perftests.o: ll_perftests.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g `pkg-config --cflags linkedlist` `pkg-config --cflags uepwide`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ll_perftests.o ll_perftests.c

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
