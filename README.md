TMLanguage
==========

You call the compiler by doing: ./Parsing SOURCEFILE OUTPUTDOT

A program written in TML is converted to a turing machine. It can manipulate the head and read/write from it.    
By default a program rejects everything.

The first thing in a program has to be a list of all characters allowed on the tape.    
Then there can be a set of statements.

The default blank symbol is: "BLANK"    
Symbols may be of any length and can be the full unicode range.    
They should not be empty strings as those can not be written.

The following keywords are available:

IF, WHILE, END, WRITE, MOVERIGHT, MOVELEFT, ACCEPT, REJECT

These are used as control structure, output, the movement of the head and to indicate if we accept or not.

List
----

A list is a series of symbols.    
Example: ["a","b","c"]

IF and WHILE
------------

These keywords take a list behind them. You should include at least one space behind the keywords.    
Example: IF ["a","b"]

Then you should follow by a block of statements and at the end you place the keyword END

Example:

```
    ["a"]
    WHILE ["a"]
        MOVELEFT
    END
```

This program will move the head left as long as the symbol under the head is an 'a'.

WRITE
-----

Write is used to put a symbol on the tape under the head.    
It should also be followed by a list.    
Example: WRITE ["a"]

You can not write the empty string, as that means write nothing.

MOVELEFT/MOVERIGHT
------------------

These move the head one space to the left or the right.

ACCEPT/REJECT
-------------

Accept and reject can be used to put the program into either of the states.


```
    ["a", "b"]

    IF ["a"]
        ACCEPT
    END
```

The above program accepts strings that start with 'a' and rejects strings that start with 'b'.


Functions
---------

You can define a function by using *NAME*    
Followed by a block and then the keyword END

Example:

```
    *RemoveUntillNotC*
        WHILE ["c"]
        	WRITE ["BLANK"]
        	MOVERIGHT
        END
    END
```

You can call a function by using _NAME_    

Example: _RemoveUntillNotC_

Functions should not be called within functions.
