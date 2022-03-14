import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pytest

import cooklang_processor as c

@pytest.fixture
def recipe(tmp_path):
  d = tmp_path / "recipe.cook"
  yield d

def test_anon_timer(recipe):
  recipe.write_text("hello ~{5%minutes}")
  i = c.process(recipe)
  assert i['timers'][0] == ("", "5 minutes")

def test_named_timer(recipe):
  recipe.write_text("hello ~egg{5%minutes}")
  i = c.process(recipe)
  assert i['timers'][0] == ("egg", "5 minutes")
