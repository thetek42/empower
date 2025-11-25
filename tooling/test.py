import glob
import os
import subprocess
import sys

from targets import *

# collect arguments
targets_to_run = []
stdc_versions_to_run = []
for arg in sys.argv[1:]:
    if arg in TARGETS.keys():
        targets_to_run.append(arg)
    elif arg in STDC_VERSIONS.values():
        for i in STDC_VERSIONS.keys():
            if STDC_VERSIONS[i] == arg:
                stdc_versions_to_run.append(i)
                break
    else:
        print("Invalid arugment")
        exit(1)
if len(targets_to_run) == 0:
    targets_to_run = TARGETS.keys()
if len(stdc_versions_to_run) == 0:
    stdc_versions_to_run = STDC_VERSIONS.keys()

# create build dir and set the cwd to it
# changing the cwd is required because msvc is a stupid piece of shit that
# gobbles the current working directory with its garbage
os.makedirs("build", exist_ok=True)
os.chdir("build")

# collect test files
files = []
for file in glob.iglob("*.c", root_dir="../tests"):
    files.append("../tests/" + file)

# do tests for all targets and c standard versions
for target_name in targets_to_run:
    for stdc in stdc_versions_to_run:
        target = TARGETS[target_name]
        stdc_name = STDC_VERSIONS[stdc]
        output = target_name + "-" + stdc_name + target["outsuffix"]

        # skip unsupported c standard versions
        stdcflag = target["stdcflags"][stdc]
        if stdcflag is None:
            continue

        test_display_name = target_name + " (" + stdc_name + ")"
        print("\x1b[1m", test_display_name, "\x1b[0m", sep="")
        print("\x1b[2mCompiling...\x1b[0m ", end="", flush=True)

        # compile test binary
        command = []
        command.append(target["compiler"])
        if stdcflag != "":
            command.append(stdcflag)
        command.extend(target["flags"])
        command.extend(files)
        for outflag in target["outflag"]:
            command.append(outflag.replace("$FILE$", output))
        command.extend(target["linkflags"])
        command_output = subprocess.run(command, capture_output=True)

        # check if compilation was successful
        is_ok = command_output.returncode == 0 and len(command_output.stderr) == 0
        if is_ok:
            print("\x1b[32mOK\x1b[0m")
        elif command_output.returncode == 0:
            print("\x1b[33mWARN\x1b[0m")
        else:
            print("\x1b[31mERROR\x1b[0m")
        if not is_ok:
            for line in command_output.stdout.splitlines():
                print("   ", line.decode())
        for line in command_output.stderr.splitlines():
            print("   ", line.decode())
        if command_output.returncode != 0:
            print()
            continue

        # run test binary
        print("\x1b[2mRunning...\x1b[0m   ", end="", flush=True)
        command_output = subprocess.run(["./" + output], capture_output=True)

        # check if test run was successful
        if command_output.returncode == 0:
            print("\x1b[32mOK\x1b[0m\n")
        else:
            print("\x1b[31mERROR\x1b[0m")
            for line in command_output.stderr.splitlines():
                if line.startswith(b"\x1b[32mDONE\x1b[0m"):
                    break
                print("   ", line.decode())
            print()
