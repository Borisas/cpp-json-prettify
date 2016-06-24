# json-prettify
C++. Make ugly json string into a readable pretty json string.

# What does it do:

Turns this:
    
    {"a":4,"b":{"p":"2","d":4,"e":[1,2,3]},"c":["t",4,2,3,{"a":1,"b":2}]}
    
Into this:

    {
        "a" : 4,
        "b" : {
            "p" : "2",
            "d" : 4,
            "e" : [
                1,
                2,
                3
            ]
        },
        "c" : [
            "t",
            4,
            2,
            3,
            {
                "a" : 1,
                "b" : 2
            }
        ]
    }

## How to use:

```c++
    #include "JSONPrettify.h"
```
    
  This one doesn't add spaces inbetween colons.
  
```c++
    std::string json = "... some json data ...";
    std::string pretty = boris::JSONPrettify( json );
```
    
  Neither does this one
  
```c++
    std::string json = "... some json data ...";
    std::string pretty = boris::JSONPrettify( json , boris::Colons::TIGHT );
```
    
  This one adds spaces inbetween colons.
  Spaces inbetween colons aren't fully complete - if you have colons in the middle of variable names or values it will put spaces. ie.: "a:b":1  ->  "a : b" : 1
  
```c++
    std::string json = "... some json data ...";
    std::string pretty = boris::JSONPrettify( json , boris::Colons::SPACED );
```
