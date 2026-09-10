package main

import "fmt"

func main() {
	var a int
	var b int
	_, err := fmt.Scan(&a, &b)
    if err != nil {
        return
    }

	if a*b == 9 || a+b == 9 || a-b == 9 || (a/b == 9 && a%b == 0) {
		fmt.Println("Nine")
	} else {
		fmt.Println("Nein")
	}
}
