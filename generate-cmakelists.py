import os
import re

extensions = ['.cpp', '.hpp']

def generate(startPath, target):
	for (dirpath, dirnames, filenames) in os.walk(startPath):

		# Add source files (.hpp and .cpp)
		cmake = "target_sources(\n\t${" + target +"}\nPRIVATE\n"

		for n in filenames:
			ext = os.path.splitext(n)[-1]
			if ext in extensions:
				cmake += "\t" + n + "\n"

		cmake += ")\n"

		# Add directiories 
		if len(dirnames) > 0:
			cmake += "\n"

		for n in dirnames:
			cmake += "add_subdirectory(" + n + ")\n"

		# Write file
		cmakepath = re.sub('\\\\', '/', dirpath) + "/CMakeLists.txt"
		file = open(cmakepath, "w")
		file.write(cmake)
		file.close()

# This means we only do these, if we run this directly from python aka as main program
if __name__ == '__main__':
	path = os.getcwd()
	generate (path + "/src", "TARGET_NAME")
# 	generate (path + "/Engine", "SHOPHORN_TARGET_ENGINE")
# 	generate (path + "/Game", "SHOPHORN_TARGET_GAME")