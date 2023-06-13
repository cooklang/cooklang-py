



# [WIP] cook-in-c



Cook-in-c is a Cooklang language parser written in C. It features a Python3 C extension module to make interacting with it easier.

## Dependencies:

The following programs are needed:
* python3.10-dev
* python3.10-devutils
* pip for pytho3.10
* python3.10 -m pip install django
* pip3 install flask

The code has been checked with black, flake8, mypy and pylint

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
If you would strictly like to run the module, you will need these files:

- include/*
- src/*
- Cooklang.tab.c
- Cooklang.tab.h
- lex.yy.c
- setup.py

The module has three functions, parseRecipe(), parseShoppingList(), and printRecipe().  

### parseRecipe()
The parseRecipe function takes one argument, a string that represents the path to a recipe file that the user desires to parse using the Cooklang Language specification. The output is a python dictionary representing the parsed recipe, which will follow this format:
```
{
   "metadata":[
      
   ],
   "ingredients":[
      
   ],
   "cookware":[
      
   ],
   "steps":[
      
   ]
}
```

As an example the following file, named testRecipe.cook :
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
The parseShoppingList() function works very similarly to the parseRecipe() function in that it takes one input, which is path leading to a file that the user desires to parse. The formatting of this file must follow the shopping list specification. The output from this function is a python dictionary representing the parsed shopping list, which will follow a format similar to this:
```
   {
      "category":"Category 1",
      "items":[
         {
            "name":"item 1",
            "synonyms":[
               
            ]
         },
         {
            "name":"item 2",
            "synonyms":[
               "synonym 1"
            ]
         }
      ]
   },
   {
      "category":"Category 1",
      "items":[
         {
            "name":"item 1",
            "synonyms":[
               
            ]
         },
         {
            "name":"item 2",
            "synonyms":[
               
            ]
         },
         {
            "name":"item 3",
            "synonyms":[
               
            ]
         }
      ]
   }
]
```



As an example this file, called testShoppingList.cook :
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

### printRecipe()
The functin printRecipe() takes in a recipe in the same format as output by the function parseRecipe() and outputs a neat, easily to read recipe that is simply meant to be quickly readable. The function does not have a return value.

For example, the recipe that was parsed in the parseRecipe() function example could then be input into the printRecipe function with this command:
```
cooklang.printRecipe(parsedRecipe)
```
Which would produce the output:
```
Metadata:
  - servings: 1

Ingredients:
  - avocado, 0.500
  - mayonnaise, 0.750 tablespoon
  - tarragon, 0.250 teaspoon
  - lemon juice, 0.062 teaspoon
  - bacon, 2.000 slices

Cookware
  -  large saut pan

Steps
  - 1:
    - Direction 1:
      - type: text
      - To make the avocado mayonnaise, in a small bowl, combine the
    - Direction 2:
      - type:     ingredient
      - name:     avocado
      - quantity: 0.500
      - units:
    - Direction 3:
      - type: text
      -
    - Direction 4:
      - type: text
      - (peeled, pitted, and coarsely mashed),
    - Direction 5:
      - type:     ingredient
      - name:     mayonnaise
      - quantity: 0.750
      - units:    tablespoon
    - Direction 6:
      - type: text
      - ,
    - Direction 7:
      - type:     ingredient
      - name:     tarragon
      - quantity: 0.250
      - units:    teaspoon
    - Direction 8:
      - type: text
      -
    - Direction 9:
      - type: text
      - (fresh, finely chopped), and
    - Direction 10:
      - type:     ingredient
      - name:     lemon juice
      - quantity: 0.062
      - units:    teaspoon
    - Direction 11:
      - type: text
      -  and mix well.
  - 2:
    - Direction 1:
      - type: text
      - To make the sandwiches, warm a
    - Direction 2:
      - type:     cookware
      - name:     large saut pan
      - quantity:
    - Direction 3:
      - type: text
      -  over medium heat. Add
    - Direction 4:
      - type:     ingredient
      - name:     bacon
      - quantity: 2.000
      - units:    slices
    - Direction 5:
      - type: text
      -  and saut until most of the fat is rendered and the bacon is crisp on the edges but still chewy at the center, about
    - Direction 6:
      - type:     timer
      - name:
      - quantity: 5.000
      - units:    minutes
    - Direction 7:
      - type: text
      - .
```
