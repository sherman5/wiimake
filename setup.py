from setuptools import setup

setup(
    name='wiimake',
    version='0.0.1',
    entry_points={
        'console_scripts': [
            'wiimake=wiimake.wiimake:main',
            'wiimake-isotool=wiimake.wiimake_isotool:main'
        ]
    }
)
