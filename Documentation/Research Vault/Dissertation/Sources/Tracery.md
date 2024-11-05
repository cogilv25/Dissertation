Tracery is a [[Text Generation]] Method where a grammar is used to generate sentences or other bodies of text, it uses an [[Axiom]] as a starting point and a set of [[Re-Write Rule]]s to expand this into one of the set of possible output texts. Tracery's [[Re-Write Rule]]s are basically just named lists of potential text's which can also contain other [[Re-Write Rule]]s surrounded by # symbols, Tracery then starts from the [[Axiom]] and expands any [[Re-Write Rule]]s it contains by choosing a random item from the list, it does this recursively until a final text is acquired. Tracery also supports "modifiers" inside [[Re-Write Rule]]s such as .capitalize or .pluralize, and high level functions which are registered and then called within the [[Re-Write Rule]]s such as push and pop.

### Real World Usage
- Eternal Night Vale
- Interruption Junction
- Neverbar (can't seem to find this online...)

### Paper
- [Local](file:///W:/Bsc%20Computing/Dissertation/Third%20Party/Papers/Tracery.pdf)
- [Web](https://link.springer.com/chapter/10.1007/978-3-319-27036-4_14)



#source