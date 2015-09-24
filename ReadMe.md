
This project analyzes the similarity of code within a set of C++ source files. 
Similarity is based on the scope analysis you implemented in Project #1. 
Suppose that several lines or more of code have been copied from one file and pasted into another. 
These cut and paste operations are sometimes used when developing code that contains functionality similar to that in another file.
This duplication will help a developer get code working quickly, but causes maintenance difficulties because there are now two or more pieces of code to understand and fix if there are latent errors in the copied code. Once we detect very similar code in multiple packages we can create a function to be called by all of the other code, at the site of their duplications, allowing us to improve maintainability of all the affected packages.

For more details refer
Source: Object oriented analysis and design, Spring 2014 Project, Dr Fawcett, Syracuse University
