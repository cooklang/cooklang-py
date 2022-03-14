import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pytest

import cooklang_processor as c

@pytest.fixture
def recipe(tmp_path):
  d = tmp_path / "recipe.cook"
  yield d

def test_bare_ingredient(recipe):
  recipe.write_text("hello #steamer pot")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer", "")

def test_bare_ingredient_with_hyphen(recipe):
  recipe.write_text("hello #steamer-pot")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer-pot", "")

def test_bare_ingredient_with_underscore(recipe):
  recipe.write_text("hello #steamer_pot")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer_pot", "")

def test_brace_ingredient(recipe):
  recipe.write_text("hello #steamer pot{}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer pot", "")

def test_brace_ingredient_with_hyphen(recipe):
  recipe.write_text("hello #steamer-pot{}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer-pot", "")

def test_brace_ingredient_with_underscore(recipe):
  recipe.write_text("hello #steamer_pot{}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer_pot", "")

def test_brace_ingredient_with_quantity(recipe):
  recipe.write_text("hello #steamer pot{3}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer pot", "3")

def test_brace_ingredient_with_quantity_and_unit(recipe):
  recipe.write_text("hello #steamer pot{3%boxes}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer pot", "3 boxes")

def test_bare_then_brace(recipe):
  recipe.write_text("hello #pan and #steamer pot{3%boxes}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("pan", "")
  assert i['cookware'][1] == ("steamer pot", "3 boxes")

def test_brace_then_bare(recipe):
  recipe.write_text("hello #steamer pot{3%boxes} and #pan")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer pot", "3 boxes")
  assert i['cookware'][1] == ("pan", "")

def test_bare_surrounded_braces(recipe):
  recipe.write_text("hello #steamer pot{3%boxes} and #pan and #cupcake rack{}")
  i = c.process(recipe)
  assert i['cookware'][0] == ("steamer pot", "3 boxes")
  assert i['cookware'][1] == ("pan", "")
  assert i['cookware'][2] == ("cupcake rack", "")

def test_brace_surrounded_bares(recipe):
  recipe.write_text("hello #pan and #cupcake rack{} and #napkin")
  i = c.process(recipe)
  assert i['cookware'][0] == ("pan", "")
  assert i['cookware'][1] == ("cupcake rack", "")
  assert i['cookware'][2] == ("napkin", "")

@pytest.mark.parametrize("punc", [" ", ",", "!", "?", "\"", ";", "'"])
def test_ending_punctuation(recipe, punc):
  recipe.write_text("hello #pan" + punc)
  i = c.process(recipe)
  assert i['cookware'][0] == ("pan", "")