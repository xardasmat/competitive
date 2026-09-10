package main

import (
	"container/heap"
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type MaxHeap []int

func (h MaxHeap) Len() int           { return len(h) }
func (h MaxHeap) Less(i, j int) bool { return h[i] > h[j] } // max
func (h MaxHeap) Swap(i, j int)      { h[i], h[j] = h[j], h[i] }

func (h *MaxHeap) Push(x any) {
	*h = append(*h, x.(int))
}

func (h *MaxHeap) Pop() any {
	old := *h
	n := len(old)
	x := old[n-1]
	*h = old[0 : n-1]
	return x
}

func main() {
	var v, q int
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Split(bufio.ScanWords)
	scanner.Scan()
	q, _ =  strconv.Atoi(scanner.Text())
	scanner.Scan()
	v, _ =  strconv.Atoi(scanner.Text())

	pq := &MaxHeap{}
	heap.Init(pq)

	for i := 0; i<q; i++ {
		var c int
		scanner.Scan()
		c, _ =  strconv.Atoi(scanner.Text())
		if c == 1 {
			var t int
			var w int
			scanner.Scan()
			t, _ =  strconv.Atoi(scanner.Text())
			scanner.Scan()
			w, _ =  strconv.Atoi(scanner.Text())
			heap.Push(pq, w-t)
		} else {
			var t int
			scanner.Scan()
			t, _ =  strconv.Atoi(scanner.Text())
			if pq.Len() == 0 {
				fmt.Println("-1")
			} else {
				val := heap.Pop(pq)
				fmt.Println(min(val.(int)+t, v))
			}
		}
	}
}
