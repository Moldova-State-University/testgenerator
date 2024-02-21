# test generator

## Description

This is a test generator for Programming Contest tasks. Application reads a test description and generate a set of tests (text files) that satisfy the description.

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

## Test description file

Test description file is a YAML file that contains the following fields:

- filename: pattern for test files
- nr_tests: number of tests to generate
- description: description of the test
- structure: structure of the test

### Structure of the test

Structure of the test defines

- number of lines in the test file
- format of each line

Line can contain:

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
filename: "test%02d.txt"
nr_tests: 10
description: "Test description"
lines:
  - line:
      type: integer
      min: 1
      max: 100
      name: N
  - line:
      type: array
      size: N
      element:
        type: integer
        min: -100
        max: 100
```
