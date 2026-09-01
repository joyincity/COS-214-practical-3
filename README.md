# COS-214-practical-3
EventFlow - Designing a Live Event Coordination Engine


OVERVIEW
--------
EventFlow models a Renaissance-style fair as a tree of event areas (zones,
stages, tents, vendors, security) using the Composite pattern, combined with
the Observer pattern for broadcasting fair-wide notices (weather alerts,
evacuations, capacity warnings) that cascade down the tree and trigger a
different reaction from each concrete unit.

DESIGN PATTERNS
----------------
Composite:
  EventComponent   - abstract Component interface
  EventZone        - the only Composite; owns and recurses over children
  Leaves           - Tent, Performance, FoodVendor, MerchVendor, TheWatch

Observer (pull model):
  Observer         - abstract Observer interface
  Control          - base Subject
  EventControl     - concrete Subject (the fair's control centre)
  EventZone        - also acts as both Observer and Subject, forming a
                     "bridge" that lets notices cascade through nested zones
  VipPass, JoustingTournament - original features that observe EventControl
                     directly without being part of the Composite tree

FILES
-----
EventComponent.h        Component interface
EventZone.h             Composite / Observer / Subject
Tent.h                  Leaf
Performance.h           Leaf
FoodVendor.h            Leaf
MerchVendor.h           Leaf
TheWatch.h              Leaf
Observer.h              Observer interface
Control.h               Base Subject
EventControl.h          Concrete Subject
Notice.h                Notice types and Notice object
VipPass.h               Original feature
JoustingTournament.h    Original feature
main.cpp                Demonstration driver
makefile                Build script
Doxyfile                Doxygen configuration

BUILD AND RUN
--------------
make            build the project (compiles main.cpp; all logic is in headers)
make run        build and run ./eventflow
make clean      remove build artifacts
make memcheck   run under valgrind for leak checking

Requires a C++11-capable compiler (g++ by default).

DOCUMENTATION
-------------
Run: doxygen Doxyfile
Output is written to docs/doxygen

WHAT main.cpp DEMONSTRATES
---------------------------
- Building the Composite tree and registering Observer relationships
  separately from ownership.
- A WEATHER_ALERT cascading through 3+ runtime levels, with each leaf type
  reacting differently.
- Runtime reorganisation of a unit between zones via EventZone::transfer().
- All notice types being issued and handled.

