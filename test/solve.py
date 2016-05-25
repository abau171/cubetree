import cubetree

random_depth = int(input("random depth: "))

cubetree.load_lookups()

c = cubetree.Cube()
c.shuffle(random_depth)

solution = cubetree.solve(c)
if solution == None:
    print("no solution found")
else:
    for move in solution:
        print(move)

