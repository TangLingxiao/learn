package main

import (
	"fmt"
	"time"
	spider "wenku"
)

func main() {
	begin := time.Now()
	client := spider.Client()
	spider.Login(client)
	spider.Run(client)
	end := time.Now()
	fmt.Println("totaltime:", end.Sub(begin)/time.Second)
}
