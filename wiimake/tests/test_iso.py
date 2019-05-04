import os
from shutil import copyfile
from wiimake.iso import Iso

DIR_PATH = os.path.dirname(os.path.realpath(__file__))
ISO_PATH = DIR_PATH + '/test.iso'
TEMP_ISO_PATH = DIR_PATH + '/temp.iso'
TEMP_SAVE_PATH = DIR_PATH + '/temp_save.out'

def testIsoInit():
    iso = Iso(ISO_PATH)
    assert iso.file == ISO_PATH
    assert iso.dolTable.tablePos == 0x1e800
    assert iso.dolTable.bssAddress == 0x804316c0
    assert iso.dolTable.bssSize == 0xa6309
    assert iso.dolTable.entryPoint == 0x8000522c
    assert len(iso.dolTable.unsortedSections) == 18
    assert len(iso.dolTable.sections) == 18
    assert iso.dolTable.codeStart == 0x80003100
    assert iso.dolTable.codeEnd == 0x804dec00

def testIsoRead():
    iso = Iso(ISO_PATH)
    assert iso.read(0x420) == 0x1e800
    assert iso.read(0x80377998) == 0x7ee3bb78
    assert iso.read(0x801b15cc) == 0x38800004
    assert iso.read(0x801a633c) == 0x7c7f1b78

def testIsoMd5():
    iso = Iso(ISO_PATH)
    assert iso.md5() == '02f92dfa9ae67586a725579b0b360afd'

def testIsoWrite():
    copyfile(ISO_PATH, TEMP_ISO_PATH)
    iso = Iso(TEMP_ISO_PATH)
    assert iso.md5() == '02f92dfa9ae67586a725579b0b360afd'
    code = {}
    code[0x80004000] = 0xDEADBEEF
    code[0x80005000] = 0xBAADF00D
    iso.bulkWrite(code)
    assert iso.read(0x80004000) == 0xDEADBEEF
    assert iso.read(0x80005000) == 0xBAADF00D
    os.remove(TEMP_ISO_PATH)

def testIsoSaveLoad():
    # modify iso and create save file
    copyfile(ISO_PATH, TEMP_ISO_PATH)
    iso1 = Iso(TEMP_ISO_PATH)
    assert iso1.md5() == '02f92dfa9ae67586a725579b0b360afd'
    code = {}
    code[0x80004000] = 0xDEADBEEF
    code[0x80005000] = 0xBAADF00D
    iso1.bulkWrite(code)
    assert iso1.md5() == 'c79342c1497b817337c39ea8cf6952c6'
    iso1.save(TEMP_SAVE_PATH)
    os.remove(TEMP_ISO_PATH)

    # load save file to a fresh iso
    copyfile(ISO_PATH, TEMP_ISO_PATH)
    iso2 = Iso(TEMP_ISO_PATH)
    assert iso2.md5() == '02f92dfa9ae67586a725579b0b360afd'
    iso2.load(TEMP_SAVE_PATH)
    assert iso2.md5() == 'c79342c1497b817337c39ea8cf6952c6'
    os.remove(TEMP_ISO_PATH)
    os.remove(TEMP_SAVE_PATH)
