file(REMOVE_RECURSE
  "../generated/ANTLR/TLexer.cpp"
  "../generated/ANTLR/TParser.cpp"
  "../generated/ANTLR/TParserBaseListener.cpp"
  "../generated/ANTLR/TParserBaseVisitor.cpp"
  "../generated/ANTLR/TParserListener.cpp"
  "../generated/ANTLR/TParserVisitor.cpp"
  "CMakeFiles/GenerateParser"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/GenerateParser.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
