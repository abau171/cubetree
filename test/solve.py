import cubetree

random_depth = int(input("random depth: "))

cubetree.load_lookups()

c = cubetree.Cube()
c.shuffle(random_depth)

solution = None
for depth in range(random_depth + 1):
    solution = c.search_depth(depth)
    if solution != None:
        break
if solution == None:
    print("no solution found")
else:
    for move in solution:
        print(move)

