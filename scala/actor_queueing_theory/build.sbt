name := "Examples in queueing theory"

version := "0.1"

scalaVersion := "2.10.4"

resolvers +=
  "Typesafe Repository" at "http://repo.typesafe.com/typesafe/releases/"

libraryDependencies +=
  "com.typesafe.akka" %% "akka-actor" % "2.3.2"

scalaSource in Compile := file("src")
