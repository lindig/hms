
[![Build Status](https://github.com/lindig/hms/actions/workflows/makefile.yml/badge.svg)](https://github.com/lindig/hms/actions/workflows/makefile.yml)

----

# HMS

HMS is an extension for [sqlite] to parse a duration denoted in a string
as hours:minutes:seconds into seconds. It implements two new
primitives:

* `duration(string)` - parse hh:mm:ss.s to seconds
* `hms(number)`- convert seconds to a `hh:mm:ss.s` string

```
sqlite> .load hms
sqlite> select duration("0:2:5.2");;
125.199996948242

sqlite> select hms(122.3);
0:02:02.300
```

As with other [sqlite] extensions, it is loaded at runtime using `.load`
into the [sqlite] shell. See the [sqlite] documentation how to use
extensions when embedding [sqlite] into an application as a library.

## Building

## macOS

The Makefile builds this extension on macOS using a path to the [sqlite]
source code. The resulting `hms.dylib` can be loaded into the sqlite
shell.

```sh
$ make
```

## Linux

On Linux the required package is:

* Debian [libsqlite3-dev](https://packages.debian.org/trixie/libsqlite3-dev)

```sh
$ make
```

The above is also used in the GitHub CI workflow.

## Testing

Some simple regression testing.

```sh
$ make test
```

## Error Handling

I am not sure about the best strategy. I am changing to let functions
return NULL instead of a runtime error; otherwise working with large
tables can become annoying.

## Limitations

* Durations must be positive, hence `-2:0.0` is not recognised as a
  duration. Likewise, `hms(-120.0)` is not supported.
* Parsing is not entirely robust.

[sqlite]: https://sqlite.org
