This paper uses a [[L-System]] to generate a [[Level]]/[[Level]]s for a virtual reality escape room game. An [[L-System]] plugin for blender is used to generate the level/s, 2 iterations are used for the L-System to produce the desired level/s which is then exported into the Unity game engine to be used in the game. the L-System generates pairs of lines that form left and right turns these then have cubes placed around them in Unity to apply a wall-like texture.
### Notes
- This is very preliminary work, the L-System is single use and has no way to be reused in order to produce multiple levels.
- It's not completely clear if this system generates multiple levels or just one, multiple floors are mentioned but again it isn't clear if these floors are generated together by a single L-System instance or done in stages, although I would assume they are done all at once as only one L-System implementation is presented.
### Criticism
- As the authors state, it would be much more useful to have the L-System integrated into the game (Unity in this case) rather than using an external plugin and having to import the generated content each time.
### Thoughts on Usage
Possibly an example of how little has been done in the area of level generation with this being extremely preliminary in nature.
### Paper
- [Local](file:///W:/Bsc%20Computing/Dissertation/Third%20Party/Papers/A%20Virtual%20Reality%20Game%20Utilizing%20L-Systems%20for%20Dynamic%20Level%20Generation.pdf)
- [Web](https://www.jait.us/uploadfile/2024/JAIT-V15N2-276.pdf)

#source