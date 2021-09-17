# Sokoban 0.1
A sokoban (倉庫番) game written in C. It should be run under UNIX.

## How to Run
Under UNIX, simply run
```
$ make
```
under the directory `sokoban` to build the execution files, `game` and `verifier`.

## Legend
`#`: wall square

`@`: player on a non-goal square

`+`: player on a goal square

`$`: box on a non-goal square

`*`: box on a goal square

`.`: goal square

`-`: non-goal square

## Control
Use arroy keys or `W`, `A`, `S`, `D` to move.

Press `P` to pause. You can either resume, go to the next stage, or quit the game directly.
