package selecting

type Config struct {
}

type Cmd string

var (
	Diff  = Cmd("diff")
	Clean = Cmd("clean")
)

func (c *Config) Do(cmd Cmd) {
	switch cmd {
	case Diff:
		c.cmd_diff()
	case Clean:
		c.cmd_clean()
	}
}

func (c *Config) cmd_diff() {
}

func (c *Config) cmd_clean() {
}
