# Script written by Christian Möllinger <ch.moellinger@gmail.com>, 02/2015
#
# This script finds all *.CR2 files in a given directory (or, in the working directory, if no directory is given)
# which have no corresponding *.JPEG/*.JPG file. The idea is that the user which get both *.CR2 and *.JPEG files 
# from his DSLR, can remove bad or not usable pictures using only the (fast processable) JPEG files, while this script 
# synchronizes his mucking result to the *.CR2 files.

import os
import sys

JPEG_EXTENSIONS = [".jpeg", ".JPEG", ".jpg", ".JPG"]
CR2_EXTENSIONS = [".CR2", ".cr2"]
TARGET_SUBDIRECTORY = "_to_delete"

def print_help():
    """
    Prints the help.
    """
    print ("This script moves *.CR2 files which do not have a corresponding *.JPEG file to a subdirectory.")
    print ("Usage: python RawCleaner.py [directory]")
    print ("       (if no directory was given, the current working directory is scanned)")

def find_uncovered_cr2_files(working_directory):
    """
    Returns a list of uncovered *.CR2 files for the given directory.
    """
    print ("Search for files in " + str(working_directory))

    uncovered_cr2_files = []
    files_in_dir = os.listdir(working_directory)
    for file in files_in_dir:
        splitted_filename = os.path.splitext(file)
        is_cr2 = splitted_filename[1] in CR2_EXTENSIONS

        if is_cr2:
            basename = splitted_filename[0]
            corresponding_jpeg_exists = bool([True for extension in JPEG_EXTENSIONS if os.path.isfile(working_directory + os.sep + basename + extension)])

            if not corresponding_jpeg_exists:
                uncovered_cr2_files.append(file)

    return uncovered_cr2_files

def move_files_to_subdirectory(file_list, working_directory, subdirectory):
    """
    Moves the given files from the working directory to "$working_directory/$subdirectory".
    """
    # create subdirectory, if it does not exist
    if not os.path.exists(working_directory + os.sep + subdirectory):
        os.makedirs(working_directory + os.sep + subdirectory)

    # move files
    for file in file_list:
        os.rename(working_directory + os.sep + file, working_directory + os.sep + subdirectory + os.sep + file)

def main():
    """
    Main entry point.
    """
    if len(sys.argv) == 2:
        # Use given argument as directory
        working_dir = sys.argv[1]
    elif len(sys.argv) == 1:
        # Use current working directory if no other directory was given as argument
        working_dir = os.getcwd()
    else:
        # Wrong script usage
        print_help()
        sys.exit(1);

    uncovered_cr2_files = find_uncovered_cr2_files(working_dir)
    if uncovered_cr2_files:
        print ("\nThe following files were found without having a corresponding JPEG file:")
        for file in uncovered_cr2_files:
            print (file)
        print ("")
        print ("Should these files be moved into the subdirectory \"" + TARGET_SUBDIRECTORY + "\"? (y|N)")

        user_response = input()
        if user_response in ("y", "Y", "yes"):
            move_files_to_subdirectory(uncovered_cr2_files, working_dir, TARGET_SUBDIRECTORY)
        else:
            print("you canceled")
    else:
        print ("\n No CR2 files without a corresponding JPEG file were found.")

if __name__ == "__main__":
    main()
