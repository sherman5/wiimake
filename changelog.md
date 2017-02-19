# Change Log
This documents all differences between versions of the program

## [1.2.0] - 2/19/17
### Added

- output code size
- use generic function for saving/loading registers

### Changed

## [1.1.3] - 2/10/17
### Added

- memory regions given in config file are aligned to
    multiples of 0x4
- memory regions are zereod out before code is injected

### Changed

## [1.1.2] - 2/5/17
### Added

### Changed

- Interpret addresses below start of code (in RAM) as pure
    offsets in the iso file

## [1.1.1] - 2/2/17
### Added

### Changed

- Fixed bug with section mis-alignment

## [1.1.0] - 2/2/17
### Added

- User can supply multiple 'fixed symbols'
- Increased register preservation before branching to fixed symbol

### Changed

- Old system of single ENTRY point has been removed 


