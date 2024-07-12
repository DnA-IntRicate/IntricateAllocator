import os
from SetupPython import PythonConfiguration as PyReqs
import GenerateVS as VS


os.chdir("../")

print("Validating python...")
PyReqs.Validate()

print("\nGenerating Visual Studio projects...")
VS.Generate()

print("\nSetup complete!")
input("Press Enter to continue...")
