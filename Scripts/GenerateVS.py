import os
import subprocess


def Generate():
    subprocess.run(["Vendor/premake/premake5.exe", "vs2022"])


def Delete():
    if os.path.isfile("MangoSoftware.sln"):
        os.remove("MangoSoftware.sln")
        print("Deleted: MangoSoftware.sln")

    if os.path.isfile("MangoClient/MangoClient.vcxproj"):
        os.remove("MangoClient/MangoClient.vcxproj")
        print("Deleted: MangoClient/MangoClient.vcxproj")

    if os.path.isfile("MangoClient/MangoClient.vcxproj.filters"):
        os.remove("MangoClient/MangoClient.vcxproj.filters")
        print("Deleted: MangoClient/MangoClient.vcxproj.filters")

    if os.path.isfile("MangoClient/MangoClient.vcxproj.user"):
        os.remove("MangoClient/MangoClient.vcxproj.user")
        print("Deleted: MangoClient/MangoClient.vcxproj.user")

    if os.path.isfile("Mango/Mango.csproj"):
        os.remove("Mango/Mango.csproj")
        print("Deleted: Mango/Mango.csproj")

    if os.path.isfile("Mango/Mango.csproj.filters"):
        os.remove("Mango/Mango.csproj.filters")
        print("Deleted: Mango/Mango.csproj.filters")

    if os.path.isfile("Mango/Mango.csproj.user"):
        os.remove("Mango/Mango.csproj.user")
        print("Deleted: Mango/Mango.csproj.user")

    if os.path.isfile("Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj"):
        os.remove("Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj")
        print("Deleted: Vendor/IntricateEngine/IntricateEngine-Vulkan/IntricateEngine-Vulkan.vcxproj")

    if os.path.isfile("Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj.filters"):
        os.remove("Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj.filters")
        print("Deleted: Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj.filters")

    if os.path.isfile("Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj.user"):
        os.remove("Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj.user")
        print("Deleted: Vendor/IntricateEngine/IntricateEngine/IntricateEngine.vcxproj.user")

    if os.path.isfile("Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj"):
        os.remove("Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj")
        print("Deleted: Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj")

    if os.path.isfile("Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj.filters"):
        os.remove("Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj.filters")
        print("Deleted: Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj.filters")

    if os.path.isfile("Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj.user"):
        os.remove("Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj.user")
        print("Deleted: Vendor/IntricateEngine/IntricateEngine.CSharp/IntricateEngine.CSharp.csproj.user")


if __name__ == "__main__":
    os.chdir("../")
    Delete()
    print("\n")
    Generate()

    input("Press Enter to continue...")
