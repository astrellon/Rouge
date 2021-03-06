# Python script for loosly parsing Lua wrapper documentation straight from the .cpp file
# Usage is to pass in a file containing a list of .cpp files to parse with
# the Lua class name.
# python luaDocs.py classList.txt
#
# Class List text file format.
# Each line should contain the class name and then a path to the .cpp file.
# eg:
# Character ../lua/wrappers/game/lua_character.cpp
# 
# This can be simplifed somewhat if the __basePath is set.
# This path will then be used to create the other paths that follow it.
# Multiple __basePath's can be set and all paths will be affect immediately following.
# eg:
# __basePath ../lua/wrappers/game/
# Character lua_character.cpp
# BodyPart lua_body_part.cpp
# __basePath ../lua/wrappers/gfx/
# Sprite lua_sprite.cpp
#
# Single line comments (//) can also be used.

import luaDocs;
import htmlOutput;
import sys;
import os;
import shutil;

usage = "Usage: " + sys.argv[0] + " filename";
usage += "\n\nA script for parsing Lua documentation from the function comments in the \nC++ wrapper code.";
usage += "\nBy default the filename argument refers to a txt file which contains a list of ";
usage += "\nclass names and file paths to parse.";
usage += "\n\nThe txt file format parsed per line and is as such:\n";
usage += "<className> <path_to_cpp>"
usage += "\n\nIf __basePath is used as the className, then the associated path is"
usage += "\nis appended to all the paths that come after that line. "
usage += "\n__basePath can be set multiple times to set different base paths."
if len(sys.argv) == 1:
    print(usage);
else:
    docs = luaDocs.Documentation();
    docs.parseList(sys.argv[1]);
    
    try:
        shutil.rmtree("build");
    except OSError as exc:
        print("");

    try:
        os.makedirs("build", 0o777);
    except:
        pass
    
    for classDocName, classDoc in docs.classes.items():
        output = htmlOutput.HtmlOutput();
        output.load(classDoc);
        output.write("build/" + classDocName + ".html");

    outputIndex = htmlOutput.HtmlOutput();
    outputIndex.createIndex(docs.classes, "Index File", "Index file for all the available classes with documentation for Rouge.");
    outputIndex.write("build/index.html");


    try:
        shutil.copytree("css", "build/css");
    except OSError as exc:
        print("Unable to copy css to build.");

    try:
        shutil.copytree("images", "build/images");
    except OSError as exc:
        print("Unable to copy css to build.");

        
    if len(luaDocs.warnings) > 0:
        print("Warnings:");
        print("- " + luaDocs.arrayJoin(luaDocs.warnings, "\n- "));
        
