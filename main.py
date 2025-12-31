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
        subprocess.run(args = ["cmake", "--build", "./build/"])
        if int(choice) >= len(self.tests):
            subprocess.run(args= ["ctest", "-V"], cwd="./build/", text=True)
        else: 
            subprocess.run(args= ["ctest", "-V", "-R", self.tests[int(choice)]], cwd="./build/", text=True)


class Exporter:
    def __init__(self):
        self.path: str = ""
        if not os.path.exists("./export-config.json"):
            print("export-config.json not found, creating...")
            with open(file="./export-config.json", mode="w") as file:
                json.dump({"target":""},fp=file)
                print("please enter the path in the created export-config.json")
    def export(self) -> None:
        with open(file="./export-config.json",mode="r") as file:
            try:
                self.path = json.load(file)["target"]
                # If path not found, early return
                if len(self.path) == 0:
                    print("Invalid Path: Empty Path.")
                if not os.path.exists(self.path):
                    print("Invalid Path: \033[4m"+self.path+"\033[0m")
                    return
                print("Selected Path: \033[4m"+self.path+"\033[0m")
                print("Exporting...")
                shutil.copytree(src="./include/smath/",dst=self.path+"/smath/", dirs_exist_ok=True)
                print("Export Finished.")
            except KeyError:
                print("Invalid Config: Missing Field \"target\".")
def main():
    choice:str = ""
    choice = input("0. Debug\n1. Export\nSelection (0): ")
    if (choice==""):
        choice = "0"
    match choice:
        case "0":
            try:
                print("\033[1m\tClean & Test\033[0m")
                Debugger().debug()

            except Exception as e:
                print(e)
        case "1":
            print("\033[1m\tExport\033[0m")
            Exporter().export()
            pass
        case _:
            raise RuntimeError("Invalid Input")
try:
    main()
except KeyboardInterrupt as e:
    print("Exit")
