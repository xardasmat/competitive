package main

import (
	"fmt"
	"strings"
)

func main() {
	var n int
	fmt.Scan(&n)
	m := make(map[string]int)
	var top int
	top = 0
	for i := 0; i < n; i++ {
		var s string
        fmt.Scan(&s)
		s = strings.ToLower(s)
		m[s]++
		top = max(top, m[s]) 
    }
	fmt.Println(top)
}
