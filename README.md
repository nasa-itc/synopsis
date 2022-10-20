SYNOPSIS: Science Yield improvemeNt via Onboard Prioritization and Summary of Information System
================================================================================================

SYNOPSIS is a system to prioritize the downlink of data products, including
both raw data products as well as "autonomous science data products" that are
extracted onboard the spacecraft to summarize data.

## Team
 - Mark Wronkiewicz
 - Jack Lightholder
 - Lukas Mandrake
 - Gary Doran

## Setup

1. Run `cmake -S . -B build` to set up the build directory
2. Run `cmake --build build` to build the code
3. Run `cd build && ctest` to execute the tests

## Testing

To test from scratch, run the following commands:

1. `ssh itlinux-b` (or equivalent machine)
2. `git clone git@github.jpl.nasa.gov:SYNOPSIS/synopsis.git`
3. `cd synopsis`
4. Proceed with the Setup steps above

## Acknowledgements

Funding and support for this work was provided by the Multi-mission Ground
Systems and Services (MGSS) program office.
