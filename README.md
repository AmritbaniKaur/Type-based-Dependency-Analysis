============================================================================================================================================

DependencyAnalyzer Solution:
----------------------------

1 : used Visual Studio 2015 and its C++ Windows Console Projects.

2 : used the C++ standard library's streams for all I/O and new and delete for all heap-based memory management.

3 : provides C++ packages. Enforced the "Single Responsiblity Principle" as illustrated in the package structure.

4 : provides a TypeAnalysis package that identifies all of the types defined in a specified file. It is expected that this project will
	be built on the analysis machinery provided in the CodeAnalyzer.
	
5 : provides a DependencyAnalysis package that identifies all of the dependencies between files in a specified file collection.

6 : finds the strong components in the dependency graph defined by the relationships evaluated in the previous requirement.

7 : writes the analysis results, in XML format, to a specified file.

8 : processes the command line, accepting:
	. Path to the directory tree containing files to analyze.
	. List of file patterns to match for selection of files to analyze.
	. Specification of the XML results file, supplying a default if no specification is provided.
	
9 : includes an automated unit test suite that demonstrates all the requirements of this project.

============================================================================================================================================