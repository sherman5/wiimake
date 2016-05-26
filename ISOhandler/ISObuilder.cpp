#include "ISOhandler.hpp"

void CreateMainBranch(ISOhandler& iso_file) {

  //branch (b) to 801910E0 (custom code runs right before the instruction at 80377998)
  iso_file.IsoWrite(0x80377998, 0x4BE19748);

  iso_file.IsoWrite(0x801910E0, 0x4800000D); //branch (bl) to 801910EC (start of main)
  iso_file.IsoWrite(0x801910E4, 0x7EE3BB78); //line of code at 80377998
  iso_file.IsoWrite(0x801910E8, 0x481E68B4); //branch (b) to 8037799C

  iso_file.IsoWrite(0x801910EC, 0x9421FFF0); //branch (b) to 8037799C
  iso_file.IsoWrite(0x801910F0, 0x93E1000C); //branch (b) to 8037799C
  iso_file.IsoWrite(0x801910F4, 0x7C3F0B78); //branch (b) to 8037799C
  iso_file.IsoWrite(0x801910F8, 0x3D208000); //branch (b) to 8037799C
  iso_file.IsoWrite(0x801910FC, 0x61291800); //branch (b) to 8037799C
  iso_file.IsoWrite(0x80191100, 0x3D400180); //branch (b) to 8037799C
  iso_file.IsoWrite(0x80191104, 0xC00A0404); //branch (b) to 8037799C
  iso_file.IsoWrite(0x80191108, 0xD0090000); //branch (b) to 8037799C
  iso_file.IsoWrite(0x8019110C, 0x397F0010); //branch (b) to 8037799C
  iso_file.IsoWrite(0x80191110, 0x83EBFFFC); //branch (b) to 8037799C
  iso_file.IsoWrite(0x80191114, 0x4E800020); //branch (b) to 8037799C

}

int main(int argc, char** /*argv[]*/) {

  ISOhandler iso("CustomMelee");

  iso.CleanISO();

  if (argc < 2) { 

    CreateMainBranch(iso);
//    PrepCode();
//    InjectCode();

  }

}
