package conf

type C struct {
	Source   string
	Staging  string
	Working  string
	Output   string
	Exts     []string
	DirOfExt map[string]string
}
