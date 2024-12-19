# Language

Project of translation system - from my language to assembler 

> [!Important]
> The project has a submodule [```Processor```](https://github.com/bstrsanya/Processor) that compiles the assembly code and runs the program.

## Grammar

```cpp
Tokens
    TName   ::= token_name
    TNumber ::= token_number

Main Rules
    G ::= M END
    M ::= {D}*
    D ::= TName '(){' {S}* '}'
    S ::= {V ';'}*
    V ::= I | W | F | A

Structural analysis
    I ::= 'if(' C '){' S '}' [['else{' S '}']]
    W ::= 'while(' C '){' S '}'
    F ::= TName '(' [[E]] { ',' E}* ')'
    A ::= ([['int' | 'double']] TName '=') F | C

Expression analysis
    C ::= E{['==' '<' '>' '<=' '>=' '!=']E}*
    E ::= T{[+-]T}*
    T ::= P{[*/]P}*
    P ::= '('E')' | N
    N ::= TName | TNumber
```

## Compare C & My language

*Example of code (C):*

```cpp
int main ()
{
    int factorial = 1;
    int counter = 1;
    int value = 0;
    scanf ("%d", &value);
    fact ();
    printf ("%d", factorial);
}

void fact ()
{
    while (counter <= value)
    {
        factorial = factorial * counter;
        counter = counter + 1;
    }
}
```

*Example of code (My language):*

```cpp
main () 
udos
    int factorial = 1;
    int counter = 1;
    int value = 0;
    input (value);
    fact (); 
    output (factorial);
peresda

fact () 
udos
    when_good (counter <= value) 
    udos
        factorial = factorial * counter;
        counter = counter + 1;
    peresda
peresda
```


