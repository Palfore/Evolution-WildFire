import os

EXE = R'C:\Windows\System32\cmd.exe'
CODEBLOCKS = R'"C:\Program Files (x86)\CodeBlocks\codeblocks.exe"'

def replace_eye_size(size):
	text = []
	with open(PARAMETER_FILE, 'r') as f:
		for line in f:
			if line.startswith("static int horizontalEyeSize = ") and line.endswith(';\n'):
				line = f"static int horizontalEyeSize = {size};\n"
			text.append(line)
	with open(PARAMETER_FILE, 'w') as f:
		f.writelines(text)

if __name__ == '__main__':
	PROJECT_PATH = R"C:\Users\Nawar\Documents\GitHub\Evolution-Project"
	BUILD_FILE = os.path.join(PROJECT_PATH, "Evolution-Project.cbp")
	BUILD_COMMAND = f'{CODEBLOCKS} --no-splash-screen --rebuild {BUILD_FILE} --target="Debug"'
	PARAMETER_FILE = os.path.join(PROJECT_PATH, R"src\Simulation\Creatures\Types\EyeWalker.cpp")
	EVOLUTION_EXECUTABLE = os.path.join(PROJECT_PATH, R"bin\Debug\Evolution-Project.exe")

	for i in range(3, 15):
		replace_eye_size(i)
		if i != 3:
			print("Finished")
		print(i, end=' ')
		print("Building", end=' ')
		os.system(f'{EXE} /C "{BUILD_COMMAND}"')
		print("Executing", end=' ')
		os.system(EVOLUTION_EXECUTABLE)

