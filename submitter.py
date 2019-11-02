import os

EXE = R'C:\Windows\System32\cmd.exe'
CODEBLOCKS = R'"C:\Program Files (x86)\CodeBlocks\codeblocks.exe"'



PROJECT_PATH = R"C:\Users\Nawar\Documents\GitHub\Evolution-Project\Evolution-Project.cbp"
BUILD_COMMAND = f'{CODEBLOCKS} --no-splash-screen --rebuild {PROJECT_PATH} --target="Debug"'

print(BUILD_COMMAND)
os.system(f'{EXE} /C "{BUILD_COMMAND}"')