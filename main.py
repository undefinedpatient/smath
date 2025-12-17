import subprocess
import os
import shutil


def main():

    choice:str = input("0. Debug\nSelection (0): ")
    if (choice==""):
        choice = "0"
    match choice:
        case "0":
            try:
                print("0. Clean & Test")
                # Cleaning
                shutil.rmtree("./build/")
                os.mkdir("./build/")
                # "-B" specify the output directory
                subprocess.run(args = ["cmake", "-B", "./build/"])
                subprocess.run(args = ["cmake", "--build", "./build/", "--config", "Debug"])
                subprocess.run(args= ["ctest", "-V"], cwd="./build/", text=True)
            except Exception as e:
                print(e)
        case _:
            raise RuntimeError("Invalid Input")

main()
