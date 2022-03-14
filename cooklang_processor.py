import re

def process(recipe):
  recipe_file = open(recipe, 'r')

  # todo: comments

  ingredients = []
  cookware = []
  timers = []
  meta = []
  for line in recipe_file:
    ingredients += _do_regex("@", line)
    cookware += _do_regex("#", line)
    timers += _do_regex("~", line)
    meta.append(_do_meta(line))

  # remove all None elemenets from lists
  ingredients = list(filter(None, ingredients))
  cookware = list(filter(None, cookware))
  timers = list(filter(None, timers))
  meta = list(filter(None, meta))

  return {
    'ingredients': ingredients,
    'cookware': cookware,
    'timers': timers,
    'meta': meta,
  }


def _do_regex(symbol, line):
  ret = []
  # get indices of all symbols in the line
  at_indices = [index for index,value in enumerate(line) if value == symbol]

  # walk through each symbol
  for num,idx in enumerate(at_indices):
    # only check the text between two symbols
    if idx != at_indices[-1]:
      nowline = line[idx:at_indices[num+1]]
    else:
      nowline = line[idx:]

    # check if it's a brace-ingredient
    if (match := re.search(symbol+"(.*?)\{(.*?)\}", nowline)):
      ingredient = match.group(1)
      quantity = re.sub("\%", " ", match.group(2))
    elif (match := re.search(symbol+"([A-Za-z0-9\-_]*)", nowline)):
      ingredient = match.group(1)
      quantity = ''
    else:
      print('Error!')

    ret.append((ingredient, quantity))

  return ret

def _do_meta(line):
  if (match := re.match(">>\s*(.*?):\s*(.*)", line)):
    return match.groups()
  else:
    return None