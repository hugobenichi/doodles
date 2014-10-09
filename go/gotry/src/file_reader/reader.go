package main

import (
  "fmt"
  //"io"
  //"io/ioutil"
  "os"
  "bufio"
)

func main() {
  fmt.Println("hello world")

/*
  file, err := ioutil.ReadFile("./reader.go")

  if err != nil {
    panic(err)
  }

  fmt.Println(string(file))
*/

  f, err := os.Open("./reader.go")

  if err != nil {
    panic(err)
  }

  // reader := bufio.NewReader(f)
  //fmt.Println(reader.ReadLine())

  scan := bufio.NewScanner(f)

  for scan.Scan() == true {
    fmt.Println(scan.Text())
  }

}
