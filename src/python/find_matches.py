#!python3
"""
This simple script finds which files are exact match.
"""
import sys
import os


def fill_md5(content, md5):
    for currline in content:
        currtokens = currline.split("= ")
        currmd5 = currtokens[1].strip()
        # Removes 'MD5 (' and last parenthesis.
        currfile = currtokens[0].strip()[5:-1]
        if currmd5 in md5:
            md5[currmd5].append(currfile)
        else:
            md5[currmd5] = [currfile]


def check_self_hashes(md5):
    for currmd5, filevector in md5.items():
        if len(filevector) != 1:
            print("Same files:")
            for currfile in filevector:
                print(currfile)


def check_hashes(md5_1, md5_2):
    for currmd5, filevector in md5_1.items():
        if currmd5 in md5_2:
            print("Same files:")
            for currfile in filevector:
                print(currfile, end='')
            print("")
            for currfile in md5_2[currmd5]:
                print(currfile, end='')
            print("")
            print("")


def main():
    global FILE_1_PATH, FILE_2_PATH
    if len(sys.argv) != 3:
        print("./" + sys.argv[0] + " LIST_1.txt LIST_2.txt")
        print("where LIST_1.txt and LIST_2.txt are generated by 'md5 *' command")
        exit()

    # This is where all the hashes will be stored.
    source_md5 = {}
    dest_md5 = {}
    source_content = []
    dest_content = []

    with open(sys.argv[1]) as source_hash_file:
        source_content = source_hash_file.readlines()

    with open(sys.argv[2]) as dest_hash_file:
        dest_content = dest_hash_file.readlines()

    # TODO Check whether both files were properly opened and read.

    fill_md5(source_content, source_md5)
    fill_md5(dest_content, dest_md5)

    print("Looking for duplicates just in file 1")
    check_self_hashes(source_md5)

    print("Looking for duplicates just in file 2")
    check_self_hashes(dest_md5)

    print("Looking for duplicates from file 1 in file 2")
    check_hashes(source_md5, dest_md5)


if __name__ == "__main__":
    main()
