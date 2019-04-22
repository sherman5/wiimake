SIZE_LINKER_SCRIPT_SECTION = '''
wiimake_section_{num} :
{{
    {section}
}}
_sizeof_section_{num}=SIZEOF(wiimake_section_{num});
'''

FINAL_LINKER_SCRIPT_SECTION = '''
wiimake_section_{num} {address} : /* section size = {size} */
{{
    {section}
}}
'''

def getFinalLinkerScript(sections):
    script = "SECTIONS {\n"
    for i, sec in enumerate(sections):
        script += FINAL_LINKER_SCRIPT_SECTION.format(num=i+1, section=sec,
                                                     address=hex(sec.address),
                                                     size=hex(sec.size))
    script += "}\n"
    return script

def getSizeLinkerScript(sections):
    script = "SECTIONS {\n"
    for i, sec in enumerate(sections):
        script += SIZE_LINKER_SCRIPT_SECTION.format(num=i+1, section=sec)
    script += "}\n"
    return script
