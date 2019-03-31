# A.I. solver for Traffic Jam game
A.I. solver using **Iterative deepening depth-first search**

![entry state](http://www2.fiit.stuba.sk/~kapustik/krizovatky.gif) ![final state](http://www2.fiit.stuba.sk/~kapustik/krizovatky_ciel.gif)
## Installation
on linux:
```
gcc -o traffic-solver main.c
```
## Usage
Prepare input text file:
first line: number of 'cars'
other lines: [is escape car][length of a car][horizontal position][vertical position][orientation]
* **x, y** positions match **top-left corner** of a car
* orientation of a car is set to **0 if horizontal** or **1 if vertical**
#### Example of input file
```
8
0 2 1 1 0
0 3 6 1 1
0 3 1 2 1
0 3 4 2 1
1 2 2 3 0
0 2 1 5 1
0 2 5 5 0
0 3 3 6 0
```
#### Generated map from input file
```
+ + + + + + + +
+ a a - - - b +
+ c - - d - b +
+ c e e d - b +
+ c - - d - - +
+ f - - - g g +
+ f - h h h - +
+ + + + + + + +
```
#### Execution
```
./traffic-solver input.txt
```
#### Generated solution
Numbers reflecting movement on X, Y axis where **X points right** and **Y points down**
```
1: a horizontal 1
2: c vertical -1
3: f vertical -1
4: g horizontal -3
5: b vertical 3
6: h horizontal -2
7: d vertical 2
8: e horizontal 3
```
## Implementation
   The alghorithm is implemented using *iterative deepening*.
   It's a depth-first algorithm with maximum search depth which is increasing iteratively.

   Depth-first alghorithm is implemented as recursion.
   Search node is represented as a structure with the same pattern as input data
   Solution is represented as an array of steps, where each step is added on the way back from successful recursion


