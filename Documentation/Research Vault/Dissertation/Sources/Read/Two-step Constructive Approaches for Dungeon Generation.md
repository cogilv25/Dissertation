The paper presents a 2D [[Multi-Layer Generation]] technique using [[Wave Function Collapse]] to generate dungeon "[[Level]]s". The first step generates the layout of the dungeon which they called the "creator" and the second step generates the game objects which they called the "furnisher". Three options for each step are provided and all 9 combinations are evaluated. The options are [[Constraint-Based Generation]], [[Cellular Automata]] Based Generation and [[Agent-Based Generation]] with slight variations between "creator" and "furnisher" generators. A number of metrics are used to perform [[Structural Analysis]] of the generated levels, additionally [[Playability]] is measured by allowing Artificial agents to "play" the game using a number of different "personas" (Play Styles) and evaluating metrics collected from these sessions to see how successful the agent was in fulfilling their objectives. There were three "personas":
- "Runner" complete the level as fast as possible
- "Killer" kill as many monsters as possible AND complete the level
- "Collector" collect as much treasure as possible AND complete the level
### Notes
- It seems that by balancing the outcomes of each of the "personas" a level generator could be selected that satisfies all "personas" to a lesser or greater extent depending on the desired outcome.
### Criticism
The paper describes it's methods as [[Constructive Generation]], however, the [[Cellular Automata]] "creator" presented iterates an unknown number of times making this (according to their own definition) a non-constructive algorithm.

### Quotes
- Introduction: "level generation in published games is mostly carried out via constructive algorithms. Unlike generate-and-test processes, constructive generators do not evaluate and re-generate output; for example, cellular automata and grammars can be used for constructive generation, as well as more freeform approaches such as diggers [10]. Such generators are computationally lightweight since they do not evaluate their generated output. This allows games to quickly create endless variations to game-play by generating maps as in Minecraft (Mojang 2011), weapons as in Borderlands (Gearbox 2009) or NPCs as in Skyrim (Bethesda 2011) in real-time."

### Thoughts on Usage
- Possibly we could generate something with L-Systems then use another algorithm on the output to create our final asset (I'm thinking [[Cellular Automata]] to add detail to the L-System output) then we could link back to this as the inspiration for the multi-layer generation approach. 
- It's conceivable that the metrics used to assess the structure and playability of the levels generated could be adapted and used for my own generators.

### Paper
- [Local](file:///W:/Bsc%20Computing/Dissertation/Third%20Party/Papers/Twostep%20Constructive%20Approaches%20for%20Dungeon%20Generation.pdf)
- [Web](https://arxiv.org/pdf/1906.04660)


#source