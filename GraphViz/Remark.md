# GraphViz

## 1 Introduction

In this assignment, professors provide a template folder for students to visualize graph. However, using the original scaffolding 
in Qt Creator, there will always be some issues with fstream and file name typing. In addition, personally I prefer CLion than Qt 
Creator. Thus, I made changes to the original files to make it suitable for CLion, and provide it in the GraphViz_diysample folder.

## 2 Changes to notice

In my version of scaffolding, I use CMake instead of QMake. Plus, in the original folder there are two main functions, which are 
messy and difficult for CLion to use the main function in main.cpp. So I made small changes to the structure of them, pulling the 
main function of SimpleGraph.cpp into the main function of main.cpp.

## 3 Want to know more?

Please check for the whole repository!
