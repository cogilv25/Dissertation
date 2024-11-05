This paper, as the title suggests presents the generation of mazes using L-Systems, specifically using [[Stochastic L-System]]s and [[Parametric L-System]]s to generate random mazes with varying lengths or corridors. Interestingly although it is a small point it mentions the idea of node rewriting where a node on a grid is rewritten with a group of nodes rather than the typical edge rewriting seen in most L-Systems. Image is taken from the paper.
![[node_rewriting.png]]
Although this point is not of particular note and is in-fact mentioned in the original book the visualization is very good and expands my ability to imagine the way in which such a system would operate where I had been thinking more about the bothersome low-level concern of traversing across multiple dimensions. This would be an excellent way to allow the declaration of [[Node Re-Writing]] based rules visually!

### Notes
The mazes output by the system were very asymmetric and seemed to use a small percentage of the available nodes within the game area. This gives the distinct feeling that the mazes were not created by a human, however, I can't quite describe why nor am I sure if this is a negative.

### Thoughts on Usage
- Perhaps an extension to this is to give the player the L-System in some form to allow them to use it as a clue, leveraging the self-similarity of L-Systems.. Perhaps this is too confusing and/or convoluted to present to the player with the expectation of it being useful?
- There's not a huge difference between maze-creation and level creation so perhaps some crossover lessons could be used from this paper if level generation is performed.

### Paper
- [Local](file:///W:%5CBsc%20Computing%5CDissertation%5CThird%20Party%5CPapers%5CL-Systems%20and%20Procedural%20Generation%20of%20Virtual%20Game%20Maze%20Sceneries.pdf)
- [Web](https://www.sbgames.org/sbgames2017/papers/ComputacaoShort/174978.pdf)

#source