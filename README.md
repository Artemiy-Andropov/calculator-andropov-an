# calculator-andropov-an

## What has been done?

This repository contains an application written in **C** that is capable of processing *arithmetic expressions* subject to the following restrictions:  
1) The length of the input data does not exceed 1024 characters
2) The operations that the program processes are indicated by a signs `+`, `-`, `*`, `/`, `(`and `)`.
3) The largest number entered must not exceed $2 * 10^9$
4) The unary `-` and `+` operations should not be present in the arithmetic expression

## How to run/use it?

The following commands can be used to run this program:
|Command|For what?|
|:-:|:-:|
|make all|to build app.exe and unit tests (unit-tests.exe)|
|make clean|to clean build artifacts|
|make run-int|to run app.exe|
|make run-float|to run app.exe --float|
|make run-unit-test|to run unit-tests.exe|
|make run-integration-tests|to build and run integration tests|  

Make run-int and make run-float require additional user input of arithmetic expressions

## How it's made?

* As mentioned earlier, the program is written in C using standard libraries
* For convenient compilation and launch of the program, a Makefile was written
* For conducting unit and integration tests, programs were written in C++ and Python, respectively
