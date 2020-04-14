from random import shuffle, randrange 
import sys 
  
sys.setrecursionlimit(10**6)
 
def make_maze(w = 256, h = 40):
    vis = [[0] * w + [1] for _ in range(h)] + [[1] * (w + 1)]
    ver = [["x  "] * w + ['x'] for _ in range(h)] + [[]]
    hor = [["xxx"] * w + ['x'] for _ in range(h + 1)]
 
    def walk(x, y):
        vis[y][x] = 1
 
        d = [(x - 1, y), (x, y + 1), (x + 1, y), (x, y - 1)]
        shuffle(d)
        for (xx, yy) in d:
            if vis[yy][xx]: continue
            if xx == x: hor[max(y, yy)][x] = "x  "
            if yy == y: ver[y][max(x, xx)] = "   "
            walk(xx, yy)
 
    walk(randrange(w), randrange(h))
 
    s = ""
    for (a, b) in zip(hor, ver):
        s += ''.join(a + ['\n'] + b + ['\n'])
    return s
 
if __name__ == '__main__':
    maze1 = make_maze()
    maze_list = maze1.split('\n')
    maze_list = [line for line in maze_list if line != '']
    row_length = len(maze_list[-1])
    for i in range(row_length):
        if maze_list[-2][i] == " "  and maze_list[1][i] == " ":
            line = list(maze_list[-1])
            line[i] = ' '
            line = "".join(line)
            maze_list[-1] = line 
            break

    additional_maze = maze_list[1:len(maze_list)] 
    maze_list.extend(additional_maze)
    maze_list.extend(additional_maze)
    maze_list.extend(additional_maze)
    maze_list.extend(additional_maze)

    for line in maze_list:
        if line == '':
            continue
        print(line)

