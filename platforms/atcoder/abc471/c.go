package main

import (
	"fmt"
	"slices"
)

func abs(a int) int {
	if a >= 0 {
		return a
	} else {
		return -a
	}
}

func main() {
	var n int
	fmt.Scan(&n)
	neg := make([]int, 0)
	pos := make([]int, 0)
	for i := 0; i < n; i++ {
		var v int
        fmt.Scan(&v)
		if v < 0 {
			neg = append(neg, -v)
		} else {
			pos = append(pos, v)
		}
    }
	var inf = 10000000000
	pos = append(pos, 0)
	neg = append(neg, inf)
	pos = append(pos, inf)
	slices.Sort(neg)
	slices.Sort(pos)

	var total int
	total = 0
	var a,b int
	a = 0
	b = 0
	var swap bool
	swap = false
	for i := 0; i < n; i++ {
		if pos[a+1]-pos[a] < pos[a]+neg[b] || pos[a+1]-pos[a] == pos[a]+neg[b] && swap == true {
			total += pos[a+1]-pos[a]
			a++
		} else {
			total += pos[a]+neg[b]
			a++
			pos, neg = neg, pos
			a, b = b, a
			swap = !swap
		}
	}

	fmt.Println(total)
}
