This repository includes my attempt at implementing somewhat of a compiler for C language. 

Lexical tokenization and parsing are currently working for most of the language, according to the grammar included in `GRAMATICA.md`, which is inspired by [yacc](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html).

Semantic analysis is very primitive so far, including only the necessary for the generation of intermediate code of a "for" loop.
