Lindenmayer Systems or L-Systems for short are text re-writing algorithms at heart which have been extensively put to use for simulation of the lifecycle of botanical organisms.

An L-System consists of an [[Axiom]], [[Alphabet]], and a set of [[Re-Write Rule]]s. The [[Alphabet]] is simply the set of allowed characters. The algorithm works by starting with the [[Axiom]] then repeatedly applying the [[Re-Write Rule]]s for the desired number of iterations. Here is an extremely simple example.

- [[Axiom]]: "F"
- [[Alphabet]]: "F", "-", "+"
- [[Re-Write Rule]]s:
	- "F" -> "F+F--F+F"

First Iteration:
"F" -> "F+F--F+F"

Second Iteration:
"F+F--F+F" -> "F+F--F+F+F+F--F+F--F+F--F+F+F+F--F+F"

And so on... 
Clearly these can get very large very quickly!

# Parametric L-Systems





#algorithm