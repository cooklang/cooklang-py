from distutils.core import Extension, setup

setup(
    name="cooklang",
    version="1.0",
    ext_modules=[
        Extension(
            "cooklang",
            [
                "CooklangExtension.c",
                "cooklang-c/src/CooklangParser.c",
                "cooklang-c/parserFiles/Cooklang.tab.c",
                "cooklang-c/src/LinkedListLib.c",
                "cooklang-c/src/CooklangRecipe.c",
                "cooklang-c/src/ShoppingListParser.c",
            ],
        )
    ],
)
