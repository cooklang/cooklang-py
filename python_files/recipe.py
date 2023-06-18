"""
File containing Recipe class

This class contains data related to recipe.
"""

from helper import convert_str_to_float
from cooklang import parseRecipe


class Recipe:
    """Class containing recipe."""

    def __init__(self, path):
        """
        Parse input recipe and populate internal datastructure.
        It contains metadata, ingredients and steps.
        """
        recipe = parseRecipe(path)
        self._metadata = recipe["metadata"]
        self._ingredients = recipe["ingredients"]
        self._steps = recipe["steps"]

    def __str__(self):
        """Printable class"""
        string = ""
        if (meta_str := self.get_metadata_str()) != "":
            string = string + "\nMetadata:\n" + meta_str + "\n"
        if (ingredients_str := self.get_ingredients_str()) != "":
            string = string + "\nIngredients:\n" + ingredients_str + "\n"
        if (steps_str := self.get_steps_str()) != "":
            string = string + "\nSteps\n" + steps_str + "\n"
        return string

    @property
    def metadata(self):
        """Return internal metadata"""
        return self._metadata

    @property
    def metadata_str(self):
        """Return internal metadata string representation"""
        return self.get_metadata_str()

    @property
    def ingredients(self):
        """Return internal ingredients"""
        return self._ingredients

    @property
    def ingredients_str(self):
        """Return internal ingredients string representation"""
        return self.get_ingredients_str()

    @property
    def steps(self):
        """Return internal steps"""
        return self._steps

    @property
    def steps_str(self):
        """Return internal steps string representation"""
        return self.get_steps_str()

    def get_metadata_str(self):
        """Return a string representation of metadata"""
        if not self._metadata:
            return ""
        string = ""

        for key in self._metadata:
            string = string + f"    {key}: {self._metadata[key]}\n"
        # Remove last \n
        index = string.rfind("\n")
        return string[:index]

    def get_ingredients_str(self):
        """Return a string representation of ingredients"""
        if not self.ingredients:
            return ""
        # Print header
        string = ""
        # Find longest name, so that formatting can be based on that
        max_len_name = max({len(item["name"]) for item in self.ingredients})
        for item in self.ingredients:
            if item["type"] == "ingredient":
                quantity = convert_str_to_float(item["quantity"])
                string = (
                    string
                    + f'    {item["name"].ljust(max_len_name, " ")}'
                    + f'    {quantity} {item["units"]}\n'
                )
            else:
                # This is unexpected. Log this occurance!
                string = string + "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
                string += f'Problem with {item["type"]}'
                with open("log.log", encoding="UTF-8") as file:
                    file.write(self.ingredients, encoding="UTF-8")

        # Remove the final \n
        index = string.rfind("\n")
        string = string[:index]
        return string

    def get_steps_str(self):
        """Return a string representation of steps"""
        if not self.steps:
            return ""

        string = ""
        index = 1
        for step in self.steps:
            text = []
            cookware = []
            ingredient = []

            for sub_step in step:
                if sub_step["type"] == "text":
                    text.append(sub_step["value"])
                elif sub_step["type"] == "cookware":
                    # Strip trailing commas since the following
                    # type of writing is to be expected
                    # "mix [...] with a #blender, allow to settle"
                    text.append(sub_step["name"])
                    if quantity := sub_step.get("quantity", None):
                        cookware.append(f'{sub_step["name"].strip(",")}: {quantity}')
                    else:
                        cookware.append(sub_step["name"].strip(","))
                elif sub_step["type"] == "ingredient":
                    text.append(sub_step["name"])
                    quantity = convert_str_to_float(sub_step["quantity"])
                    ingredient.append(
                        (
                            f'{sub_step["name"].strip(",")}:'
                            + f'{quantity} {sub_step["units"]}'
                        )
                    )
                elif sub_step["type"] == "timer":
                    quantity = convert_str_to_float(sub_step["quantity"])
                    text.append(f"{quantity} {sub_step['units']}")

            # Print step
            string = string + f'     {index}. {"".join(text)}\n'
            # Print cookware
            if cookware:
                string = string + f'        [{"; ".join(cookware)}]\n'
            # Print ingredients
            if ingredient:
                string = string + f'        [{"; ".join(ingredient)}]\n'
            else:
                string = string + "        [-]\n"
            index = index + 1

        return string
