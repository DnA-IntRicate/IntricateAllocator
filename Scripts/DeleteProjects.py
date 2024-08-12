import os

def DeleteFile(path: str):
    if os.path.isfile(path):
        os.remove(path)
        print(f"Deleted: {path}")

def DeleteTests():
    DeleteFile("Tests/Test-Allocation/Test-Allocation.vcxproj")
    DeleteFile("Tests/Test-Allocation/Test-Allocation.vcxproj.filters")
    DeleteFile("Tests/Test-Allocation/Test-Allocation.vcxproj.user")

def Delete():
    DeleteFile("IntricateAllocator.sln")

    DeleteFile("IntricateAllocator/IntricateAllocator.vcxproj")
    DeleteFile("IntricateAllocator/IntricateAllocator.vcxproj.filters")
    DeleteFile("IntricateAllocator/IntricateAllocator.vcxproj.user")

    DeleteTests()

if __name__ == "__main__":
    os.chdir("../")
    print("Deleting Visual Studio projects...")
    Delete()
    input("Press Enter to continue...")
