package main

import (
  "fmt"
  "net"
  "bytes"
)

func echoOn(con net.Conn) {
  for {
    buf := make([]byte, 16)
    nbs, err := con.Read(buf)
    if err != nil {
      continue
    }
    fmt.Println("Read ", nbs, " bytes")
    _, err = con.Write(buf)
  }
}

func listenFrom(con net.Conn, ch chan []byte) {
  header := append([]byte(con.RemoteAddr().String()), " => "...)
  for {
    buf := make([]byte, 4096)
    _, err := con.Read(buf)
    if err != nil {
      continue
    }
    ch <- append(header, buf...)
  }
}

func servFrom(l net.Listener) {
  var cons []net.Conn
  input := make(chan []byte)
  go func() {
    for {
      data := <- input
      for _, c := range cons {
        c.Write(bytes.ToUpper(data))
      }
    }
  } ()
  for {
    con, err := l.Accept()
    if err != nil {
      fmt.Println("error while accepting connections: %s", err)
      continue
    }
    fmt.Println("new connection from ", con.RemoteAddr())
    cons = append(cons,con)
    go listenFrom(con, input)
  }
}

func main() {

  fmt.Println("hello hugo")

  serv, err := net.Listen("tcp", ":9000")

  if err != nil {
    fmt.Println("could not open tcp listener on port 9000: %s", err)
    return
  }

  servFrom(serv)
}
