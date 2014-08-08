##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=icsneoAPISharedLibrary
ConfigurationName      :=Release
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite"
ProjectPath            := "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Mike
Date                   :=08/05/2014
CodeLitePath           :="/home/mike/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/libicsneoAPI.so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             :=  $(Preprocessors)
LinkOptions            :=  -O2
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." "$(IncludeSwitch)/usr/include" "$(IncludeSwitch)../../NeoVIServerApplication/Core" "$(IncludeSwitch)../../coremini" "$(IncludeSwitch)../../core/iefs" "$(IncludeSwitch)../../core" "$(IncludeSwitch)../../OsAbstraction" "$(IncludeSwitch)../../ISO15765_2_CM_Scripts" 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)ftdi $(LibrarySwitch)pthread $(LibrarySwitch)rt 
LibPath                := "$(LibraryPathSwitch)." "$(LibraryPathSwitch)/usr/lib" 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/icsneoLinuxAPI$(ObjectSuffix) $(IntermediateDirectory)/cicsneoVI$(ObjectSuffix) $(IntermediateDirectory)/OsAbstraction_OCriticalSection$(ObjectSuffix) $(IntermediateDirectory)/OsAbstraction_OEvent$(ObjectSuffix) $(IntermediateDirectory)/OsAbstraction_OSAbstraction$(ObjectSuffix) $(IntermediateDirectory)/OsAbstraction_OThread$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/icsneoLinuxAPI$(ObjectSuffix): icsneoLinuxAPI.cpp $(IntermediateDirectory)/icsneoLinuxAPI$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/icsneoLinuxAPI.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/icsneoLinuxAPI$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/icsneoLinuxAPI$(DependSuffix): icsneoLinuxAPI.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/icsneoLinuxAPI$(ObjectSuffix) -MF$(IntermediateDirectory)/icsneoLinuxAPI$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/icsneoLinuxAPI.cpp"

$(IntermediateDirectory)/icsneoLinuxAPI$(PreprocessSuffix): icsneoLinuxAPI.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/icsneoLinuxAPI$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/icsneoLinuxAPI.cpp"

$(IntermediateDirectory)/cicsneoVI$(ObjectSuffix): cicsneoVI.cpp $(IntermediateDirectory)/cicsneoVI$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/cicsneoVI.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/cicsneoVI$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cicsneoVI$(DependSuffix): cicsneoVI.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/cicsneoVI$(ObjectSuffix) -MF$(IntermediateDirectory)/cicsneoVI$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/cicsneoVI.cpp"

$(IntermediateDirectory)/cicsneoVI$(PreprocessSuffix): cicsneoVI.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cicsneoVI$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/cicsneoVI.cpp"

$(IntermediateDirectory)/OsAbstraction_OCriticalSection$(ObjectSuffix): OsAbstraction/OCriticalSection.cpp $(IntermediateDirectory)/OsAbstraction_OCriticalSection$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OCriticalSection.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/OsAbstraction_OCriticalSection$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OsAbstraction_OCriticalSection$(DependSuffix): OsAbstraction/OCriticalSection.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/OsAbstraction_OCriticalSection$(ObjectSuffix) -MF$(IntermediateDirectory)/OsAbstraction_OCriticalSection$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OCriticalSection.cpp"

$(IntermediateDirectory)/OsAbstraction_OCriticalSection$(PreprocessSuffix): OsAbstraction/OCriticalSection.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OsAbstraction_OCriticalSection$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OCriticalSection.cpp"

$(IntermediateDirectory)/OsAbstraction_OEvent$(ObjectSuffix): OsAbstraction/OEvent.cpp $(IntermediateDirectory)/OsAbstraction_OEvent$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OEvent.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/OsAbstraction_OEvent$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OsAbstraction_OEvent$(DependSuffix): OsAbstraction/OEvent.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/OsAbstraction_OEvent$(ObjectSuffix) -MF$(IntermediateDirectory)/OsAbstraction_OEvent$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OEvent.cpp"

$(IntermediateDirectory)/OsAbstraction_OEvent$(PreprocessSuffix): OsAbstraction/OEvent.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OsAbstraction_OEvent$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OEvent.cpp"

$(IntermediateDirectory)/OsAbstraction_OSAbstraction$(ObjectSuffix): OsAbstraction/OSAbstraction.cpp $(IntermediateDirectory)/OsAbstraction_OSAbstraction$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OSAbstraction.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/OsAbstraction_OSAbstraction$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OsAbstraction_OSAbstraction$(DependSuffix): OsAbstraction/OSAbstraction.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/OsAbstraction_OSAbstraction$(ObjectSuffix) -MF$(IntermediateDirectory)/OsAbstraction_OSAbstraction$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OSAbstraction.cpp"

$(IntermediateDirectory)/OsAbstraction_OSAbstraction$(PreprocessSuffix): OsAbstraction/OSAbstraction.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OsAbstraction_OSAbstraction$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OSAbstraction.cpp"

$(IntermediateDirectory)/OsAbstraction_OThread$(ObjectSuffix): OsAbstraction/OThread.cpp $(IntermediateDirectory)/OsAbstraction_OThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/OsAbstraction_OThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OsAbstraction_OThread$(DependSuffix): OsAbstraction/OThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/OsAbstraction_OThread$(ObjectSuffix) -MF$(IntermediateDirectory)/OsAbstraction_OThread$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OThread.cpp"

$(IntermediateDirectory)/OsAbstraction_OThread$(PreprocessSuffix): OsAbstraction/OThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OsAbstraction_OThread$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/OsAbstraction/OThread.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/icsneoLinuxAPI$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/icsneoLinuxAPI$(DependSuffix)
	$(RM) $(IntermediateDirectory)/icsneoLinuxAPI$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/cicsneoVI$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/cicsneoVI$(DependSuffix)
	$(RM) $(IntermediateDirectory)/cicsneoVI$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OCriticalSection$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OCriticalSection$(DependSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OCriticalSection$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OEvent$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OEvent$(DependSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OEvent$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OSAbstraction$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OSAbstraction$(DependSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OSAbstraction$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/OsAbstraction_OThread$(PreprocessSuffix)
	$(RM) $(OutputFile)


