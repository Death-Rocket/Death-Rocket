# Death rocket

## Summary

- [Death rocket](#death-rocket)
  - [Summary](#summary)
  - [Contributors](#contributors)
  - [Concept](#concept)
  - [Controls](#controls)
- [Features](#features)
  - [Rocket](#rocket)
  - [Ult](#ult)
  - [Items](#items)
- [Screenshots](#screenshots)

## Contributors

- [Felix](https://gitlabstudents.isartintra.com/f.becaud)
- [Gabriel](https://github.com/piking77420)
- [Paul](https://github.com/susanoo1004)
- [Yohann](https://github.com/YohannDR)
  
## Concept

A multiplayer death match game where you use rockets to kill your opponent(s).

## Controls

![controls](imgs/controls.png)

# Features

## Rocket

You can shoot a basic rocket to your opponent, if they are directly touched they'll die but if they are touched by the explosion that happens when a rocket hits something they'll only take one damage, knowing that a player has 2 HP. If they are in the deflagration of the explosion, they'll just be blown away.

## Ult 

You have an ultimate ability, which can be triggered once your bar (at the bottom of the screen) is charged at 100%. You have to press ▲ to activate your ultimate. Once you press it your bar will be put at 0%. There are 4 Ultimates :
- Invincible\
    You can't take any damage
- Invisible\
    You are invisible to your opponent
- NoReload\
    You don't have to reload to shoot
- OverDrive\
    You can't get hurt by your own rockets, a direct hit won't kill you and your reload will be halved

## Items

- Implosive rocket\
  After explosion, attracts anything inside the purple radius.
<img alt=implosion src=Content/Blueprints/BuffImage/ImpulseRocketDarkMode.png width=256>

- Triple Rocket\
  Shoots three rockets at an angle of -10, 0 and 10 degrees.
<img alt=triple src=Content/Blueprints/BuffImage/T_TripleRocket.png width=256>

- Mine\
  First stick to the surface it lands on, then explode if anyone approches it. Launcher can exit the radius if they's in it at landing.
<img alt=mine src=Content/Blueprints/BuffImage/Mine.png width=256>
  
- Homing Rocket\
    Whenever an enemy enters the detection zone, the rocket starts following them.\
<img alt=homing src=Content/Blueprints/BuffImage/Radar.png width=256>

- Barrel\
    Explodes when hit causing an explosion or an implosion.

    ![Alt text](imgs/barrel.png)

# Screenshots
  ![screenshot](imgs/gameplay.png)
  ![inv](imgs/inv.png)