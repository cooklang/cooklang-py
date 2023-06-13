from distutils.core import Extension, setup

setup(
    name="cooklang",
    version="1.0",
    ext_modules=[
        Extension(
            "cooklang",
            [
<<<<<<< HEAD
                "cooklang-c/src/CooklangExtension.c",
                "cooklang-c/src/CooklangParser.c",
                "cooklang-c/Cooklang.tab.c",
=======
                "CooklangExtension.c",
                "cooklang-c/src/CooklangParser.c",
                "cooklang-c/parserFiles/Cooklang.tab.c",
>>>>>>> 476ef9a... put setup.py back
                "cooklang-c/src/LinkedListLib.c",
                "cooklang-c/src/CooklangRecipe.c",
                "cooklang-c/src/ShoppingListParser.c",
            ],
        )
    ],
)
