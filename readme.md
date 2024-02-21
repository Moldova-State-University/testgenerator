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

Example:

```yaml
filename: "input%.txt"
from: 1
to: 10
description: "Test description"
lines:
  - # line definition
  - # line definition
  - # line definition
  # other lines
```

### Structure of the test

Structure of the test defines format of each line. Line can contain one element.

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
symbols: "abc"
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
  symbols: "abc"
```

## Example

This example defines 10 tests. Test contains 2 lines. First line contains one integer `N` value in the range from 1 to 100. Second line contains `N` integer values in the range from -100 to 100.

```yaml
filename: "input%.txt"
from: 1
to: 10
description: "Test description"
lines:
  - type: integer
    min: 1
    max: 100
    name: N
  - type: float
    min: 0
    max: 1
  - type: array
    size: 100
    element:
      type: integer
      min: -100
      max: 100
```

## Plans for the future

- define more than one element in the line
- define more than one test definition in the file
- define lines generator
