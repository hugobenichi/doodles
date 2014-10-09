package col

type Col interface {
  Empty() Col
  Add(interface{}) Col
  Union(interface{}) Col
  IsEmpty() bool
}
