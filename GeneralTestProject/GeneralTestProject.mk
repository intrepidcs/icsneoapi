##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=GeneralTestProject
ConfigurationName      :=Release
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite"
ProjectPath            := "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject"
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             :=  $(Preprocessors)
LinkOptions            :=  -O2
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)icsneoAPI 
LibPath                := "$(LibraryPathSwitch)." "$(LibraryPathSwitch)/usr/lib" 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/OCriticalSection$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject/main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject/main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject/main.cpp"

$(IntermediateDirectory)/OCriticalSection$(ObjectSuffix): OCriticalSection.cpp $(IntermediateDirectory)/OCriticalSection$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject/OCriticalSection.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/OCriticalSection$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OCriticalSection$(DependSuffix): OCriticalSection.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/OCriticalSection$(ObjectSuffix) -MF$(IntermediateDirectory)/OCriticalSection$(DependSuffix) -MM "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject/OCriticalSection.cpp"

$(IntermediateDirectory)/OCriticalSection$(PreprocessSuffix): OCriticalSection.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OCriticalSection$(PreprocessSuffix) "/home/mike/workspace/vspy3_600_sangeetha/icsneoLinuxAPICodeLite/icsneoAPISharedLibrary/GeneralTestProject/OCriticalSection.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/OCriticalSection$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/OCriticalSection$(DependSuffix)
	$(RM) $(IntermediateDirectory)/OCriticalSection$(PreprocessSuffix)
	$(RM) $(OutputFile)


