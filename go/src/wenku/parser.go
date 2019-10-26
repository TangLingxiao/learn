package spider

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"regexp"
	"strconv"

	"github.com/soup"
)

var url = "https://www.wenku8.net/modules/article/articlelist.php?page="

func getDownloadUrl(client *http.Client, name string, url string) string {
	req, err := http.NewRequest("GET", url, nil)
	resp, err := client.Do(req)
	if err != nil {
		fmt.Printf("get book:%s downloadurl error", name)
		return ""
	}
	defer resp.Body.Close()
	content, _ := ioutil.ReadAll(resp.Body)
	body := ConvertToString(string(content), "gbk", "utf-8")
	pattern := regexp.MustCompile("https.*txtfull")
	return string(pattern.Find([]byte(body)))
}

func parserOnePage(client *http.Client, content string) map[string]string {
	doc := soup.HTMLParse(content)
	nodes := doc.FindAll("div", "style", "width:95px;float:left;")
	res := make(map[string]string)
	for _, node := range nodes {
		link := node.Find("a")
		res[link.Attrs()["title"]] = getDownloadUrl(client, link.Attrs()["title"], link.Attrs()["href"])
	}
	fmt.Println(res)
	return res
}

func parserPages(client *http.Client, begin int, end int, out chan<- map[string]string) {
	for i := begin; i < end; i++ {
		page := url + strconv.Itoa(i)
		req, err := http.NewRequest("GET", page, nil)
		resp, err := client.Do(req)
		if err != nil {
			fmt.Printf("get page %d error", i)
			return
		}
		defer resp.Body.Close()
		content, _ := ioutil.ReadAll(resp.Body)
		body := ConvertToString(string(content), "gbk", "utf-8")

		out <- parserOnePage(client, body)
		fmt.Println("page: ", i, "done")
	}
	done := map[string]string{
		"done": "",
	}

	out <- done
}
