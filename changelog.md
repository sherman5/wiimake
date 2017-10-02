# Change Log
This documents all differences between versions of the program

## [1.5.0] - ??

### Added

- direct branches
- linker symbols
- allow for only having direct branches
- build checksum

### changed

## [1.3.5] - 3/15/17

### Added

- Travis CI

### Changed

- fixed bug seen with some compilers

## [1.3.4] - 3/9/17

### Changed

- endian agnostic
- checksum algorithm updated
- save/load times improved (NEW FILES NOT COMPATIBLE WITH OLDER VERSIONS)
- verify iso file before any operations done by isotool

## [1.3.3] - 3/2/17

### Changed

- fixed command line options bug

## [1.3.2] - 3/1/17

### Changed

- updated description and usage

## [1.3.1] - 2/26/17
### Added

- display code size info even on error

## [1.3.0] - 2/22/17
### Added

- checksum calculation in wiimake-isotool
- keep only core files with --save-temps
- display percent of memory used

## [1.2.0] - 2/19/17
### Added

- output code size
- use generic function for saving/loading registers

## [1.1.3] - 2/10/17
### Added

- memory regions given in config file are aligned to
    multiples of 0x4
- memory regions are zereod out before code is injected

## [1.1.2] - 2/5/17

### Changed

- Interpret addresses below start of code (in RAM) as pure
    offsets in the iso file

## [1.1.1] - 2/2/17

### Changed

- Fixed bug with section mis-alignment

## [1.1.0] - 2/2/17
### Added

- User can supply multiple 'fixed symbols'
- Increased register preservation before branching to fixed symbol

### Changed

- Old system of single ENTRY point has been removed 


