
# HMS

Extension for [sqlite] to parse a duration denoted in
a string as hours:minutes:seconds into seconds. It implements:

* `duration(string) - parse hh:mm:ss.s to seconds`
* `hms(number)`- convert seconds to a hh:mm:ss.s string

```
sqlite> .load hms
sqlite> select duration("0:2:5.2");;
125.199996948242

sqlite> select duration("abc");;
Runtime error: duration failed to parse argument

sqlite> select hms(122.3);
0:02:02.300

```

## Building

The Makefile builds this extension on macOS using a path to the [sqlite]
source code. The resulting `hms.dylib` can be loaded into the sqlite
shell.

## Error Handling

As can be seen above, if duration() can't parse its argument it throws
an error. Should this result in NULL instead?


[sqlite]: https://sqlite.org
