X11()

data <- rbinom(n=1000, size=100, prob=0.1)
hist(data)

message("Press Return To Continue")
invisible(readLines("stdin", n=1))
