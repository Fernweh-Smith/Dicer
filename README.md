# Dicer

A simple command line application for throwing virtual dice using common dice notation. Dice notation is specified as `[count]d<n-sides>[(+|-)modifier]`. Count and modifier are both positive integers. If a modifier is included it must be preceeded by either a `+` or `-`, depeding on the desired effect. If no count is present, then it assumed to be 1. Some examples of valid dice notaion are;

- d20 : 1 d20
- d6+6 : 1 d6 with a positive modifier of 6
- 2d10 : 2 d10's
- 4d4-2 :  4 d4's with a negative modifier of 2

The application enforces limits on the values than can be provided. As these may change, I will not list them here but they can be found using the help flag ('-h' or '--help').

## Purpose

The purpose of this project was to learn about parsing command line arguments. I did not use any libraries to do this, instead building a (limited) solution from the ground up. As this is a first attempt, I have limited the project to only accept the dice notation in the first argument position (from the user perspective) and do not accept arguments for any of the optional flags.

## Approach

The program first converts each argument to one or more tokens, and stores each token in an array. Then the token array is analysed to check if the tokens form a valid command (ie: void of any bad or unidentified tokens, correct order etc). If the tokens form a valid command, a function corresponding with the command described by the tokens is generated and returned, and then executed in the main function. If any problems are present in the token array, or if generating of the command function fails, then an error function is generated and returned. The return value of the main function is decided by the output of the command function.

## Issues

This project begins the tokenisation process at index 1 of `argv`. For the majority of cases this should be fine. However I am aware that there are some scenarios where the system may populate more than the first element of `argv` wiht non-user arguments. In such a case, this program will fail and return an error, even if the user enters arguments correctly. This is a problem I intend to fix at a later time.
