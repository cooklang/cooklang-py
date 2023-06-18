"""
File containing helper functions.
"""
import math
from collections import namedtuple

Ingredient = namedtuple("Ingredient", "quantity unit")
RecipeFile = namedtuple("RecipeFile", "file path")


def convert_str_to_float(string):
    """Input a string (50.000) for instance. Return integer 50"""
    try:
        tmp_val = float(string)
        if (tmp_val % 1) == 0:
            # No decimal value. Can be converted to an integer.
            quantity = f"{int(tmp_val)}"
        else:
            # Float value with decimal value. Keep as it is.
            quantity = f"{tmp_val}"
    except ValueError:
        # This is probably a range that has been specified.
        quantity = string

    return quantity


def standardize_units(quantity, unit):
    """Input volume or weight. Return ml or g"""
    # This will return unit in ml or g

    try:
        # The input can be a string, which will give us a fun
        # behaviour. 2 dl will return the string '2'*100, i.e. 100 2's
        quantity = float(quantity)
    except ValueError:
        print("Failed to convert this. Set amount to 0")
        quantity = 0.0

    if unit in {"tbsp", "msk"}:
        output = (quantity * 15, "ml")
    elif unit in {"tsp", "tsk"}:
        output = (quantity * 5, "ml")
    elif unit in {"cups", "cup"}:
        output = (quantity * 235, "ml")
    elif unit == "dl":
        output = (quantity * 100, "ml")
    elif unit == "l":
        output = (quantity * 1000, "ml")
    elif unit == "pint":
        output = (quantity * 473, "ml")
    elif unit == "oz":
        output = (quantity * 29.5, "ml")
    elif unit in {"pinch", "pinches", "krm"}:
        output = (quantity, "ml")
    elif unit == "kg":
        output = (quantity * 1000, "g")
    elif unit == "hg":
        output = (quantity * 100, "g")
    elif unit == "g":
        output = (quantity, "g")
    else:
        print("Hm?")
        output = (0.0, "Unknown unit")

    return output


def combine_units(units, form):
    """
    Input g and ml,
    Return a string with smallest amount of measurements.

    Example input: 1250 ml.
    Output: 1l, 2dl, 3tbsp, 1tsp
    """
    string = ""
    if form == "g":
        # Combine to kg and g
        kilogram = math.floor(units / 1000)
        units = units - 1000 * kilogram
        gram = units
        if kilogram > 0:
            string = f"{kilogram}kg"
            if gram:
                string = string + ", "
        if gram > 0:
            string = string + f"{gram}g"
    elif form == "ml":
        # Combine to l, dl, tbsp, tsp
        liters = math.floor(units / 1000)
        units = units - 1000 * liters
        deciliter = math.floor(units / 100)
        units = units - 100 * deciliter
        tbsp = math.floor(units / 15)
        units = units - tbsp * 15
        tsp = math.floor(units / 5)
        units = units - tsp * 5
        pinch = units

        if liters > 0:
            string = f"{liters}l"
            if any([deciliter, tbsp, tsp, pinch]):
                string = string + ", "
        if deciliter > 0:
            string = string + f"{deciliter}dl"
            if any([tbsp, tsp, pinch]):
                string = string + ", "
        if tbsp > 0:
            string = string + f"{tbsp}tbsp"
            if any([tsp, pinch]):
                string = string + ", "
        if tsp > 0:
            string = string + f"{tsp}tsp"
            if pinch > 0:
                string = string + ", "
        if pinch > 0:
            string = string + f"{pinch}pinch"
    else:
        string = f"{units} {form}"

    return string


# This one should be put into the ShoppingList class
def merge_ingredients(ingredients_list):
    """
    Input ingredients list which can contain duplicate items.
    Output a list where each item is unique, but the previous multiple items
    have been combined (quantityes are summed)
    """
    for item in ingredients_list:
        if len(ingredients_list[item]) > 1:
            # There are multiple elements here
            sum_units = 0
            form = ""
            for quantity, unit in ingredients_list[item]:
                if unit:
                    sum_units = sum_units + standardize_units(quantity, unit)
                if form != "" and unit in {
                    "l",
                    "dl",
                    "tbsp",
                    "msk",
                    "tsp",
                    "tsk",
                    "cups",
                    "pint",
                    "oz",
                    "pinch",
                    "pinches",
                    "krm",
                }:
                    form = "ml"
                elif form != "":
                    form = "g"
            ingredients_list[item] = (sum_units, form)
        else:
            ingredients_list[item] = ingredients_list[item][0]
