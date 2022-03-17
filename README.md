# [WIP] cooklang-py
Cooklang parser in Python based on https://github.com/cooklang/cooklang-c.



# [WIP] cook-in-c



Cook-in-c is a Cooklang language parser written in C. It features a Python3 C extension module to make interacting with it easier.


## Install:


Download the code as a zip file and extract, or use the git clone command to download:

```
git clone https://github.com/cooklang/cook-in-c.git
```


## Setup
It is easiest to use the parser with the python module. To do so, navigate to the directory containing the setup.py file. The module can be built using the command:
```
python setup.py build
```
The Python module will be built and automatically placed in a folder that is named after the operating system the module is built on. The build will create a folder called build, inside of which will be a few folders. Inside one of these folders the python module will be located. The module can then be accessed in this folder through python.

Users can also install the module to their python environment so that it can be accessed from anywhere using this command:
```
python setup.py install
```


Then, the user may import the parser into their project using the following:
```
import cooklang
```

## Usage

The module has two functions, parseRecipe() and parseShoppingList().

### parseRecipe()
The parseRecipe function takes one argument, a string that represents the path to a recipe file that the user desires to parse using the Cooklang Language specification. The output is a python dictionary representing the parsed recipe. As an example the following file, named testRecipe.cook :
```
>> servings: 1
-- this is a comment, I have shortened this recipe for the sake of conciseness
To make the avocado mayonnaise, in a small bowl, combine the @avocado{1/2} (peeled, pitted, and coarsely mashed), @mayonnaise{3/4 %tablespoon}, @tarragon{1/4 %teaspoon} (fresh, finely chopped), and @lemon juice{1/16 %teaspoon} and mix well.

To make the sandwiches, warm a #large saut pan{} over medium heat. Add @bacon{2 %slices} and saut until most of the fat is rendered and the bacon is crisp on the edges but still chewy at the center, about ~{5%minutes}.
```

with this sequence of commands:
```
import cooklang
parsedRecipe = cooklang.parseRecipe("testRecipe.cook")
parsedRecipe
```

would produce the following output:
```
{
   "metadata":[
      {
         "servings":"1"
      }
   ],
   "ingredients":[
      {
         "type":"ingredient",
         "name":"avocado",
         "quantity":"0.500",
         "units":""
      },
      {
         "type":"ingredient",
         "name":"mayonnaise",
         "quantity":"0.750",
         "units":"tablespoon"
      },
      {
         "type":"ingredient",
         "name":"salt",
         "quantity":"some",
         "units":""
      },
      {
         "type":"ingredient",
         "name":"pepper9.",
         "quantity":"some",
         "units":""
      }
   ],
   "cookware":[

   ],
   "steps":[
      [
         {
            "type":"text",
            "value":"To make the avocado mayonnaise, in a small bowl, combine the "
         },
         {
            "type":"ingredient",
            "name":"avocado",
            "quantity":"0.500",
            "units":""
         },
         {
            "type":"text",
            "value":" "
         },
         {
            "type":"text",
            "value":"(peeled, pitted, and coarsely mashed) and "
         },
         {
            "type":"ingredient",
            "name":"mayonnaise",
            "quantity":"0.750",
            "units":"tablespoon"
         },
         {
            "type":"text",
            "value":"."
         }
      ],
      [
         {
            "type":"text",
            "value":"Season with "
         },
         {
            "type":"ingredient",
            "name":"salt",
            "quantity":"some",
            "units":""
         },
         {
            "type":"text",
            "value":" and "
         },
         {
            "type":"ingredient",
            "name":"pepper9.",
            "quantity":"some",
            "units":""
         }
      ]
   ]
}
```



### parseShoppingList()
The parseShoppingList() function works very similarly to the parseRecipe() function in that it takes one input, which is path leading to a file that the user desires to parse. The formatting of this file must follow the shopping list specification. The output from this function is a python dictionary representing the parsed shopping list. As an example this file, called testShoppingList.cook :
```
[Costco]
potatoes
milk
butter

[Tesco]
bread
salt

[deli]
chicken

[canned goods]
tuna|chicken of the sea
```

with this sequence of commands:
```
import cooklang
parsedShoppingList = cooklang.parseShoppingList("testRecipe.cook")
parsedShoppingList
```

would produce the following output:
```
[
   {
      "category":"Costco",
      "items":[
         {
            "name":"potatoes",
            "synonyms":[

            ]
         },
         {
            "name":"milk",
            "synonyms":[

            ]
         },
         {
            "name":"butter",
            "synonyms":[

            ]
         }
      ]
   },
   {
      "category":"Tesco",
      "items":[
         {
            "name":"bread",
            "synonyms":[

            ]
         },
         {
            "name":"salt",
            "synonyms":[

            ]
         }
      ]
   },
   {
      "category":"deli",
      "items":[
         {
            "name":"chicken",
            "synonyms":[

            ]
         }
      ]
   },
   {
      "category":"canned goods",
      "items":[
         {
            "name":"tuna",
            "synonyms":[
               "chicken of the sea"
            ]
         }
      ]
   }
]
```
