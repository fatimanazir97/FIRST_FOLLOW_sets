# FIRST_FOLLOW_sets
Computing first and follow sets in compiler construction 
First and Follow Sets
You are required to write a C++/Python program to calculate all F irst & F ollow sets in a CF G.
Please read the instructions carefully!

1. Your code must read the grammar from a txt file. The data in the file should be tab sepa-
rated.The format of the grammar file is as follows:

• Every production is written in a single line. The first item of (the left most item) every
line.
• Non terminal representing the left hand side of the production where as the rest of the
items constitute the right hand side of the production. Each item is tab separated.
• ε is the symbol representing epsilon. The grammar given below has eight productions
and is shown in the samplegrammar.txt
E → T EP
EP → + T EP | ε
T → F TP
TP → * F TP | ε
F → ( E ) | id
2. Your code must be generic and it should give correct results on any CF G.
3. The first word of each line will be a non-terminal(variable). Non-Terminals are combination
of one or more capital letters {A-Z}+
4. The right hand side of any production is restricted to at max 10 literals.
5. The terminals could be combination of lowercase letters or any numbers or any operator(including
; . , characters) or any bracket. For now, you can assume two character operators like <<
>> −− ++ && == etc. are not part of your productions. So terminals will be:
{a-z}+ | {0-9}+ | ε | any operator | any bracket

6. Your program should output two txt files. The name of the output files should be RollNum-
ber first.txt(containing all first sets) and RollNumber follow.txt(containing all follow sets).

7. The format of your output files should be as follows:
• The first item of each line is a non terminal whose first or follow set appear in the same
line such that each item is tab separated.
• Please write only one set per line.
