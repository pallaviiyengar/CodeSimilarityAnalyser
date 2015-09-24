@echo off

echo ***** Demonstrating Requirement 3, 4, 5, 7 without /S option *****
echo ***** Demonstrating similarity detected with different types of scopes *****
echo ***** Command Line Input: TEST\Test_3 *.cpp *****
Debug\Executive.exe TEST\Test_3 *.cpp

echo ***** Demonstrating Requirement 3, 4, 5, 7 *****
echo ***** Demonstrating similarity detected with nested scopes *****
echo ***** Command Line Input: TEST\Test_2 *.cpp *.h *****
Debug\Executive.exe TEST\Test_2 *.cpp *.h

echo ***** Demonstrating Requirement 3, 4, 5, 7 *****
echo ***** Demonstrating similarity detected with scopes(miscellaneous) *****
echo ***** Command Line Input: TEST\Test_4 *.cpp *.h *****
Debug\Executive.exe TEST\Test_4 *.cpp *.h

echo ***** Demonstrating Requirement 3, 4, 6(/d for showing differences), 7 *****
echo ***** Demonstrating differences between similar scopes. Note use "/d" to see the differences *****
echo ***** Command Line Input: TEST\Test_3 *.* /d *****
Debug\Executive.exe TEST\Test_3 *.* /d

echo ***** Demonstrating Requirement 3, 4, 5, 7 with /S option *****
echo ***** Demonstrating class similarity *****
echo ***** Command Line Input: TEST\Test_1 *.cpp *.h /S *****
Debug\Executive.exe TEST\Test_1 *.cpp *.h /S
 
REM echo ***** F. Demonstrating Requirement 6 and 5 *****
REM echo **** Command line option /b passed to display function size and complexity ****
REM echo ***** Command Line Input: TEST\TestFolder5 Parser.cpp /b
REM Debug\Executive.exe TEST\TestFolder5 Parser.cpp /b

REM echo ***** C. Demonstrating Requirement 3, with more than one pattern like *.cpp, *.h ******
REM Debug\Executive.exe TEST\stFolder3 *.cpp *.txt /s
REM echo *************** E. Demonstrating Requirement 3, with no pattern(Default value for pattern *.* assumed) ***************
REM Debug\Executive.exe TEST\TestFolder3
REM echo *************** E.Demonstrating Requirement 5***************
REM echo *************** Displaying XML representation of the tree with largest scope at end of each file***************
REM echo ***** Command Line Input: TEST\TestFolder6 *.cpp
REM Debug\Executive.exe TEST\TestFolder6 *.cpp
REM echo ***** H. General Demonstration 1  *****
REM Debug\Executive.exe TEST test_odd_cases.cpp
REM echo ***** I. General Demonstration 2  *****
REM Debug\Executive.exe . *.*

