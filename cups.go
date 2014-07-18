package main

import "fmt"
import "strconv"
import "container/list"

const (
    VOLUME_X int = 4
    VOLUME_Y int = 9
    TARGET_VOLUME int = 2
)

type Cups struct {
    x int
    y int
}

type CupsState struct {
    steps int
    parent *Cups
}

func (c *Cups) toString() string {
    return "(" + strconv.Itoa(c.x) + ", " + strconv.Itoa(c.y) + ")"
}

func (c *Cups) valid() bool {
    return c.x == TARGET_VOLUME || c.y == TARGET_VOLUME
}

func (c *Cups) fillX() *Cups {
    return &Cups{x: VOLUME_X, y: c.y}
}

func (c *Cups) fillY() *Cups {
    return &Cups{x: c.x, y: VOLUME_Y}
}

func (c *Cups) emptyX() *Cups {
    return &Cups{x: 0, y: c.y}
}

func (c *Cups) emptyY() *Cups {
    return &Cups{x: c.x, y: 0}
}

func (c *Cups) pourXY() *Cups {
    newY := int(min(VOLUME_Y, c.x + c.y))
    newX := c.x - (newY - c.y)
    return &Cups{x: newX, y: newY}
}

func (c *Cups) pourYX() *Cups {
    newX := int(min(VOLUME_X, c.x + c.y))
    newY := c.y - (newX - c.x)
    return &Cups{x: newX, y: newY}
}

func (c *Cups) mutate() [6]*Cups {
    return [6]*Cups{c.fillX(), c.fillY(), c.emptyX(), c.emptyY(), c.pourXY(), c.pourYX()}
}

func min(a, b int) int {
    if a < b {
        return a
    } else {
        return b
    }
}

func main() {
    var currentCups *Cups
    var nextCups [6]*Cups
    var currentState, nextState CupsState
    queue := list.New()
    hash := make(map[Cups]CupsState)

    currentCups = new(Cups)
    queue.PushBack(*currentCups)
    hash[*currentCups] = CupsState{steps: 0, parent: nil}

    for queue.Len() > 0 {
        e := queue.Front()
        queue.Remove(e)
        currentCups = new(Cups)
        *currentCups = e.Value.(Cups)
        currentState = hash[*currentCups]
        nextState = CupsState{steps: currentState.steps + 1, parent: currentCups}

        if currentCups.valid() {
            fmt.Println(currentState.steps)
            outputStack := list.New()
            for currentCups != nil {
                outputStack.PushBack(currentCups.toString())
                currentCups = hash[*currentCups].parent
            }
            for outputStack.Len() > 0 {
                e := outputStack.Back()
                outputStack.Remove(e)
                fmt.Println(e.Value)
            }
            break
        }

        nextCups = currentCups.mutate()
        for _, cups := range nextCups {
            _, exists := hash[*cups]
            if !exists {
                queue.PushBack(*cups)
                hash[*cups] = nextState
            }
        }
    }
}
