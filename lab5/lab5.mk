##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=lab5
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=/home/william/Documents/eda482
ProjectPath            :=/home/william/Documents/eda482/lab5
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/lab5
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=William
Date                   :=03/09/2023
CodeLitePath           :=/home/william/.codelite
MakeDirCommand         :=mkdir -p
LinkerName             :=/usr/share/codelite/tools/gcc-arm/bin/arm-none-eabi-g++
SharedObjectLinkerName :=/usr/share/codelite/tools/gcc-arm/bin/arm-none-eabi-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=/home/william/Documents/eda482/build-$(WorkspaceConfiguration)/bin
OutputFile             :=../build-$(WorkspaceConfiguration)/bin/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)SIMULATOR 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  -T$(ProjectPath)/md407-ram.x -L$(ARM_V6LIB) -L$(ARM_GCC_V6LIB) -nostdlib -nostartfiles
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)gcc 
ArLibs                 :=  "gcc" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := /usr/share/codelite/tools/gcc-arm/bin/arm-none-eabi-ar rcu
CXX      := /usr/share/codelite/tools/gcc-arm/bin/arm-none-eabi-g++
CC       := /usr/share/codelite/tools/gcc-arm/bin/arm-none-eabi-gcc
CXXFLAGS :=  -g -O0 -W $(Preprocessors)
CFLAGS   :=  -g -O0 -w -mthumb -march=armv6-m  -mfloat-abi=soft -std=c99 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/share/codelite/tools/gcc-arm/bin/arm-none-eabi-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
ARM_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v6-m/nofp
ARM_GCC_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v6-m/nofp
ARM_M4SFPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/softfp
ARM_GCC_M4SFPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v7e-m+fp/softfp
ARM_M4HFPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/hard
ARM_GCC_M4HFPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v7e-m+fp/hard
Objects0=$(IntermediateDirectory)/delay.c$(ObjectSuffix) $(IntermediateDirectory)/asciidisp.c$(ObjectSuffix) $(IntermediateDirectory)/geometry.c$(ObjectSuffix) $(IntermediateDirectory)/display.c$(ObjectSuffix) $(IntermediateDirectory)/keypad.c$(ObjectSuffix) $(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IntermediateDirectory)/graphics.c$(ObjectSuffix) $(IntermediateDirectory)/math.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	$(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-objcopy -S -O srec  $(IntermediateDirectory)/$(ProjectName) $(IntermediateDirectory)/$(ProjectName).s19
	$(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-objdump -D -S $(IntermediateDirectory)/$(ProjectName) > $(IntermediateDirectory)/$(ProjectName).lst
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@$(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/delay.c$(ObjectSuffix): delay.c $(IntermediateDirectory)/delay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/delay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/delay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/delay.c$(DependSuffix): delay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/delay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/delay.c$(DependSuffix) -MM delay.c

$(IntermediateDirectory)/delay.c$(PreprocessSuffix): delay.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/delay.c$(PreprocessSuffix) delay.c

$(IntermediateDirectory)/asciidisp.c$(ObjectSuffix): asciidisp.c $(IntermediateDirectory)/asciidisp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/asciidisp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/asciidisp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/asciidisp.c$(DependSuffix): asciidisp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/asciidisp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/asciidisp.c$(DependSuffix) -MM asciidisp.c

$(IntermediateDirectory)/asciidisp.c$(PreprocessSuffix): asciidisp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/asciidisp.c$(PreprocessSuffix) asciidisp.c

$(IntermediateDirectory)/geometry.c$(ObjectSuffix): geometry.c $(IntermediateDirectory)/geometry.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/geometry.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/geometry.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/geometry.c$(DependSuffix): geometry.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/geometry.c$(ObjectSuffix) -MF$(IntermediateDirectory)/geometry.c$(DependSuffix) -MM geometry.c

$(IntermediateDirectory)/geometry.c$(PreprocessSuffix): geometry.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/geometry.c$(PreprocessSuffix) geometry.c

$(IntermediateDirectory)/display.c$(ObjectSuffix): display.c $(IntermediateDirectory)/display.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/display.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/display.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/display.c$(DependSuffix): display.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/display.c$(ObjectSuffix) -MF$(IntermediateDirectory)/display.c$(DependSuffix) -MM display.c

$(IntermediateDirectory)/display.c$(PreprocessSuffix): display.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/display.c$(PreprocessSuffix) display.c

$(IntermediateDirectory)/keypad.c$(ObjectSuffix): keypad.c $(IntermediateDirectory)/keypad.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/keypad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/keypad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/keypad.c$(DependSuffix): keypad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/keypad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/keypad.c$(DependSuffix) -MM keypad.c

$(IntermediateDirectory)/keypad.c$(PreprocessSuffix): keypad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/keypad.c$(PreprocessSuffix) keypad.c

$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c $(IntermediateDirectory)/startup.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(DependSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c

$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c

$(IntermediateDirectory)/utils.c$(ObjectSuffix): utils.c $(IntermediateDirectory)/utils.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/utils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utils.c$(DependSuffix): utils.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utils.c$(ObjectSuffix) -MF$(IntermediateDirectory)/utils.c$(DependSuffix) -MM utils.c

$(IntermediateDirectory)/utils.c$(PreprocessSuffix): utils.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utils.c$(PreprocessSuffix) utils.c

$(IntermediateDirectory)/graphics.c$(ObjectSuffix): graphics.c $(IntermediateDirectory)/graphics.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/graphics.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/graphics.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/graphics.c$(DependSuffix): graphics.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/graphics.c$(ObjectSuffix) -MF$(IntermediateDirectory)/graphics.c$(DependSuffix) -MM graphics.c

$(IntermediateDirectory)/graphics.c$(PreprocessSuffix): graphics.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/graphics.c$(PreprocessSuffix) graphics.c

$(IntermediateDirectory)/math.c$(ObjectSuffix): math.c $(IntermediateDirectory)/math.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/william/Documents/eda482/lab5/math.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math.c$(DependSuffix): math.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math.c$(ObjectSuffix) -MF$(IntermediateDirectory)/math.c$(DependSuffix) -MM math.c

$(IntermediateDirectory)/math.c$(PreprocessSuffix): math.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math.c$(PreprocessSuffix) math.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


