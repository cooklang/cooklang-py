"""
DataWrapper class which creates a recipe tree and shopping list.
"""
from recipe_tree import RecipeTree
from shoppinglist import ShoppingList


class DataWrapper:
    """DataWrapper class. Contains recipe_tree, shopping_list and"""

    def __init__(self, path):
        """Create a recipe tree and shopping list at the start."""
        self.recipe_tree = RecipeTree(path)
        self.shopping_list = ShoppingList()
        self.recipes_in_list = []

    def __str__(self):
        """This doesn't really do anything"""
        string = ""
        if self.recipe_tree:
            string += "Recipe tree is loaded\n"
        if self.shopping_list:
            string += "Shopping list is loaded\n"
        index = string.rfind("\n")
        output = string[:index]
        return output
