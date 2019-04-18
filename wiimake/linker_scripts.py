sizeLinkerScriptSection='''wiimake_section_{num} :
{{
    {section}
}}
_sizeof_section_{num}=SIZEOF(wiimake_section_{num});
'''

finalLinkerScriptSection='''
wiimake_section_{num} {address} : /* section size = {size} */
{{
    {section}
}}
'''

def getFinalLinkerScript(sections):
    script = "SECTIONS {\n"
    for i, sec in enumerate(sections):
        script += finalLinkerScriptSection.format(num=i+1, section=sec,
            address=hex(sec.address), size=hex(sec.size))
    script += "}\n"
    return script

def getSizeLinkerScript(sections):
    script = "SECTIONS {\n"
    for i, sec in enumerate(sections):
        script += sizeLinkerScriptSection.format(num=i+1, section=sec)
    script += "}\n"
    return script
