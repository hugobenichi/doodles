package conf

type C struct {
	Source   string
	Staging  string
	Output   string
	Exts     []string
	DirOfExt map[string]string
}
