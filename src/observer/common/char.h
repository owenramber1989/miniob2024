#pragma once
#include <string>
std::string double2string(double v);
int* chars2ints(char* val_chars);
char* ints2chars(int* val_ints);
float* chars2floats(const char* val_chars);
char* floats2chars(float* val_floats);
float* ints2floats(int* val_ints);
int* floats2ints(float* val_floats);
bool isLetter(char c);
bool isLowercaseLetter(char c);
bool isUppercaseLetter(char c);
char toLowercaseLetter(char c);
char toUppercaseLetter(char c);
