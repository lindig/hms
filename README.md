
# HMS

Extension for [sqlite] to parse a duration denoted in
a string as hours:minutes:seconds into seconds. It implements a
`duration()` function.

```
sqlite> .load hms
sqlite> select duration("0:2:5.2");;
125.199996948242

sqlite> select duration("abc");;
Runtime error: duration failed to parse argument
```

## Building

The Makefile builds this extension on macOS using a path to the [sqlite]
source code. The resulting `hms.dylib` can be loaded into the sqlite
shell.

## Error Handling

As can be seen above, if duration() can't parse its argument it throws
an error. Should this result in NULL instead?


[sqlite]: https://sqlite.org
