package secure

import (
	"golang.org/x/crypto/nacl/box"
)

const (
	Key            = 32                                // size of a cipher key.
	Seed           = 24                                // size of the cipher salt.
	Block          = 1024                              // size of the message blocks.
	EncryptedBlock = Block + box.Overhead              // size of an encrypted block.
	MessageBound   = 32 * 1024                         // maximum supported size of a message.
	BlockBound     = MessageBound / Block              // maximum number of blocks
	Buffer         = (BlockBound + 1) * EncryptedBlock // size of encryption buffers.
)
