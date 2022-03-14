import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pytest

import cooklang_processor as c

@pytest.fixture
def recipe(tmp_path):
  d = tmp_path / "recipe.cook"
  yield d

def test_bare_ingredient(recipe):
  recipe.write_text("hello @chicken mcnugget")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken", "")

def test_bare_ingredient_with_hyphen(recipe):
  recipe.write_text("hello @chicken-mcnugget")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken-mcnugget", "")

def test_bare_ingredient_with_underscore(recipe):
  recipe.write_text("hello @chicken_mcnugget")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken_mcnugget", "")

def test_brace_ingredient(recipe):
  recipe.write_text("hello @chicken mcnugget{}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken mcnugget", "")

def test_brace_ingredient_with_hyphen(recipe):
  recipe.write_text("hello @chicken-mcnugget{}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken-mcnugget", "")

def test_brace_ingredient_with_underscore(recipe):
  recipe.write_text("hello @chicken_mcnugget{}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken_mcnugget", "")

def test_brace_ingredient_with_quantity(recipe):
  recipe.write_text("hello @chicken mcnugget{3}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken mcnugget", "3")

def test_brace_ingredient_with_quantity_and_unit(recipe):
  recipe.write_text("hello @chicken mcnugget{3%boxes}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken mcnugget", "3 boxes")

def test_bare_then_brace(recipe):
  recipe.write_text("hello @fries and @chicken mcnugget{3%boxes}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("fries", "")
  assert i['ingredients'][1] == ("chicken mcnugget", "3 boxes")

def test_brace_then_bare(recipe):
  recipe.write_text("hello @chicken mcnugget{3%boxes} and @fries")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken mcnugget", "3 boxes")
  assert i['ingredients'][1] == ("fries", "")

def test_bare_surrounded_braces(recipe):
  recipe.write_text("hello @chicken mcnugget{3%boxes} and @fries and @coca cola{}")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("chicken mcnugget", "3 boxes")
  assert i['ingredients'][1] == ("fries", "")
  assert i['ingredients'][2] == ("coca cola", "")

def test_brace_surrounded_bares(recipe):
  recipe.write_text("hello @fries and @coca cola{} and @napkin")
  i = c.process(recipe)
  assert i['ingredients'][0] == ("fries", "")
  assert i['ingredients'][1] == ("coca cola", "")
  assert i['ingredients'][2] == ("napkin", "")

@pytest.mark.parametrize("punc", [" ", ",", "!", "?", "\"", ";", "'"])
def test_ending_punctuation(recipe, punc):
  recipe.write_text("hello @fries" + punc)
  i = c.process(recipe)
  assert i['ingredients'][0] == ("fries", "")