"""
File containing ShoppingList class

This class handles the shoppinglist data and allows
for adding recipes to the shopping list.
"""
from helper import combine_units, standardize_units, Ingredient, convert_str_to_float


class ShoppingList:
    """ShoppingList class. Contains shopping_list data."""

    def __init__(self):
        """Creates an empty shopping_list dictionary on creation"""
        self.shopping_list = {}

    def __str__(self):
        """Printable class. Prints the items in the shopping list"""
        string = "Items:\n"
        for item in self.shopping_list:
            elem = self.shopping_list[item]
            new_units = combine_units(elem.quantity, elem.unit)
            string = string + f"{item}: {new_units}\n"
        return string

    @property
    def items(self):
        """Returns the internal shopping list"""
        return self.shopping_list

    def add_recipe(self, recipe):
        """Add a recipe to the shopping list"""
        self.fill_ingredients(recipe)

    def add_ingredient(self, item, quantity, unit):
        """Add ingredients to the shopping list"""
        converted = False
        if unit:
            (int_q, int_unit) = standardize_units(quantity, unit)
            converted = True
        elif quantity != "some":
            converted = True
            (int_q, _) = standardize_units(quantity, unit)
            int_unit = ''
        else:
            int_q = quantity
            int_unit = unit
        if item not in self.shopping_list:
            if converted:
                self.shopping_list[item] = Ingredient(int_q, int_unit)
            else:
                self.shopping_list[item] = Ingredient(0, int_unit)
        elif isinstance(int_q, float):
            current = self.shopping_list[item]
            self.shopping_list[item] = Ingredient(
                current.quantity + int_q, current.unit
            )

    def fill_ingredients(self, recipe):
        """Parse the recipe and add ingredients to shopping list"""
        for item in recipe["ingredients"]:
            if item["type"] == "ingredient":
                quantity = convert_str_to_float(item["quantity"])
                self.add_ingredient(item["name"], quantity, item["units"])
