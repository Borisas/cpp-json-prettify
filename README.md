# json-pretify
C++. Make ugly json string into a readable pretty json string.

## How to use:
    
  This one doesn't add spaces inbetween colons.
  
    std::string json = "... some json data ...";
    std::string pretty = JSONPretify(json);
  
  Neither does this one
  
    std::string json = "... some json data ...";
    std::string pretty = JSONPretify(json,false);
    
  This one adds spaces inbetween colons.
  
    std::string json = "... some json data ...";
    std::string pretty = JSONPretify(json,true);
