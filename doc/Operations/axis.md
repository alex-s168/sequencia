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

