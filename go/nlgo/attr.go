package nlgo

type Attr struct {
  Len uint16
  Id  uint16
  Value []byte
}

func shortOf(msg []byte, int offset) uint16 {
  var s uint 16
  s = msg[offset] << 8 + msg[1 + offset]
  return s
}

func Parse (msg []byte) Attr {
  length := shortOf(msg, 0)
  return Attr { Len: length, Id: shortOf(msg, 2),  msg[4:(length - 4)] }
}

func NextAttr (msg []byte) []byte {
  return msg[Align(shortOf(msg,0)):]
}

func Align (int index) int {
  if (index & 3) == 0 { return index + 4 } else { return (index  & ~3) + 4 }
}
