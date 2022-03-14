import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pytest

import cooklang_processor as c

@pytest.fixture
def recipe(tmp_path):
  d = tmp_path / "recipe.cook"
  yield d

# src: https://biowaffeln.github.io/cooklang-parser/
def test_biowaffeln(recipe):
  recipe.write_text('''
>> source: https://www.dinneratthezoo.com/wprm_print/6796
>> total time: 6 minutes
>> servings: 2

Place the @apple juice{1,5%cups}, @banana{one sliced}, @frozen mixed berries{1,5%cups} and @vanilla greek yogurt{3/4%cup} in a #blender{}; blend until smooth. If the smoothie seems too thick, add a little more liquid (1/4 cup). 

Taste and add @honey{} if desired. Pour into two glasses and garnish with fresh berries and mint sprigs if desired.
  ''')
  i = c.process(recipe)
  assert i['meta'][1] == ("total time", "6 minutes")
  assert i['ingredients'][0] == ("apple juice", "1,5 cups")
  assert i['cookware'][0] == ("blender", "")
  assert i['timers'] == []

# src: https://raw.githubusercontent.com/cooklang/CookCLI/main/seed/Breakfasts/Easy%20Pancakes.cook
def test_cookcli_seed_1(recipe):
  recipe.write_text('''
>> servings: 2

Crack the @eggs{3} into a blender, then add the @flour{125%g}, @milk{250%ml} and @sea salt{1%pinch}, and blitz until smooth.

Pour into a bowl and leave to stand for 15 minutes.

Melt the butter (or a drizzle of @olive oil{} if you want to be a bit healthier) in a large non-stick frying pan on a medium heat, then tilt the pan so the butter coats the surface.

Pour in 1 ladle of batter and tilt again, so that the batter spreads all over the base, then cook for 1 to 2 minutes, or until it starts to come away from the sides.

Once golden underneath, flip the pancake over and cook for 1 further minute, or until cooked through.

Serve straightaway with your favourite topping.
  ''')
  i = c.process(recipe)
  assert i['meta'][0] == ("servings", "2")
  assert i['ingredients'][0] == ("eggs", "3")
  assert i['cookware'] == []
  assert i['timers'] == []

def test_cookcli_seed_2(recipe):
  recipe.write_text('''
>> servings: 6

Preheat your oven to the lowest setting. Drain the @cannellini beans{2%tins} in a sieve. Place a saucepan on a medium heat.

Peel and dinely slice the @garlic clove{2}. add the @olive oil{1%tbsp} and sliced garlic to the hot pan.

Crubmle the @red chilli{1%item} into the pan, then stir and fry until the grlic turns golden.

Add the @tinned tomatoes{2%tins} and drained cannellini beans to the pan, reduce to a low heat and simmer gently for around 20 minutes, or until reduced and nice and thick. Meanwhile...

Peel, halve and finely chop the @red onion{}. Roughly chop the @cherry tomatoes{10}. Finely chop the @coriander{1%bunch} stalks and roughly chop the leaves.

Coarsely grate the @cheddar cheese{75%g}. Cut @lime{} in half and the other @lime{} into six wedges.

Cut the @avocados{2} in half lengthways, use a spppon to sccoop out and dicard the stone, then scoop the fles into a bowl to make your guacamole.

Roughly mash the avocado with the back of a fork, then add the onion, cherry tomatoes, coriander stalks and @ground cumin{1%pinch}. Season with @sea salt{} and @black pepper{} and squeeze in the juice from one of the lime halves.

Mix well then have a taste of your guacoamole and tweak with more salt, pepper and lime jouice until you've got a good balance of flovours and its tasing delicious. Set aside.

Loosely wrap the @tortillas{6%large} in tin foil then pop in the hot oven to warm through, along with two plates. Finely chop the @fresh red chilli{2} and put it aside for later.

Make your table look respectable - get the cutlery, salt and pepper and drinks laid out nicely.

By now your beans should be done, so have a taste and season with salt and pepper. Turn the heat off and pop a lid on th pan sothey stay nice and warm.

Put a small non-stick saucepan on a low heat. Add the @butter{30%g} and leave to melt. Meanwhile...

Crack the @eggs{8%large} into a bowl, add a pinch of @salt{} and @black pepper{} and beat with a fork. When the buter has melted, add the eggs to the pan. Stir the eggs slowly with a spatula, getting right into the sides of the pan. Cook gently for 5 to 10 minutes until they just start to scramble then turn the heat off - they'll continute to cook on their own.

Get two plates and pop a warm tortilla on each one. Divide the scrambled eggs between them then top with a good spoonful of you home-made beans.

Scatter each portion with grated cheese and as much chilli as youdare, then roll each tortilla up.

Spoon guacamole and @sour cream{200%ml} on top of each one, scatter with coriander leaves and dust with a little @smoked paprika{1%pinch}. Serve each portion with wedge of lime for squeezing over, and tuck in.
  ''')
  i = c.process(recipe)
  assert i['meta'][0] == ("servings", "6")
  assert i['ingredients'][2] == ("olive oil", "1 tbsp")
  assert i['cookware'] == []
  assert i['timers'] == []

def test_cookcli_seed_3(recipe):
  recipe.write_text('''
>> servings: 1

Place @bacon{2%slices} in a #large skillet{}. Cook over medium heat until browned. Drain, crumble, and set aside.

In a #stock pot{}, melt @margarine{1/9%cup} over medium heat. Whisk in @flour{1/9%cup} until smooth. Gradually stir in @milk{7/6%cup}, whisking constant until thickened. Stir in @large baked potatoes{2/3} and @green onions{2/3}. Bring to a boil, stirring frequently.

Reduce heat to low, and simmer for ~{10%minutes}. Mix in bacon, @shredded cheddar cheese{1/5%cup}, and @sour cream{1/6%cup}. Then add @salt, and @pepper to taste. Continue cooking, stirring frequently until cheese is melted.
  ''')
  i = c.process(recipe)
  assert i['meta'][0] == ("servings", "1")
  assert i['ingredients'][0] == ("bacon", "2 slices")
  assert i['ingredients'][1] == ("margarine", "1/9 cup")
  assert i['cookware'][0] == ("large skillet", "")
  assert i['timers'][0] == ("", "10 minutes")