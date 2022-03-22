import tempfile
import unittest
from typing import Dict, Tuple

import cooklang
import yaml

POSSIBLE_FIELDS = {
    "type",
    "name",
    "quantity",
    "units",
}


def pretty_print_result(result: Dict) -> None:
    j = 0
    for i, step in enumerate(result["steps"]):
        print("\n  Step " + str(i) + ":")

        for thing in step:
            j += 1
            print("   Item " + str(j) + ":")
            print("    Type: " + thing["type"])

            if "value" in thing:
                print("    Value: " + thing["value"])
            elif "name" in thing:
                print("    Name: " + thing["name"])
            elif "quantity" in thing:
                print("    Quantity: " + str(thing["quantity"]))
            elif "units" in thing:
                print("    Units: " + thing["units"])

    print("\n  Metadata:")
    print("\n".join(result["metadata"]))


def json_print_result(result: Dict) -> str:
    step_string = ""
    for step in result["steps"]:
        for thing in step:
            step_string += '{ "type": "' + thing["type"] + '"'

            if "value" in thing:
                step_string += ', "value": "' + thing["value"] + '"'

            if "name" in thing:
                step_string += ', "name": "' + thing["name"] + '"'

            if "quantity" in thing:
                step_string += ', "quantity": "' + str(thing["quantity"]) + '"'

            if "units" in thing:
                step_string += ', "units": "' + thing["units"] + '"'

            step_string += "}"

    for meta in result["metadata"]:
        step_string += '{ "Identifier": "' + meta + '", "Content": "' + result["metadata"][meta] + '"}'

    return step_string


def equal_quantities(quantity1: any, quantity2: any) -> bool:
    return (quantity1 == quantity2) or (float(quantity1) == float(quantity2))


def equal_directions(direction1: Dict, direction2: Dict) -> bool:
    if direction1 == direction2:
        return True
    if len(direction1) != len(direction2):
        return False
    for item in direction1.keys():
        if (item not in POSSIBLE_FIELDS) or (item not in direction2):
            return False
        if item == "quantity":
            if not equal_quantities(direction1[item], direction2[item]):
                print("  Test failed. Quantities did not match")
                return False
        # otherwise check which item is different
        elif direction1[item] != direction2[item]:
            print("Item: " + item + " did not match in actual output:")
            print("  Expected: " + str(direction1[item]) + "   Actual: " + (direction2[item]))
            return False
    return True


def equal_steps(expected_steps: list, actual_steps: list) -> bool:
    if expected_steps == actual_steps:
        return True

    # check length of the list, then iterate on both of them
    if len(expected_steps) != len(actual_steps):
        return False

    for expected_step, actual_step in zip(expected_steps, actual_steps):
        # check again length of the step lists and iterate on direc
        if len(expected_step) != len(actual_step):
            return False
        for expected_direc, actual_direc in zip(expected_step, actual_step):
            if not equal_directions(expected_direc, actual_direc):
                return False
    return True


def equal_inputs(expected_input: Dict, actual_input: Dict) -> bool:
    """
    Returns:
        True if input are correct else False
    """

    if not equal_steps(expected_input["steps"], actual_input["steps"]):
        return False

    # test meta data
    if expected_input["metadata"] != actual_input["metadata"]:
        print("  Test failed. Metadata did not match")
        return False

    return True


def test_parsing(file_content: str, expected_result: Dict) -> Tuple[bool, Dict]:
    with tempfile.NamedTemporaryFile() as test_file:
        test_file.write(file_content.encode())
        test_file.seek(0)
        actual_result = cooklang.parseRecipe(test_file.name)
    return equal_inputs(expected_result, actual_result), actual_result


class TestTestingProcess(unittest.TestCase):
    r"""this class test the test_parsing function
    and yes, in this project we are testing the unit-tests :p Meta testing \o/
    """

    def test_equal_quantities(self) -> None:
        self.assertTrue(equal_quantities(1, 1.0))
        self.assertTrue(equal_quantities("1", "1"))
        self.assertTrue(equal_quantities("1", "1.0"))
        self.assertTrue(equal_quantities("one", "one"))
        self.assertFalse(equal_quantities("1", "2"))
        self.assertFalse(equal_quantities(1, 2))

    def test_equal_direction(self) -> None:
        self.assertTrue(
            equal_directions(
                {"type": "text", "value": "Add a bit of butter"},
                {"type": "text", "value": "Add a bit of butter"},
            )
        )
        self.assertFalse(
            equal_directions(
                {"type": "text"},
                {"type": "text", "value": "Add a bit of chilli"},
            )
        )
        self.assertFalse(
            equal_directions(
                {"type": "text"},
                {"type2": "text", "value": "Add a bit of chilli"},
            )
        )
        self.assertFalse(
            equal_directions(
                {"type": "text", "value": "Add a bit of butter"},
                {"type": "text", "value": "Add a bit of chilli"},
            )
        )

    def test_equal_steps(self) -> None:
        self.assertTrue(
            equal_steps(
                [[{"type": "text", "value": "Add a bit of chilli"}]],
                [[{"type": "text", "value": "Add a bit of chilli"}]],
            )
        )
        self.assertFalse(
            equal_steps(
                [[{"type": "text", "value": "Add a bit of chilli"}]],
                [[{"type": "text", "value": "Add a bit of butter"}]],
            )
        )

    def test_one_step(self) -> None:
        expected_result = {"steps": [[{"type": "text", "value": "Add a bit of chilli"}]], "metadata": {}}
        r, actual_result = test_parsing("Add a bit of chilli\n", expected_result)
        self.assertTrue(r)
        r, actual_result = test_parsing("Add a bit of butter\n", expected_result)
        self.assertFalse(r)


class TestCanonical(unittest.TestCase):
    def test_canonical(self) -> None:
        # parse input test file and get each test
        with open("testing/tests.yaml") as tests_input_file:
            tests_input = yaml.safe_load(tests_input_file)

        passed = 0
        total = 0
        unpassed = []

        # for each test found, put the test in the file
        for test in tests_input["tests"]:
            expected_result = tests_input["tests"][test]["result"]
            r, actual_result = test_parsing(tests_input["tests"][test]["source"], expected_result)
            if r:
                passed += 1
            else:
                # print the details
                print("\n\n____  Test: " + test)
                print("____           Expected Output           ____")
                pretty_print_result(expected_result)
                print("____            Actual Output            ____")
                pretty_print_result(actual_result)
                print("____           Compare Results           ____")
                unpassed.append(test)
            total += 1

        print("\n\nTests passed: " + str(passed) + "/" + str(total))
        self.assertEqual(unpassed, [])


if __name__ == "__main__":
    unittest.main()
