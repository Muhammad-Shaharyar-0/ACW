# ACW

AI and Phyics simulation made using opengl.

Project Title: Tower Defence Area Control Game

High Concept
Two (or more) teams of AI agents compete to control cannons mounted on towers on a map for the longest amount of time within a timed round. Each team will be represented by a different colour, and should have a base around the edges of a level where they will respawn if their health drops to zero or less. Towards the centre of a level there should be several towers that can fire cannon ball like projectiles at agents at some rate. If a cannon ball hits an agent the cannon ball is destroyed and the agent suffers damage reducing their health. If a cannon ball does not hit an agent it will bounce around the level and eventually come to rest.

Initially towers are neutral, firing cannon balls at any agent within range. Ownership of each tower is represented by a numerical scale of influence within the range 0-10. Close to the base of each tower is a safe zone where agents are safe from attack of the tower they are closest to. When an agent occupies a defined area at the base of a tower that agent can begin to increase its team’s influence over that tower. When a team’s influence over a tower becomes 5 or greater that tower becomes aligned to the team and will only fire cannon balls at agents from other teams. Before another team can take ownership of a tower they much first reduce the current owner’s influence to 0 before they can increase their own team’s influence. Whilst an agent is influencing a tower they cannot do anything else.
Towers will generate ammo over time at a rate that is proportional to the amount of influence the current owner has over the tower. Agents can also provide the tower with more ammo by collecting spent cannon balls and returning them to the base of the tower.

Agents can attack opposing agents by performing a melee attack on agents that they are very close to, or then can pick up spent cannon balls and throw them at other agents. Agents can also use cannon balls defensively by throwing them at incoming cannon balls to deflect them away. Agents can also find power ups spawned randomly at specific places on the map that can increase their health (making them able to take more damage), speed (making them move faster) or strength (making attacking other agents stronger) or influencing power (increasing the rate that they are able to influence towers). If an enemy takes damage that reduces their health to zero they are respawned in that team’s base losing any benefits they accumulated.

Whilst a team has ownership over a tower they begin to score points at a particular rate. The winning team is the team with the most points at the end of a set time limit.
 
Everything in the game will be constructed from cylinders, spheres and cubes. The terrain should include stacks of cubes that form walls that agents cannot cross. The terrain should include areas of ground that have a colour aligned with agents such that agents of the same colour can move more quickly over those areas.
