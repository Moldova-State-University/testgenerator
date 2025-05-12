# test generator

## Description

This is a test generator for Programming Contest tasks. Application reads a test description and generate a set of tests (text files) that satisfy the description.

## Requirements

- CMake 3.28 or later
- C++17 compiler
- yaml-cpp library

## Build

Download the source code and build the application using the following commands:

```bash
mkdir build
cd build
cmake ../src
make
```

## Usage

```bash
./testgenerator <test_description_file>
```

You can generate a sample of the test description file using the following command:

```bash
./testgenerator --sample
```

## Test description file

Test description file is a YAML file that contains the following fields:

- `filename`: pattern for test files, the percent (`%`) sign is replaced with the test number
- `from`: start number of tests to generate
- `to`: end number of tests to generate
- `description`: description of the test (?) - not used
- `lines`: lines of the test
- `block`: block of lines

Example:

```yaml
filename: "input%.txt"
from: 1
to: 10
description: "Test description"
blocks: # blocks of lines
  - nr_of_lines: 1 # number of lines in the block
    line: # line definition
      - type: integer
        min: 1
        max: 100
        name: N
      - type: integer
        min: 1
        max: 100
        name: M
  - nr_of_lines: N # number of lines in the block
    line: # line definition
      - type: array
        size: M
        element:
          type: integer
          min: -100
          max: 100
# other blocks
```

### Structure of the test

Structure of the test defines blocks of lines. Block of lines contain number of lines with the same structure and line definition. Line can contain one or more elements. 

Each element can be of the following types:

- integer
- float
- string
- array of integers
- array of floats
- array of strings

For each integer you can define a range of values, also can have a name for reference in the next lines.

For each float you can define a range of values and maximal number of digits in the decimal part.

For each string, you can define minimal and maximal length and allowed symbols.

For each array, you can define a number of elements and a range of values for each element. Number of elements can be a constant or a reference to the previous lines.

### Integer value definition

```yaml
type: integer
min: 1
max: 100
name: N
```

### Float value definition

```yaml
type: float
min: 0.0
max: 1.0
digits: 2
```

If digits is not defined, the number of digits is not limited (number is not truncated).

### String value definition

```yaml
type: string
min_length: 1
max_length: 10
characters: "abc"
```

### Array of integers definition

```yaml
type: array
size: 5
element:
  type: integer
  min: 1
  max: 100
```

### Array of floats definition

```yaml
type: array
size: 5
element:
  type: float
  min: 0.0
  max: 1.0
  digits: 2
```

### Array of strings definition

```yaml
type: array
size: 5
element:
  type: string
  min_length: 1
  max_length: 10
  characters: "abc"
```

## Examples

### Simple example

This example defines 10 tests. Test contains 2 lines. First line contains one integer `N` value in the range from 1 to 100. Second line contains `N` integer values in the range from -100 to 100.

```yaml
filename: "input%.txt"
from: 1
to: 10
description: "Test description"
blocks:
  - nr_of_lines: 1
    line:
      - type: integer
        min: 1
        max: 100
        name: N
      - type: integer
        min: 1
        max: 100
        name: M
  - nr_of_lines: N
    line:
      - type: array
        size: M
        element:
          type: string
          min_length: 10
          max_length: 10
          characters: "abc"
```

### Multiple document definition example

Yet another sample, with multiple document definition:

```yaml
---
# simple tests
filename: "input%.txt"
from: 1
to: 3
description: "Test description"
blocks:
  - nr_of_lines: 1
    line:
      - type: integer
        min: 1
        max: 100
      - type: integer
        min: 1
        max: 100
---
# medium tests
filename: "input%.txt"
from: 4
to: 7
description: "Test description"
blocks:
  - nr_of_lines: 1
    line:
      - type: integer
        min: 100
        max: 1000
      - type: integer
        min: 100
        max: 1000
---
# complex tests
filename: "input%.txt"
from: 8
to: 10
description: "Test description"
blocks:
  - nr_of_lines: 1
    line:
      - type: integer
        min: 1000
        max: 10000
      - type: integer
        min: 1000
        max: 10000
```

This sample will generate:

- 3 files with 2 integers in the line, values between 1 and 100
- 4 files with 2 integers in the line, values between 100 and 1000
- 3 files with 2 integers in the line, values between 1000 and 10000

### Block of lines example

This example defines 10 tests. Each test contains `N` number of lines in the first line and `M` number of elements in the line. The next `N` lines contain `M` integer values in the range from -100 to 100.

```yaml
filename: "input%.txt"
from: 1
to: 10
description: "Test description"
blocks:
lines:
  - nr_of_lines: 1
    line:
      - type: integer
        min: 1
        max: 100
        name: N
      - type: integer
        min: 1
        max: 100
        name: M
  - nr_of_lines: N
    line:
      type: string
      min_length: 1
      max_length: M

```

## Plans for the future

- [X] define more than one element in the line
- [X] define more than one test definition in the file
- [ ] define lines generator
