package main

import (
	"fmt"
	"math"
)

const (
	// Create a huge number by shifting a 1 bit left 100 places.
	// In other words, the binary number that is 1 followed by 100 zeroes.
	Big = 1 << 100
	// Shift it right again 99 places, so we end up with 1<<1, or 2.
	Small = Big >> 99
)

func needInt(x int) int { return x*10 + 1 }
func needFloat(x float64) float64 {
	return x
}

func sqrt(x float64) string {
	if x < 0 {
		return sqrt(-x) + "i"
	}
	return fmt.Sprint(math.Sqrt(x))
}

//z -= (z*z - x) / (2 * z)
func Sqrt(x float64) float64 {
	z := 1.0
	mi := x - z
	for i := 0; i < 10; i++ {
		fmt.Println(z)
		temp := z
		z -= (z*z - x) / (2 * z)
		if temp == z {
			return z
		}

		if x-z >= mi {
			return z
		}
		mi = x - z
	}
	return z
}

func hello() {
	defer fmt.Println("world")
	defer fmt.Println("hello")
}

type t struct {
	a *int
	b int
}

func printslice(s []int) {
	fmt.Printf("len = %d, cap = %d, %v\n", len(s), cap(s), s)
}

func Interface(x []int, fn func([]int)) {
	fn(x)
}

func adder() func(int) int {
	sum := 0
	return func(x int) int {
		sum += x
		return sum
	}
}

func (tt t) fibonacci() func() int {
	times := 0
	a := 0
	b := 1
	return func() int {
		if times == 0 {
			times++
			return 0
		}
		if times == 1 {
			times++
			return 1
		}
		times++
		c := b
		b, a = a+b, c
		return a
	}
}

type Int int

func (v *Int) pp() {
	fmt.Println(*v)
}
func main() {
	var v Int

	v = 1
	v.pp()
}
