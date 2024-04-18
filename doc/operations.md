# Operations
Note: All operations that take in indexes support negative indexes (index from the back of array)

Note: All values that are null are ignored: operation skipped, not joined, ...

## `split`
Split the given string at the given delimiter

### Arguments
- delimiter string
### Inputs
- String
### Outputs
- Array of strings
### Special behaviour
- When the delimiter is an empty string, split at every character in the string.

## `join`
Joins a list of strings or numbers together to a single string with the given delimiter

### Arguments
- delimiter string
### Inputs
- Array of strings or numbers
### Outputs
- String

## `select`
Select elements at indexes from a list

### Arguments
- Array of indexes -> return array
- OR single index -> return element
### Inputs
- Array of T
### Outputs
- Array of T
- OR T

## `tokens`
Tokenize the given string (group by the following kinds of characters):
- Letters
- Numbers
- Others

### Inputs
- String
### Outputs
- Array of strings

## `parse`
Converts the given string to a number

### Inputs
- String
### Outputs
- Number

## `str`
Converts the given number to a string

### Inputs
- Number
### Outputs
- String

## `map`
Execute a block for every element in an array and replace array with results

### Arguments
- OPTIONAL: array of indexes or index to modify
- Block
### Inputs
- Array
### Outputs
- Array

## `transform`
Identical to:
```
make 2
map 1
    BLOCK
```

## `len`
Returns the length of the given string or array

### Inputs
- String or array
### Outputs
- Number

## `invert`
If the given number is a 0 (false), converts it to a 1 (true), otherwise converts it to a 0 (false)

### Inputs
- Number
### Outputs
- Number

## `same`
Check if all the elements in the array are equal

### Inputs
- Array
### Outputs
- Number: 0 (false), 1 (true)
### Special behaviour
- If there are no elements or one element in the array, returns 1 (true)

## `any`
Checks if any element in the array is not zero (= if any element is true)

### Inputs
- Array of numbers
### Outputs
- Number: 0 (false), 1 (true)
### Special behaviour
- If there are no elements in the array, return 0 (false)

## `make`
Creates an array of the given length and fills it with the input

### Arguments
- Number: length
### Inputs
- T: fill value
### Outputs
- Array of T

## `axis`
Creates a 2D array of every element after the first element 
and set the first column to the first element in the original array.

### Pseudocode
```kotlin
fun <T> Array<T>.axis(): Array<Array<T>> {
    val axis = get(0)
    val new = new Array()
    for (element in subList(1)) {
        new.append(new Array(axis, element))
    }
    return new
}
```
or:
`[A, 0, 1, 2, ..] -> [[A, 0], [A, 1], [A, 2], ..]`

### Inputs
- Array
### Outputs
- Array

## `filter`
Creates a new array of only elements for that the given block returns not 0 (= true)

### Arguments
- Block
### Inputs
- Array
### Outputs
- Array

## `contains`
Checks if a substring or subarray is present in the array

### Arguments
- Array or string
### Inputs
- Array or string
### Outputs
- Number: 0 (false), 1 (true)

## `ranges`
Select a set of ranges from an array

### Arguments
- Array of numbers: flattened array of pairs of start and end index
### Inputs
- Array of T
### Outputs
- Array of T

## `noempty`
If the value is an empty string or array, replace it with null.

### Inputs
- Any
### Outputs
- Any

## `append`
Appends a string or array onto a string or array

### Arguments
- String or array
### Inputs
- String or array
### Outputs
- String or array

## `prepend`
Prepends a string or array onto a string or array

### Arguments
- String or array
### Inputs
- String or array
### Outputs
- String or array

## `group`
Combine the elements of the array into groups of N

### Arguments
- Number
### Inputs
- Array of T
### Outputs
- Array of Array of T

## `rev`
Reverse the elements of an array

### Inputs
- Array
### Outputs
- Array

## `use`
Select all elements at the index(es) specified by the argument and use
them to execute the given block. Then return the original array except all
the elements at the index(es) specified by the argument and with the
result of the block as first element.

### Arguments
- Array or index
- Block
### Inputs
- Array
### Outputs
- Array

## `run`
Run a shell command.

### Arguments
- String
### Inputs
- String
### Outputs
- String

## `with`
Use the whole input or the first element of the input (if the input is an array)
as argument for all commands in the given block that don't have an argument explicitly
specified, and executes the block. Then returns the result or, if the input is an array,
returns an array of the result and the rest of the elements in the array (everything but the first element).

### Arguments
- Block
### Inputs
- Array or any
### Outputs
- Array or any

## `flatten`
Zero- to one-dimensional flatten.

### Example
`[[1, [2]], 3]` -> `[1, [2], 3]`

### Inputs
- Array or any
### Outputs
- Array

## `deshape`
N-dimensional flatten.

### Example
`[[1, [2]], 3]` -> `[1, 2, 3]`

### Inputs
- Array or any
### Outputs
- Array

## `flatmap`
Map and then flatten

## `falpha`, `fdigit`, `falnum`, `fpunct`, `fwhite`
Remove all characters from the string that are not:
- for `falpha`: a letter ('a' - 'z', 'A' - 'Z')
- for `fdigit`: a digit ('0' - '9')
- for `falnum`: a letter or a digit ('a' - 'z', 'A' - 'Z', '0' - '9')
- for `fpunct`: any of ```! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~```
- for `fwhite`: space, newline, horizontal and vertical tab, form feed and carriage return

## `fnalpha`, `fndigit`, `fnalnum`, `fnpunct`, `fnwhite`
Remove all characters from the string that are:
- for `falpha`: a letter ('a' - 'z', 'A' - 'Z')
- for `fdigit`: a digit ('0' - '9')
- for `falnum`: a letter or a digit ('a' - 'z', 'A' - 'Z', '0' - '9')
- for `fpunct`: any of ```! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~```
- for `fwhite`: space, newline, horizontal and vertical tab, form feed and carriage return
