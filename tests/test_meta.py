import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pytest

import cooklang_processor as c

@pytest.fixture
def recipe(tmp_path):
  d = tmp_path / "recipe.cook"
  yield d

def test_meta(recipe):
  recipe.write_text(">> title: hello")
  i = c.process(recipe)
  assert i['meta'][0] == ("title", "hello")

def test_non_meta(recipe):
  recipe.write_text("hello")
  i = c.process(recipe)
  assert i['meta'] == []
