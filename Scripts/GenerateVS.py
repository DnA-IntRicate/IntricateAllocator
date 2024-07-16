import os
import subprocess


def Generate():
    subprocess.run(["Vendor/premake/premake5.exe", "vs2022"])

def Delete():
    if os.path.isfile("IntricateAllocator.sln"):
        os.remove("IntricateAllocator.sln")
        print("Deleted: IntricateAllocator.sln")

    if os.path.isfile("intricate_allocator.vcxproj"):
        os.remove("intricate_allocator.vcxproj")
        print("Deleted: intricate_allocator.vcxproj")

    if os.path.isfile("intricate_allocator.vcxproj.filters"):
        os.remove("intricate_allocator.vcxproj.filters")
        print("Deleted: intricate_allocator.vcxproj.filters")

    if os.path.isfile("intricate_allocator.vcxproj.user"):
        os.remove("intricate_allocator.vcxproj.user")
        print("Deleted: intricate_allocator.vcxproj.user")

if __name__ == "__main__":
    os.chdir("../")
    Delete()
    print("\n")
    Generate()

    input("Press Enter to continue...")
