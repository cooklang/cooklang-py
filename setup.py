from distutils.core import Extension, setup

setup(
    name="cooklang",
    version="1.0",
    ext_modules=[
        Extension(
            "cooklang",
            [
                "src/CooklangExtension.c",
                "src/CooklangParser.c",
                "Cooklang.tab.c",
                "src/LinkedListLib.c",
                "src/CooklangRecipe.c",
                "src/ShoppingListParser.c",
            ],
        )
    ],
)
