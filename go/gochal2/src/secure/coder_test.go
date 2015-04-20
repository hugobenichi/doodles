package secure

import (
	"bytes"
	"testing"
)

func TestWrite(t *testing.T) {
	// TODO
}

func TestFirstBlock(t *testing.T) {
	testCases := []struct {
		descr    string
		in       []byte
		want     []byte
		wantTail []byte
	}{{
		descr:    "a small message",
		in:       []byte("hello world"),
		want:     append([]byte{0, 0, 0, 11}, []byte("hello world")...),
		wantTail: nil,
	}, {
		descr:    "an empty message",
		in:       []byte(""),
		want:     nil,
		wantTail: nil,
	}, {
		descr:    "a long message",
		in:       append(make([]byte, Block-4), []byte("hello world")...),
		want:     append([]byte{0, 0, 4, 7}, make([]byte, Block-4)...),
		wantTail: []byte("hello world"),
	}}

	for _, tt := range testCases {
		got := make([]byte, Block)
		tail := firstBlock(got, tt.in)
		if !bytes.Equal(got[0:len(tt.want)], tt.want) {
			t.Errorf("%s: got %v, want %v", tt.descr, got, tt.want)
		}
		if !bytes.Equal(tail, tt.wantTail) {
			t.Errorf("%s: got %v, want %v", tt.descr, tail, tt.wantTail)
		}
	}
}

func TestNextBlock(t *testing.T) {
	testCases := []struct {
		descr string
		in    [2][]byte
		want  [2][]byte
	}{{
		descr: "not the last fragment",
		in: [2][]byte{
			[]byte("anything"),
			append([]byte("head"), append(make([]byte, 1020), []byte("tail")...)...),
		},
		want: [2][]byte{
			append([]byte("head"), make([]byte, 1020)...),
			[]byte("tail"),
		},
	}, {
		descr: "the last fragment",
		in: [2][]byte{
			append([]byte("something"), make([]byte, 1015)...),
			[]byte("something else"),
		},
		want: [2][]byte{
			append([]byte("something else"), make([]byte, 1010)...),
			[]byte{},
		},
	}, {
		descr: "after the last fragment",
		in:    [2][]byte{[]byte("something"), []byte{}},
		want:  [2][]byte{[]byte{}, []byte{}},
	}}

	for _, tt := range testCases {
		gotBlock, gotTail := nextBlock(tt.in[0], tt.in[1])
		if want := tt.want[0]; !bytes.Equal(gotBlock, want) {
			t.Errorf("%s: got %v, want %v", tt.descr, gotBlock, want)
		}
		if want := tt.want[1]; !bytes.Equal(gotTail, want) {
			t.Errorf("%s: got %v, want %v", tt.descr, gotTail, want)
		}
	}
}
