This paper presents a method for generating large scale cities composed of a road network and buildings using [[Extended L-Systems]]. [[Parametric L-System]]s are allowed to run producing a template, the parameters of the produced L-System are then modified externally to comply with a set of global goals (non-essential but desired) and then these are finally modified by a set of local constraints (essential, e.g. "building can't be on water"). This allows the output to be modified by changing these constraints without modifying the actual L-Systems.
### Quotes
- "The idea of L-system branches growing together and forming closed loops could not be found in existing applications, although a similar approach to the one we propose can be found in the procedural generation of blood vessels [21]. In traffic sys tems the dead end road is the exception. Most roads end when crossing other roads or circling back to themselves." p305.
### Thoughts on Usage
- Most likely used to justify not pursuing City generation as this paper and the following work by Pascal Müller extensively covers this area of research and the software he produced and it's derivatives are extensively used in movies and games.
- The technique outlined in this paper (I believe there is another paper which coined this as "[[Functional L-System]]" which I prefer and will have to find) sounds very promising and I can see uses for it in the generation of other assets where loose artistic control is desired (Items, Terrain (impose limits on how rough or similar neighbouring points are), ) .
- Quote #1 relates to a method for closing the loop where it would be reasonable to do so when generating a road network. I can see this having applications in many areas such as texture generation and level generation. See included image: ![[lsystem_circuit_closing.png]]
### Further Work
There were a few papers released later that built upon this research which I will list here as I work through them:
### Paper
- [Local](file:///W:%5CBsc%20Computing%5CDissertation%5CThird%20Party%5CPapers%5CProcedural%20Modeling%20of%20Cities.pdf)
- [Web](https://web.archive.org/web/20060114082225/http://www.vision.ee.ethz.ch/~pmueller/documents/procedural_modeling_of_cities__siggraph2001.pdf)


#source