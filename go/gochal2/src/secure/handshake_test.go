package secure

import (
	"bytes"
	"testing"
)

func TestHandshake(t *testing.T) {
	testCases := []struct {
		descr   string
		in      []byte
		out     []byte
		wantErr bool
	}{{
		descr: "first prefix does not match",
	}}

	// what can I test exactly ???
	_ = testCases
}

func TestListen(t *testing.T) {
	testCases := []struct {
		descr   string
		in      []byte
		prefix  []byte
		key     []byte
		wantErr bool
	}{{
		descr:   "prefix does not match",
		in:      []byte("garbage:mykey"),
		prefix:  []byte("prefix:"),
		key:     make([]byte, 5),
		wantErr: true,
	}, {
		descr:   "reader has not enough bytes",
		in:      []byte("prefix:mykey"),
		prefix:  []byte("prefix:"),
		key:     make([]byte, 10),
		wantErr: true,
	}, {
		descr:   "prefix matches",
		in:      []byte("prefix:mykey"),
		prefix:  []byte("prefix:"),
		key:     make([]byte, 5),
		wantErr: false,
	}, {
		descr:   "prefix is empty",
		in:      []byte("mykey"),
		prefix:  []byte(""),
		key:     make([]byte, 5),
		wantErr: false,
	}}

	for _, tt := range testCases {
		err := listen(bytes.NewReader(tt.in), tt.prefix, tt.key)
		switch {
		case err == nil && tt.wantErr:
			t.Errorf("%s: got nil, want error", tt.descr)
		case err != nil && !tt.wantErr:
			t.Errorf("%s: got %q, want no error", tt.descr, err)
		}
	}
}
