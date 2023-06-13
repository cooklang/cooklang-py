"""
File containing RecipeTree class.

This class contains the parsed file structure with directories
and .cook-files.
"""

import os
from helper import RecipeFile


class RecipeTree:
    """
    RecipeTree parses the working directory and creates an
    internal structure with all
    the .cook files and their paths"""

    def __init__(self, path):
        """The tree structure is setup on creation of class item"""
        self._tree = {}
        if not os.path.exists(path + os.sep + "recipes"):
            print("Can not find recipes folder")
            return

        root = os.walk(path + os.sep + "recipes")
        for node in root:
            if node[1]:
                # These are directories
                for directory in node[1]:
                    if directory:
                        self.add_directory(directory)
            if node[2]:
                # These are files
                for file_name in node[2]:
                    # Find what directory we are in
                    if (folder := os.path.basename(node[0])) == "recipes":
                        self.add_file_to_dir(node[0], "Undefined", file_name)
                    else:
                        self.add_file_to_dir(node[0], folder, file_name)

    @property
    def tree(self):
        """Return the internal _tree object"""
        return self._tree

    def __str__(self):
        """
        Create a printable version of this class for terminal applications.
        """
        string = "Recipes:\n"
        for folder in self.tree.items():
            if folder != "Undefined":
                string += f"  {folder}\n"
                for node in self.tree[folder]:
                    string += f"  ├── {node.file}\n"
        index = string.rfind("├")
        string = string[:index] + "└" + string[index + 1 :]
        for folder in self.tree:
            if folder == "Undefined":
                for node in self.tree[folder]:
                    string = string + f"- {node.file}\n"

        return string

    def add_directory(self, folder):
        """Add a directory to the internal structure"""
        if folder:
            if folder not in self.tree:
                self._tree[folder] = []

    def add_file_to_dir(self, path, folder, file_name):
        """Update internal dictionary with folder and RecipeFile(file, path)"""
        if file_name.endswith("cook"):
            if folder not in self.tree:
                self._tree[folder] = [RecipeFile(file_name, path + os.sep + file_name)]
            elif not self.tree[folder]:
                self._tree[folder] = [RecipeFile(file_name, path + os.sep + file_name)]
            else:
                self._tree[folder].append(
                    RecipeFile(file_name, path + os.sep + file_name)
                )
