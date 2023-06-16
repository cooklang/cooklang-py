#!/usr/bin/env python
"""
Main entry point for the CookCLI python implementation.

The terminal version includes a few commands.
recipe read <file>  - Prints <file>
seed                - Prints file tree
server <IP>         - Starts the CookCLI webpagei. IP is optional.
"""
import sys
import os

from cooklang import parseRecipe

from recipe import Recipe
from shoppinglist import ShoppingList
from recipe_tree import RecipeTree
import flask_app


def print_help():
    """Print usage"""
    print("Usage: mainfile.py [seed|recipe|shopping-list|server] <options>")
    print("seed: <root-path to .cook files>")
    print("recipe: read, ...")
    print("shopping-list: <file1.cook> <file2.cook> ...")
    print("server does not need any arguments")
    print()


if __name__ == "__main__":
    CMD = sys.argv[1]
    EARLY_EXIT = False
    if CMD not in ["seed", "recipe", "shopping-list", "server"]:
        print(f'Command "{CMD}" not found.')
        EARLY_EXIT = True
    if len(sys.argv) < 3 and CMD != "server" and CMD != "seed":
        print("Too few arguments")
        EARLY_EXIT = True
    if EARLY_EXIT:
        print()
        print_help()
        sys.exit(-1)

    # Path to this directory
    path = os.getcwd()

    match CMD:
        case "seed":
            tree = RecipeTree(path)
            print(tree)
        case "recipe":
            if len(sys.argv) < 4:
                print(f"Wrong number of arguments for {CMD}")
                print_help()
                sys.exit(-1)
            sub_cmd = sys.argv[2]

            recipe = Recipe(sys.argv[3])

            match sub_cmd:
                case "read":
                    # Handle read
                    # Using % to define units in metadata is broken.
                    # So don't do that!
                    print(recipe)
                case _:
                    print("Everything except read")
        case "shopping-list":
            recipe_paths = []
            IDX = 2
            while IDX < len(sys.argv):
                recipe_paths.append(sys.argv[IDX])
                IDX += 1
            print(f"Got {IDX-2} recipes!")
            ingredients = []
            shopping_list = ShoppingList()
            for path in recipe_paths:
                recipe = parseRecipe(path)
                shopping_list.add_recipe(recipe)
            print(shopping_list)
        case "server":
            ip = "0.0.0.0"
            if len(sys.argv) > 2:
                ip = sys.argv[2]
            flask_app.main(ip=ip)
