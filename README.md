Pebble Todo
==========

Gets open issues from my [todo repo](github.com/scottopell/todo) and displays
them on my pebble.

Uses typescript to write the JS side, modified the wscript to compile the typescript,
but can't get `pebble clean` to remove the generated JS files, which is annoying.

Also, since Pebble doesn't support source maps, the logs have line numbers that
refer to the js code, which is annoying too.

Overall probably not worth it to use typescript, but it was fun.

Would be cool to see typescript supported in Pebble dev, could provide a definition
file for the Pebble object.
