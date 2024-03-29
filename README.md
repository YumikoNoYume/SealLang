# SealLang

SealLang is a programming language created in educational purposes and for fun. If for any reason you decide to use it here is some info about the language.

## How to write a program

SealLang allows you to perform logic operations and simple arithmetic operations such as addition, subtraction, multiplication and division.
Variable type is integer. 
Program file format is .txt.

> [!IMPORTANT]
> + Each code line must be written at new line.
> + Variable names must start only with letter amd must contain only letters and numbers.
> + While loops and if statements must be finished with 'sneeze' written at new line.
> + Variables created in while loops and if statements are usable only within loops' and statements' body.

**Key words:**

+ `is` -> value assignment
```
variableName is 10
```
+ `eat` -> addition operation
+ `spit` -> subtraction operation
+ `imagine` -> multiplication operation
+ `bite` -> division operation
```
variableName is 10 imagine 2 eat 10 bite 6 spit 1
```
+ `snif` -> get variable value 
```
snif varibaleName
```
+ `bark` -> print variable or arithmetic expression value
```
bark varibaleName
```
or
```
bark varibaleName1 eat varibaleName2
```
+  `tasty` -> ==
+  `small` -> <
+  `big` -> >
+  `nah tasty` -> !=
+  `nah small` -> >=
+  `nah big` -> <=
+  `bouncing` -> while loop
```
bouncing varibaleName tasty 10
```
+ `slap` -> if statement
```
slap varibaleName tasty 10
```
+ `sneeze` -> marks the end of a while loop or if statement body
```
slap/bouncing <some logic>
<some code>
sneeze
```

### SealLang program file example

```
snif fish1
snif fish2
meal is fish1 eat fish2
party is 7
bouncing meal bite party small 1
slap fish1 nah tasty 10
fish1 is 10
sneeze
meal is meal eat (fish1 eat fish2) imagine 2
sneeze
slap meal big 10
bark meal spit 1
sneeze
```

## How to integrate in c++ program

You can integrate SealLang in your c++ code. For using some features you need to use `seal` namespace.

+ `seal::Value` is the class which is used to contain a variable's value. At the current moment the only available value type is integer.
```cpp
seal::Value value {13};
```
+ `seal::Context` is a class which can optionally contain variable names and their values, if you want to have access to any variables in your SealLang code or if you want your code just to perform operations without advance assignment, or can be empty. To construct `Context` object you need `std::unordered_map` which contains `std::string` for the name and `seal::Value` for the value. To get access to the variable use `.variables` of type unordered map.
```cpp
std::unordered_map<std::string, seal::Value> context_to_add {
    { "seal1", seal::Value{5}},
    { "seal2", seal::Value{5}} }; // creating map with variables
seal::Context context_1; // empty 
seal::Context context_2 {context_to_add}; // creating context
context2.variables.insert({"seal3", seal::Value{5}}); // example of getting access to the context
```
+ `seal::Executor` is a class which performs code written in SealLang. The `Executor` object is constructed with the `Context` object. The code to execute must be `std::string`. To execute code use `.Execute(std::string code)` method.
```cpp
std::string code; // any code in SealLang
seal::Executor exe {context_2}; // creating executor
exe.Execute(code); // executing code
```