package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
	"net/http/cookiejar"
)

type formdata struct {
	Username  string
	Password  string
	Usecookie string
	Action    string
	Submit    string
}

func main() {
	host := "https://www.wenku8.net/login.php?do=submit&jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php"
	data := "username=&password=&usecookie=0&action=login&submit=%26%23160%3B%B5%C7%26%23160%3B%26%23160%3B%C2%BC%26%23160%3B"
	req, err := http.NewRequest("POST", host, strings.NewReader(data))
	if err != nil {
		fmt.Println("json marshl error")
		return
	}
	req.Header.Set("origin", "https://www.wenku8.net")
	req.Header.Set("content-type", "application/x-www-form-urlencoded")
	req.Header.Set("referer", "https://www.wenku8.net/login.php?do=submit&jumpurl=http://www.wenku8.net/index.php")
	req.Header.Set("upgrade-insecure-requests", "1")
	req.Header.Set("user-agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36")
	jar,_:=cookiejar.New(nil)

	client := http.Client{Jar:jar}
	resp, err := client.Do(req)
	defer resp.Body.Close()
	if err != nil {
		fmt.Println("login error")
		return
	}
	reqIndex, err:=http.NewRequest("GET","http://www.wenku8.net/index.php", nil)
	respIndex,err:=client.Do(reqIndex)
	defer respIndex.Body.Close()
	if err!=nil{
		fmt.Println("get index error")
		return
	}
	con, _:=ioutil.ReadAll(respIndex.Body)
	fmt.Println(string(con))
}
