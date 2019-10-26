package spider

import (
	"fmt"
	"net/http"
	"strings"
)

func Login(client *http.Client) {
	host := "https://www.wenku8.net/login.php?do=submit&jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php"
	data := "username=&password=&usecookie=0&action=login&submit=%26%23160%3B%B5%C7%26%23160%3B%26%23160%3B%C2%BC%26%23160%3B"
	req, err := http.NewRequest("POST", host, strings.NewReader(data))
	if err != nil {
		fmt.Println("build new request error")
		return
	}
	req.Header.Set("origin", "https://www.wenku8.net")
	req.Header.Set("content-type", "application/x-www-form-urlencoded")
	req.Header.Set("referer", "https://www.wenku8.net/login.php?do=submit&jumpurl=http://www.wenku8.net/index.php")
	req.Header.Set("upgrade-insecure-requests", "1")
	req.Header.Set("user-agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36")

	resp, err := client.Do(req)
	if err != nil {
		fmt.Println("login error")
		return
	}
	defer resp.Body.Close()
}
