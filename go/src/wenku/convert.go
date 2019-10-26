package spider

import (
	"github.com/axgle/mahonia"
)

func ConvertToString(src string, strCode string, tagCode string) string {
	srcCoder := mahonia.NewDecoder(strCode)
	srcResult := srcCoder.ConvertString(src)
	tagCoder := mahonia.NewDecoder(tagCode)
	_, data, _ := tagCoder.Translate([]byte(srcResult), true)
	result := string(data)
	return result
}
