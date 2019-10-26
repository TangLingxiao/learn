package spider

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"net/http/cookiejar"
	"os"
)

func Client() *http.Client {
	jar, _ := cookiejar.New(nil)
	client := http.Client{Jar: jar}
	return &client
}

func Run(client *http.Client) {
	fp, err := os.OpenFile("data.json", os.O_RDWR|os.O_TRUNC|os.O_CREATE, 0755)
	if err != nil {
		log.Fatal(err)
	}
	defer fp.Close()

	var done = 0
	num := 10
	res := make(chan map[string]string, num)
	for i := 0; i < num; i++ {
		go parserPages(client, i*10+1, 1+(i+1)*10, res)
	}

	//go parserPages(client, 0, 1, res)
	for {
		select {
		case x := <-res:
			if _, ok := x["done"]; ok {
				done++
				if done == num {
					close(res)
					return
				}
			} else {
				data, err := json.MarshalIndent(x, "", "	")
				if err != nil {
					fmt.Println("json Marshal error", x)
				}
				_, err = fp.Write(data)
				if err != nil {
					log.Fatal(err)
				}
			}
		}
	}
}
