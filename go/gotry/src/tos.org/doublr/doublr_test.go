package doublr

import "testing"

func Test(t *testing.T) {
  var test = []struct{
    in, out string
  }{
    { "", "" },
    { "a", "aa"},
    { "foo", "foofoo" },
  }

  for _, x := range test {
    got := Double(x.in)
    if got != x.out {
      t.Errorf("got %s from %s but wanted %s", got, x.in, x.out)
    }
  }

}
