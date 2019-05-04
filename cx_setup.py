import sys
import re
from cx_Freeze import setup, Executable

version = re.search(
    '^__version__\s*=\s*\'(.*)\'',
    open('wiimake/_version.py').read(),
    re.M
    ).group(1)

with open("README.rst", "rb") as f:
    long_descr = f.read().decode("utf-8")

setup(
    name='wiimake',
    version=version,
    packages=['wiimake'],
    description='Command line tool for building Gamecube/Wii mods',
    long_description=long_descr,
    author='Tom Sherman',
    author_email='tomsherman159@gmail.com',
    options={'build_exe': {
        'packages': ['os', 'sys', 'argparse','pathlib', 'hashlib', 'bisect',
                     'struct', 'copy', 'subprocess'],
        'include_files': [],
        'include_msvcr': False,
    }},
    executables=[Executable('wiimake/wiimake.py', base="Console")]
)