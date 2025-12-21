import subprocess
import os
import shutil
import json

class Debugger:
    def __init__(self):
        # "abc_test.cpp" => "abc_test_unit"
        temp: list[str] = list(filter(lambda path: ".cpp" in path,os.listdir("./test/")))
        self.tests: list[str] = list(map(lambda path: str(path)[0:len(path)-4]+"_unit" ,temp))
    def debug(self) -> None:
        for c in zip(range(len(self.tests)),self.tests,strict=True):
            print(c[0],": ",c[1])
        print(len(self.tests),": ", "All")
        choice:str = input("Selection (0): ")
        if choice == "":
            choice = "0"
        # Cleaning
        shutil.rmtree("./build/")
        os.mkdir("./build/")
        # "-B" specify the output directory
        subprocess.run(args = ["cmake", "-B", "./build/"])
        subprocess.run(args = ["cmake", "--build", "./build/", "--config", "Debug"])
        if int(choice) >= len(self.tests):
            subprocess.run(args= ["ctest", "-V"], cwd="./build/", text=True)
        else: 
            subprocess.run(args= ["ctest", "-V", "-R", self.tests[int(choice)]], cwd="./build/", text=True)


# NOT IMPLEMENTED
class Exporter:
    def __init__(self):
        if not os.path.exists("./export-config.json"):
            print("export-config.json not found, creating...")
            self.path = input("desired output path")
            pass

        with open(file="./export-config.json",mode="r") as file:
            self.path = json.load(file)["target_dir"]
    def export(self) -> None:
        pass
def main():
    choice:str = ""
    print("0. Debug\n1. Export\nSelection (0): ")
    choice = input()
    if (choice==""):
        choice = "0"
    match choice:
        case "0":
            try:
                print("\033[1mClean & Test\033[0m")
                Debugger().debug()

            except Exception as e:
                print(e)
        case "1":
            pass
        case _:
            raise RuntimeError("Invalid Input")
try:
    main()
except KeyboardInterrupt as e:
    print("Exit")
