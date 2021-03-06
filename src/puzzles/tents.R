# -*- makefile -*-

TENTS_EXTRA = maxflow

tents    : [X] GTK COMMON tents TENTS_EXTRA tents-icon|no-icon

tents    : [G] WINDOWS COMMON tents TENTS_EXTRA tents.res|noicon.res

ALL += tents[COMBINED] TENTS_EXTRA

tentssolver :   [U] tents[STANDALONE_SOLVER] TENTS_EXTRA STANDALONE
tentssolver :   [C] tents[STANDALONE_SOLVER] TENTS_EXTRA STANDALONE

!begin gtk
GAMES += tents
!end

!begin >list.c
    A(tents) \
!end

!begin >wingames.lst
tents.exe:Tents
!end
